#include <string>
#include <vector>
#include <algorithm>

#include "rbl_value_vector.h"
#include "rbl_error.h"
#include "rbl_utils.h"

RValueVector::RValueVector ()
{
    this->_init ();
} /* RValueVector::RValueVector */


RValueVector::RValueVector (const RValueVector &valueVector)
{
    this->_init (&valueVector);
} /* RValueVector::RValueVector (copy) */


RValueVector::~RValueVector ()
{
} /* RValueVector::~RValueVector */


void RValueVector::_init (const RValueVector *valueVector)
{
    if (valueVector)
    {
        this->setName (valueVector->getName());
        this->setUnits (valueVector->getUnits());
        this->resize (valueVector->size());
        for (uint i=0;i<this->size();i++)
        {
            (*this)[i] = (*valueVector)[i];
        }
    }
} /* RValueVector::_init */


const QString &RValueVector::getName() const
{
    return this->name;
} /* RValueVector::get_name */


void RValueVector::setName (const QString &name)
{
    this->name = name;
} /* RValueVector::set_name */


const QString &RValueVector::getUnits() const
{
    return this->units;
} /* RValueVector::get_units */


void RValueVector::setUnits (const QString &units)
{
    this->units = units;
} /* RValueVector::set_units */


uint RValueVector::size() const
{
    return (uint)this->values.size();
} /* RValueVector::size */


void RValueVector::resize (uint nmemb)
{
    this->values.resize (nmemb);
} /* RValueVector::resize */


void RValueVector::add (double value)
{
    this->values.push_back (value);
} /* RValueVector::add */


void RValueVector::remove (uint position)
{
    std::vector<double>::iterator iter;

    R_ERROR_ASSERT (position < this->values.size());

    iter = this->values.begin();
    std::advance (iter, position);

    this->values.erase (iter);
}

void RValueVector::remove(const std::vector<uint> &valueBook)
{
    RRVector valuesNew;
    valuesNew.reserve(this->values.size());
    for (uint i=0;i<this->values.size();i++)
    {
        if (valueBook[i] != RConstants::eod)
        {
            valuesNew.push_back(this->values[i]);
        }
    }
    this->values = valuesNew;
} /* RValueVector::remove */


void RValueVector::fill(double value)
{
    std::fill(this->values.begin(),this->values.end(),value);
} /* RValueVector::fill */


RValueVector & RValueVector::operator = (const RValueVector &valueVector)
{
    this->_init (&valueVector);
    return (*this);
} /* RValueVector::operator = */


double RValueVector::operator [] (uint position) const
{
    R_ERROR_ASSERT (position < this->size());

    return this->values[position];
} /* RValueVector::operator [] */


double & RValueVector::operator [] (uint position)
{
    R_ERROR_ASSERT (position < this->size());

    return this->values[position];
} /* RValueVector::operator [] */

