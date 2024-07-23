#include "rbl_imatrix.h"
#include "rbl_error.h"


RIMatrix::RIMatrix()
{
    this->_init();
} /* RIMatrix::RIMatrix */


RIMatrix::RIMatrix(uint nRows,
                   uint nColumns, int value)
{
    this->_init();
    this->resize(nRows,nColumns,value);
} /* RIMatrix::RIMatrix */


RIMatrix::RIMatrix(const RIMatrix &matrix)
{
    this->_init(&matrix);
} /* RIMatrix::RIMatrix (copy) */


RIMatrix::~RIMatrix()
{
} /* RIMatrix::~RIMatrix */


RIMatrix & RIMatrix::operator =(const RIMatrix &matrix)
{
    this->_init(&matrix);
    return (*this);
} /* RIMatrix::operator = */


void RIMatrix::_init(const RIMatrix *pMatrix)
{
    if (pMatrix)
    {
        this->resize(pMatrix->getNRows(),pMatrix->getNColumns());
        for (uint i=0;i<pMatrix->getNRows();i++)
        {
            for (uint j=0;j<pMatrix->getNColumns();j++)
            {
                this->setValue(i,j,pMatrix->getValue(i,j));
            }
        }
    }
} /* RIMatrix::_init */


uint RIMatrix::getNRows() const
{
    return (uint)this->array.size();
} /* RIMatrix::getNRows */


uint RIMatrix::getNColumns() const
{
    if (this->getNRows() > 0)
    {
        return (uint)this->array[0].size();
    }
    else
    {
        return 0;
    }
} /* RIMatrix::getNColumns */


void RIMatrix::resize(uint nRows, uint nColumns, int value)
{
    this->array.resize(nRows);
    for (uint i=0;i<nRows;i++)
    {
        this->array[i].resize(nColumns,value);
    }
} /* RIMatrix::resize */


int RIMatrix::getValue(uint row, uint column) const
{
    R_ERROR_ASSERT(row < this->getNRows());
    R_ERROR_ASSERT(column < this->getNColumns());

    return this->array[row][column];
} /* RIMatrix::getValue */


void RIMatrix::setValue(uint row, uint column, int value)
{
    R_ERROR_ASSERT(row < this->getNRows());
    R_ERROR_ASSERT(column < this->getNColumns());

    this->array[row][column] = value;
} /* RIMatrix::setValue */


void RIMatrix::transpose()
{
    uint nr = this->getNRows();
    uint nc = this->getNColumns();

    if (nr == nc)
    {
        int itmp;
        for (uint i=0;i<nr;i++)
        {
            for (uint j=0;j<nc;j++)
            {
                if (i == j)
                {
                    continue;
                }
                itmp = this->getValue(j,i);
                this->setValue(j,i,this->getValue(i,j));
                this->setValue(i,j,itmp);
            }
        }
    }
    else
    {
        RIMatrix itmp(*this);

        this->resize(nc,nr);
        for (uint i=0;i<nr;i++)
        {
            for (uint j=0;j<nc;j++)
            {
                if (i == j)
                {
                    continue;
                }
                this->setValue(j,i,itmp.getValue(i,j));
            }
        }
    }
} /* RIMatrix::transpose */


const RIVector & RIMatrix::operator [](uint row) const
{
    R_ERROR_ASSERT (row < this->getNRows());

    return this->array[row];
} /* RIMatrix::operator [] */


RIVector & RIMatrix::operator [](uint row)
{
    R_ERROR_ASSERT (row < this->getNRows());

    return this->array[row];
} /* RIMatrix::operator [] */


void RIMatrix::clear()
{
    for (uint i=0;i<this->getNRows();i++)
    {
        this->array[i].clear();
    }
    this->array.clear();
} /* RIMatrix::clear */
