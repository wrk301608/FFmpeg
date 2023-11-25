#ifndef PLAYIMAGE_H
#define PLAYIMAGE_H

#include <QWidget>
#include<qmutex.h>
class PlayImage : public QWidget
{
    Q_OBJECT
public:
    explicit PlayImage(QWidget *parent = nullptr);
    void  updateImage(const QImage& image);
    //声明更新QImage格式的方法
    void updatePixmap(const QPixmap& pixmap);
    //声明更新QPixmap格式的方法

protected:
    void paintEvent(QPaintEvent *event) override;
    //声明绘图重写
signals:
private:
    QPixmap  m_pixmap;
    //维持一个pixmap值
    QMutex m_mutex;
};

#endif // PLAYIMAGE_H
