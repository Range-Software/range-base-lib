#include <QStringList>

#include "rbl_version.h"

void RVersion::_init(const RVersion *pVersion)
{
    if (pVersion)
    {
        this->mMajor = pVersion->mMajor;
        this->mMinor = pVersion->mMinor;
        this->mRelease = pVersion->mRelease;
    }
}

RVersion::RVersion()
    : mMajor{0}
    , mMinor{0}
    , mRelease{0}
{
    this->_init();
}

RVersion::RVersion(uint major, uint minor, uint release)
    : mMajor{major}
    , mMinor{minor}
    , mRelease{release}
{
    this->_init();
}

RVersion::RVersion(const QString &versionStr, char delimiter)
    : mMajor{0}
    , mMinor{0}
    , mRelease{0}
{
    this->_init();

    QStringList query = versionStr.split(QChar(delimiter));

    if (query.size() > 0)
    {
        this->mMajor = query[0].toUInt();
    }
    if (query.size() > 1)
    {
        this->mMinor = query[1].toUInt();
    }
    if (query.size() > 2)
    {
        this->mRelease = query[2].toUInt();
    }
}

RVersion::RVersion(const RVersion &version)
{
    this->_init(&version);
}

RVersion::~RVersion()
{
}

QString RVersion::toString(char delimiter) const
{
    QString versionStr;

    versionStr = QString::number(this->mMajor)
               + delimiter
               + QString::number(this->mMinor)
               + delimiter
               + QString::number(this->mRelease);
    return versionStr;
}

RVersion RVersion::fromString(const QString &versionStr, char delimiter)
{
    return RVersion(versionStr, delimiter);
}

RVersion &RVersion::operator =(const RVersion &version)
{
    this->_init(&version);
    return (*this);
}

bool RVersion::operator ==(const RVersion &version) const
{
    return (this->mMajor == version.mMajor &&
            this->mMinor == version.mMinor &&
            this->mRelease == version.mRelease);
}

bool RVersion::operator !=(const RVersion &version) const
{
    return !((*this)==version);
}

bool RVersion::operator >(const RVersion &version) const
{
    if (this->mMajor > version.mMajor)
    {
        return true;
    }
    else if (this->mMajor == version.mMajor)
    {
        if (this->mMinor > version.mMinor)
        {
            return true;
        }
        else if (this->mMinor == version.mMinor)
        {
            if (this->mRelease > version.mRelease)
            {
                return true;
            }
        }
    }
    return false;
}

bool RVersion::operator <(const RVersion &version) const
{
    if (this->mMajor < version.mMajor)
    {
        return true;
    }
    else if (this->mMajor == version.mMajor)
    {
        if (this->mMinor < version.mMinor)
        {
            return true;
        }
        else if (this->mMinor == version.mMinor)
        {
            if (this->mRelease < version.mRelease)
            {
                return true;
            }
        }
    }
    return false;
}

bool RVersion::operator >=(const RVersion &version) const
{
    if (this->mMajor > version.mMajor)
    {
        return true;
    }
    else if (this->mMajor == version.mMajor)
    {
        if (this->mMinor > version.mMinor)
        {
            return true;
        }
        else if (this->mMinor == version.mMinor)
        {
            if (this->mRelease >= version.mRelease)
            {
                return true;
            }
        }
    }
    return false;
}

bool RVersion::operator <=(const RVersion &version) const
{
    if (this->mMajor < version.mMajor)
    {
        return true;
    }
    else if (this->mMajor == version.mMajor)
    {
        if (this->mMinor < version.mMinor)
        {
            return true;
        }
        else if (this->mMinor == version.mMinor)
        {
            if (this->mRelease <= version.mRelease)
            {
                return true;
            }
        }
    }
    return false;
}

uint RVersion::getMajor() const
{
    return this->mMajor;
}

uint RVersion::getMinor() const
{
    return this->mMinor;
}

uint RVersion::getRelease() const
{
    return this->mRelease;
}
