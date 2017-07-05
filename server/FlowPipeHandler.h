// Copyright INTENT 2016 - Current

#pragma once

#include "gen-cpp/FlowPipe.h"
#include "lib/ThreadPool.h"

#include <map>
#include <boost/thread/lock_guard.hpp>

using namespace ::flow;
using namespace std;
using namespace boost::chrono;
using namespace boost;

namespace intent { namespace flow {

class FlowPipeHandler : virtual public FlowPipeIf {

public:
    FlowPipeHandler();

    bool initiate(const int32_t clientId);

    bool reset(const int32_t clientId);

    bool close(const int32_t clientId);

    bool sendHeartbeat(const int32_t clientId);

    void getConfig(string& configString, const int32_t clientId);

    void send(const int32_t clientId, const Object& object);

private:
    void checkHeartbeat();

    map<int32_t, uint64_t> clientStatus;
    std::shared_ptr<ThreadPool> threadPool;
    mutex clientStatusMtx;
};

}} // namespace intent::flow
