
#include "brofiler_dyn/NetworkActivity.h"

#include <iostream>

NetworkActivity::NetworkActivity() :
    name_(""),
    threadId_(0),
    actId_(0),
    parentId_(0)
{}

NetworkActivity::~NetworkActivity()
{
    if(callback_)
    {
        ActivityResult result;
        result.Name = name_;
        result.ThreadId = threadId_;
        result.ActId = actId_;
        result.ParentId = parentId_;
        result.StartTime = 0;
        result.StopTime = 0;
        
        callback_(result);
    }
}

void NetworkActivity::setName(const std::string& name)
{
    name_ = name;
}
void NetworkActivity::setThreadId(unsigned int threadId)
{
    threadId_ = threadId;
}
void NetworkActivity::setActId(unsigned int actId)
{
    actId_ = actId;
}
void NetworkActivity::setParentId(unsigned int parentId)
{
    parentId_ = parentId;
}

void NetworkActivity::setResultCallback(ResultCallback callback)
{
    callback_ = callback;
}

void NetworkActivity::start()
{
    std::cout << "Activity " << name_ << " started..." << std::endl;
}