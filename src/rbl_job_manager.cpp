#include <QThreadPool>

#include "rbl_logger.h"
#include "rbl_job_manager.h"

RJobManager::RJobManager(QObject *parent)
    : QObject(parent)
    , jobIsStarting(false)
{
    R_LOG_TRACE;
}

RJobManager & RJobManager::getInstance()
{
    R_LOG_TRACE;
    static RJobManager jobManager;
    return jobManager;
}

void RJobManager::submit(RJob *job)
{
    R_LOG_TRACE_IN;
    R_LOG_TRACE_MESSAGE("Enqueue job");
    if (job->getParallel())
    {
        this->waitingParallelJobs.enqueue(job);
    }
    else
    {
        this->waitingJobs.enqueue(job);
    }
    this->processWaitingJobs();
    this->processFinishedJobs();
    R_LOG_TRACE_OUT;
}

uint RJobManager::getNWaiting() const
{
    R_LOG_TRACE;
    return (uint)this->waitingJobs.size();
}

uint RJobManager::getNRunning(bool excludeParallel) const
{
    R_LOG_TRACE_IN;
    QList<RJob*>::const_iterator iter;
    uint nRunning = 0;

    for (iter = this->runningJobs.begin();
         iter != this->runningJobs.end();
         ++iter)
    {
        if (!(*iter)->isFinished())
        {
            if ((excludeParallel && !(*iter)->getParallel()) || !excludeParallel)
            {
                nRunning ++;
            }
        }
    }
    R_LOG_TRACE_OUT;
    return nRunning;
}

QList<uint> RJobManager::getRunningIDs() const
{
    R_LOG_TRACE_IN;
    QList<uint> runningIDs;

    QList<RJob*>::const_iterator iter;
    for (iter = this->runningJobs.begin();
         iter != this->runningJobs.end();
         ++iter)
    {
        if (!(*iter)->isFinished())
        {
            runningIDs.push_back((*iter)->getID());
        }
    }

    R_LOG_TRACE_OUT;
    return runningIDs;
}

void RJobManager::onJobBlocking(bool blocking)
{
    R_LOG_TRACE_IN;
    emit this->jobBlocking(blocking);
    R_LOG_TRACE_OUT;
}

void RJobManager::onJobStarted()
{
    R_LOG_TRACE_IN;
    this->jobIsStarting = false;
    emit this->jobStarted();
    R_LOG_TRACE_OUT;
}

void RJobManager::onJobFinished()
{
    R_LOG_TRACE_IN;
    emit this->jobFinished();
    this->processWaitingJobs();
    this->processFinishedJobs();
    R_LOG_TRACE_OUT;
}

void RJobManager::onJobFailed()
{
    R_LOG_TRACE_IN;
    emit this->jobFailed();
    this->processWaitingJobs();
    this->processFinishedJobs();
    R_LOG_TRACE_OUT;
}

void RJobManager::processWaitingJobs()
{
    R_LOG_TRACE_IN;
    while (this->waitingParallelJobs.size() > 0)
    {
        this->startJob(this->waitingParallelJobs.dequeue());
    }
    if (this->getNRunning(true) == 0 && this->getNWaiting() > 0 && !this->jobIsStarting)
    {
        this->startJob(this->waitingJobs.dequeue());
    }
    R_LOG_TRACE_OUT;
}

void RJobManager::processFinishedJobs()
{
    R_LOG_TRACE_IN;
    this->removeFinishedJobs();
    R_LOG_TRACE_OUT;
}

void RJobManager::startJob(RJob *job)
{
    R_LOG_TRACE_IN;
    this->jobIsStarting = true;

    QObject::connect(job,&RJob::isBlocking,this,&RJobManager::onJobBlocking);
    QObject::connect(job,&RJob::started,this,&RJobManager::onJobStarted);
    QObject::connect(job,&RJob::finished,this,&RJobManager::onJobFinished);
    QObject::connect(job,&RJob::failed,this,&RJobManager::onJobFailed);

    QThreadPool::globalInstance()->start(job);

    this->runningJobs.append(job);
    R_LOG_TRACE_OUT;
}

RJob * RJobManager::findFinishedJob() const
{
    R_LOG_TRACE_IN;
    QList<RJob*>::const_iterator iter;
    for (iter = this->runningJobs.begin();
         iter != this->runningJobs.end();
         ++iter)
    {
        if ((*iter)->isFinished())
        {
            R_LOG_TRACE_OUT;
            return (*iter);
        }
    }
    R_LOG_TRACE_OUT;
    return 0;
}

uint RJobManager::removeFinishedJobs()
{
    R_LOG_TRACE_IN;
    RJob *job;
    uint nRemovedTotal = 0;
    int nRemoved = 0;

    while ((job = this->findFinishedJob()) != 0)
    {
        nRemoved = this->runningJobs.removeAll(job);
        if (job->getAutoDelete())
        {
            delete job;
        }
        if (nRemoved == 0)
        {
            RLogger::error("No job could be found in the running queue.\n");
        }
        else if (nRemoved > 1)
        {
            RLogger::error("Multiple occurences of one jobe wre found in the running queue.\n");
            nRemovedTotal += (uint) nRemoved;
        }
        else if (nRemoved < 0)
        {
            RLogger::error("Unknown error while removing the job from the running queue.\n");
        }
    }

    R_LOG_TRACE_OUT;
    return nRemovedTotal;
}
