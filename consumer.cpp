#include "pch.h"
#include "consumer.h"


EVENT_TRACE_LOGFILEW consumer_init_realtime(LPWSTR loggerName, PEVENT_RECORD_CALLBACK event_record_callback) {
	EVENT_TRACE_LOGFILEW etl;
	memset(&etl, 0x0, sizeof(etl));
	etl.LogFileName = NULL;
	etl.LoggerName = loggerName;
	etl.ProcessTraceMode = PROCESS_TRACE_MODE_EVENT_RECORD | PROCESS_TRACE_MODE_REAL_TIME;
	etl.BufferCallback = NULL;
	etl.EventRecordCallback = event_record_callback;
	return etl;
}