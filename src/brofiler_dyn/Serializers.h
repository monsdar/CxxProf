
#ifndef _SERIALIZERS_H_
#define _SERIALIZERS_H_

#include <boost/archive/text_oarchive.hpp> 

#include "brofiler_dyn/ActivityResult.h"

namespace boost {
    namespace serialization {


template <typename Archive>
void serialize(Archive &ar, ActivityResult &instance, const unsigned int version)
{
    ar & instance.ActId;
    ar & instance.ParentId;
    ar & instance.StartTime;
    ar & instance.StopTime;
    ar & instance.ThreadId;
}


    }; //namespace serialization
}; //namespace boost

#endif //_SERIALIZERS_H_
