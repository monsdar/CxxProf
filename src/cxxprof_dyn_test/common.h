
#ifndef _CXXPROF_DYN_TEST_COMMON_H_
#define _CXXPROF_DYN_TEST_COMMON_H_


#ifdef WIN32
	#ifdef CXXPROF_DYN_TEST
	    #define CxxProf_Dyn_Test_EXPORT __declspec( dllexport )
	#else
	    #define CxxProf_Dyn_Test_EXPORT __declspec( dllimport )
	#endif //CXXPROF_DYN_TEST
#else
	#define CxxProf_Dyn_Test_EXPORT 
#endif


#endif //_CXXPROF_DYN_TEST_COMMON_H_
