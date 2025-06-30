#ifndef RBL_APPLICATION_STATE_H
#define RBL_APPLICATION_STATE_H

typedef enum _RApplicationStateType
{
    R_APPLICATION_STATE_NONE = 0,
    R_APPLICATION_STATE_INIT,
    R_APPLICATION_STATE_RUN,
    R_APPLICATION_STATE_STOP,
    R_APPLICATION_STATE_N_TYPES
} RApplicationStateType;

class RApplicationState
{

    protected:

        //! Application state type.
        RApplicationStateType stateType;

    private:

        //! Internal initialization function.
        void _init(const RApplicationState *pApplicationState = nullptr);

    public:

        //! Constructor.
        RApplicationState();

        //! Copy constructor.
        RApplicationState(const RApplicationState &applicationState);

        //! Destructor.
        ~RApplicationState();

        //! Assignment operator.
        RApplicationState & operator =(const RApplicationState &applicationState);

        //! Return static instance to application state.
        static RApplicationState & getInstance();

        //! Return application state type.
        RApplicationStateType getStateType() const;

        //! Set application state type.
        void setStateType(RApplicationStateType stateType);

};

#endif // RBL_APPLICATION_STATE_H
