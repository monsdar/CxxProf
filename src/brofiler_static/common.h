
#ifndef _BROFILERSTATIC_COMMON_H_
#define _BROFILERSTATIC_COMMON_H_

#ifdef BROFILERSTATIC
    #define BrofilerStatic_EXPORT __declspec( dllexport )
#else
    #define BrofilerStatic_EXPORT __declspec( dllimport )
#endif //BROFILERSTATIC


#endif //_BROFILERSTATIC_COMMON_H_
