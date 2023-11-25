#include "playimage.h"
#include<QPainter>
//PlayImage的默认构造函数
PlayImage::PlayImage(QWidget *parent) : QWidget(parent)
{
    //在构造函数里，我们设置一下默认的样式即可
    //把背景换成黑的，更好看点一点
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background,Qt::black);
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}

//定义Image更新
void PlayImage::updateImage(const QImage& image)
{
    //由于QPixmap用于绘画事件更稳定更快速，这里不处理Image格式的图片
    //直接转换为QPixmap再调用updatePixmap
    updatePixmap(QPixmap::fromImage(image));
}

//定义QPixmap更新
void PlayImage::updatePixmap(const QPixmap &pixmap)
{
    //因为这里在多线程访问的时候，可能会对m_pixmap造成问题，给这个变量的更新上锁
    //上锁
    m_mutex.lock();
    m_pixmap = pixmap;
    //更新完解锁
    m_mutex.unlock();

    //调用重绘函数,这里发送信号
    update();
}

//重写绘图事件
void PlayImage::paintEvent(QPaintEvent *event)
{
    //有图就重绘
    if(!m_pixmap.isNull())
    {
        //实例化一个绘图对象
        QPainter painter(this);
        //这里也需要上锁，需要先把图片裁剪为合适新尺寸
        m_mutex.lock();
        //把图像按父窗口的大小，保持宽高比缩小,原始图片可能不适配播放器尺寸
        QPixmap pixmap = m_pixmap.scaled(this->size(),Qt::KeepAspectRatio);
        //解锁
        m_mutex.unlock();
        //居中绘制
        int x = (this->width()-pixmap.width())/2;
        int y = (this->height()-pixmap.height())/2;
        painter.drawPixmap(x,y,pixmap);
    }
    //确保绘制成功
    QWidget::paintEvent(event);
}

