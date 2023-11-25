#include "readthread.h"
#include"videodocode.h"
#include<QDebug>
#include<qimage.h>
#include<QEventLoop>
#include<QTimer>
#define LOG 1

readThread::readThread(QObject *parent) : QThread(parent)
{
    //在线程类中需要先实例化视频解码的对象
    m_videoDocode = new videoDocode();
    //因为PlayState是自定义的枚举类型，qt不了解
    //在信号和槽中，我们要注册自定义的数据类型
    qRegisterMetaType<PlayState>("PlayState");
}

readThread::~readThread()
{
    //释放视频解码类对象
    if(m_videoDocode)
    {
        delete m_videoDocode;
    }
}

//定义获取打开视频地址的函数
const QString& readThread::url()
{
    return m_url;
}

//非阻塞延时实现
//实现经过指定时间退出，不阻塞主线程的执行
void sleepMesc(int msec)
{
    //不延时，直接退出
    if(msec <= 0)   return;
    //定义一个新的事件循环,处理定时器超时事件
    QEventLoop loop;
    //创建一个定时器，槽函数为线程的退出
    //超时，发送信号，视频解码线程退出
    QTimer::singleShot(msec,&loop,SLOT(quit()));
    //事件循环开始执行，视频解码线程阻塞到这里
    //超时，线程退出
    loop.exec();

}

//重写run函数
void readThread::run()
{
    //首先调用open函数,开始视频解码
    bool ret = m_videoDocode->open(m_url);
    if(ret)
    {
        //视频解码成功
        //设置播放标志位为真
        m_play = true;
        //以当前线程的时间为起点，计算时间
        m_etime2.start();
        //给窗口线程发送视频状态变为play的信号
        emit playState(play);
    }
    //open失败
    else
    {
#if LOG
        qDebug() << "read thread open err";
        qWarning() << "打开失败";
#else
        Q_UNUSED(ret)
#endif
    }
    //开始read
    while(m_play)
    {
        //暂停
        while(m_pause)
        {
            //非阻塞的睡眠200毫秒
            sleepMesc(200);
        }
        //继续read
        //接受视频解码返回来的image图像
        QImage image = m_videoDocode->read();
        //读取成功,有内容
        if(!image.isNull())
        {
            //实现匀速播放，以计算时间差值延时更新图片的方式实现匀速播放
            sleepMesc(int(m_videoDocode->pts()-m_etime2.elapsed()));
            //发送图片更新的信号
            emit updateImage(image);
        }
        //读取到空图像
        else
        {
            if(m_videoDocode->isEnd())
            {
                //读取完成
                qDebug() << "read thread读完了";
                break;
            }
            sleepMesc(1);
        }
    }
    //全部搞完了
    qDebug() << "播放结束";
    //关掉视频解码
    m_videoDocode->close();
    //发送视频播放完的信号
    emit playState(end);
    //到这里，视频解码线程的主要逻辑已经实现完毕
}

//定义pause函数
void readThread::pause(bool flag)
{
    m_pause = flag;
}

//定义线程打开函数,需要外部调用,不调用不开启线程
void readThread::open(const QString &url)
{
    //没有线程在运行
    if(!this->isRunning())
    {
        m_url = url;
        qDebug() << "open video url: " << m_url;
        //发生开始信号
        emit this->start();
    }
}

//定义线程close函数
void readThread::close()
{
    m_play = false;
    m_pause = false;
}
