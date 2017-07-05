// Copyright INTENT 2016 - Current

#include "FlowPipeHandler.h"

#include <glog/logging.h>
#include <gflags/gflags.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

DEFINE_int32(sleepTime, 60000, "sleep time for heartbeat checking");
DEFINE_int64(expireTime, 120000000000, "expire time for unresponsive clients");

namespace intent { namespace flow {

FlowPipeHandler::FlowPipeHandler() {
    threadPool = make_shared<intent::ThreadPool>(2);
    threadPool->enqueue([&]{ this->checkHeartbeat(); });
}

bool FlowPipeHandler::initiate(const int32_t clientId) {
    lock_guard<mutex> guard(clientStatusMtx);
    LOG(INFO) << "Initiate client, id = " << clientId;
    uint64_t currentTimeStamp = (uint64_t) duration_cast<nanoseconds>(
            high_resolution_clock::now().time_since_epoch()
    ).count();
    clientStatus.insert(pair<int32_t, uint64_t>(clientId, currentTimeStamp));
    return true;
}

bool FlowPipeHandler::reset(const int32_t clientId) {
    LOG(INFO) << "Reset client, id = " << clientId;
    return true;
}

bool FlowPipeHandler::close(const int32_t clientId) {
    lock_guard<mutex> guard(clientStatusMtx);
    LOG(INFO) << "Close client, id = " << clientId;
    clientStatus.erase(clientId);
    return true;
}

bool FlowPipeHandler::sendHeartbeat(const int32_t clientId) {
    lock_guard<mutex> guard(clientStatusMtx);
    uint64_t currentTimeStamp = (uint64_t) duration_cast<nanoseconds>(
            high_resolution_clock::now().time_since_epoch()
    ).count();
    clientStatus[clientId] = currentTimeStamp;
    return true;
}

void FlowPipeHandler::getConfig(string& configString, const int32_t clientId) {
    string configFileName = "config/Client";
    configFileName += to_string(clientId);
    configFileName += "Config.json";
    ifstream configFile;
    configFile.open(configFileName);
    if (! configFile.is_open()) {
        LOG(ERROR) << "Config file for client not found, using default config.";
        configFile.open("config/DefaultConfig.json");
    }
    stringstream configStringStream;
    configStringStream << configFile.rdbuf();
    configString = configStringStream.str();
}

void FlowPipeHandler::send(const int32_t clientId, const Object& object) {
    std::cout << "clientId=" << clientId 
        << ",objectId=" << object.objectId 
        << ",startTimeStamp=" << object.startTimeStamp
        << ",lastTimeStamp=" << object.lastTimeStamp 
        << ",positionX=" << object.positionX 
        << ",positionY=" << object.positionY
        << ",countFrames=" << object.countFrames
        << ",skippedFrames=" << object.skippedFrames << std::endl;
}

void  FlowPipeHandler::checkHeartbeat() {
    while (true) {
        LOG(INFO) << "Check heartbeat";
        uint64_t currentTimeStamp = (uint64_t) duration_cast<nanoseconds>(
                high_resolution_clock::now().time_since_epoch()
        ).count();
        map<int32_t, uint64_t>::iterator  clientStatusIterator;
        for (clientStatusIterator = clientStatus.begin();
             clientStatusIterator != clientStatus.end();
             clientStatusIterator++) {
            lock_guard<mutex> guard(clientStatusMtx);
            if (currentTimeStamp - clientStatusIterator->second
                > FLAGS_expireTime) {
                reset(clientStatusIterator->first);
            }  
        }
        boost::this_thread::sleep_for(
            milliseconds(FLAGS_sleepTime)
        );
    }
}

}} // namespace intent::flow
