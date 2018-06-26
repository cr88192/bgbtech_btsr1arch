global __printstr
global __allocmem
global __freemem

global __read
global __write
global __open
global __close
global __lseek
global __unlink
global __fork
global __exit
global __llseek

global __IntTab

global __eof
global __tell

global __LoadLibrary
global __GetProcAddress
global __GetModuleHandle
global __NativeCall
global __NativeCall2
global __MetaGetKey
global __MetaSetKey

%macro intxt 1
db 0x0F, 0x3F
dw %1
%endmacro

[section .text]
[bits 32]


__printstr:
mov ecx, [esp+4]
intxt 4096
ret

__allocmem:
mov eax, [esp+4]
intxt 4097
ret

__freemem:
mov eax, [esp+4]
intxt 4098
ret

__read:
mov eax, [esp+4]
mov ecx, [esp+8]
mov edx, [esp+12]
intxt 4099
ret

__write:
mov eax, [esp+4]
mov ecx, [esp+8]
mov edx, [esp+12]
intxt 4100
ret

__open:
mov eax, [esp+4]
mov ecx, [esp+8]
mov edx, [esp+12]
intxt 4101
ret

__close:
mov eax, [esp+4]
intxt 4102
ret

__lseek:
mov eax, [esp+4]
mov ecx, [esp+8]
mov edx, [esp+12]
intxt 4103
ret

__unlink:
mov eax, [esp+4]
intxt 4102
ret

__fork:
intxt 4105
ret

__exit:
mov eax, [esp+4]
intxt 4106
ret

__llseek:
mov eax, [esp+4]
movq xmm0, [esp+8]
mov ecx, [esp+16]
intxt 4107
ret

__LoadLibrary:
mov eax, [esp+4]
intxt 4108
ret

__GetProcAddress:
mov eax, [esp+4]
mov ecx, [esp+8]
intxt 4109
ret

__GetModuleHandle:
mov eax, [esp+4]
intxt 4110
ret

__NativeCall:
mov eax, [esp+4]
mov ecx, [esp+8]
intxt 4111
ret

__NativeCall2:
mov eax, [esp+4]
mov ecx, [esp+8]
mov edx, [esp+12]
intxt 4112
ret

__MetaGetKey:
mov eax, [esp+4]
intxt 4113
ret

__MetaSetKey:
mov eax, [esp+4]
mov ecx, [esp+8]
intxt 4114
ret


__IntTab:
mov eax, [esp+4]
mov ecx, [esp+8]
mov edx, [esp+12]
intxt 4127
ret

__eof:
mov eax, [esp+4]
intxt 4128
ret

__tell:
mov eax, [esp+4]
intxt 4129
ret
