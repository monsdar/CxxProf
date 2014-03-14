
#ifndef __COMMON_H_
#define __COMMON_H_

#ifdef BROFILER
    #define Brofiler_EXPORT __declspec( dllexport )
#else
    #define Brofiler_EXPORT __declspec( dllimport )
#endif //BROFILER


#endif //__COMMON_H_
