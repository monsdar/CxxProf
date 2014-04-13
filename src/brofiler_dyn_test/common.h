
#ifndef _BROFILER_DYN_TEST_COMMON_H_
#define _BROFILER_DYN_TEST_COMMON_H_

#ifdef BROFILER_DYN_TEST
    #define Brofiler_Dyn_Test_EXPORT __declspec( dllexport )
#else
    #define Brofiler_Dyn_Test_EXPORT __declspec( dllimport )
#endif //BROFILER_DYNTEST


#endif //_BROFILER_DYN_TEST_COMMON_H_
