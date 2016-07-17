#ifndef KINECTMOUSEWINDOW_H
#define KINECTMOUSEWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include "ui_kinectmousewindow.h"
#include "Pointer.h"

#include <boost/circular_buffer.hpp>

class KinectMouseWindow : public QMainWindow
{
    Q_OBJECT

public:
    KinectMouseWindow(QWidget *parent = 0);
    ~KinectMouseWindow();

    Q_SLOT void startTracking(bool checked);
    Q_SLOT void setHistory(int maxHistory);

    Q_SLOT void pointerMove(QPoint p);

    Q_SIGNAL void requestNextPosition();

private:
    Ui::KinectMouseWindowClass ui;
    QPixmap m_desktop;
    boost::circular_buffer<QPoint> m_points;
    Pointer pointer;
};

#endif // KINECTMOUSEWINDOW_H
