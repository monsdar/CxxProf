
#ifndef _CXXPROF_DYN_COMMON_H_
#define _CXXPROF_DYN_COMMON_H_


#ifdef WIN32
	#ifdef CXXPROF_DYN_NETWORK
	    #define CxxProf_Dyn_Network_EXPORT __declspec( dllexport )
	#else
	    #define CxxProf_Dyn_Network_EXPORT __declspec( dllimport )
	#endif //CXXPROF_DYN_NETWORK
#else
	#define CxxProf_Dyn_Network_EXPORT 
#endif


#endif //_CXXPROF_DYN_COMMON_H_
