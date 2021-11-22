#pragma once

#include <ntddk.h>

NTSTATUS LoggerCreateClose(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP IRP);
NTSTATUS LoggerDeviceControl(_In_ PDEVICE_OBJECT DeviceObject, _In_ PIRP IRP);