#ifndef RBL_JOB_H
#define RBL_JOB_H

#include <QMutex>
#include <QObject>
#include <QList>

class RJob : public QObject
{

    Q_OBJECT

    protected:

        //! Job ID.
        uint id;

        //! Indicates whether the job is finished or not.
        bool jobFinished;

        //! Whether the job should be automatically deleted after
        //! completion.
        bool autoDelete;

        //! Whether the job is blocking.
        bool blocking;

        //! Whether the job can run in parallel.
        bool parallel;

        //! Number of OMP threads.
        uint nOmpThreads;

        //! List of processes to lock when emitting signals.
        QList<QMutex*> emitMutexList;

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
