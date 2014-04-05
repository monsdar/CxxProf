
#include "brofiler_dyn/NetworkActivity.h"

#include <boost/date_time/posix_time/posix_time.hpp>
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
        result.StartTime = startTime_;
        result.StopTime = getCurrentTime();
        
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
    startTime_ = getCurrentTime();
}

unsigned int NetworkActivity::getCurrentTime()
{
    //TODO: Find a better (read: faster) method of getting the current time...
    //TODO: Use some sort of global starttime-object here instead of this string-based method...
    boost::posix_time::ptime epoch = boost::posix_time::time_from_string("2014-04-05 00:00:00.000");
    boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();

    boost::posix_time::time_duration const diff = now - epoch;
    return diff.total_milliseconds();
}
