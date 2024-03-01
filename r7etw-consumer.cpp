#include <Windows.h>
#include <stdio.h>

#include "consumer.h"

static void event_record_callback(PEVENT_RECORD EventRecord) {
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

	CloseTrace(th);
	return 0;
}
