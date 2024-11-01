#include "rbl_job_settings.h"

void RJobSettings::_init(const RJobSettings *pJobSettings)
{
    if (pJobSettings)
    {
        this->autoDelete = pJobSettings->autoDelete;
        this->blocking = pJobSettings->blocking;
        this->parallel = pJobSettings->parallel;
        this->nOmpThreads = pJobSettings->nOmpThreads;
    }
}

RJobSettings::RJobSettings()
    : autoDelete{true}
    , blocking{true}
    , parallel{false}
    , nOmpThreads{1}
{

}

RJobSettings::RJobSettings(const RJobSettings &jobSettings)
{
    this->_init(&jobSettings);
}

RJobSettings::~RJobSettings()
{

}

RJobSettings &RJobSettings::operator =(const RJobSettings &jobSettings)
{
    this->_init(&jobSettings);
    return (*this);
}

bool RJobSettings::getAutoDelete() const
{
    return this->autoDelete;
}

void RJobSettings::setAutoDelete(bool autoDelete)
{
    this->autoDelete = autoDelete;
}

bool RJobSettings::getBlocking() const
{
    return this->blocking;
}

void RJobSettings::setBlocking(bool blocking)
{
    this->blocking = blocking;
}

bool RJobSettings::getParallel() const
{
    return this->parallel;
}

void RJobSettings::setParallel(bool parallel)
{
    this->parallel = parallel;
}

bool RJobSettings::getNOmpThreads() const
{
    return this->nOmpThreads;
}

void RJobSettings::setNOmpThreads(uint nOmpThreads)
{
    this->nOmpThreads = nOmpThreads;
}

const QList<QMutex *> &RJobSettings::getEmitMutexList() const
{
    return this->emitMutexList;
}

void RJobSettings::setEmitMutexList(const QList<QMutex *> &emitMutexList)
{
    this->emitMutexList = emitMutexList;
}

void RJobSettings::registerEmitMutex(QMutex *pEmitMutexList)
{
    this->emitMutexList.append(pEmitMutexList);
}
