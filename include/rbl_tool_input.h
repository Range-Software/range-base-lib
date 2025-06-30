#ifndef RBL_TOOL_INPUT_H
#define RBL_TOOL_INPUT_H

#include <QString>
#include <QList>
#include <QSharedPointer>

#include "rbl_tool_action.h"

class RToolTask;

class RToolInput
{

    protected:

        //! List of actions to be performed
        QList<QSharedPointer<RToolAction>> actions;

    private:

        //! Internal initialization function.
        void _init(const RToolInput *pRToolInput = nullptr);

    public:

        //! Constructor.
        RToolInput();

        //! Copy constructor.
        RToolInput(const RToolInput &toolInput);

        //! Destructor.
        ~RToolInput();

        //! Assignment operator.
        RToolInput & operator =(const RToolInput &toolInput);

        //! Add action.
        void addAction(const QSharedPointer<RToolAction> &action);

        friend class RToolTask;

};

#endif // RBL_TOOL_INPUT_H
