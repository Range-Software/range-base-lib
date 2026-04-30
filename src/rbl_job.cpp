#ifdef _OPENMP
#include <omp.h>
#endif

#include <QReadWriteLock>

#include "rbl_logger.h"
#include "rbl_job.h"

static std::atomic<uint> lastID{0};
static QReadWriteLock defaultJobSettingsLock;

RJobSettings RJob::defaultJobSettings = RJobSettings();

void RJob::setDefaultJobSettings(const RJobSettings &jobSettings)
{
    QWriteLocker locker(&defaultJobSettingsLock);
    RJob::defaultJobSettings = jobSettings;
}

RJobSettings RJob::getDefaultJobSettings()
{
    QReadLocker locker(&defaultJobSettingsLock);
    return RJob::defaultJobSettings;
}

RJob::RJob()
    : id(++lastID)
{
    R_LOG_TRACE;
}

void RJob::run()
{
    R_LOG_TRACE_IN;
    const RJobSettings settings = this->jobSettings;
    this->lockEmitMutexes(settings);
    emit this->started();
    if (settings.getBlocking())
    {
        emit this->isBlocking(true);
    }
    this->unlockEmitMutexes(settings);
#ifdef _OPENMP
    omp_set_num_threads(settings.getNOmpThreads());
#endif
    int retVal = this->perform();
    this->lockEmitMutexes(settings);
    this->jobFinished = true;
    if (settings.getBlocking())
    {
        emit this->isBlocking(false);
    }
    this->unlockEmitMutexes(settings);
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

void RJob::lockEmitMutexes(const RJobSettings &settings)
{
    for (QMutex *pMutex : settings.getEmitMutexList())
    {
        pMutex->lock();
    }
}

void RJob::unlockEmitMutexes(const RJobSettings &settings)
{
    for (QMutex *pMutex : settings.getEmitMutexList())
    {
        pMutex->unlock();
    }
}

void RJob::cancel()
{
    emit this->canceled();
}

void RJob::setProgress(qint64 step, qint64 nSteps)
{
    emit this->progressChanged(step,nSteps);
}
