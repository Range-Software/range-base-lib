#include "rbl_locker.h"


RLocker::RLocker() : lockHandler(0)
{
    this->_init();
} /* RLocker::RLocker */


RLocker::RLocker(const RLocker &locker)
{
    this->_init(&locker);
} /* RLocker::RLocker (copy) */


RLocker::~RLocker ()
{
} /* RLocker::~RLocker */


RLocker & RLocker::operator =(const RLocker &locker)
{
    this->_init(&locker);
    return (*this);
} /* RLocker::operator = */


void RLocker::_init(const RLocker *pLocker)
{
    if (pLocker)
    {
        this->setLockHandler(pLocker->getLockHandler());
    }
} /* RLocker:_init */


RLocker & RLocker::getInstance()
{
    static RLocker locker;
    return locker;
} /* RLocker:getInstance */


RLockHandler RLocker::getLockHandler() const
{
    return this->lockHandler;
} /* RLocker:getLockHandler */


void RLocker::setLockHandler(RLockHandler lockHandler)
{
    this->lockHandler = lockHandler;
} /* RLocker:setLockHandler */


void RLocker::setLock(bool lock)
{
    if (this->lockHandler)
    {
        this->lockHandler(lock);
    }
    else
    {
        if (lock)
        {
            this->mutex.lock();
        }
        else
        {
            this->mutex.unlock();
        }
    }
} /* RLocker:setLock */


void RLocker::lock()
{
    RLocker::getInstance().setLock(true);
} /* RLocker:lock */


void RLocker::unlock()
{
    RLocker::getInstance().setLock(false);
} /* RLocker:unlock */
