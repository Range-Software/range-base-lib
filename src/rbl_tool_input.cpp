#include "rbl_tool_input.h"

void RToolInput::_init(const RToolInput *pRToolInput)
{
    if (pRToolInput)
    {
        this->actions = pRToolInput->actions;
    }
}

RToolInput::RToolInput()
{
    this->_init();
}

RToolInput::RToolInput(const RToolInput &toolInput)
{
    this->_init(&toolInput);
}

RToolInput::~RToolInput()
{

}

RToolInput &RToolInput::operator =(const RToolInput &toolInput)
{
    this->_init(&toolInput);
    return (*this);
}

void RToolInput::addAction(const QSharedPointer<RToolAction> &action)
{
    this->actions.append(QSharedPointer<RToolAction>(action));
}

