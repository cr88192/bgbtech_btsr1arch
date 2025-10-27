typedef void *TKEFI_HANDLE;
typedef void *TKEFI_EVENT;

typedef u16 TKEFI_CHAR16;
typedef u16 TKEFI_U16;
typedef u32 TKEFI_U32;
typedef u64 TKEFI_U64;

typedef s16 TKEFI_S16;
typedef s32 TKEFI_S32;
typedef s64 TKEFI_S64;

struct TKEFI_TABLE_HEADER_s {
u64 signature;		//00
u32 revision;		//08
u32 headerSize;		//0C
u32 crc32;			//10
u32 resv;			//14
//18
};

typedef struct TKEFI_INPUT_KEY_s					TKEFI_INPUT_KEY;
typedef struct TKEFI_SIMPLE_TEXT_OUTPUT_MODE_s		TKEFI_SIMPLE_TEXT_OUTPUT_MODE;

struct TKEFI_INPUT_KEY_s {
TKEFI_U16		scanCode;
TKEFI_CHAR16	unicodeChar;
};

typedef struct TKEFI_SIMPLE_TEXT_OUTPUT_MODE_s {
TKEFI_S32	maxMode;
TKEFI_S32	mode;
TKEFI_S32	attribute;
TKEFI_S32	cursorColumn;
TKEFI_S32	cursorRow;
TKEFI_BOOL	cursorVisible;
};

struct TKEFI_SIMPLE_TEXT_INPUT_PROTOCOL_s {
TKEFI_STATUS	(*Reset)(
	TKEFI_SIMPLE_TEXT_INPUT_PROTOCOL	*self,
	TKEFI_BOOL							extVerify);
TKEFI_STATUS	(*ReadKeyStroke)(
	TKEFI_SIMPLE_TEXT_INPUT_PROTOCOL	*self,
	TKEFI_INPUT_KEY						*key);
TKEFI_EVENT             waitForKey;
};

struct TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_s {
TKEFI_STATUS		(*Reset)(
	TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*self,
	TKEFI_BOOL							extVerify);
TKEFI_STATUS		(*OutputString)(
	TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*self,
	TKEFI_CHAR16						*str);
TKEFI_STATUS		(*TestString)(
	TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*self,
	TKEFI_CHAR16						*str);
TKEFI_STATUS		(*QueryMode)(
	TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*self,
	TKEFI_UINT							mode,
	TKEFI_UINT							*cols,
	TKEFI_UINT							*rows);
TKEFI_STATUS		(*SetMode)(
	TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*self,
	TKEFI_UINT							mode);
TKEFI_STATUS		(*SetAttribute)(
	TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*self,
	TKEFI_UINT							attrib);
TKEFI_STATUS		(*ClearScreen)(
	TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*self);
TKEFI_STATUS		(*SetCursorPosition)(
	TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*self,
	TKEFI_UINT							col,
	TKEFI_UINT							row);
TKEFI_STATUS		(*EnableCursor)(
	TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*self,
	TKEFI_BOOL							visible);
TKEFI_SIMPLE_TEXT_OUTPUT_MODE	*mode;
 };

typedef struct EFI_BOOT_SERVICES_s TKEFI_BOOT_SERVICES;

