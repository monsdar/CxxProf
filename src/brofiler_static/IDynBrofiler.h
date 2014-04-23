
#ifndef _I_DYN_BROFILER_H_
#define _I_DYN_BROFILER_H_

#include "brofiler_static/IActivity.h"
#include <boost/shared_ptr.hpp>

namespace Brofiler
{

    class IDynBrofiler
    {
    public:
        virtual ~IDynBrofiler() {};

        virtual boost::shared_ptr<IActivity> createActivity(const std::string& name) = 0;
        virtual void addMark(const std::string& name) = 0;
        virtual void addPlotValue(const std::string& name, double value) = 0;
        virtual void shutdown() = 0;

        virtual std::string toString() const = 0;
    };

} //namespace Brofiler

#endif //_I_DYN_BROFILER_H_
