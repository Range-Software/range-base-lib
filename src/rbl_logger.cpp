#include <QFile>
#include <QMessageLogContext>
#include <QTextStream>
#include <QDateTime>
#include <QThread>

#include <vector>

#include "rbl_logger.h"
#include "rbl_locker.h"
#include "rbl_error.h"

void RLogger::_init(const RLogger *pLogger)
{
    if (pLogger)
    {
        this->logFileName = pLogger->logFileName;
        this->logLevel = pLogger->logLevel;
        this->halted = pLogger->halted;
        this->printTime = pLogger->printTime;
        this->printThreadId = pLogger->printThreadId;
        this->addNewLine = pLogger->addNewLine;
        this->logHandler = pLogger->logHandler;
        this->indentLevel = pLogger->indentLevel;
        // Copy unprocessed messages.
    }
}

RLogger::RLogger(RLogLevelMask logLevel)
    : logLevel(logLevel)
    , halted(false)
    , printTime(true)
    , printThreadId(false)
    , addNewLine(false)
    , logHandler(0)
    , indentLevel(0)
{
    this->_init();
}

RLogger::RLogger(const RLogger &logger)
{
    this->_init(&logger);
}

RLogger::~RLogger()
{
}

RLogger & RLogger::operator =(const RLogger &logger)
{
    this->_init(&logger);
    return (*this);
}

RLogger & RLogger::getInstance()
{
    static RLogger logger;
    return logger;
}

RLogLevelMask RLogger::getLevel() const
{
    RLogLevelMask tmpLevel;
    RLocker::lock();
    tmpLevel = this->logLevel;
    RLocker::unlock();
    return tmpLevel;
}

void RLogger::setLevel(RLogLevelMask level)
{
    R_ERROR_ASSERT(R_LOG_LEVEL_IS_VALID(level));

    RLocker::lock();
    this->logLevel = level;
    RLocker::unlock();
}

bool RLogger::getHalted() const
{
    bool tmpHalted;
    RLocker::lock();
    tmpHalted = this->halted;
    RLocker::unlock();
    return tmpHalted;
}

void RLogger::setHalted(bool halt)
{
    RLocker::lock();
    this->halted = halt;
    RLocker::unlock();
    if (!this->getHalted())
    {
        this->flush();
    }
}

bool RLogger::getPrintTimeEnabled() const
{
    bool tmpTime = true;
    RLocker::lock();
    tmpTime = this->printTime;
    RLocker::unlock();
    return tmpTime;
}

void RLogger::setPrintTimeEnabled(bool printTime)
{
    RLocker::lock();
    this->printTime = printTime;
    RLocker::unlock();
}

bool RLogger::getPrintThreadIdEnabled() const
{
    return this->printThreadId;
}

void RLogger::setPrintThreadIdEnabled(bool printThreadId)
{
    RLocker::lock();
    this->printThreadId = printThreadId;
    RLocker::unlock();
}

bool RLogger::getAddNewLine() const
{
    bool tmpNewLine;
    RLocker::lock();
    tmpNewLine = this->addNewLine;
    RLocker::unlock();
    return tmpNewLine;
}

void RLogger::setAddNewLine(bool addNewLine)
{
    RLocker::lock();
    this->addNewLine = addNewLine;
    RLocker::unlock();
}

const QString RLogger::getFile() const
{
    QString tmpFileName;
    RLocker::lock();
    tmpFileName = this->logFileName;
    RLocker::unlock();
    return tmpFileName;
}

void RLogger::setFile(const QString & logFileName)
{
    RLocker::lock();
    this->logFileName = logFileName;
    RLocker::unlock();
}

RLogHandler RLogger::getLogHandler() const
{
    RLogHandler tmpHandler;
    RLocker::lock();
    tmpHandler = this->logHandler;
    RLocker::unlock();
    return tmpHandler;
}

void RLogger::setLogHandler(RLogHandler logHandler)
{
    RLocker::lock();
    this->logHandler = logHandler;
    RLocker::unlock();
}

uint RLogger::getIndentLevel() const
{
    uint tmpLevel;
    RLocker::lock();
    tmpLevel = this->indentLevel;
    RLocker::unlock();
    return tmpLevel;
}

void RLogger::setIndentLevel(uint indentLevel)
{
    RLocker::lock();
    this->indentLevel = indentLevel;
    RLocker::unlock();
}

void RLogger::increaseIndent()
{
    RLocker::lock();
    this->indentLevel++;
    RLocker::unlock();
}

