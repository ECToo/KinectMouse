#pragma once

#include <QPoint>

class BaseFilter
{
public:
    virtual const char* name() const noexcept { return "BaseFilter"; }
    //virtual QPoint compute() const = 0;
    //virtual void add_point(QPoint& p) = 0;
};