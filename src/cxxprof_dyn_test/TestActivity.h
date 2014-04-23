
#ifndef _TEST_ACTIVITY_H_
#define _TEST_ACTIVITY_H_

#include "cxxprof_dyn_test/common.h"
#include "cxxprof_static/IActivity.h"

namespace CxxProf
{

    class CxxProf_Dyn_Test_EXPORT TestActivity : public IActivity
    {
    public:
        TestActivity(const std::string& name);
        virtual ~TestActivity();

    private:
        std::string name_;
    };

}

#endif //_TEST_ACTIVITY_H_
