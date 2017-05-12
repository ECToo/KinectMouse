#include "KinectPointer.h"

#include <cassert>
#include <QDebug>
#include <comdef.h>

KinectPointer::KinectPointer(unsigned width, unsigned height):
    m_sensor(nullptr),
    m_nextFrameRequested(),
    m_QNextFrameEvent(),
    m_x_scale(width*1.f/320.f),
    m_y_scale(height*1.f/240.f)
{

    int sensorCount = 0;
    HRESULT hr = NuiGetSensorCount(&sensorCount);

    if (!sensorCount)
        return;

    do
    {
        hr = NuiCreateSensorByIndex(0, &m_sensor);
    } while (hr != S_OK);
    
    m_sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_SKELETON);
    assert(NUI_INITIALIZE_FLAG_USES_SKELETON == m_sensor->NuiInitializationFlags());

    m_QNextFrameEvent.setHandle(CreateEventW(NULL, TRUE, FALSE, NULL));
    connect(&m_QNextFrameEvent, SIGNAL(activated(HANDLE)), this, SLOT(processNextFrame()));
    m_QNextFrameEvent.setEnabled(true);

}

void KinectPointer::startTracking()
{
    m_sensor->NuiSkeletonTrackingEnable(m_QNextFrameEvent.handle(), 0);
}

void KinectPointer::stopTracking()
{
    m_sensor->NuiSkeletonTrackingDisable();
}

std::tuple<QPoint,USHORT> KinectPointer::skeletonPosToScreenPos(const Vector4& pos)
{
    LONG x, y;
    USHORT depth;

    // Calculate the skeleton's position on the screen
    // NuiTransformSkeletonToDepthImage returns coordinates in NUI_IMAGE_RESOLUTION_320x240 space
    NuiTransformSkeletonToDepthImage(pos, &x, &y, &depth);

    float screenPointX = x*m_x_scale;
    float screenPointY = y*m_y_scale;

    return std::tie(QPoint(screenPointX, screenPointY),depth);
}

void KinectPointer::processNextFrame()
{

    NUI_SKELETON_FRAME skeleton_frame = { 0 };

    HRESULT hr = m_sensor->NuiSkeletonGetNextFrame(0, &skeleton_frame);
    if (FAILED(hr))
    {
        _com_error er(hr);
        LPCTSTR errMsg = er.ErrorMessage();
        qDebug() << "Could not get next frame " << QString::fromWCharArray(errMsg) << " " << hr;
        return;
    }

    /// smooth
    //////////////
    m_sensor->NuiTransformSmooth(&skeleton_frame, NULL);
    //////////////

    //draw first skeleton
    QPoint p = { 0,0 };
    USHORT d = 0;
    NUI_SKELETON_DATA* sk_data = nullptr;
    for (int i = 0; i < NUI_SKELETON_COUNT; ++i)
    {
        if (skeleton_frame.SkeletonData[i].eSkeletonPositionTrackingState[NUI_SKELETON_POSITION_HAND_RIGHT] != NUI_SKELETON_NOT_TRACKED)
        {
            Vector4 v = skeleton_frame.SkeletonData[i].SkeletonPositions[NUI_SKELETON_POSITION_HAND_RIGHT];
            auto r = skeletonPosToScreenPos(v);
            p = std::get<0>(r);
            d = std::get<1>(r);
        }
    }
    Q_EMIT newPosition(p);
    Q_EMIT newZPos(d);
    m_nextFrameRequested = false;
}

void KinectPointer::requestNextPosition()
{
    m_nextFrameRequested = true;
}


KinectPointer::~KinectPointer()
{

}
