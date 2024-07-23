#ifndef RBL_TOOL_TASK_H
#define RBL_TOOL_TASK_H

#include <QObject>

#include "rbl_job.h"
#include "rbl_tool_input.h"

class RToolTask : public RJob
{

    Q_OBJECT

    protected:

        //! Tool input.
        RToolInput toolInput;

    public:

        //! Constructor.
        explicit RToolTask(const RToolInput &toolInput, QObject *parent = nullptr);

    protected:

        //! Run task.
        int run();

    signals:

        void actionFinished(const QSharedPointer<RToolAction> &action);
        void actionFailed(const QSharedPointer<RToolAction> &action);

};

#endif // RBL_TOOL_TASK_H
