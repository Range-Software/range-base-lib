#include "rbl_tool_action.h"

void RToolAction::_init(const RToolAction *pRToolAction)
{
    if (pRToolAction)
    {
        this->input = pRToolAction->input;
    }
}

RToolAction::RToolAction()
{
    this->_init();
}

RToolAction::RToolAction(const RToolAction &toolAction)
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

