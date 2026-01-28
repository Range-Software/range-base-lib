#ifndef RBL_MESSAGE_H
#define RBL_MESSAGE_H

#include <QString>

#include <vector>

#define R_MESSAGE_TYPE_IS_VALID(_type) \
( \
  _type == RMessage::Type::Info    || \
  _type == RMessage::Type::Notice  || \
  _type == RMessage::Type::Error   || \
  _type == RMessage::Type::Warning || \
  _type == RMessage::Type::Debug   || \
  _type == RMessage::Type::Trace      \
)

//! Message class.
class RMessage : public QString
{

    public:

        enum Type
        {
            None    = 1 << 0,
            Info    = 1 << 1,
            Notice  = 1 << 2,
            Error   = 1 << 3,
            Warning = 1 << 4,
            Debug   = 1 << 5,
            Trace   = 1 << 6
        };

    private:

        //! Internal initialization function.
        void _init(const RMessage *pMessage = nullptr);

    protected:

        //! Message type.
        Type type;
        //! Message assignment time.
        //! Value of this time variable is set by constructors and
        //! changed whenever new value is set using assignment
        //! operator.
        qint64 aTime;
        //! Indicates whether the assignment time is frozen or not.
        bool aTimeFrozen;

    public:

        //! Constructor.
        RMessage(RMessage::Type type = RMessage::Type::Info);

        //! Copy constructor (message).
        RMessage(const RMessage &message);

        //! Copy constructor (c++ string).
        RMessage(const QString &cppString, RMessage::Type type = RMessage::Type::Info);

        //! Copy constructor (c string).
        RMessage(const char *cString, RMessage::Type type = RMessage::Type::Info);

        //! Copy constructor (c char).
        RMessage(size_t n, char cChar, RMessage::Type type = RMessage::Type::Info);

        //! Destructor.
        ~RMessage ();

        //! Get message type.
        RMessage::Type getType() const;

        //! Set message type
        void setType(RMessage::Type type);

        //! Return message assignment time.
        qint64 getAtime() const;

        //! Return message assignment time.
        static QString aTimeToString(qint64 aTime);

        //! Set message assignment time to current time.
        void setAtimeToNow();

        //! Return whether the modification of assignment time is enabled.
        inline bool getAtimeFrozen() const
        {
            return this->aTimeFrozen;
        }

        //! Disable modification of assignment time.
        inline void setAtimeFrozen(bool frozen)
        {
            this->aTimeFrozen = frozen;
        }

        //! Assignment operator (message).
        RMessage & operator=(const RMessage &message);

        //! Assignment operator (c++ string).
        RMessage & operator =(const QString &cppString);

        //! Assignment operator (c string).
        RMessage & operator =(const char *cString);

        //! Assignment operator (c char).
        RMessage & operator =(char cChar);

        //! Separate string into vector of strings.
        static std::vector<QString> explode(const QString &str, char ch, bool keepCh = false);

};

#endif /* RBL_MESSAGE_H */
