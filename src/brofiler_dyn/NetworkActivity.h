
#ifndef _NETWORK_ACTIVITY_H_
#define _NETWORK_ACTIVITY_H_

#include "brofiler/IActivity.h"

#include "brofiler_dyn/common.h"
#include "brofiler_dyn/ActivityResult.h"

#include <boost/function.hpp>
#include <stdint.h>

class Brofiler_EXPORT NetworkActivity: public IActivity
{
public:
    typedef boost::function1<void, const ActivityResult&> ResultCallback;

public:
    NetworkActivity();
    virtual ~NetworkActivity();

    void setName(const std::string& name);
    void setThreadId(unsigned int threadId);
    void setActId(unsigned int actId);
    void setParentId(unsigned int parentId);
    void setResultCallback(ResultCallback callback);
    
    void start();
    
private:
    std::string name_;
    unsigned int threadId_;
    unsigned int actId_;
    unsigned int parentId_;
    ResultCallback callback_;
    unsigned int startTime_;

    unsigned int getCurrentTime();
};

#endif //_NETWORK_ACTIVITY_H_
