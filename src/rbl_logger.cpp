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
} /* RLogger::_init */


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
} /* RLogger::RLogger */


RLogger::RLogger(const RLogger &logger)
{
    this->_init(&logger);
} /* RLogger::RLogger (copy) */


RLogger::~RLogger()
{
} /* RLogger::~RLogger */


RLogger & RLogger::operator =(const RLogger &logger)
{
    this->_init(&logger);
    return (*this);
} /* RLogger::operator = */


RLogger & RLogger::getInstance()
{
    static RLogger logger;
    return logger;
} /* RLogger::getInstance */


RLogLevelMask RLogger::getLevel() const
{
    RLogLevelMask tmpLevel;
    RLocker::lock();
    tmpLevel = this->logLevel;
    RLocker::unlock();
    return tmpLevel;
} /* RLogger::getLevel */


void RLogger::setLevel(RLogLevelMask level)
{
    R_ERROR_ASSERT(R_LOG_LEVEL_IS_VALID(level));

    RLocker::lock();
    this->logLevel = level;
    RLocker::unlock();
} /* RLogger::setLevel */


bool RLogger::getHalted() const
{
    bool tmpHalted;
    RLocker::lock();
    tmpHalted = this->halted;
    RLocker::unlock();
    return tmpHalted;
} /* RLogger::getHalted */


void RLogger::setHalted(bool halt)
{
    RLocker::lock();
    this->halted = halt;
    RLocker::unlock();
    if (!this->getHalted())
    {
        this->flush();
    }
} /* RLogger::setHalted */


bool RLogger::getPrintTimeEnabled() const
{
    bool tmpTime = true;
    RLocker::lock();
    tmpTime = this->printTime;
    RLocker::unlock();
    return tmpTime;
} /* RLogger::getPrintTimeEnabled */


void RLogger::setPrintTimeEnabled(bool printTime)
{
    RLocker::lock();
    this->printTime = printTime;
    RLocker::unlock();
} /* RLogger::setPrintTimeEnabled */


bool RLogger::getPrintThreadIdEnabled() const
{
    return this->printThreadId;
} /* RLogger::getPrintThreadIdEnabled */


void RLogger::setPrintThreadIdEnabled(bool printThreadId)
{
    RLocker::lock();
    this->printThreadId = printThreadId;
    RLocker::unlock();
} /* RLogger::setPrintThreadIdEnabled */


bool RLogger::getAddNewLine() const
{
    bool tmpNewLine;
    RLocker::lock();
    tmpNewLine = this->addNewLine;
    RLocker::unlock();
    return tmpNewLine;
} /* RLogger::getAddNewLine */


void RLogger::setAddNewLine(bool addNewLine)
{
    RLocker::lock();
    this->addNewLine = addNewLine;
    RLocker::unlock();
} /* RLogger::setAddNewLine */


const QString RLogger::getFile() const
{
    QString tmpFileName;
    RLocker::lock();
    tmpFileName = this->logFileName;
    RLocker::unlock();
    return tmpFileName;
} /* RLogger::getFile */


void RLogger::setFile(const QString & logFileName)
{
    RLocker::lock();
    this->logFileName = logFileName;
    RLocker::unlock();
} /* RLogger::setFile */


RLogHandler RLogger::getLogHandler() const
{
    RLogHandler tmpHandler;
    RLocker::lock();
    tmpHandler = this->logHandler;
    RLocker::unlock();
    return tmpHandler;
} /* RLogger::getLogHandler */


void RLogger::setLogHandler(RLogHandler logHandler)
{
    RLocker::lock();
    this->logHandler = logHandler;
    RLocker::unlock();
} /* RLogger::setLogHandler */


uint RLogger::getIndentLevel() const
{
    uint tmpLevel;
    RLocker::lock();
    tmpLevel = this->indentLevel;
    RLocker::unlock();
    return tmpLevel;
} /* RLogger::getIndentLevel */


