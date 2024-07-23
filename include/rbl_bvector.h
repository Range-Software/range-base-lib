#ifndef RBL_BVECTOR_H
#define RBL_BVECTOR_H

#include <QtGlobal>
#include <vector>

//! Vector class.
class RBVector : public std::vector<bool>
{

    private:

        //! Internal initialization function.
        void _init(const RBVector *pArray = nullptr);

    public:

        //! Constructor.
        RBVector(uint nRows = 0, bool value = bool());

        //! Copy constructor.
        RBVector(const RBVector &array);

        //! Destructor.
        ~RBVector();

        //! Assignment operator.
        RBVector & operator =(const RBVector &array);

        //! Return number of rows (size of the vector).
        inline uint getNRows() const
        {
            return (uint)this->size();
        }

        //! Fill vector with specified value.
        void fill(bool value);

        //! Print content.
        void print(bool oneLine = false, bool newLine = false) const;
};

#endif /* RBL_BVECTOR_H */
