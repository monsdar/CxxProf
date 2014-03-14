
#ifndef _TEST_BROFILER_H_
#define _TEST_BROFILER_H_

#include "brofiler_dyntest/common.h"
#include "brofiler/IDynBrofiler.h"

class Brofiler_EXPORT TestBrofiler: public IDynBrofiler
{
public:
    TestBrofiler();
    virtual ~TestBrofiler();
    
    boost::shared_ptr<IActivity> createActivity(const std::string& name) const;
};

#endif //_TEST_BROFILER_H_
