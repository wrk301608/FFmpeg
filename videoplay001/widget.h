#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"readthread.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    //选择视频文件按钮的槽函数
    void on_but_file_clicked();
    //开始播放按钮的槽函数
    void on_but_open_clicked();
    //暂停播放按钮的槽函数
    void on_btn_pause_clicked();
    //自定义槽函数，用于处理页面播放状态
    void on_playState(readThread::PlayState state);

private:
    Ui::Widget *ui;
    //维持视频解码线程
    readThread* m_readThread = NULL;
};
#endif // WIDGET_H
