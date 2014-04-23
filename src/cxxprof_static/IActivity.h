
#ifndef _I_ACTIVITY_H_
#define _I_ACTIVITY_H_

#include <string>

namespace CxxProf
{
    /**
     * This interface acts as an interface to Activities.
     * As CxxProf let's the implementation decide on how Activities should behave,
     * this interface is mostly empty
     *
     * CxxProf just needs to know that there are Activities anywhere
     *
     */
    class IActivity
    {
    public:
        virtual ~IActivity() {};
    };

} //namespace CxxProf
#endif //_I_ACTIVITY_H_
