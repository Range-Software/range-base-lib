#include <QDateTime>
#include <cmath>

#include "rbl_stop_watch.h"

void RStopWatch::_init(const RStopWatch *pStopWatch)
{
    if (pStopWatch)
    {
        this->startTime = pStopWatch->startTime;
        this->pauseTime = pStopWatch->pauseTime;
    }
}

RStopWatch::RStopWatch()
{
    this->_init();
}

RStopWatch::RStopWatch(const RStopWatch &stopWatch)
{
    this->_init(&stopWatch);
}

RStopWatch::~RStopWatch()
{

}

RStopWatch &RStopWatch::operator =(const RStopWatch &stopWatch)
{
    this->_init(&stopWatch);
    return (*this);
}

void RStopWatch::reset()
{
    this->startTime = QDateTime::currentMSecsSinceEpoch();
    this->pauseTime = this->startTime;
}

void RStopWatch::pause()
{
    this->pauseTime = QDateTime::currentMSecsSinceEpoch();
}

void RStopWatch::resume()
{
    this->startTime += QDateTime::currentMSecsSinceEpoch() - this->pauseTime;
}

qint64 RStopWatch::getMiliSeconds() const
{
    return QDateTime::currentMSecsSinceEpoch() - this->startTime;
}

void RStopWatch::addElapsedTime(qint64 elapsedTime)
{
    this->startTime -= elapsedTime;
}