void RLogger::setIndentLevel(uint indentLevel)
{
    RLocker::lock();
    this->indentLevel = indentLevel;
    RLocker::unlock();
} /* RLogger::setIndentLevel */


void RLogger::increaseIndent()
{
    RLocker::lock();
    this->indentLevel++;
    RLocker::unlock();
} /* RLogger::increaseIndent */


void RLogger::decreaseIndent()
{
    RLocker::lock();
    if (this->indentLevel > 0)
    {
        this->indentLevel--;
    }
    RLocker::unlock();
} /* RLogger::decreaseIndent */


void RLogger::printToFile(time_t pTime, const QString &cppString) const
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
        char buffer [80];
        struct tm timeinfo;
#if defined(_POSIX_SOURCE) || defined(__APPLE__) || defined(__linux__) || defined(__unix__)
        localtime_r(&pTime, &timeinfo);
#else
        localtime_s(&timeinfo, &pTime);
#endif
        strftime(buffer,80,"%Y/%m/%d - %H:%M:%S > ",&timeinfo);
        out << buffer;
    }
    out << cppString;
    if (out.status() != QTextStream::Ok)
    {
        QTextStream(stderr) << "Failed to write the log message to file '" << this->logFileName << "'\n";
    }

    logFile.close();
} /* RLogger::printToFile */


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
} /* RLogger::insertPrefix */


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

    for (const QString &prefix : this->prefixStack)
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
} /* RLogger::print */


void RLogger::print(const QString &cppString, RMessage::Type messageType)
{
    std::vector<QString> messages = RMessage::explode(cppString,'\n',true);
    for (uint i=0;i<messages.size();i++)
    {
        RMessage message(messages[i]);
        message.setType(messageType);
        this->print(message);
    }
} /* RLogger::print */


void RLogger::print(const char *cString, RMessage::Type  messageType)
{
    this->print(QString(cString),messageType);
} /* RLogger::print */


void RLogger::print(const char cChar, RMessage::Type messageType)
{
    RMessage message(1,cChar);
    message.setType(messageType);
    this->print(message);
} /* RLogger::print */


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
} /* RLogger::flush */


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
} /* RLogger::purge */


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
} /* RLogger::trace */


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
} /* RLogger::debug */


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
} /* RLogger::info */


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
} /* RLogger::notice */


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
} /* RLogger::warning */


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
} /* RLogger::error */


void RLogger::timestamp(const QString prefix)
{
    QDateTime timeStamp = QDateTime::currentDateTime();
    RLogger::info("%s%s%s\n",
                  prefix.toUtf8().constData(),
                  prefix.size()>0?" ":"",
                  timeStamp.toString("dd/MM/yyyy hh:mm:ss").toUtf8().constData());
} /* RLogger::timestamp */


void RLogger::indent()
{
    RLogger::getInstance().timerStack.push_front(QElapsedTimer());
    RLogger::getInstance().timerStack.first().start();
    RLogger::info("{\n");
    RLogger::getInstance().increaseIndent();
} /* RLogger::indent */


void RLogger::unindent(bool printTime)
{
    RLogger::getInstance().decreaseIndent();
    if (printTime)
    {
        int elapsed = 0;
        if (!RLogger::getInstance().timerStack.isEmpty())
        {
            elapsed = qRound(double(RLogger::getInstance().timerStack.first().elapsed())/1000.0);
        }
        RLogger::info("} %s\n", QDateTime::fromSecsSinceEpoch(elapsed).toUTC().toString("hh:mm:ss").toUtf8().constData());
    }
    else
    {
        RLogger::info("}\n");
    }
    if (!RLogger::getInstance().timerStack.isEmpty())
    {
        RLogger::getInstance().timerStack.pop_front();
    }
} /* RLogger::unindent */

void RLogger::pushPrefix(const QString &prefix)
{
    RLogger::getInstance().prefixStack.push(prefix);
}

void RLogger::popPrefix()
{
    RLogger::getInstance().prefixStack.pop();
} /* RLogger::popPrefix */


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
} /* RLogger::installQtMessageHandler */
