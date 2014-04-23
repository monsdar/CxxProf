
#ifndef _NETWORK_RECEIVER_H_
#define _NETWORK_RECEIVER_H_

#include "cxxprof_dyn_network/NetworkObjects.h"
#include "cxxprof_dyn_network/Serializers.h"

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

#include <zhelpers.hpp>

class NetworkReceiver
{
public:
    typedef boost::function1<void, const CxxProf::NetworkObjects&> ReceiveCallback;

public:
    NetworkReceiver();
    virtual ~NetworkReceiver();

    void setCallback(ReceiveCallback givenCallback);
    void startListening();

private:
    void receive();

    bool isRunning_;

    ReceiveCallback callback_;
    boost::shared_ptr<boost::thread> listenThread_;
    boost::shared_ptr<zmq::context_t> zmqContext_;
    boost::shared_ptr<zmq::socket_t> zmqListener_;

};

#endif //_NETWORK_RECEIVER_H_
