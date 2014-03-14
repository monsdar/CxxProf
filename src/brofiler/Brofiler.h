
#ifndef _BROFILER_H_
#define _BROFILER_H_

#include "brofiler/common.h"
#include "brofiler/IDynBrofiler.h"
#include "brofiler/IActivity.h"

#include <boost/shared_ptr.hpp>

class Brofiler_EXPORT Brofiler
{
public:
    Brofiler(boost::shared_ptr<IDynBrofiler> givenDynBrofider);
    virtual ~Brofiler();
    
    boost::shared_ptr<IActivity> createActivity(const std::string& name) const;

private:
    boost::shared_ptr<IDynBrofiler> dynBrofiler_;
};

#endif //_BROFILER_H_
