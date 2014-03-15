
#ifndef _BROFILER_H_
#define _BROFILER_H_

#include "brofiler/common.h"
#include "brofiler/IDynBrofiler.h"
#include "brofiler/IActivity.h"

#include <boost/shared_ptr.hpp>
#include <vector>

//forward declaration
namespace pluma
{
    class Pluma;
};

class Brofiler_EXPORT Brofiler
{
public:
    Brofiler();
    virtual ~Brofiler();
    
    void loadDynBrofiler();
    boost::shared_ptr<IActivity> createActivity(const std::string& name) const;

private:
    IDynBrofiler* dynBrofiler_;
    boost::shared_ptr<pluma::Pluma> manager_;
};

#endif //_BROFILER_H_
