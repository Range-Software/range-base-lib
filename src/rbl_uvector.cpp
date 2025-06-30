#include <algorithm>

#include "rbl_uvector.h"
#include "rbl_error.h"
#include "rbl_logger.h"


RUVector::RUVector (uint nRows, uint value)
{
    this->resize(nRows,value);
    this->_init();
} /* RUVector::RUVector */


RUVector::RUVector (const RUVector &array) : std::vector<uint>(array)
{
    this->_init(&array);
} /* RUVector::RUVector (copy) */


RUVector::~RUVector ()
{
} /* RUVector::~RUVector */


RUVector & RUVector::operator = (const RUVector &array)
{
    this->std::vector<uint>::operator = (array);
    this->_init(&array);
    return (*this);
} /* RUVector::operator = */


void RUVector::_init(const RUVector *pArray)
{
    if (pArray)
    {
    }
} /* RUVector::_init */


uint & RUVector::operator [] (uint n)
{
    R_ERROR_ASSERT(n<this->getNRows());

    return this->at(n);
} /* RUVector::operator [] */


const uint & RUVector::operator [] (uint n) const
{
    R_ERROR_ASSERT(n<this->getNRows());

    return this->at(n);
} /* RUVector::operator [] */

void RUVector::fill(uint value)
{
    std::fill(this->begin(),this->end(),value);
}

void RUVector::print(bool oneLine, bool newLine) const
{
    if (!oneLine)
    {
        RLogger::info("Vector - uint: [%u]\n",this->size());
    }
    for (uint i=0;i<this->size();i++)
    {
        RLogger::info("%15u",this->at(i));
        if (!oneLine)
        {
            RLogger::info("\n");
        }
    }
    if (newLine)
    {
        RLogger::info("\n");
    }
} /* RUVector::fill */
