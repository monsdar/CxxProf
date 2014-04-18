
#ifndef _BROFILER_STATIC_H_
#define _BROFILER_STATIC_H_

#include "brofiler_static/common.h"
#include "brofiler_static/IDynBrofiler.h"
#include "brofiler_static/IActivity.h"

#include <boost/shared_ptr.hpp>
#include <vector>

//forward declaration
namespace pluma
{
    class Pluma;
};

class BrofilerStatic_EXPORT BrofilerStatic
{
public:
    BrofilerStatic();
    virtual ~BrofilerStatic();
    
    void loadDynBrofiler();
    boost::shared_ptr<IActivity> createActivity(const std::string& name) const;
    void addMark(const std::string& name);
    void addPlotValue(const std::string& name, double value);

private:
    IDynBrofiler* dynBrofiler_;
    boost::shared_ptr<pluma::Pluma> manager_;
};

#endif //_BROFILER_STATIC_H_
