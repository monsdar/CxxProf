
#ifndef _BROFILER_DYN_COMMON_H_
#define _BROFILER_DYN_COMMON_H_

#ifdef BROFILER_DYN
    #define Brofiler_Dyn_EXPORT __declspec( dllexport )
#else
    #define Brofiler_Dyn_EXPORT __declspec( dllimport )
#endif //BROFILER_DYN


#endif //_BROFILER_DYN_COMMON_H_
