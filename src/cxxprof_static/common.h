
#ifndef _CXXPROFSTATIC_COMMON_H_
#define _CXXPROFSTATIC_COMMON_H_

#ifdef CXXPROFSTATIC
    #define CxxProfStatic_EXPORT __declspec( dllexport )
#else
    #define CxxProfStatic_EXPORT __declspec( dllimport )
#endif //CXXPROFSTATIC


#endif //_CXXPROFSTATIC_COMMON_H_
