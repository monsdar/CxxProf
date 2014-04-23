
#ifndef _BROFILER_STATIC_H_
#define _BROFILER_STATIC_H_

#include "brofiler_static/common.h"
#include "brofiler_static/IDynBrofiler.h"
#include "brofiler_static/IActivity.h"

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>

//forward declaration
namespace pluma
{
    class Pluma;
};

namespace Brofiler
{

    class BrofilerStatic_EXPORT BrofilerStatic : boost::noncopyable
    {
    public:
        static BrofilerStatic* getBrofiler();
        virtual ~BrofilerStatic();
    
        boost::shared_ptr<IActivity> createActivity(const std::string& name);
        void addMark(const std::string& name);
        void addPlotValue(const std::string& name, double value);

        void shutdown();

    private:
        BrofilerStatic();
        static BrofilerStatic* instance_;

        void loadDynBrofiler();
        IDynBrofiler* dynBrofiler_;

        boost::mutex mutex_;
        boost::shared_ptr<pluma::Pluma> manager_;
    };

} //namespace Brofiler

#endif //_BROFILER_STATIC_H_
