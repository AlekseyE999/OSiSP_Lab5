#include "DispatchProcedures.h"
#include "RegistryNotify.h"
#include "LoggerCommon.h"

Globals g_Globals;

void RLoggerUnload(_In_ PDRIVER_OBJECT DriverObject);

extern "C" NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT  DriverObject, _In_ PUNICODE_STRING RegistryPath) 
{
	UNREFERENCED_PARAMETER(RegistryPath);
	UNICODE_STRING devName = RTL_CONSTANT_STRING(L"\\Device\\RLogger");
	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\RLogger");
	PDEVICE_OBJECT DeviceObject;
	NTSTATUS status;
	status = IoCreateDevice(DriverObject, 0, &devName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObject);
	if (!NT_SUCCESS(status)) return status;
	status = IoCreateSymbolicLink(&symLink, &devName);
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice(DeviceObject);
		return status;
	}
	UNICODE_STRING altitude = RTL_CONSTANT_STRING(L"314.19934511");
	status = CmRegisterCallbackEx(RegistryNotify, &altitude, DriverObject, NULL, &g_Globals.regCookie, NULL);
	if (!NT_SUCCESS(status))
	{
		IoDeleteSymbolicLink(&symLink);
		IoDeleteDevice(DeviceObject);
		return status;
	}
	DriverObject->DriverUnload = RLoggerUnload;
	DriverObject->MajorFunction[IRP_MJ_CREATE] = DriverObject->MajorFunction[IRP_MJ_CLOSE] = LoggerCreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = LoggerDeviceControl;
	return STATUS_SUCCESS;
}

void RLoggerUnload(_In_ PDRIVER_OBJECT DriverObject) 
{
	UNICODE_STRING symLink = RTL_CONSTANT_STRING(L"\\??\\RLogger");
	IoDeleteSymbolicLink(&symLink);
	IoDeleteDevice(DriverObject->DeviceObject);
	CmUnRegisterCallback(g_Globals.regCookie);
}