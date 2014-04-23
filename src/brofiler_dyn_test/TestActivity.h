
#ifndef _TEST_ACTIVITY_H_
#define _TEST_ACTIVITY_H_

#include "brofiler_dyn_test/common.h"
#include "brofiler_static/IActivity.h"

namespace Brofiler
{

    class Brofiler_Dyn_Test_EXPORT TestActivity : public IActivity
    {
    public:
        TestActivity(const std::string& name);
        virtual ~TestActivity();

    private:
        std::string name_;
    };

}

#endif //_TEST_ACTIVITY_H_
