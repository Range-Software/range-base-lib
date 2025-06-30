#ifndef RBL_VERSION_H
#define RBL_VERSION_H

#include <QString>

class RVersion
{

    protected:

        //! Major number.
        uint mMajor;
        //! Minor number.
        uint mMinor;
        //! Release number.
        uint mRelease;

    private:

        //! Internal initialization function.
        void _init(const RVersion *pVersion = nullptr );

    public:

        //! Constructor.
        RVersion();

        //! Constructor.
        RVersion(uint mMajor, uint mMinor, uint mRelease);

        //! Constructor.
        RVersion(const QString &versionStr, char delimiter = '.');

        //! Copy constructor.
        RVersion(const RVersion &version);

        //! Destructor.
        ~RVersion();

        //! Return true is version is valid (0.0.0 will return false)
        inline bool isValid() const
        {
            return (this->mMajor != 0 || this->mMinor != 0 || this->mRelease != 0);
        }

        //! Convert version to string.
        QString toString(char delimiter = '.') const;

        static RVersion fromString(const QString &versionStr, char delimiter = '.');

        //! Assignment operator.
        RVersion & operator =(const RVersion &version);

        //! Equal to operator.
        bool operator ==(const RVersion& version) const;

        //! Not equal to operator.
        bool operator !=(const RVersion& version) const;

        //! Greater than operator.
        bool operator >(const RVersion& version) const;

        //! Less than operator.
        bool operator <(const RVersion& version) const;

        //! Greater than or equal to operator.
        bool operator >=(const RVersion& version) const;

        //! Less than or equal to operator.
        bool operator <=(const RVersion& version) const;

        //! Return major number.
        uint getMajor() const;

        //! Return minor number.
        uint getMinor() const;

        //! Return release number.
        uint getRelease() const;

};

#endif /* RBL_VERSION_H */
