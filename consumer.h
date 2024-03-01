#pragma once
#include <Windows.h>
#include <evntcons.h>
#include <evntrace.h>

EVENT_TRACE_LOGFILEW consumer_init_realtime(LPWSTR loggerName, PEVENT_RECORD_CALLBACK event_record_callback);