
#ifndef _TEST_ACTIVITY_H_
#define _TEST_ACTIVITY_H_

#include "brofiler_dyntest/common.h"
#include "brofiler/IActivity.h"

class Brofiler_EXPORT TestActivity: public IActivity
{
public:
    TestActivity(const std::string& name);
    virtual ~TestActivity();
    
private:
    std::string name_;
};

#endif //_TEST_ACTIVITY_H_
