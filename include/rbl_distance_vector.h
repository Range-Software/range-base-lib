#ifndef RBL_DISTANCE_VECTOR_H
#define RBL_DISTANCE_VECTOR_H

#include <QtCore/QtGlobal>
#include <vector>
#include <set>
#include <algorithm>

template <class T>
class RDistanceVector : public std::vector<T>
{

    private:

        //! Internal initialization function.
        void _init(const RDistanceVector *pDistanceVector = nullptr)
        {
            if (pDistanceVector)
            {

            }
        }

    public:

        //! Constructor.
        RDistanceVector(uint n = 0, const T &value = T())
            : std::vector<T>(n,value)
        {
            this->_init();
        }

        //! Copy constructor.
        RDistanceVector(const RDistanceVector &distanceVector)
            : std::vector<T>(distanceVector)
        {
            this->_init(&distanceVector);
        }

        //! Destructor.
        ~RDistanceVector()
        {

        }

        //! Assignment operator.
        RDistanceVector &operator =(const RDistanceVector &distanceVector)
        {
            this->std::vector<T>::operator=(distanceVector);
            this->_init(&distanceVector);
            return (*this);
        }

        //! Return set of distances.
        std::set<T> getDistances() const
        {
            std::set<T> distances;

            for (uint i=0;i<this->size();i++)
            {
                distances.insert(this->at(i));
            }

            return distances;
        }

        //! Return vector of positions with given distance.
        std::vector<uint> getDistancePositions(const T &distance) const
        {
            std::vector<uint> positions;

            for (uint i=0;i<this->size();i++)
            {
                if (distance == this->at(i))
                {
                    positions.push_back(i);
                }
            }

            return positions;
        }

        //! Find minimum distance.
        T findMinimumDistance() const
        {
            if (this->size() == 0)
            {
                return T();
            }

            uint minPosition = 0;

            for (uint i=1;i<this->size();i++)
            {
                if (this->at(minPosition) > this->at(i))
                {
                    minPosition = i;
                }
            }

            return this->at(minPosition);
        }

        //! Find maximum distance.
        T findMaximumDistance() const
        {
            if (this->size() == 0)
            {
                return T();
            }

            uint maxPosition = 0;

            for (uint i=1;i<this->size();i++)
            {
                if (this->at(maxPosition) < this->at(i))
                {
                    maxPosition = i;
                }
            }

            return this->at(maxPosition);
        }

        //! Fill with values.
        void fill(const T &value)
        {
            std::fill(this->begin(),this->end(),value);
        }

};

#endif // RBL_DISTANCE_VECTOR_H
