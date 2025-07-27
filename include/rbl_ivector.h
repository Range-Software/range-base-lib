#ifndef RBL_IVECTOR_H
#define RBL_IVECTOR_H

#include <QtCore/QtGlobal>
#include <vector>

//! Vector class.
class RIVector : public std::vector<int>
{

    private:

        //! Internal initialization function.
        void _init(const RIVector *pArray = nullptr);

    public:

        //! Constructor.
        RIVector(uint nRows = 0, int value = int());

        //! Copy constructor.
        RIVector(const RIVector &array);

        //! Destructor.
        ~RIVector();

        //! Assignment operator.
        RIVector & operator =(const RIVector &array);

        //! Return number of rows (size of the vector).
        inline uint getNRows() const
        {
            return (uint)this->size();
        }

        //! Access element operator.
        int & operator[](uint n);

        //! Access element operator.
        const int & operator[](uint n) const;

        //! Fill vector with specified value.
        void fill(int value);

        //! Print content.
        void print(bool oneLine = false, bool newLine = false) const;
};

#endif /* RBL_IVECTOR_H */