struct EFI_BOOT_SERVICES_s {
   EFI_TABLE_HEADER                              Hdr;
   EFI_RAISE_TPL                                 RaiseTPL;
   EFI_RESTORE_TPL                               RestoreTPL;
   EFI_ALLOCATE_PAGES                            AllocatePages;
   EFI_FREE_PAGES                                FreePages;
   EFI_GET_MEMORY_MAP                            GetMemoryMap;
   EFI_ALLOCATE_POOL                             AllocatePool;
   EFI_FREE_POOL                                 FreePool;
   EFI_CREATE_EVENT                              CreateEvent;
   EFI_SET_TIMER                                 SetTimer;
   EFI_WAIT_FOR_EVENT                            WaitForEvent;
   EFI_SIGNAL_EVENT                              SignalEvent;
   EFI_CLOSE_EVENT                               CloseEvent;
   EFI_CHECK_EVENT                               CheckEvent;
   EFI_INSTALL_PROTOCOL_INTERFACE                InstallProtocolInterface;
   EFI_REINSTALL_PROTOCOL_INTERFACE              ReinstallProtocolInterface;
   EFI_UNINSTALL_PROTOCOL_INTERFACE              UninstallProtocolInterface;
   EFI_HANDLE_PROTOCOL                           HandleProtocol;
   VOID                                          *Reserved;
   EFI_REGISTER_PROTOCOL_NOTIFY                  RegisterProtocolNotify;
   EFI_LOCATE_HANDLE                             LocateHandle;
   EFI_LOCATE_DEVICE_PATH                        LocateDevicePath;
   EFI_INSTALL_CONFIGURATION_TABLE               InstallConfigurationTable;
   EFI_IMAGE_LOAD                                LoadImage;
   EFI_IMAGE_START                               StartImage;
   EFI_EXIT                                      Exit;
   EFI_IMAGE_UNLOAD                              UnloadImage;
   EFI_EXIT_BOOT_SERVICES                        ExitBootServices;
   EFI_GET_NEXT_MONOTONIC_COUNT                  GetNextMonotonicCount;
   EFI_STALL                                     Stall;
   EFI_SET_WATCHDOG_TIMER                        SetWatchdogTimer;
   EFI_CONNECT_CONTROLLER                        ConnectController;
   EFI_DISCONNECT_CONTROLLER                     DisconnectController;
   EFI_OPEN_PROTOCOL                             OpenProtocol;
   EFI_CLOSE_PROTOCOL                            CloseProtocol;
   EFI_OPEN_PROTOCOL_INFORMATION                 OpenProtocolInformation;
   EFI_PROTOCOLS_PER_HANDLE                      ProtocolsPerHandle;
   EFI_LOCATE_HANDLE_BUFFER                      LocateHandleBuffer;
   EFI_LOCATE_PROTOCOL                           LocateProtocol;
   EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES      InstallMultipleProtocolInterfaces;
   EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES    UninstallMultipleProtocolInterfaces;
 
   EFI_CALCULATE_CRC32                           CalculateCrc32;
   EFI_COPY_MEM                                  CopyMem;
   EFI_SET_MEM                                   SetMem;
   EFI_CREATE_EVENT_EX                           CreateEventEx;
};

typedef struct TKEFI_RUNTIME_SERVICES_s TKEFI_RUNTIME_SERVICES;

struct TKEFI_RUNTIME_SERVICES_s {
EFI_TABLE_HEADER                  Hdr;
EFI_GET_TIME                      GetTime;
EFI_SET_TIME                      SetTime;
EFI_GET_WAKEUP_TIME               GetWakeupTime;
EFI_SET_WAKEUP_TIME               SetWakeupTime;
EFI_SET_VIRTUAL_ADDRESS_MAP       SetVirtualAddressMap;
EFI_CONVERT_POINTER               ConvertPointer;
EFI_GET_VARIABLE                  GetVariable;
EFI_GET_NEXT_VARIABLE_NAME        GetNextVariableName;
EFI_SET_VARIABLE                  SetVariable;
EFI_GET_NEXT_HIGH_MONO_COUNT      GetNextHighMonotonicCount;
EFI_RESET_SYSTEM                  ResetSystem;
EFI_UPDATE_CAPSULE                UpdateCapsule;
EFI_QUERY_CAPSULE_CAPABILITIES    QueryCapsuleCapabilities;
EFI_QUERY_VARIABLE_INFO           QueryVariableInfo;
};
 
struct TKEFI_SYSTEM_TABLE_s {
TKEFI_TABLE_HEADER					hdr;
TKEFI_CHAR16						*firmwareVendor;
TKEFI_U32 							firmwareRevision;
TKEFI_HANDLE				 		consoleInHandle;
TKEFI_SIMPLE_TEXT_INPUT_PROTOCOL	*conIn;
TKEFI_HANDLE 						consoleOutHandle;
TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*conOut; 
TKEFI_HANDLE						standardErrorHandle;
TKEFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*stdErr;
TKEFI_RUNTIME_SERVICES				*runtimeServices;
TKEFI_BOOT_SERVICES					*bootServices;
TKEFI_U32 							numberOfTableEntries;
TKEFI_CONFIGURATION_TABLE			*configurationTable;
};
