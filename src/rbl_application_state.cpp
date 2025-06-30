#include "rbl_application_state.h"
#include "rbl_locker.h"

void RApplicationState::_init(const RApplicationState *pApplicationState)
{
    if (pApplicationState)
    {
        this->stateType = pApplicationState->stateType;
    }
}

RApplicationState::RApplicationState()
{
    this->_init();
}

RApplicationState::RApplicationState(const RApplicationState &applicationState)
{
    this->_init(&applicationState);
}

RApplicationState::~RApplicationState()
{

}

RApplicationState &RApplicationState::operator =(const RApplicationState &applicationState)
{
    this->_init(&applicationState);
    return (*this);
}

RApplicationState &RApplicationState::getInstance()
{
    static RApplicationState applicationState;
    return applicationState;
}

RApplicationStateType RApplicationState::getStateType() const
{
    RApplicationStateType stateType;
    RLocker::lock();
    stateType = this->stateType;
    RLocker::unlock();
    return stateType;
}

void RApplicationState::setStateType(RApplicationStateType stateType)
{
    RLocker::lock();
    this->stateType = stateType;
    RLocker::unlock();
}
