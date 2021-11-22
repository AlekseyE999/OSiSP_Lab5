#include "DispatchProcedures.h"
#include "LoggerCommon.h"

extern Globals g_Globals;

NTSTATUS CompleteIRP(PIRP IRP, NTSTATUS status, ULONG_PTR info);

NTSTATUS LoggerCreateClose(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP IRP) 
{
	UNREFERENCED_PARAMETER(DeviceObject);
	return CompleteIRP(IRP, STATUS_SUCCESS, 0);
}

NTSTATUS LoggerDeviceControl(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP IRP) 
{
	UNREFERENCED_PARAMETER(DeviceObject);
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(IRP);
	NTSTATUS status = STATUS_SUCCESS;
	switch (stack->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_RLOGGER_SET_TARGET_PID:
		if (stack->Parameters.DeviceIoControl.InputBufferLength < sizeof(ULONG)) 
		{
			status = STATUS_BUFFER_TOO_SMALL;
			break;
		}
		g_Globals.targetPID = *(ULONG*)stack->Parameters.DeviceIoControl.Type3InputBuffer;
		g_Globals.isLoggingActive = TRUE;
		break;
	case IOCTL_RLOGGER_STOP_LOGGING:
		g_Globals.isLoggingActive = FALSE;
		break;
	default:
		status = STATUS_INVALID_DEVICE_REQUEST;
		break;
	}
	return CompleteIRP(IRP, status, 0);
}

NTSTATUS CompleteIRP(PIRP IRP, NTSTATUS status, ULONG_PTR info) {
	IRP->IoStatus.Status = status;
	IRP->IoStatus.Information = info;
	IoCompleteRequest(IRP, IO_NO_INCREMENT);
	return status;
}