#ifndef RBL_TOOL_ACTION_H
#define RBL_TOOL_ACTION_H

#include <QVariant>

#include "rbl_error.h"

class RToolAction
{

    protected:

        //! Action input.
        QVariant input;
        //! Error type.
        RError::Type errorType;
        //! Error message.
        QString errorMessage;

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
        RToolAction &operator =(const RToolAction &toolAction);

        //! Return error type.
        RError::Type getErrorType() const;

        //! Return const reference to error message.
        const QString &getErrorMessage() const;

        //! Set error.
        void setError(RError::Type errorType, const QString &errorMessage);

        //! Perform action.
        virtual void perform() = 0;

};

#endif // RBL_TOOL_ACTION_H
