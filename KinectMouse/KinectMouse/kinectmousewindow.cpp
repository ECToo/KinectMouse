#include "kinectmousewindow.h"

#include <QDesktopWidget>
#include <QPainter>

KinectMouseWindow::KinectMouseWindow(QWidget *parent)
    : QMainWindow(parent),m_desktop(),m_points(250), pointer()
{
    ui.setupUi(this);
    adjustSize();
    QDesktopWidget* w = QApplication::desktop();
    QPixmap pm(w->screenGeometry().size());
    pm = QPixmap::grabWindow(w->screen()->winId());
    //pm.toImage().save("output.png");

    auto pmSize = w->screenGeometry().size();
    pmSize.scale(960,540, Qt::KeepAspectRatio);

    pm = pm.scaled(pmSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui.image->setPixmap(pm);
    adjustSize();
    m_desktop = std::move(pm);

    connect(&pointer, SIGNAL(newPosition(QPoint)), this, SLOT(pointerMove(QPoint)));
    connect(this, SIGNAL(requestNextPosition()), &pointer, SLOT(nextPosition()), Qt::QueuedConnection);

    connect(ui.pbStart, SIGNAL(clicked(bool)), this, SLOT(startTracking(bool)));
    connect(ui.sbHistory, SIGNAL(valueChanged(int)), this, SLOT(setHistory(int)));
}

KinectMouseWindow::~KinectMouseWindow()
{

}

Q_SLOT void KinectMouseWindow::startTracking(bool checked)
{
    if (checked)
        pointer.nextPosition();
}

Q_SLOT void KinectMouseWindow::setHistory(int maxHistory)
{
    m_points.set_capacity(maxHistory);
}

Q_SLOT void KinectMouseWindow::pointerMove(QPoint p)
{
    m_points.push_front(std::move(p));
    QPixmap combined{m_desktop.size()};
    QPainter painter{ &combined };
    painter.drawPixmap(m_desktop.rect(), m_desktop);

    for (auto& i : m_points)
    {
        painter.setOpacity(painter.opacity()- 1.f/m_points.capacity());
        painter.fillRect(i.x()/2, i.y()/2, 10, 10, QColor(255, 0, 0, 255));
    }
    painter.end();

    ui.image->setPixmap(combined);
    if (ui.pbStart->isChecked())
        Q_EMIT requestNextPosition();
}
