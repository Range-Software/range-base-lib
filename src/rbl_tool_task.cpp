#include "rbl_error.h"
#include "rbl_logger.h"
#include "rbl_tool_task.h"

RToolTask::RToolTask(const RToolInput &toolInput)
    : toolInput(toolInput)
{
}

int RToolTask::perform()
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
            action->setError(error.getType(),error.getMessage());
            emit this->actionFailed(action);
            return 1;
        }
    }
    return 0;
}
