#ifdef DEBUG
#  include <assert.h>
#endif

#include "rbl_error.h"


const QString errorTypeMessages [RError::nTypes] =
{
    "No error.",
    "Failed to open the file.",
    "Failed to read the file.",
    "Failed to write the file.",
    "Failed to remove the file.",
    "Failed to rename the file.",
    "Invalid file name.",
    "Invalid file format.",
    "Failed to open the directory.",
    "Failed to read the directory.",
    "Application error.",
    "Child process error.",
    "Invalid input.",
    "Not found.",
    "Connection error.",
    "Unauthorized.",
    "Unknown error."
};


RError::RError(RError::Type type, const char *file, uint line, const QString &message)
{
    this->_init();
    this->setType(type);
    QString devInfo;
#ifdef DEBUG
    devInfo = QString(file) + "@" + QString::number(line) + ":";
#else
    // Suppress unused parameter warnings
    (void)file;
    (void)line;
#endif
    this->setMessage(this->getMessage() + " " + devInfo + message);
} /* RError::RError */


RError::RError(RError::Type type, const char *file, uint line, const char *format, ...)
{
    QString buffer;

#ifdef DEBUG
    buffer = QString::asprintf("%s@%u: ",file,line);
#else
    // Suppress unused parameter warnings
    (void)file;
    (void)line;
#endif

    va_list ap;
    va_start(ap, format);
    buffer += QString::vasprintf(format,ap);
    va_end(ap);
    this->_init();
    this->setType(type);
    this->setMessage(this->getMessage() + " " + buffer);
} /* RError::RError */


RError::RError(const RError &error)
{
    this->_init(&error);
} /* RError::RError(copy) */


RError::~RError()
{
} /* RError::~RError */


void RError::_init(const RError *pError)
{
    if (pError)
    {
        this->type = pError->type;
        this->message = pError->message;
    }
} /* RError::_init */


RError & RError::operator =(const RError &error)
{
    this->_init(&error);
    return (*this);
} /* RError::operator = */


RError::Type RError::getType() const
{
    return this->type;
} /* RError::getType */


void RError::setType(RError::Type type)
{
#ifdef DEBUG
    assert(R_ERROR_IS_VALID(type));
#endif
    this->type = type;
    this->setMessage(RError::getTypeMessage(this->type));
} /* RError::setType */


QString RError::getMessage() const
{
    return this->message;
} /* RError::getMessage */


void RError::setMessage(QString message)
{
    this->message = message;
} /* RError::setMessage */


const QString & RError::getTypeMessage(RError::Type type)
{
#ifdef DEBUG
    assert(R_ERROR_IS_VALID(type));
#endif
    return errorTypeMessages[type];
} /* RError::getTypeMessage */
