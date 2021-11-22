#pragma once

#include <Windows.h>

enum EventType {
	None,
	ProcessCreate,
	ProcessExit
};

struct ProcessEventInfo {
	EventType eventType;
	ULONG processId;
};

struct ProcessEventItem {
	ProcessEventInfo eventInfo;
	LIST_ENTRY readListEntry;
	LIST_ENTRY toSearchListEntry;
	int linksCount;
};

struct Globals {
	LIST_ENTRY readListHead;
	LIST_ENTRY SearchListHead;
	FAST_MUTEX mutex;
};