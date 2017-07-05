// Copyright INTENT 2016 - Current

#include <iostream>

#include "FlowPipeHandler.h"

#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>
#include <boost/make_shared.hpp>
#include <gflags/gflags.h>

using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

using namespace std;

using namespace intent::flow;
using boost::shared_ptr;

DEFINE_int32(port, 11206, "port of the server");

class FlowPipeCloneFactory : virtual public FlowPipeIfFactory {
public:
    virtual ~FlowPipeCloneFactory() {}
    virtual FlowPipeIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
    {
        boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
        cout << "Incoming connection\n";
        cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
        cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
        cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
        cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";
        return new FlowPipeHandler;
    }
    virtual void releaseHandler( FlowPipeIf* handler) {
        delete handler;
    }
};

int main(int argc, char **argv) {
    TThreadedServer server(
            boost::make_shared<FlowPipeProcessorFactory>(boost::make_shared<FlowPipeCloneFactory>()),
            boost::make_shared<TServerSocket>(FLAGS_port), //port
            boost::make_shared<TBufferedTransportFactory>(),
            boost::make_shared<TBinaryProtocolFactory>());
    server.serve();
    return 0;
}

