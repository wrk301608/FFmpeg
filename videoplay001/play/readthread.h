#ifndef READTHREAD_H
#define READTHREAD_H


#include<QThread>
#include<QElapsedTimer>
#include<QTime>
//前向声明出来videoDocode类就行
class videoDocode;

class readThread : public QThread
{
    Q_OBJECT
public:
    //视频播放状态
    enum PlayState
    {
        play,end
    };
    explicit readThread(QObject *parent = nullptr);
    ~readThread() override;
    //传入视频地址，开始视频解码线程
    void open(const QString& url = QString());
    //关闭视频解码线程
    void close();
    //暂停
    void pause(bool flag);
    //获取打开的视频地址
    const QString& url();
protected:
    void run() override;
signals:
    //自定义信号，视频播放状态改变
    void playState(PlayState state);
    //自定义信号，更新图片
    void updateImage(const QImage& image);
private:
    videoDocode* m_videoDocode = NULL;//维持一个视频解码对象
    QString m_url;//视频地址
    bool m_play = false;//播放控制位
    bool m_pause = false;//播放暂停标志位

    QTime m_etime2;//
};

#endif // READTHREAD_H
