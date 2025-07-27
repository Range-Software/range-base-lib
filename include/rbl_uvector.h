#ifndef RBL_UVECTOR_H
#define RBL_UVECTOR_H

#include <QtCore/QtGlobal>
#include <vector>

//! Unsigned integer vector class.
class RUVector : public std::vector<uint>
{

    private:

        //! Internal initialization function.
        void _init(const RUVector *pArray = nullptr);

    public:

        //! Constructor.
        RUVector(uint nRows = 0, uint value = 0);

        //! Copy constructor.
        RUVector(const RUVector &array);

        //! Destructor.
        ~RUVector();

        //! Assignment operator.
        RUVector & operator =(const RUVector &array);

        //! Return number of rows (size of the vector).
        inline uint getNRows() const
        {
            return (uint)this->size();
        }

        //! Access element operator.
        uint & operator[](uint n);

        //! Access element operator.
        const uint & operator[](uint n) const;

        //! Fill vector with specified value.
        void fill(uint value);

        //! Print content.
        void print(bool oneLine = false, bool newLine = false) const;
};

#endif /* RBL_UVECTOR_H */
