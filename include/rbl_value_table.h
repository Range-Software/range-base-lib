#ifndef RBL_VALUE_TABLE_H
#define RBL_VALUE_TABLE_H

#include <QString>
#include <map>

//! Value table class
class RValueTable
{

    private:

        //! Internal initialization function.
        void _init(const RValueTable *pValueTable = nullptr);

    protected:

        //! Value table key name.
        QString keyName;
        //! Value table key units.
        QString keyUnits;
        //! Value table value name.
        QString valueName;
        //! Value table value units.
        QString valueUnits;
        //! Table.
        std::map<double,double> table;

    public:

        //! Constructor.
        RValueTable ();

        //! Copy constructor.
        RValueTable(const RValueTable &valueTable);

        //! Destructor.
        ~RValueTable ();

        //! Return table key name.
        const QString & getKeyName() const;

        //! Set table key name.
        void setKeyName(const QString &name);

        //! Return table key units.
        const QString & getKeyUnits() const;

        //! Set table key units.
        void setKeyUnits(const QString &units);

        //! Return table value name.
        const QString & getValueName() const;

        //! Set table value name.
        void setValueName(const QString &name);

        //! Return table value units.
        const QString & getValueUnits() const;

        //! Set table value units.
        void setValueUnits(const QString &units);

        //! Return number of values in the vector.
        uint size() const;

        //! Add new item to the value vector.
        //! If such key already exist it will be replaced by value.
        void add(double key,
                   double value = 0.0);

        //! Remove item specified by given key from the value vector.
        void remove(double key);

        //! Remove item at given position from the value vector.
        void remove(uint position);

        //! Return value for a given key.
        //! If such key is not fund an interpolated value is returned.
        double get(double key) const;

        //! Return key at given position.
        double getKey(uint position) const;

        //! Return value at given position.
        double getValue(uint position) const;

        //! Drop all keys and values
        inline void clear()
        {
            this->table.clear();
        }

        //! Test if table is empty.
        inline bool empty() const
        {
            return this->table.empty();
        }

        //! Assignment operator.
        RValueTable & operator =(const RValueTable &valueTable);

        //! Equal operator.
        bool operator ==(const RValueTable &valueTable) const;

        //! Not equal operator.
        bool operator !=(const RValueTable &valueTable) const;

};

#endif /* RBL_VALUE_TABLE_H */
