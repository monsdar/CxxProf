
#ifndef _I_BROFILER_H_
#define _I_BROFILER_H_

#include "IActivity.h"
#include <boost/shared_ptr.hpp>

class IBrofiler
{
public:
    virtual IBrofiler() {};
    virtual boost::shared_ptr<IActivity> createActivity() const = 0;
};

#endif //_I_BROFILER_H_
