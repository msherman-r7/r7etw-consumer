#include <Windows.h>
#include <stdio.h>
#include <tdh.h>

#include "consumer.h"

static unsigned int g_count = 0;
DWORD const TRACE_EVENT_INFO_BUFSIZE = 1024 * 1024 * 16;
BYTE g_buffer[TRACE_EVENT_INFO_BUFSIZE];
TRACE_EVENT_INFO* g_ptrace_event_info = (TRACE_EVENT_INFO*)g_buffer;

static void display_string_at_offset(TRACE_EVENT_INFO* ptei, ULONG offset, wchar_t const* prefix) {
	wchar_t const* value = L"None";
	if (offset) {
		BYTE* pb = (BYTE*)ptei;
		pb = pb + offset;
		value = (wchar_t const*)pb;
	}
	wprintf(L"\t%s = %s\n", prefix, value);
}

static void event_record_callback(PEVENT_RECORD EventRecord) {
	wprintf(L"%ld: count = %d\n", GetCurrentThreadId(), g_count);
	g_count++;

	memset(g_ptrace_event_info, 0xcd, TRACE_EVENT_INFO_BUFSIZE);
	DWORD cb = TRACE_EVENT_INFO_BUFSIZE;
	TDHSTATUS status = TdhGetEventInformation(EventRecord, 0, NULL, g_ptrace_event_info, &cb);
	if (ERROR_SUCCESS != status) {
		wprintf(L"%ld: TdhGetEventInformation failed: status = %ld, cb = %ld\n", GetCurrentThreadId(), status, cb);
		return;
	} else {
		wprintf(L"%ld: TdhGetEventInformation succeeded\n", GetCurrentThreadId());
		wprintf(L"%ld: \tDecoding Source = %ld\n", GetCurrentThreadId(), g_ptrace_event_info->DecodingSource);
		display_string_at_offset(g_ptrace_event_info, g_ptrace_event_info->ProviderNameOffset, L"Provider Name");
		display_string_at_offset(g_ptrace_event_info, g_ptrace_event_info->TaskNameOffset, L"Task Name");
		display_string_at_offset(g_ptrace_event_info, g_ptrace_event_info->OpcodeNameOffset, L"Opcode Name");
		display_string_at_offset(g_ptrace_event_info, g_ptrace_event_info->EventMessageOffset, L"Event Message");
	}
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
