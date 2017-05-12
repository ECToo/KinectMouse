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

    KinectPointer(unsigned width, unsigned height);
    virtual ~KinectPointer();

private:

    std::tuple<QPoint, USHORT> skeletonPosToScreenPos(const Vector4& pos);

    Q_SLOT void processNextFrame();

    INuiSensor* m_sensor;
    bool m_nextFrameRequested;
    QWinEventNotifier m_QNextFrameEvent;

    float m_x_scale, m_y_scale;
};