void RLogger::decreaseIndent()
{
    RLocker::lock();
    if (this->indentLevel > 0)
    {
        this->indentLevel--;
    }
    RLocker::unlock();
}

void RLogger::printToFile(qint64 pTime, const QString &cppString) const
{
    if (this->logFileName.isEmpty())
    {
        return;
    }

    QFile logFile(this->logFileName);

    if (!logFile.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream(stderr) << "Failed to open the log file '" << this->logFileName << "'\n";
        return;
    }

    QTextStream out(&logFile);

    if (this->printTime)
    {
        out << "[" << RMessage::aTimeToString(pTime) << "] ";
    }
    out << cppString;
    if (out.status() != QTextStream::Ok)
    {
        QTextStream(stderr) << "Failed to write the log message to file '" << this->logFileName << "'\n";
    }

    logFile.close();
}

void RLogger::insertPrefix(const QString &prefix, QString &message)
{
    for (int i=0;i<message.length();i++)
    {
        if (!message.at(i).isSpace())
        {
            message.insert(i,prefix+" ");
            break;
        }
    }
}

void RLogger::print(const RMessage &message)
{
    RMessage::Type messageType = message.getType();

    if (!(messageType & this->getLevel()))
    {
        // Message is out of the log level, so it will be dropped.
        return;
    }

    QString fullMessage;
    bool printToStderr = false;

    for (uint i=0;i<this->getIndentLevel();i++)
    {
        fullMessage += "  ";
    }
    fullMessage += message;
    if (this->getAddNewLine())
    {
        fullMessage += '\n';
    }
    
    for (const QString &prefix : std::as_const(this->prefixStack))
    {
        RLogger::insertPrefix(prefix,fullMessage);
    }

    switch (messageType)
    {
        case RMessage::Type::Error:
            RLogger::insertPrefix("ERROR:",fullMessage);
            printToStderr = true;
            break;
        case RMessage::Type::Warning:
            RLogger::insertPrefix("WARNING:",fullMessage);
            printToStderr = true;
            break;
        case RMessage::Type::Debug:
            RLogger::insertPrefix("DEBUG:",fullMessage);
            printToStderr = false;
            break;
        case RMessage::Type::Trace:
            RLogger::insertPrefix("TRACE:",fullMessage);
            printToStderr = false;
            break;
        case RMessage::Type::Notice:
            RLogger::insertPrefix("NOTICE:",fullMessage);
            printToStderr = false;
            break;
        case RMessage::Type::Info:
        default:
            printToStderr = false;
            break;
    }

    if (this->printThreadId)
    {
        QThread *ptr = QThread::currentThread();
        fullMessage = QString("0x%1").arg((quintptr)ptr, QT_POINTER_SIZE * 2, 16, QChar('0')) + " " + fullMessage;
    }

    if (printToStderr)
    {
        QTextStream(stderr) << fullMessage;
        QTextStream(stderr).flush();
    }
    else
    {
        QTextStream(stdout) << fullMessage;
        QTextStream(stdout).flush();

    }

    if (this->getHalted())
    {
        RLocker::lock();
        // Print to file is halted => store message
        this->messages.push_back(fullMessage);
        RLocker::unlock();
    }
    else
    {
        RLocker::lock();
        this->printToFile(message.getAtime(), fullMessage);
        if (this->logHandler)
        {
            this->logHandler(RMessage(fullMessage, messageType));
        }
        RLocker::unlock();
    }
}

void RLogger::print(const QString &cppString, RMessage::Type messageType)
{
    std::vector<QString> messages = RMessage::explode(cppString,'\n',true);
    for (uint i=0;i<messages.size();i++)
    {
        RMessage message(messages[i]);
        message.setType(messageType);
        this->print(message);
    }
}

void RLogger::print(const char *cString, RMessage::Type  messageType)
{
    this->print(QString(cString),messageType);
}

void RLogger::print(const char cChar, RMessage::Type messageType)
{
    RMessage message(1,cChar);
    message.setType(messageType);
    this->print(message);
}

void RLogger::flush()
{
    std::vector<RMessage>::iterator iter;

    RLocker::lock();
    for (iter = this->messages.begin();
         iter != this->messages.end();
         ++iter)
    {
        this->printToFile(iter->getAtime(), *iter);
        if (this->logHandler)
        {
            this->logHandler(*iter);
        }
    }
    RLocker::unlock();
    this->purge();
}

