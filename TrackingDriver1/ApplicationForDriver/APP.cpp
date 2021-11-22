#include <iostream>
#include <map>

#include "../TrackingDriver1/LCWC.h"
#include "../TrackingDriver1/PathesToXY.h"

using namespace std;

int main() {

	HANDLE hFile = CreateFile(L"\\\\.\\ProcessesLifeCycleWatcher", GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		cout << "Error: Driver must be running!\n";
		cin.get();
		return 1;
	}
	map<long, HANDLE> processIdPairs;
	ProcessEventInfo eventInfo;
	while (true) 
	{
		DWORD bytes;
		if (!ReadFile(hFile, &eventInfo, sizeof(ProcessEventInfo), &bytes, nullptr)) break;
		if (bytes == 0) 
		{
			Sleep(500);
			continue;
		}
		if (bytes != sizeof(ProcessEventInfo)) break;
		if (eventInfo.eventType == ProcessCreate) 
		{
			cout << "opening process id: " << eventInfo.processId << '\n';
			STARTUPINFO si = { sizeof(STARTUPINFO), 0 };
			si.cb = sizeof(si);
			PROCESS_INFORMATION pi = { 0 };
			CreateProcessW(PROCESS_Y, NULL, 0, 0, 0, CREATE_DEFAULT_ERROR_MODE, 0, 0, &si, &pi);
			CloseHandle(pi.hThread);
			processIdPairs[eventInfo.processId] = pi.hProcess;
			continue;
		}
		if (eventInfo.eventType == ProcessExit) 
		{
			cout << "terminating process id: " << eventInfo.processId << '\n';
			TerminateProcess(processIdPairs[eventInfo.processId], 1);
			CloseHandle(processIdPairs[eventInfo.processId]);
			continue;
		}
		break;
	}
	cout << "An error has occurred\n";
	CloseHandle(hFile);
	cin.get();
	return 0;
}