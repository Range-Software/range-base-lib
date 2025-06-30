#ifdef _OPENMP
#include <omp.h>
#endif

#include "rbl_logger.h"
#include "rbl_job.h"

static uint lastID = 0;

RJobSettings RJob::defaultJobSettings = RJobSettings();

void RJob::setDefaultJobSettings(const RJobSettings &jobSettings)
{
    RJob::defaultJobSettings = jobSettings;
}

const RJobSettings &RJob::getDefaultJobSettings()
{
    return RJob::defaultJobSettings;
}

RJob::RJob()
    : id(++lastID)
    , jobFinished(false)
{
    R_LOG_TRACE;
}

void RJob::run()
{
    R_LOG_TRACE_IN;
    this->lockEmitMutexes();
    emit this->started();
    if (this->jobSettings.getBlocking())
    {
        emit this->isBlocking(true);
    }
    this->unlockEmitMutexes();
#ifdef _OPENMP
    omp_set_num_threads(this->jobSettings.getNOmpThreads());
#endif
    int retVal = this->perform();
    this->lockEmitMutexes();
    this->jobFinished = true;
    if (this->jobSettings.getBlocking())
    {
        emit this->isBlocking(false);
    }
    this->unlockEmitMutexes();
    if (retVal == 0)
    {
        emit this->finished();
    }
    else
    {
        emit this->failed();
    }
    R_LOG_TRACE_OUT;
}

uint RJob::getID()
{
    R_LOG_TRACE;
    return this->id;
}

bool RJob::isFinished() const
{
    R_LOG_TRACE;
    return this->jobFinished;
}

void RJob::setBlocking(bool blocking)
{
    R_LOG_TRACE;
    this->jobSettings.setBlocking(blocking);
}

void RJob::setParallel(bool parallel)
{
    this->jobSettings.setParallel(parallel);
}

bool RJob::getParallel() const
{
    return this->jobSettings.getParallel();
}

void RJob::setNOmpThreads(uint nOmpThreads)
{
    this->jobSettings.setNOmpThreads(nOmpThreads);
}

void RJob::registerEmitMutex(QMutex *pEmitMutexList)
{
    this->jobSettings.registerEmitMutex(pEmitMutexList);
}

void RJob::lockEmitMutexes()
{
    for (QMutex *pMutex : this->jobSettings.getEmitMutexList())
    {
        pMutex->lock();
    }
}

void RJob::unlockEmitMutexes()
{
    for (QMutex *pMutex : this->jobSettings.getEmitMutexList())
    {
        pMutex->unlock();
    }
}
