#ifndef RBL_VALUE_VECTOR_H
#define RBL_VALUE_VECTOR_H

#include <QString>
#include "rbl_rvector.h"

//! Value vector class.
class RValueVector
{

    private:

        //! Internal initialization function.
        void _init(const RValueVector *valueVector = nullptr);

    protected:

        //! Value name.
        QString name;
        //! Value units.
        QString units;
        //! Values.
        RRVector values;

    public:

        //! Constructor.
        RValueVector ();

        //! Copy constructor.
        RValueVector(const RValueVector &valueVector);

        //! Destructor.
        ~RValueVector ();

        //! Return value name.
        const QString & getName() const;

        //! Set value name.
        void setName(const QString &name);

        //! Return value units.
        const QString & getUnits() const;

        //! Set value units.
        void setUnits(const QString &units);

        //! Return number of values in the vector.
        uint size() const;

        //! Resize value vector.
        void resize(uint nmemb);

        //! Add new item to the value vector.
        void add(double value = 0);

        //! Remove item at given position from the value vector.
        void remove(uint position);

        //! Remove item at given position from the value vector.
        //! If valueBook[i] == RConstants::eod then value will be removed.
        void remove(const std::vector<uint> &valueBook);

        //! Fill values with given value.
        void fill(double value);

        //! Drop all values
        inline void clear()
        {
            this->values.clear();
        }

        //! Test if value vector is empty.
        inline bool empty() const
        {
            return this->values.empty();
        }

        //! Assignment operator.
        RValueVector & operator =(const RValueVector &valueVector);

        //! Return value at given position.
        double operator [](uint position) const;

        //! Set value at given position.
        double & operator [](uint position);

        //! Return const reference to data vecor.
        inline const RRVector & getDataVector() const
        {
            return this->values;
        }

        //! Return reference to data vecor.
        inline RRVector & getDataVector()
        {
            return this->values;
        }

};

#endif /* RBL_VALUE_VECTOR_H */
