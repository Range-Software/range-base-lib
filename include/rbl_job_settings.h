#ifndef RBL_JOB_SETTINGS_H
#define RBL_JOB_SETTINGS_H

#include <QList>
#include <QMutex>

class RJobSettings
{

    protected:

        //! Whether the job is blocking.
        bool blocking;
        //! Whether the job can run in parallel.
        bool parallel;
        //! Number of OMP threads.
        uint nOmpThreads;
        //! List of processes to lock when emitting signals.
        QList<QMutex*> emitMutexList;

    private:

        //! Internal initialization function.
        void _init(const RJobSettings *pJobSettings = nullptr);

    public:

        //! Constructor.
        RJobSettings();

        //! Copy constructor.
        RJobSettings(const RJobSettings &jobSettings);

        //! Destructor.
        ~RJobSettings();

        //! Assignment operator.
        RJobSettings &operator =(const RJobSettings &jobSettings);

        //! Return information on whether the job is blocking.
        bool getBlocking() const;

        //! Set whether the job is blocking.
        void setBlocking(bool blocking);

        //! Get whether the job can run in parallel.
        bool getParallel() const;

        //! Set whether the job can run in parallel.
        void setParallel(bool parallel);

        //! Return number of OMP threads.
        bool getNOmpThreads() const;

        //! Set number of OMP threads.
        void setNOmpThreads(uint nOmpThreads);

        //! Get list of mutexes to be locked.
        const QList<QMutex*> & getEmitMutexList() const;

        //! Set list of mutexes to be locked.
        void setEmitMutexList(const QList<QMutex*> &emitMutexList);

        //! Register mutex to be locked.
        void registerEmitMutex(QMutex *pEmitMutexList);

};

#endif // RBL_JOB_SETTINGS_H
