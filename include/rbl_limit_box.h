#ifndef RBL_BOX_H
#define RBL_BOX_H

class RLimitBox
{

    protected:

        //! X limits.
        double xl, xu;
        //! Y limits.
        double yl, yu;
        //! Z limits.
        double zl, zu;

    private:

        //! Internal initialization function.
        void _init(const RLimitBox *pBox = nullptr);

    public:

        //! Constructor.
        RLimitBox();

        //! Constructor.
        RLimitBox(double xl, double xu, double yl, double yu, double zl, double zu);

        //! Copy constructor.
        RLimitBox(const RLimitBox &box);

        //! Destructor.
        ~RLimitBox();

        //! Assignment operator.
        RLimitBox &operator =(const RLimitBox &box);

        //! Set limits.
        void setLimits(double xl, double xu, double yl, double yu, double zl, double zu);

        //! Get limits.
        void getLimits(double &xl, double &xu, double &yl, double &yu, double &zl, double &zu) const;

        //! Scale limit box by factor.
        void scale(double scaleFactor);

        //! Print limit box.
        void print() const;

        //! Merge in limit box.
        void merge(const RLimitBox &lb);

        //! Return true if two boxes intersect.
        static bool areIntersecting(const RLimitBox &b1, const RLimitBox &b2);

    protected:

        //! Fix/sort limits.
        void fixLimits();

};

#endif // RBL_BOX_H
