#include "pch.h"

#include "..\consumer.h"

static void event_record_callback(PEVENT_RECORD EventRecord){
}

TEST(TestCaseName, TestName) {
	EVENT_TRACE_LOGFILEW event_trace_logfile = consumer_init_realtime(L"NT Kernel Logger", event_record_callback);
	EXPECT_EQ(NULL, event_trace_logfile.LogFileName);
	EXPECT_STREQ(L"NT Kernel Logger", event_trace_logfile.LoggerName);
	EXPECT_TRUE(PROCESS_TRACE_MODE_EVENT_RECORD & event_trace_logfile.ProcessTraceMode);
	EXPECT_TRUE(PROCESS_TRACE_MODE_REAL_TIME & event_trace_logfile.ProcessTraceMode);
	EXPECT_EQ(0, ~(PROCESS_TRACE_MODE_EVENT_RECORD | PROCESS_TRACE_MODE_REAL_TIME) & event_trace_logfile.ProcessTraceMode);
	EXPECT_EQ(nullptr, event_trace_logfile.BufferCallback);
	EXPECT_NE((void*)0xcdcdcdcdcdcdcdcd, event_trace_logfile.EventRecordCallback);

	EXPECT_TRUE(true);
}