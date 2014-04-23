
#ifndef _CXXPROF_DYN_TEST_COMMON_H_
#define _CXXPROF_DYN_TEST_COMMON_H_

#ifdef CXXPROF_DYN_TEST
    #define CxxProf_Dyn_Test_EXPORT __declspec( dllexport )
#else
    #define CxxProf_Dyn_Test_EXPORT __declspec( dllimport )
#endif //CXXPROF_DYNTEST


#endif //_CXXPROF_DYN_TEST_COMMON_H_
