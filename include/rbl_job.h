#ifndef RBL_JOB_H
#define RBL_JOB_H

#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QList>

#include "rbl_job_settings.h"

class RJob : public QObject, public QRunnable
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

        //! Return default job settings.
        static const RJobSettings &getDefaultJobSettings();

    public:

        //! Constructor.
        explicit RJob();

        //! Overloaded run function.
        //! Emit signals.
        virtual void run();

        //! Return job ID.
        uint getID();

        //! Return information on whether the job is finished or not.
        bool isFinished() const;

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

    protected:

        virtual int perform() = 0;

    private:

        //! Lock emit mutextes.
        void lockEmitMutexes();

        //! Unlock emit mutextes.
        void unlockEmitMutexes();

    public slots:

        //! Cancel job.
        void cancel();

        //! Set progress value based on curent step and number of steps.
        void setProgress(qint64 step, qint64 nSteps);

    signals:

        //! Emitted when job blocking state changed.
        void isBlocking(bool blocking);

        //! Emitted when job is canceled.
        void canceled();

        //! Emitted when job is started.
        void started();

        //! Emitted when job is finished.
        void finished();

        //! Emitted when job has failed.
        void failed();

        //! Progress changed.
        void progressChanged(qint64 step, qint64 nSteps);



};

#endif /* RBL_JOB_H */
