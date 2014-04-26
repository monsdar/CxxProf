#!/bin/bash

BASEDIR=$(dirname $BASH_SOURCE)

rm $BASEDIR/libcxxprof_dyn_network.so
cp $BASEDIR/../../cxxprof_dyn_network/lib/libcxxprof_dyn_network.so .

export LD_LIBRARY_PATH=$BASEDIR/../../../thirdparty/pluma/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BASEDIR/../../../thirdparty/zmq/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$BASEDIR/../../cxxprof_static/lib

$BASEDIR/cxxprof_howto

