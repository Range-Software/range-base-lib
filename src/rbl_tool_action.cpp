#include "rbl_tool_action.h"

void RToolAction::_init(const RToolAction *pRToolAction)
{
    if (pRToolAction)
    {
        this->input = pRToolAction->input;
        this->errorType = pRToolAction->errorType;
        this->errorMessage = pRToolAction->errorMessage;
    }
}

RToolAction::RToolAction()
    : errorType{RError::None}
{
    this->_init();
}

RToolAction::RToolAction(const RToolAction &toolAction)
    : errorType{RError::None}
{
    this->_init(&toolAction);
}

RToolAction::~RToolAction()
{

}

RToolAction &RToolAction::operator =(const RToolAction &toolAction)
{
    this->_init(&toolAction);
    return (*this);
}

RError::Type RToolAction::getErrorType() const
{
    return this->errorType;
}

const QString &RToolAction::getErrorMessage() const
{
    return this->errorMessage;
}

void RToolAction::setError(RError::Type errorType, const QString &errorMessage)
{
    this->errorType = errorType;
    this->errorMessage = errorMessage;
}
