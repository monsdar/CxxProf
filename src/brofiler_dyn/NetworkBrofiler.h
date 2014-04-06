
#ifndef _NETWORK_BROFILER_H_
#define _NETWORK_BROFILER_H_

#include <stack>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "brofiler_dyn/common.h"
#include "brofiler/IDynBrofiler.h"

#include "brofiler_dyn/ActivityResult.h"

class Brofiler_EXPORT NetworkBrofiler: public IDynBrofiler
{
public:
    NetworkBrofiler();
    virtual ~NetworkBrofiler();
    
    boost::shared_ptr<IActivity> createActivity(const std::string& name);
    std::string toString() const;
    
private:
    void addResult(const ActivityResult& result);
    void printDepth();
    
    unsigned int actCounter_;
    std::stack<unsigned int> activeActivity_;
    boost::posix_time::ptime profilingStart_;
};

#endif //_NETWORK_BROFILER_H_
