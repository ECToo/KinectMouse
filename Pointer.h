#pragma once
#include <QObject>
#include <QPoint>

class Pointer :
    public QObject
{
    Q_OBJECT
public:
    Pointer();
    virtual ~Pointer();

    Q_SLOT void requestNextPosition();

    Q_SIGNAL void newPosition(QPoint p);
};

