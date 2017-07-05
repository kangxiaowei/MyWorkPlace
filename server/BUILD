cc_library(
	name="FlowPipeHandler",
	srcs=["FlowPipeHandler.cpp"],
	hdrs=["FlowPipeHandler.h"],
	deps=["//gen-cpp/FlowPipe","//lib/ThreadPool"],
	copts=["-I.","-Iexternal/THRIFT","-Iexternal/BOOST","-Iexternal/GFLAG","-Iexternal/GLOG"],
)

cc_binary(
	name="FlowServer",
	deps=[":FlowPipeHandler"],
	copts=["-I.","-Iexternal/THRIFT","-Iexternal/BOOST","-Iexternal/GFLAG"],
	linkopts=["-L/usr/local/lib","-lthrift"],
	data=["//config/DefaultConfig.json"],
	visibility=["__pkg__"],
)
