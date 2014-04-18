
#ifndef _SERIALIZERS_H_
#define _SERIALIZERS_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

#include "brofiler_dyn_network/ActivityResult.h"
#include "brofiler_dyn_network/NetworkMark.h"
#include "brofiler_dyn_network/NetworkObjects.h"
#include "brofiler_dyn_network/NetworkPlot.h"

namespace boost {
    namespace serialization {


template <typename Archive>
void serialize(Archive &ar, ActivityResult &instance, const unsigned int version)
{
    ar & instance.Name;
    ar & instance.ThreadId;
    ar & instance.ActId;
    ar & instance.ParentId;
    ar & instance.StartTime;
    ar & instance.StopTime;
}

template <typename Archive>
void serialize(Archive &ar, NetworkMark &instance, const unsigned int version)
{
    ar & instance.Name;
    ar & instance.Timestamp;
}

template <typename Archive>
void serialize(Archive &ar, NetworkPlot &instance, const unsigned int version)
{
    ar & instance.Name;
    ar & instance.Timestamp;
    ar & instance.Value;
}

template <typename Archive>
void serialize(Archive &ar, NetworkObjects &instance, const unsigned int version)
{
    ar & instance.ActivityResults;
    ar & instance.Marks;
    ar & instance.Plots;
}


    }; //namespace serialization
}; //namespace boost

#endif //_SERIALIZERS_H_
