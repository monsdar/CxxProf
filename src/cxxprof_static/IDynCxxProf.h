
#ifndef _I_DYN_CXXPROF_H_
#define _I_DYN_CXXPROF_H_

#include "cxxprof_static/IActivity.h"
#include <boost/shared_ptr.hpp>

namespace CxxProf
{

    class IDynCxxProf
    {
    public:
        virtual ~IDynCxxProf() {};

        virtual boost::shared_ptr<IActivity> createActivity(const std::string& name) = 0;
        virtual void addMark(const std::string& name) = 0;
        virtual void addPlotValue(const std::string& name, double value) = 0;
        virtual void shutdown() = 0;

        virtual std::string toString() const = 0;
    };

} //namespace CxxProf

#endif //_I_DYN_CXXPROF_H_
