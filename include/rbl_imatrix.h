#ifndef RBL_IMATRIX_H
#define RBL_IMATRIX_H

#include <vector>

#include "rbl_ivector.h"

//! Integer matrix class.
class RIMatrix
{

    protected:

        //! Vector of integer vectors values.
        std::vector<RIVector> array;

    private:

        //! Internal initialization function.
        void _init(const RIMatrix *pMatrix = nullptr);

    public:

        //! Constructor.
        RIMatrix();

        //! Constructor.
        RIMatrix(uint nRows, uint nColumns, int value = int());

        //! Copy constructor.
        RIMatrix(const RIMatrix &matrix);

        //! Destructor.
        ~RIMatrix();

        //! Assignment operator.
        RIMatrix & operator =(const RIMatrix &matrix);

        //! Return number of rows.
        uint getNRows() const;

        //! Return number of coluns.
        uint getNColumns() const;

        //! Resize matrix array.
        void resize(uint nRows, uint nColumns, int value = int());

        //! Return value at given position.
        int getValue(uint row, uint column) const;

        //! Set value at given position.
        void setValue(uint row, uint column, int value);

        //! Transpose matrix.
        void transpose ();

        //! Access operator - return const reference to RIVector.
        const RIVector & operator [](uint row) const;

        //! Access operator - return const reference to RIVector.
        RIVector & operator [](uint row);

        //! Clear matrix.
        void clear();

};

#endif /* RBL_IMATRIX_H */
