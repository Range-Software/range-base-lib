#ifdef _OPENMP
#include <omp.h>
#endif

#include "rbl_logger.h"
#include "rbl_job.h"

static uint lastID = 0;

RJob::RJob(QObject *parent)
    : QObject(parent)
    , id(++lastID)
    , jobFinished(false)
    , autoDelete(true)
    , blocking(true)
    , parallel(false)
    , nOmpThreads(1)
{
    R_LOG_TRACE;
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

bool RJob::getAutoDelete() const
{
    R_LOG_TRACE;
    return this->autoDelete;
}

void RJob::setAutoDelete(bool autoDelete)
{
    R_LOG_TRACE;
    this->autoDelete = autoDelete;
}

void RJob::setBlocking(bool blocking)
{
    R_LOG_TRACE;
    this->blocking = blocking;
}

void RJob::setParallel(bool parallel)
{
    this->parallel = parallel;
}

bool RJob::getParallel() const
{
    return this->parallel;
}

void RJob::setNOmpThreads(uint nOmpThreads)
{
    this->nOmpThreads = nOmpThreads;
}

void RJob::registerEmitMutex(QMutex *pEmitMutexList)
{
    this->emitMutexList.append(pEmitMutexList);
}

void RJob::lockEmitMutexes()
{
    for (QMutex *pMutex : this->emitMutexList)
    {
        pMutex->lock();
    }
}

void RJob::unlockEmitMutexes()
{
    for (QMutex *pMutex : this->emitMutexList)
    {
        pMutex->unlock();
    }
}

void RJob::exec()
{
    R_LOG_TRACE_IN;
    this->lockEmitMutexes();
    emit this->started();
    if (this->blocking)
    {
        emit this->isBlocking(true);
    }
    this->unlockEmitMutexes();
#ifdef _OPENMP
    omp_set_num_threads(this->nOmpThreads);
#endif
    int retVal = this->run();
    this->lockEmitMutexes();
    this->jobFinished = true;
    if (this->blocking)
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

void RJob::process()
{
    R_LOG_TRACE_IN;
    this->exec();
    R_LOG_TRACE_OUT;
}

int RJob::run()
{
    R_LOG_TRACE;
    return 0;
}
