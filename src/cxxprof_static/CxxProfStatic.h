
#ifndef _CXXPROF_STATIC_H_
#define _CXXPROF_STATIC_H_

#include "cxxprof_static/common.h"
#include "cxxprof_static/IDynCxxProf.h"
#include "cxxprof_static/IActivity.h"

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <vector>

//forward declaration
namespace pluma
{
    class Pluma;
};

namespace CxxProf
{

    class CxxProfStatic_EXPORT CxxProfStatic : boost::noncopyable
    {
    public:
        static CxxProfStatic* getCxxProf();
        virtual ~CxxProfStatic();
    
        boost::shared_ptr<IActivity> createActivity(const std::string& name);
        void addMark(const std::string& name);
        void addPlotValue(const std::string& name, double value);

        void initialize();
        void shutdown();

    private:
        CxxProfStatic();
        static CxxProfStatic* instance_;

        void loadDynCxxProf();
        IDynCxxProf* dynCxxProf_;

        boost::mutex mutex_;
        boost::shared_ptr<pluma::Pluma> manager_;
    };

} //namespace CxxProf

#endif //_CXXPROF_STATIC_H_
