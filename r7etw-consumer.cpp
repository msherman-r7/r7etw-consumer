#include <Windows.h>
#include <stdio.h>

#include "consumer.h"

static unsigned int g_count = 0;

static void event_record_callback(PEVENT_RECORD EventRecord) {
	wprintf(L"%ld: count = %d\n", GetCurrentThreadId(), g_count);
	g_count++;
}

int wmain(int argc, wchar_t* argv[]) {

	wchar_t logger[] = L"NT Kernel Logger";
	EVENT_TRACE_LOGFILEW event_trace_logfile = consumer_init_realtime(logger, event_record_callback);

	TRACEHANDLE th = OpenTraceW(&event_trace_logfile);
	if (INVALID_PROCESSTRACE_HANDLE == th) {
		DWORD le = ::GetLastError();
		wprintf(L"OpenTrace failed: err = %ld\n", le);
		return le;
	}

	wprintf(L"OpenTrace succeeded, trace handle = 0x%08llx\n", th);
	wprintf(L"CurrentTime = 0x%llx\n", event_trace_logfile.CurrentTime);
	wprintf(L"BuffersRead = 0x%lx\n", event_trace_logfile.BuffersRead);
	wprintf(L"BufferSize = 0x%lx\n", event_trace_logfile.BufferSize);
	wprintf(L"Filled = 0x%lx\n", event_trace_logfile.Filled);
	wprintf(L"IsKernelTrace = 0x%lx\n", event_trace_logfile.IsKernelTrace);

	ULONG retval = ProcessTrace(&th, 1, NULL, NULL);
	if (ERROR_SUCCESS != retval) {
		wprintf(L"ProcessTrace failed: err = %ld\n", retval);
	} else {
		wprintf(L"%ld: ProcessTrace returned succeessfully, count = %d\n", GetCurrentThreadId(), g_count);
	}

	CloseTrace(th);
	return 0;
}
