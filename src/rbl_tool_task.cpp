#include "rbl_error.h"
#include "rbl_logger.h"
#include "rbl_tool_task.h"

RToolTask::RToolTask(const RToolInput &toolInput, QObject *parent)
    : RJob(parent)
    , toolInput(toolInput)
{
}

int RToolTask::run()
{
    foreach (const QSharedPointer<RToolAction> &action, this->toolInput.actions)
    {
        try
        {
            action.data()->perform();
            emit this->actionFinished(action);
        }
        catch (const RError &error)
        {
            RLogger::error("Failed to perform action. %s\n", error.getMessage().toUtf8().constData());
            emit this->actionFailed(action);
            return 1;
        }
    }
    return 0;
}
