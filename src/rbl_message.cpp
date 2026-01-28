#include <QDateTime>
#include <QStringList>
#include <QRegularExpression>

#include "rbl_message.h"
#include "rbl_error.h"

RMessage::RMessage(RMessage::Type type)
{
    this->_init();
    this->setAtimeFrozen(false);
    this->setType(type);
}

RMessage::RMessage(const RMessage &message) : QString(message)
{
    this->_init(&message);
    this->setAtimeFrozen(false);
    this->setType(type);
}

RMessage::RMessage(const QString &cppString, RMessage::Type type) : QString(cppString)
{
    this->_init();
    this->setAtimeFrozen(false);
    this->setType(type);
}

RMessage::RMessage(const char *cString, RMessage::Type type) : QString(cString)
{
    this->_init();
    this->setAtimeFrozen(false);
    this->setType(type);
}

RMessage::RMessage(size_t n, char cChar, RMessage::Type type) : QString(int(n),cChar)
{
    this->_init();
    this->setAtimeFrozen(false);
    this->setType(type);
}

RMessage::~RMessage()
{
}

void RMessage::_init(const RMessage *pMessage)
{
    this->setAtimeToNow();
    if (pMessage)
    {
        if (pMessage->getAtimeFrozen())
        {
            this->aTime = pMessage->getAtime();
        }
        this->setAtimeFrozen(pMessage->getAtimeFrozen());
        this->setType(pMessage->getType());
    }
}

RMessage::Type RMessage::getType() const
{
    return this->type;
}

void RMessage::setType(RMessage::Type type)
{
    R_ERROR_ASSERT(R_MESSAGE_TYPE_IS_VALID(type));

    this->type = type;
}

qint64 RMessage::getAtime() const
{
    return this->aTime;
}

QString RMessage::aTimeToString(qint64 aTime)
{
    return QDateTime::fromMSecsSinceEpoch(aTime).toString("yyyy.MM.dd hh:mm:ss.zzz");
}

void RMessage::setAtimeToNow()
{
    this->aTime = QDateTime::currentMSecsSinceEpoch();
}

RMessage & RMessage::operator =(const RMessage &message)
{
    this->QString::operator =(message);
    this->_init(&message);
    return (*this);
}

RMessage & RMessage::operator =(const QString &cppString)
{
    this->QString::operator =(cppString);
    this->_init();
    return (*this);
}

RMessage & RMessage::operator =(const char *cString)
{
    this->QString::operator =(cString);
    this->_init();
    return (*this);
}

RMessage & RMessage::operator =(char cChar)
{
    this->QString::operator =(cChar);
    this->_init();
    return (*this);
}

std::vector<QString> RMessage::explode(const QString &str, char ch, bool keepCh)
{
    std::vector<QString> result;

    QStringList query = str.split(QRegularExpression(QString("(\\") + ch + ")"));

    for (int i=0;i<query.size();i++)
    {
        if (i == query.size()-1 && query[i].isEmpty() && query.size() > 1)
        {
            continue;
        }
        if (keepCh && query.size() > 1)
        {
            query[i].append(ch);
        }
        result.push_back(query[i]);
    }

    return result;
}
