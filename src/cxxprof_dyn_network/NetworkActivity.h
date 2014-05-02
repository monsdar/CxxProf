
#ifndef _NETWORK_ACTIVITY_H_
#define _NETWORK_ACTIVITY_H_

#include "cxxprof_static/IActivity.h"

#include "cxxprof_dyn_network/common.h"
#include "cxxprof_dyn_network/ActivityResult.h"

#include <boost/cstdint.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/function.hpp>
#include <stdint.h>
#include <string>

namespace CxxProf
{

    class CxxProf_Dyn_Network_EXPORT NetworkActivity : public IActivity
    {
    public:
        typedef boost::function1<void, const ActivityResult&> ResultCallback;

    public:
        NetworkActivity();
        NetworkActivity(const std::string& name, unsigned int actId, unsigned int threadId, unsigned int parentId, const boost::posix_time::ptime& profilingStart, ResultCallback callback);
        virtual ~NetworkActivity();

        void setName(const std::string& name);
        void setThreadId(unsigned int threadId);
        void setActId(unsigned int actId);
        void setParentId(unsigned int parentId);
        void setResultCallback(ResultCallback callback);
        void setProfilingStart(const boost::posix_time::ptime& profilingStart);

        void start();
        void shutdown();

    private:
        void executeCallback();

        std::string name_;
        unsigned int threadId_;
        unsigned int actId_;
        unsigned int parentId_;
        ResultCallback callback_;
        boost::int64_t starttime_;
        boost::posix_time::ptime profilingStart_;

        boost::int64_t getCurrentTime();
    };

}

#endif //_NETWORK_ACTIVITY_H_
