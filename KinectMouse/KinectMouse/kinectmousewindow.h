#ifndef KINECTMOUSEWINDOW_H
#define KINECTMOUSEWINDOW_H

#include <vector>
#include <memory>

#include <QtWidgets/QMainWindow>
#include <QPainter>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>

#include "ui_kinectmousewindow.h"

#include <boost/circular_buffer.hpp>

#include "Pointer.h"
#include "KinectPointer.h"
#include "SimpleFilter.h"

class KinectMouseWindow : public QMainWindow
{
    Q_OBJECT

public:
    KinectMouseWindow(QWidget *parent = 0);
    ~KinectMouseWindow();

    Q_SLOT void startTracking(bool checked);
    Q_SLOT void setHistory(int maxHistory);

    Q_SLOT void pointerMove(QPoint p);

    Q_SLOT void trackZvalue(float z);

    Q_SIGNAL void requestNextPosition();

private:
    Ui::KinectMouseWindowClass ui;
    QPixmap m_desktop;
    boost::circular_buffer<QPoint> m_points;
    std::unique_ptr<KinectPointer> pointer;

    std::vector<std::unique_ptr<BaseFilter>> m_filters;

    QtCharts::QLineSeries* m_zSeries;
    QtCharts::QLineSeries* m_zdSeries;
    int m_zTime;
    int lastDepth;
    QtCharts::QChart* m_zChart;
    QtCharts::QChart* m_zdChart;
};

#endif // KINECTMOUSEWINDOW_H
