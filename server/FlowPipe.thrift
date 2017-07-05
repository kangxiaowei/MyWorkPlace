// Copyright INTENT, 2016 - Current

namespace cpp flow

struct Object {
   1:i64 objectId,
   2:i64 startTimeStamp,
   3:i64 lastTimeStamp,
   4:double positionX,
   5:double positionY,
   6:i32 countFrames,
   7:i32 skippedFrames
}

service FlowPipe {

   bool initiate(1:i32 clientId),
   bool reset(1:i32 clientId),
   bool close(1:i32 clientId),
   bool sendHeartbeat(1:i32 clientId),
   string getConfig(1:i32 clientId),

   oneway void send(1:i32 clientId, 2:Object object)

}
