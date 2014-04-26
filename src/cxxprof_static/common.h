
#ifndef _CXXPROFSTATIC_COMMON_H_
#define _CXXPROFSTATIC_COMMON_H_


#ifdef WIN32
	#ifdef CXXPROFSTATIC
	    #define CxxProfStatic_EXPORT __declspec( dllexport )
	#else
	    #define CxxProfStatic_EXPORT __declspec( dllimport )
	#endif //CXXPROFSTATIC
#else
	#define CxxProfStatic_EXPORT 
#endif


#endif //_CXXPROFSTATIC_COMMON_H_
