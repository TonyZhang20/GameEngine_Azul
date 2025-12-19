//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef PROTO_BUFF_H
#define PROTO_BUFF_H

#include <google/protobuf/message_lite.h>
#include <google/protobuf/stubs/common.h>

class ProtoBuf
{
public:
	ProtoBuf()
	{
		// Verify that the version of the library that we linked against is
		// compatible with the version of the headers we compiled against.
		GOOGLE_PROTOBUF_VERIFY_VERSION;

		Trace::out("\n");
	}

	ProtoBuf(const ProtoBuf &) = delete;
	ProtoBuf &operator = (const ProtoBuf &) = delete;
	ProtoBuf(ProtoBuf &&) = delete;
	ProtoBuf &operator = (ProtoBuf &&) = delete;

	~ProtoBuf()
	{
		google::protobuf::ShutdownProtobufLibrary();
	}

};


#endif

// --- End of File ---
