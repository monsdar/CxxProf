
#ifndef _NETWORK_BROFILER_H_
#define _NETWORK_BROFILER_H_

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
    
    unsigned int actCounter_;
};

#endif //_NETWORK_BROFILER_H_
