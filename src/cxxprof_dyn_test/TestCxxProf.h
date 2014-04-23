
#ifndef _TEST_CXXPROF_H_
#define _TEST_CXXPROF_H_

#include "cxxprof_dyn_test/common.h"
#include "cxxprof_static/IDynCxxProf.h"

namespace CxxProf
{

    class CxxProf_Dyn_Test_EXPORT TestCxxProf : public IDynCxxProf
    {
    public:
        TestCxxProf();
        virtual ~TestCxxProf();

        boost::shared_ptr<IActivity> createActivity(const std::string& name);
        void addMark(const std::string& name);
        void addPlotValue(const std::string& name, double value);
        void shutdown();

        std::string toString() const;
    };

}

#endif //_TEST_CXXPROF_H_
