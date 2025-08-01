#ifndef RBL_ARGUMENT_OPTION_H
#define RBL_ARGUMENT_OPTION_H

#include <QString>
#include <QVariant>

class RArgumentOption
{

    public:

        //! Option type.
        enum Type
        {
            None = 0,
            Switch,
            Char,
            String,
            StringList,
            Path,
            Integer,
            Real,
            Date
        };

        //! Option category
        enum Category
        {
            Mandatory = 0,
            File,
            Action,
            Optional,
            Logger,
            Help
        };

    protected:

        //! Flag.
        QString flag;
        //! Type.
        Type type;
        //! Value.
        QVariant value;
        //! Description.
        QString description;
        //! Category.
        Category category;
        //! Exclusive flag.
        bool exclusive;

    private:

        //! Internal initialization function.
        void _init(const RArgumentOption *pArgumentOption = nullptr);

    public:

        //! Constructor.
        RArgumentOption();

        //! Constructor.
        RArgumentOption(const QString &flag, Type type, const QVariant &value, const QString &description, Category category, bool exclusive);

        //! Copy constructor.
        RArgumentOption(const RArgumentOption &argumentOption);

        //! Destructor.
        ~RArgumentOption();

        //! Assignment operator.
        RArgumentOption &operator =(const RArgumentOption &argumentOption);

        //! Return const reference to flag.
        const QString &getFlag() const;

        //! Set flag.
        void setFlag(const QString &flag);

        //! Return type.
        Type getType() const;

        //! Set type.
        void setType(Type type);

        //! Return const reference to value.
        const QVariant &getValue() const;

        //! Set value.
        void setValue(const QVariant &value);

        //! Return const reference to description.
        const QString &getDescription() const;

        //! Set description.
        void setDescription(const QString &description);

        //! Return category.
        Category getCategory() const;

        //! Set category.
        void setCategory(Category category);

        //! Return exclusive.
        bool getExclusive() const;

        //! Set exclusive.
        void setExclusive(bool exclusive);

        //! Generate flag message.
        QString getFlagMessage() const;

        //! Print help.
        QString getHelpMessage(uint width=0) const;

        //! Generate help option.
        static RArgumentOption generateHelpOption();

        //! Generate version option.
        static RArgumentOption generateVersionOption();

        //! Check if argument is option.
        static bool isOption(const QString &argument);

};

#endif // RBL_ARGUMENT_OPTION_H
