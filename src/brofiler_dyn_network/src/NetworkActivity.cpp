
#include "brofiler_dyn_network/NetworkActivity.h"

#include <iostream>

namespace Brofiler
{

    NetworkActivity::NetworkActivity() :
        name_(""),
        threadId_(0),
        actId_(0),
        parentId_(0),
        profilingStart_(boost::posix_time::time_from_string("2014-01-01 00:00:00.000"))
    {}

    NetworkActivity::NetworkActivity(const std::string& name, unsigned int actId, unsigned int threadId, unsigned int parentId, const boost::posix_time::ptime& profilingStart, ResultCallback callback) :
        name_(name),
        threadId_(threadId),
        actId_(actId),
        parentId_(parentId),
        callback_(callback),
        profilingStart_(profilingStart)
    {}

    NetworkActivity::~NetworkActivity()
    {
        executeCallback();
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

    void NetworkActivity::setProfilingStart(const boost::posix_time::ptime& profilingStart)
    {
        profilingStart_ = profilingStart;
    }

    void NetworkActivity::start()
    {
        startTime_ = getCurrentTime();
    }

    void NetworkActivity::shutdown()
    {
        executeCallback();
        callback_ = 0;
    }

    void NetworkActivity::executeCallback()
    {
        if (callback_)
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

    unsigned int NetworkActivity::getCurrentTime()
    {
        boost::posix_time::time_duration const diff = boost::posix_time::microsec_clock::local_time() - profilingStart_;
        return diff.total_microseconds();
    }

}
