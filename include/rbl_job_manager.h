#ifndef RBL_JOB_MANAGER_H
#define RBL_JOB_MANAGER_H

#include <QObject>
#include <QQueue>
#include <QList>
#include <QThread>

#include "rbl_job.h"

class RJobManager : public QObject
{
    Q_OBJECT

    protected:

        //! Waiting jobs to be processed.
        QQueue<RJob*> waitingJobs;
        //! Waiting parallel jobs to be processed.
        QQueue<RJob*> waitingParallelJobs;
        //! Running jobs.
        QList<RJob*> runningJobs;
        //! Job is starting - indicator.
        bool jobIsStarting;

    public:

        //! Constructor.
        explicit RJobManager(QObject *parent = nullptr);

        //! Return static instance of the manager.
        static RJobManager & getInstance();

        //! Add new job.
        void submit(RJob *job);

        //! Return number of waiting jobs.
        uint getNWaiting() const;

        //! Return number of running jobs.
        uint getNRunning(bool excludeParallel = false) const;

        //! Get running IDs;
        QList<uint> getRunningIDs() const;

    signals:

        //! Signal emitted when job is blocking.
        void jobBlocking(bool blocking);

        //! Signal emitted when new job is started.
        void jobStarted();

        //! Signal emitted when job is finished.
        void jobFinished();

    protected slots:

        //! Called when blocking() signal is emitted.
        void onJobBlocking(bool blocking);

        //! Called when started() signal is emitted.
        void onJobStarted();

        //! Called when finished() signal is emitted.
        void onJobFinished();

    protected:

        //! Process waiting jobs.
        //! If no queued job is running then start first in the list.
        void processWaitingJobs();

        //! Process finished jobs.
        //! Remove all jobs which are finished from running queue.
        void processFinishedJobs();

        //! Start job.
        void startJob(RJob *job);

        //! Find finished job in the running queue.
        RJob * findFinishedJob() const;

        //! Remove finished jobs from the running queue.
        uint removeFinishedJobs();

};

#endif /* RBL_JOB_MANAGER_H */
