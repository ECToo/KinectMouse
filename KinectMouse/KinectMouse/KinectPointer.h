#pragma once

#include <QObject>
#include <QPoint>

#include <QWinEventNotifier>

#include <Windows.h>
#include <NuiApi.h>

class KinectPointer : public QObject
{
    Q_OBJECT
public:

    Q_SLOT void startTracking();
    Q_SLOT void stopTracking();

    Q_SLOT void requestNextPosition();

    Q_SIGNAL void newPosition(QPoint p);
    Q_SIGNAL void newZPos(float z);

    Q_SIGNAL void mousePressed(QPoint p);
    Q_SIGNAL void mouseReleased(QPoint p);

    void setMouseClickThreshold(float th)
    {
        m_clickThreshold = th;
    }

    KinectPointer(unsigned width, unsigned height);
    virtual ~KinectPointer();

private:

    std::tuple<QPoint, float> skeletonPosToScreenPos(const Vector4& pos);

    void checkMousePressed(QPoint current, float Z);

    Q_SLOT void processNextFrame();

    INuiSensor* m_sensor;
    bool m_nextFrameRequested;
    QWinEventNotifier m_QNextFrameEvent;

    float m_x_scale, m_y_scale;
    float m_clickThreshold;
};

