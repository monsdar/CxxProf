
#ifndef _NETWORK_ACTIVITY_H_
#define _NETWORK_ACTIVITY_H_

#include "brofiler_dyn/common.h"
#include "brofiler/IActivity.h"

class Brofiler_EXPORT NetworkActivity: public IActivity
{
public:
    NetworkActivity(const std::string& name);
    virtual ~NetworkActivity();

private:
    std::string name_;
};

#endif //_NETWORK_ACTIVITY_H_
