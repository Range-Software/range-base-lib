#ifndef RBL_TOOL_ACTION_H
#define RBL_TOOL_ACTION_H

#include <QVariant>

class RToolAction
{

    protected:

        //! Action input.
        QVariant input;

    private:

        //! Internal initialization function.
        void _init(const RToolAction *pRToolAction = nullptr);

    public:

        //! Constructor.
        RToolAction();

        //! Copy constructor.
        RToolAction(const RToolAction &toolAction);

        //! Destructor.
        virtual ~RToolAction();

        //! Assignment operator.
        RToolAction & operator =(const RToolAction &toolAction);

        //! Perform action.
        virtual void perform() = 0;

};

#endif // RBL_TOOL_ACTION_H
