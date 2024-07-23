#include "rbl_argument_option.h"
#include "rbl_utils.h"

void RArgumentOption::_init(const RArgumentOption *pArgumentOption)
{
    if (pArgumentOption)
    {
        this->flag = pArgumentOption->flag;
        this->type = pArgumentOption->type;
        this->value = pArgumentOption->value;
        this->description = pArgumentOption->description;
        this->category = pArgumentOption->category;
        this->exclusive = pArgumentOption->exclusive;
    }
}

RArgumentOption::RArgumentOption()
    : type(None)
{
    this->_init();
}

RArgumentOption::RArgumentOption(const QString &flag, RArgumentOption::Type type, const QVariant &value, const QString &description, RArgumentOption::Category category, bool exclusive)
    : flag(flag)
    , type(type)
    , value(value)
    , description(description)
    , category(category)
    , exclusive(exclusive)
{
    this->_init();
}

RArgumentOption::RArgumentOption(const RArgumentOption &argumentOption)
{
    this->_init(&argumentOption);
}

RArgumentOption::~RArgumentOption()
{

}

RArgumentOption &RArgumentOption::operator =(const RArgumentOption &argumentOption)
{
    this->_init(&argumentOption);
    return (*this);
}

const QString &RArgumentOption::getFlag() const
{
    return this->flag;
}

void RArgumentOption::setFlag(const QString &flag)
{
    this->flag = flag;
}

RArgumentOption::Type RArgumentOption::getType() const
{
    return this->type;
}

void RArgumentOption::setType(RArgumentOption::Type type)
{
    this->type = type;
}

const QVariant &RArgumentOption::getValue() const
{
    return this->value;
}

void RArgumentOption::setValue(const QVariant &value)
{
    this->value = value;
}

const QString &RArgumentOption::getDescription() const
{
    return this->description;
}

void RArgumentOption::setDescription(const QString &description)
{
    this->description = description;
}

RArgumentOption::Category RArgumentOption::getCategory() const
{
    return this->category;
}

void RArgumentOption::setCategory(RArgumentOption::Category category)
{
    this->category = category;
}

bool RArgumentOption::getExclusive() const
{
    return this->exclusive;
}

void RArgumentOption::setExclusive(bool exclusive)
{
    this->exclusive = exclusive;
}

QString RArgumentOption::getFlagMessage() const
{
    QString message;
    message += "--" + this->flag;
    switch (this->type)
    {
        case Char:
        {
            message += "=<C>";
            break;
        }
        case String:
        {
            message += "=<STRING>";
            break;
        }
        case StringList:
        {
            message += "=<STRING1[,STRING2,...]>";
            break;
        }
        case Path:
        {
            message += "=<PATH>";
            break;
        }
        case Integer:
        {
            message += "=<N>";
            break;
        }
        case Real:
        {
            message += "=<REAL NUMBER>";
            break;
        }
        case Date:
        {
            message += "=<DD.MM.YYYY>";
            break;
        }
        default:
        {
            break;
        }
    }
    return message;
}

QString RArgumentOption::getHelpMessage(uint width) const
{
    QString message = this->getFlagMessage();
    message += " ";
    for (uint i=uint(message.length());i<width;i++)
    {
        message += " ";
    }
    message += this->description;
    if (this->type != Switch && !this->value.isNull())
    {
        message += " (default=" + this->value.toString() + ")";
    }
    return message;
}

RArgumentOption RArgumentOption::generateHelpOption()
{
    return RArgumentOption("help",Switch,QVariant(false),"Print help",Help,true);
}

RArgumentOption RArgumentOption::generateVersionOption()
{
    return RArgumentOption("version",Switch,QVariant(RVendor::version.toString()),"Print version",Help,true);
}

bool RArgumentOption::isOption(const QString &argument)
{
    if (argument.size() == 2)
    {
        if (argument[0] == '-' && argument[1] != '-')
        {
            return true;
        }
    }
    else if (argument.size() >= 4)
    {
        if (argument[0] == '-' && argument[1] == '-')
        {
            return true;
        }
    }
    return false;
}
