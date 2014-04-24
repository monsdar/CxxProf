
#ifndef _SERIALIZERS_H_
#define _SERIALIZERS_H_

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include "cxxprof_dyn_network/ActivityResult.h"
#include "cxxprof_dyn_network/AppInfo.h"
#include "cxxprof_dyn_network/NetworkMark.h"
#include "cxxprof_dyn_network/NetworkObjects.h"
#include "cxxprof_dyn_network/NetworkPlot.h"

namespace boost {
    namespace serialization {

template <typename Archive>
void serialize(Archive &ar, CxxProf::ActivityResult &instance, const unsigned int version)
{
    ar & instance.Name;
    ar & instance.ThreadId;
    ar & instance.ActId;
    ar & instance.ParentId;
    ar & instance.Starttime;
    ar & instance.Stoptime;
}

template <typename Archive>
void serialize(Archive &ar, CxxProf::AppInfo &instance, const unsigned int version)
{
    ar & instance.Name;
    ar & instance.Starttime;
    ar & instance.ThreadAliases;
}

template <typename Archive>
void serialize(Archive &ar, CxxProf::NetworkMark &instance, const unsigned int version)
{
    ar & instance.Name;
    ar & instance.Timestamp;
}

template <typename Archive>
void serialize(Archive &ar, CxxProf::NetworkPlot &instance, const unsigned int version)
{
    ar & instance.Name;
    ar & instance.Timestamp;
    ar & instance.Value;
}

template <typename Archive>
void serialize(Archive &ar, CxxProf::NetworkObjects &instance, const unsigned int version)
{
    ar & instance.Info;
    ar & instance.ActivityResults;
    ar & instance.Marks;
    ar & instance.Plots;
}


    }; //namespace serialization
}; //namespace boost

#endif //_SERIALIZERS_H_
