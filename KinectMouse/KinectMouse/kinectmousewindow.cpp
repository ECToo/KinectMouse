#include "kinectmousewindow.h"

#include <QDesktopWidget>
#include <QPainter>
#include <QScreen>

KinectMouseWindow::KinectMouseWindow(QWidget *parent)
    : QMainWindow(parent),m_desktop(),m_points(250), pointer(nullptr), m_zTime(0), lastDepth(0.f), maxDepth(0.f)
{
    ui.setupUi(this);
    adjustSize();
    QScreen* w = QGuiApplication::primaryScreen();
    QPixmap pm = w->grabWindow(0);
    
    auto pmSize = pm.size();
    pointer = std::make_unique<KinectPointer>(pmSize.width(), pmSize.height());
    pmSize.scale(960,540, Qt::KeepAspectRatio);

    pm = pm.scaled(pmSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui.image->setPixmap(pm);
    adjustSize();
    m_desktop = std::move(pm);

    connect(pointer.get(), SIGNAL(newPosition(QPoint)), this, SLOT(pointerMove(QPoint)));
    connect(pointer.get(), SIGNAL(newZPos(float)), this, SLOT(trackZvalue(float)));
    connect(this, SIGNAL(requestNextPosition()), pointer.get(), SLOT(requestNextPosition()), Qt::QueuedConnection);
    connect(ui.spClickThreshold, SIGNAL(valueChanged(double)), pointer.get(), SLOT(setMouseClickThreshold(double)));
    pointer->setMouseClickThreshold(ui.spClickThreshold->value());


    connect(ui.pbStart, SIGNAL(clicked(bool)), this, SLOT(startTracking(bool)));
    connect(ui.sbHistory, SIGNAL(valueChanged(int)), this, SLOT(setHistory(int)));

    m_zSeries = new QtCharts::QLineSeries();
    m_zdSeries = new QtCharts::QLineSeries();
    m_zChart = new QtCharts::QChart();
    m_zdChart = new QtCharts::QChart();
    QtCharts::QChartView *chartView = new QtCharts::QChartView(m_zChart);
    ui.groupBox->layout()->addWidget(chartView);
    chartView = new QtCharts::QChartView(m_zdChart);
    ui.groupBox->layout()->addWidget(chartView);


    // set up filters
    m_filters.push_back(std::make_unique<BaseFilter>());
    
    for (const auto&i : m_filters)
        ui.filterBox->addItem(i->name());
    
}

KinectMouseWindow::~KinectMouseWindow()
{

}

Q_SLOT void KinectMouseWindow::startTracking(bool checked)
{
    if (checked)
    {
        ui.pbStart->setText("Stop Tracking");
        pointer->requestNextPosition();
        pointer->startTracking();
    }
    else
    {
        ui.pbStart->setText("Start Tracking");
        pointer->stopTracking();
    }
}

Q_SLOT void KinectMouseWindow::setHistory(int maxHistory)
{
    m_points.set_capacity(maxHistory);
}

Q_SLOT void KinectMouseWindow::trackZvalue(float z)
{
    if (m_zSeries->points().size() == 300)
    {
        m_zSeries->clear();
        m_zdSeries->clear();
    }
    m_zdSeries->append(QPointF((float)m_zTime, lastDepth - z));
    lastDepth = z;
    m_zdChart->removeSeries(m_zdSeries);
    m_zdChart->addSeries(m_zdSeries);

    maxDepth = std::max(maxDepth, z);

    m_zSeries->append(QPointF((float)m_zTime++,z));
    m_zChart->removeSeries(m_zSeries);
    m_zChart->addSeries(m_zSeries);


    /*m_zChart->createDefaultAxes();
    m_zChart->axisX()->setMin(0.f);
    m_zChart->axisY()->setMax(1.f);

    m_zdChart->createDefaultAxes();
    m_zdChart->axisX()->setMin(0.f);
    m_zdChart->axisY()->setMax(1.f);*/
}

Q_SLOT void KinectMouseWindow::pointerMove(QPoint p)
{
    m_points.push_back(std::move(p));
    QPixmap combined{m_desktop.size()};
    QPainter painter{ &combined };
    painter.drawPixmap(m_desktop.rect(), m_desktop);

    painter.setOpacity(0.0);

    for (auto& i : m_points)
    {
        painter.setOpacity(painter.opacity()+ 1.f/m_points.size());
        painter.fillRect(i.x()/2, i.y()/2, 10, 10, QColor(255, 0, 0, 255));
    }
    painter.end();

    ui.image->setPixmap(combined);
    if (ui.pbStart->isChecked())
        Q_EMIT requestNextPosition();
}
