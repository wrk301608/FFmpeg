#include "widget.h"
#include "ui_widget.h"
#include<QFileDialog>
#include<QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //设置标题
    this->setWindowTitle(QString("VideoPlay Version 1.00"));
    //实例化视频解码线程
    m_readThread = new readThread();
    //将解码线程的自定义信号updateImage信号与PlayImage绑定,直接调用槽函数，槽函数不执行完，阻塞
    connect(m_readThread,&readThread::updateImage,ui->playimage,&PlayImage::updateImage,Qt::DirectConnection);
    //将解码线程的自定义播放状态改变的信号与窗口线程的on_PlayState槽函数绑定
    connect(m_readThread,&readThread::playState,this,&Widget::on_playState);
}

Widget::~Widget()
{

    //释放视频解码线程
    if(m_readThread)
    {
        m_readThread->close();
        //阻塞式等待解码线程执行完毕
        m_readThread->wait();
        delete m_readThread;
    }
    delete ui;
}

//定义文件选择按钮的槽函数
void Widget::on_but_file_clicked()
{
    //设置文件过滤器
    QString strName = QFileDialog::getOpenFileName(this,//指定父窗口
                                                   "选择播放视频",//dialog标题
                                                   "/",//从根目录开始
                                                   "视频(*.mp4 *.m4v *.avi *.flv);;其它(*)"//过滤器字符串
                                                   );
    //文件路径获取失败
    if(strName.isEmpty())
    {
        qDebug() << "窗口线程中获取文件路径失败";
        return;
    }
    qDebug() << strName;
    //设置文件路径显示在文本框上
    ui->com_url->setCurrentText(strName);
}

//定义视频开始播放按钮的槽函数
void Widget::on_but_open_clicked()
{
    //判断有没有视频在播放
    if(ui->but_open->text() == "开始播放")
    {
        //调用视频读取线程，开始视频解码
        m_readThread->open(ui->com_url->currentText());
    }
    //有视频在播放，再点击开始播放按钮，应该是停止播放的功能
    else
    {
        m_readThread->close();
    }
}

//定义暂停播放按钮的槽函数
void Widget::on_btn_pause_clicked()
{
    if(ui->btn_pause->text() == "暂停播放")
    {
        m_readThread->pause(true);
        ui->btn_pause->setText("继续播放");
    }
    //继续播放
    else
    {
        m_readThread->pause(false);
        ui->btn_pause->setText("暂停播放");
    }
}

//定义自定义槽函数playState
void Widget::on_playState(readThread::PlayState state)
{
    //当前在播放视频
    if(state == readThread::play)
    {
        this->setWindowTitle(QString("正在播放: %1").arg(m_readThread->url()));
        ui->but_open->setText("停止播放");
    }
    //没有播放,复原
    else
    {
        ui->but_open->setText("开始播放");
        ui->btn_pause->setText("暂停播放");
        this->setWindowTitle(QString("VideoPlay Version 1.00"));
    }
}
