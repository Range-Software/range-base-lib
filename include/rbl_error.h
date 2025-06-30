#ifndef RBL_ERROR_H
#define RBL_ERROR_H

#include <QString>

#define R_ERROR_IS_VALID(_errorType) \
(_errorType >= RError::Type::None && _errorType < RError::Type::nTypes)

#define R_ERROR_REF __FILE__,__LINE__

#ifdef DEBUG
#define R_ERROR_ASSERT(_cond) assert(_cond)
#else
#define R_ERROR_ASSERT(_cond) {if (!(_cond)) throw RError(RError::Type::Application,R_ERROR_REF,"Condition failed \'%s\'.",#_cond);}
#endif

//! Error class.
class RError
{

    public:

        enum Type
        {
            None = 0,
            OpenFile,
            ReadFile,
            WriteFile,
            RemoveFile,
            RenameFile,
            InvalidFileName,
            InvalidFileFormat,
            OpenDir,
            ReadDir,
            Application,
            ChildProcess,
            InvalidInput,
            NotFound,
            Connection,
            Unauthorized,
            Unknown,
            nTypes
        };

    private:

        //! Internal initialization function.
        void _init(const RError *pError = nullptr);

    protected:

        //! Error type.
        Type type;

        //! Error message.
        QString message;

    public:

        //! Constructor.
        RError(Type type, const char *file, uint line, const QString &message);

        //! Constructor.
        RError(Type type, const char *file, uint line, const char *format, ...);

        //! Copy constructor.
        RError(const RError &error);

        //! Destructor.
        ~RError();

        //! Assignment operator.
        RError & operator =(const RError &error);

        //! Return error type.
        Type getType() const;

        //! Set error type.
        //! By setting error type an appropriate message will be
        //! set as well.
        void setType(Type type);

        //! Return error message.
        QString getMessage() const;

        //! Set error message.
        void setMessage(QString message);

        static const QString & getTypeMessage(RError::Type type);

};

#endif /* RBL_ERROR_H */