void RLogger::purge(uint nMessages)
{
    RLocker::lock();
    if (nMessages == 0)
    {
        this->messages.clear();
        RLocker::unlock();
        return;
    }

    uint n;
    std::vector<RMessage>::iterator iter = this->messages.begin();

    n = (uint)this->messages.size();
    if (n > nMessages)
    {
        n = nMessages;
    }

    std::advance(iter,n);

    this->messages.erase(this->messages.begin(),iter);
    RLocker::unlock();
}

int RLogger::trace(const char *format, ...)
{
    RMessage::Type messageType(RMessage::Type::Trace);
    if (!(messageType & RLogger::getInstance().getLevel()))
    {
        return 0;
    }
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), messageType);
    va_end(ap);
    return 0;
}

int RLogger::debug(const char *format, ...)
{
    RMessage::Type messageType(RMessage::Type::Debug);
    if (!(messageType & RLogger::getInstance().getLevel()))
    {
        return 0;
    }
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), messageType);
    va_end(ap);
    return 0;
}

int RLogger::info(const char *format, ...)
{
    RMessage::Type messageType(RMessage::Type::Info);
    if (!(messageType & RLogger::getInstance().getLevel()))
    {
        return 0;
    }
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), messageType);
    va_end(ap);
    return 0;
}

int RLogger::notice(const char *format, ...)
{
    RMessage::Type messageType(RMessage::Type::Notice);
    if (!(messageType & RLogger::getInstance().getLevel()))
    {
        return 0;
    }
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), messageType);
    va_end(ap);
    return 0;
}

int RLogger::warning(const char *format, ...)
{
    RMessage::Type messageType(RMessage::Type::Warning);
    if (!(messageType & RLogger::getInstance().getLevel()))
    {
        return 0;
    }
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), messageType);
    va_end(ap);
    return 0;
}

int RLogger::error(const char *format, ...)
{
    RMessage::Type messageType(RMessage::Type::Error);
    if (!(messageType & RLogger::getInstance().getLevel()))
    {
        return 0;
    }
    va_list ap;
    va_start(ap, format);
    RLogger::getInstance().print(QString::vasprintf(format, ap), messageType);
    va_end(ap);
    return 0;
}

void RLogger::timestamp(const QString prefix)
{
    RLogger::info("%s%s%s\n",
                  prefix.toUtf8().constData(),
                  prefix.size()>0?" ":"",
                  RMessage::aTimeToString(QDateTime::currentMSecsSinceEpoch()).toUtf8().constData());
}

void RLogger::indent()
{
    RLogger::getInstance().timerStack.push_front(QElapsedTimer());
    RLogger::getInstance().timerStack.first().start();
    RLogger::info("{\n");
    RLogger::getInstance().increaseIndent();
}

void RLogger::unindent(bool printTime)
{
    RLogger::getInstance().decreaseIndent();
    if (printTime)
    {
        qint64 elapsed = 0;
        if (!RLogger::getInstance().timerStack.isEmpty())
        {
            elapsed = RLogger::getInstance().timerStack.first().elapsed();
        }
        RLogger::info("} %s\n", QDateTime::fromMSecsSinceEpoch(elapsed).toUTC().toString("hh:mm:ss:zzz").toUtf8().constData());
    }
    else
    {
        RLogger::info("}\n");
    }
    if (!RLogger::getInstance().timerStack.isEmpty())
    {
        RLogger::getInstance().timerStack.pop_front();
    }
}

void RLogger::pushPrefix(const QString &prefix)
{
    RLogger::getInstance().prefixStack.push(prefix);
}

void RLogger::popPrefix()
{
    RLogger::getInstance().prefixStack.pop();
}


static void rloggerQtMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString formatted;
    if (context.category && qstrcmp(context.category, "default") != 0)
    {
        formatted = QString("[Qt:%1] %2").arg(context.category, msg);
    }
    else
    {
        formatted = QString("[Qt] %1").arg(msg);
    }

    switch (type)
    {
        case QtDebugMsg:
            RLogger::debug("%s\n", formatted.toUtf8().constData());
            break;
        case QtInfoMsg:
            RLogger::info("%s\n", formatted.toUtf8().constData());
            break;
        case QtWarningMsg:
            RLogger::warning("%s\n", formatted.toUtf8().constData());
            break;
        case QtCriticalMsg:
        case QtFatalMsg:
            RLogger::error("%s\n", formatted.toUtf8().constData());
            break;
    }
} /* rloggerQtMessageHandler */


void RLogger::installQtMessageHandler()
{
    qInstallMessageHandler(rloggerQtMessageHandler);
}
