
#ifndef _NETWORK_BROFILER_H_
#define _NETWORK_BROFILER_H_

#include "brofiler_dyn/common.h"
#include "brofiler/IDynBrofiler.h"

class Brofiler_EXPORT NetworkBrofiler: public IDynBrofiler
{
public:
    NetworkBrofiler();
    virtual ~NetworkBrofiler();
    
    boost::shared_ptr<IActivity> createActivity(const std::string& name) const;
};

#endif //_NETWORK_BROFILER_H_
