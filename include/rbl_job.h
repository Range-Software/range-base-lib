#ifndef RBL_JOB_H
#define RBL_JOB_H

#include <QMutex>
#include <QObject>
#include <QList>

#include "rbl_job_settings.h"

class RJob : public QObject
{

    Q_OBJECT

    protected:

        //! Default job settings.
        static RJobSettings defaultJobSettings;

        //! Job ID.
        uint id;
        //! Indicates whether the job is finished or not.
        bool jobFinished;
        //! Job settings.
        RJobSettings jobSettings;

    public:

        //! Set default job settings.
        static void setDefaultJobSettings(const RJobSettings &jobSettings);

    public:

        //! Constructor.
        explicit RJob(QObject *parent = nullptr);

        //! Return job ID.
        uint getID();

        //! Return information on whether the job is finished or not.
        bool isFinished() const;

        //! Return information on whether the job should be
        //! automatically deleted after completion.
        bool getAutoDelete() const;

        //! Set whether the job should be automatically deleted after
        //! completion.
        void setAutoDelete(bool autoDelete);

        //! Set whether the job is blocking.
        void setBlocking(bool blocking);

        //! Set whether the job can run in parallel.
        void setParallel(bool parallel);

        //! Get whether the job can run in parallel.
        bool getParallel() const;

        //! Set number of OMP threads.
        void setNOmpThreads(uint nOmpThreads);

        //! Register mutex to be locked.
        void registerEmitMutex(QMutex *pEmitMutexList);

    private:

        //! Lock emit mutextes.
        void lockEmitMutexes();

        //! Unlock emit mutextes.
        void unlockEmitMutexes();

    public slots:

        //! Process job.
        void process();

    signals:

        //! Emitted when job blocking state changed.
        void isBlocking(bool blocking);

        //! Emitted when job is started.
        void started();

        //! Emitted when job is finished.
        void finished();

        //! Emitted when job has failed.
        void failed();

    protected:

        //! Execute job.
        //! Called by process() and is responsible for calling run()
        //! and for emmiting signals.
        void exec();

        //! Function performing actual job.
        //! Inheriting class should reimplement this function.
        virtual int run();
};

#endif /* RBL_JOB_H */
