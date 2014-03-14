
#ifndef __COMMON_H_
#define __COMMON_H_

#ifdef BROFILER_DYNTEST
    #define Brofiler_EXPORT __declspec( dllexport )
#else
    #define Brofiler_EXPORT __declspec( dllimport )
#endif //BROFILER_DYNTEST


#endif //__COMMON_H_
