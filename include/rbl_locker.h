#ifndef RBL_LOCKER_H
#define RBL_LOCKER_H

#include <QMutex>


typedef void (*RLockHandler)(bool lock);


//! Locker class.
class RLocker
{
    private:

        //! Internal initialization function.
        void _init(const RLocker *pLocker = nullptr);

    protected:

        //! Mutex.
        QMutex mutex;
        //! Lock handler.
        RLockHandler lockHandler;

    public:

        //! Constructor.
        RLocker();

        //! Copy constructor.
        RLocker(const RLocker &locker);

        //! Destructor.
        ~RLocker ();

        //! Assignment operator.
        RLocker & operator=(const RLocker &locker);

        //! Return static instance to locker.
        static RLocker & getInstance();

        //! Return lock handler function.
        RLockHandler getLockHandler() const;

        //! Set lock handler function.
        void setLockHandler(RLockHandler lockHandler);

        //! Set lock.
        void setLock(bool setLock);

        //! Convenience function to lock.
        static void lock();

        //! Convenience function to unlock.
        static void unlock();

};

#endif /* RBL_LOCKER_H */
