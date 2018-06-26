#ifndef __VXSYSCALL_H__
#define __VXSYSCALL_H__

#define VXSYSCALL_PRINTSTR		4096	// print(char *str);	//ecx=str
#define VXSYSCALL_MALLOC		4097	// malloc(int sz);
#define VXSYSCALL_FREE			4098	// free(void *ptr);
#define VXSYSCALL_READ			4099	// read(int fd, void *buf, int sz);
#define VXSYSCALL_WRITE			4100	// write(int fd, void *buf, int sz);
#define VXSYSCALL_OPEN			4101	// open(char *name, int flag, int mode);
#define VXSYSCALL_CLOSE			4102	// close(int fd);
#define VXSYSCALL_LSEEK			4103	// lseek(int fd, int offs, int mode);

#define VXSYSCALL_UNLINK		4104	// unlink(char *fd);
#define VXSYSCALL_FORK			4105	// fork();
#define VXSYSCALL_EXIT			4106	// exit(int ret);
#define VXSYSCALL_LLSEEK		4107	// llseek(int fd, s64 offs, int mode);

#define VXSYSCALL_LOADLIBRARY		4108	// HMODULE LoadLibrary(char *name);
#define VXSYSCALL_GETPROCADDRESS	4109	// LPVOID GetProcAddress(HMODULE lib, char *name);
#define VXSYSCALL_GETMODULEHANDLE	4110	// HMODULE GetModuleHandle(char *name);
#define VXSYSCALL_NATIVECALL		4111	// NativeCall(char *name, void *args);
#define VXSYSCALL_NATIVECALL2		4112	// NativeCall2(char *name, char *sig, void *args);
#define VXSYSCALL_METAGETKEY		4113	// char *MetaGetKey(char *key);
#define VXSYSCALL_METASETKEY		4114	// int MetaSetKey(char *key, char *val);

#define VXSYSCALL_INTTAB		4127	// IntTab(int idx, s64 *args, int nargs);
#define VXSYSCALL_EOF			4128	// int eof(int fd);
#define VXSYSCALL_TELL			4129	// long tell(int fd);
#define VXSYSCALL_IOCTL			4130	// int ioctl(int fd, int req, char *argp);

#define VXSYSCALL_OPENDIR		4131	// void *opendir(char *name);
#define VXSYSCALL_CLOSEDIR		4132	// void closedir(void *hdl);
#define VXSYSCALL_READDIR		4133	// char *readdir(void *hdl);
#define VXSYSCALL_REWINDDIR		4134	// void rewinddir(void *hdl);

#define VXSYSCALL_MMAP			4135	// void *mmap(void *addr, size_t len, ...);
#define VXSYSCALL_MPROTECT		4136	// void mprotect(void *addr, size_t len, int prot);
#define VXSYSCALL_MSYNC			4137	// void msync(void *addr, size_t len, int flag);
#define VXSYSCALL_MUNMAP		4138	// void munmap(void *addr, size_t len);

#define VXSYSCALL_TIMENS		4139	// int vx_timens(s64 *rsec, s32 *nsec);
#define VXSYSCALL_NSLEEP		4140	// int vx_nsleep(s64 nsec);
#define VXSYSCALL_DEVIOCTRL		4141	// int vx_nsleep(s64 nsec);

//4160-4176: socket
#define VXSYSCALL_RECVFROM		4160
#define VXSYSCALL_SENDTO		4161
#define VXSYSCALL_BIND			4162
#define VXSYSCALL_SOCKET		4163
#define VXSYSCALL_CONNECT		4164
#define VXSYSCALL_ACCEPT		4165
#define VXSYSCALL_LISTEN		4166


#define VXSYSCALL_JNI_BASE			7936
#define VXSYSCALL_JNI_END			8192

#define VXSYSCALL_JNI_GetVersion		(VXSYSCALL_JNI_BASE+4)
#define VXSYSCALL_JNI_DefineClass		(VXSYSCALL_JNI_BASE+5)
#define VXSYSCALL_JNI_FindClass			(VXSYSCALL_JNI_BASE+6)
#define VXSYSCALL_JNI_FromReflectedMethod	(VXSYSCALL_JNI_BASE+7)
#define VXSYSCALL_JNI_FromReflectedField	(VXSYSCALL_JNI_BASE+8)
#define VXSYSCALL_JNI_ToReflectedMethod		(VXSYSCALL_JNI_BASE+9)

#define VXSYSCALL_JNI_GetSuperclass		(VXSYSCALL_JNI_BASE+10)
#define VXSYSCALL_JNI_IsAssignableFrom		(VXSYSCALL_JNI_BASE+11)
#define VXSYSCALL_JNI_ToReflectedField		(VXSYSCALL_JNI_BASE+12)

#define VXSYSCALL_JNI_Throw			(VXSYSCALL_JNI_BASE+13)
#define VXSYSCALL_JNI_ThrowNew			(VXSYSCALL_JNI_BASE+14)
#define VXSYSCALL_JNI_ExceptionOccurred		(VXSYSCALL_JNI_BASE+15)
#define VXSYSCALL_JNI_ExceptionDescribe		(VXSYSCALL_JNI_BASE+16)
#define VXSYSCALL_JNI_ExceptionClear		(VXSYSCALL_JNI_BASE+17)
#define VXSYSCALL_JNI_FatalError		(VXSYSCALL_JNI_BASE+18)

#define VXSYSCALL_JNI_PushLocalFrame		(VXSYSCALL_JNI_BASE+19)
#define VXSYSCALL_JNI_PopLocalFrame		(VXSYSCALL_JNI_BASE+20)

#define VXSYSCALL_JNI_NewGlobalRef		(VXSYSCALL_JNI_BASE+21)
#define VXSYSCALL_JNI_DeleteGlobalRef		(VXSYSCALL_JNI_BASE+22)
#define VXSYSCALL_JNI_DeleteLocalRef		(VXSYSCALL_JNI_BASE+23)
#define VXSYSCALL_JNI_IsSameObject		(VXSYSCALL_JNI_BASE+24)

#define VXSYSCALL_JNI_NewLocalRef		(VXSYSCALL_JNI_BASE+25)
#define VXSYSCALL_JNI_EnsureLocalCapacity	(VXSYSCALL_JNI_BASE+26)

#define VXSYSCALL_JNI_AllocObject		(VXSYSCALL_JNI_BASE+27)
#define VXSYSCALL_JNI_NewObjectA		(VXSYSCALL_JNI_BASE+30)

#define VXSYSCALL_JNI_GetObjectClass		(VXSYSCALL_JNI_BASE+31)
#define VXSYSCALL_JNI_IsInstanceOf		(VXSYSCALL_JNI_BASE+32)

#define VXSYSCALL_JNI_GetMethodID		(VXSYSCALL_JNI_BASE+33)

#define VXSYSCALL_JNI_CallObjectMethodA		(VXSYSCALL_JNI_BASE+36)
#define VXSYSCALL_JNI_CallBooleanMethodA	(VXSYSCALL_JNI_BASE+39)
#define VXSYSCALL_JNI_CallByteMethodA		(VXSYSCALL_JNI_BASE+42)
#define VXSYSCALL_JNI_CallCharMethodA		(VXSYSCALL_JNI_BASE+45)
#define VXSYSCALL_JNI_CallShortMethodA		(VXSYSCALL_JNI_BASE+48)
#define VXSYSCALL_JNI_CallIntMethodA		(VXSYSCALL_JNI_BASE+51)
#define VXSYSCALL_JNI_CallLongMethodA		(VXSYSCALL_JNI_BASE+54)
#define VXSYSCALL_JNI_CallFloatMethodA		(VXSYSCALL_JNI_BASE+57)
#define VXSYSCALL_JNI_CallDoubleMethodA		(VXSYSCALL_JNI_BASE+60)
#define VXSYSCALL_JNI_CallVoidMethodA		(VXSYSCALL_JNI_BASE+63)

#define VXSYSCALL_JNI_CallNonvirtualObjectMethodA	(VXSYSCALL_JNI_BASE+66)
#define VXSYSCALL_JNI_CallNonvirtualBooleanMethodA	(VXSYSCALL_JNI_BASE+69)
#define VXSYSCALL_JNI_CallNonvirtualByteMethodA		(VXSYSCALL_JNI_BASE+72)
#define VXSYSCALL_JNI_CallNonvirtualCharMethodA		(VXSYSCALL_JNI_BASE+75)
#define VXSYSCALL_JNI_CallNonvirtualShortMethodA	(VXSYSCALL_JNI_BASE+78)
#define VXSYSCALL_JNI_CallNonvirtualIntMethodA		(VXSYSCALL_JNI_BASE+81)
#define VXSYSCALL_JNI_CallNonvirtualLongMethodA		(VXSYSCALL_JNI_BASE+84)
#define VXSYSCALL_JNI_CallNonvirtualFloatMethodA	(VXSYSCALL_JNI_BASE+87)
#define VXSYSCALL_JNI_CallNonvirtualDoubleMethodA	(VXSYSCALL_JNI_BASE+90)
#define VXSYSCALL_JNI_CallNonvirtualVoidMethodA		(VXSYSCALL_JNI_BASE+93)

#define VXSYSCALL_JNI_GetFieldID		(VXSYSCALL_JNI_BASE+94)
#define VXSYSCALL_JNI_GetObjectField		(VXSYSCALL_JNI_BASE+95)
#define VXSYSCALL_JNI_GetBooleanField		(VXSYSCALL_JNI_BASE+96)
#define VXSYSCALL_JNI_GetByteField		(VXSYSCALL_JNI_BASE+97)
#define VXSYSCALL_JNI_GetCharField		(VXSYSCALL_JNI_BASE+98)
#define VXSYSCALL_JNI_GetShortField		(VXSYSCALL_JNI_BASE+99)
#define VXSYSCALL_JNI_GetIntField		(VXSYSCALL_JNI_BASE+100)
#define VXSYSCALL_JNI_GetLongField		(VXSYSCALL_JNI_BASE+101)
#define VXSYSCALL_JNI_GetFloatField		(VXSYSCALL_JNI_BASE+102)
#define VXSYSCALL_JNI_GetDoubleField		(VXSYSCALL_JNI_BASE+103)

#define VXSYSCALL_JNI_SetObjectField		(VXSYSCALL_JNI_BASE+104)
#define VXSYSCALL_JNI_SetBooleanField		(VXSYSCALL_JNI_BASE+105)
#define VXSYSCALL_JNI_SetByteField		(VXSYSCALL_JNI_BASE+106)
#define VXSYSCALL_JNI_SetCharField		(VXSYSCALL_JNI_BASE+107)
#define VXSYSCALL_JNI_SetShortField		(VXSYSCALL_JNI_BASE+108)
#define VXSYSCALL_JNI_SetIntField		(VXSYSCALL_JNI_BASE+109)
#define VXSYSCALL_JNI_SetLongField		(VXSYSCALL_JNI_BASE+110)
#define VXSYSCALL_JNI_SetFloatField		(VXSYSCALL_JNI_BASE+111)
#define VXSYSCALL_JNI_SetDoubleField		(VXSYSCALL_JNI_BASE+112)

#define VXSYSCALL_JNI_GetStaticMethodID		(VXSYSCALL_JNI_BASE+113)

#define VXSYSCALL_JNI_CallStaticObjectMethodA	(VXSYSCALL_JNI_BASE+116)
#define VXSYSCALL_JNI_CallStaticBooleanMethodA	(VXSYSCALL_JNI_BASE+119)
#define VXSYSCALL_JNI_CallStaticByteMethodA	(VXSYSCALL_JNI_BASE+122)
#define VXSYSCALL_JNI_CallStaticCharMethodA	(VXSYSCALL_JNI_BASE+125)
#define VXSYSCALL_JNI_CallStaticShortMethodA	(VXSYSCALL_JNI_BASE+128)
#define VXSYSCALL_JNI_CallStaticIntMethodA	(VXSYSCALL_JNI_BASE+131)
#define VXSYSCALL_JNI_CallStaticLongMethodA	(VXSYSCALL_JNI_BASE+134)
#define VXSYSCALL_JNI_CallStaticFloatMethodA	(VXSYSCALL_JNI_BASE+137)
#define VXSYSCALL_JNI_CallStaticDoubleMethodA	(VXSYSCALL_JNI_BASE+140)       
#define VXSYSCALL_JNI_CallStaticVoidMethodA	(VXSYSCALL_JNI_BASE+143)

#define VXSYSCALL_JNI_GetStaticFieldID		(VXSYSCALL_JNI_BASE+144)
#define VXSYSCALL_JNI_GetStaticObjectField	(VXSYSCALL_JNI_BASE+145)
#define VXSYSCALL_JNI_GetStaticBooleanField	(VXSYSCALL_JNI_BASE+146)
#define VXSYSCALL_JNI_GetStaticByteField	(VXSYSCALL_JNI_BASE+147)
#define VXSYSCALL_JNI_GetStaticCharField	(VXSYSCALL_JNI_BASE+148)
#define VXSYSCALL_JNI_GetStaticShortField	(VXSYSCALL_JNI_BASE+149)
#define VXSYSCALL_JNI_GetStaticIntField		(VXSYSCALL_JNI_BASE+150)
#define VXSYSCALL_JNI_GetStaticLongField	(VXSYSCALL_JNI_BASE+151)
#define VXSYSCALL_JNI_GetStaticFloatField	(VXSYSCALL_JNI_BASE+152)
#define VXSYSCALL_JNI_GetStaticDoubleField	(VXSYSCALL_JNI_BASE+153)

#define VXSYSCALL_JNI_SetStaticObjectField	(VXSYSCALL_JNI_BASE+154)
#define VXSYSCALL_JNI_SetStaticBooleanField	(VXSYSCALL_JNI_BASE+155)
#define VXSYSCALL_JNI_SetStaticByteField	(VXSYSCALL_JNI_BASE+156)
#define VXSYSCALL_JNI_SetStaticCharField	(VXSYSCALL_JNI_BASE+157)
#define VXSYSCALL_JNI_SetStaticShortField	(VXSYSCALL_JNI_BASE+158)
#define VXSYSCALL_JNI_SetStaticIntField		(VXSYSCALL_JNI_BASE+159)
#define VXSYSCALL_JNI_SetStaticLongField	(VXSYSCALL_JNI_BASE+160)
#define VXSYSCALL_JNI_SetStaticFloatField	(VXSYSCALL_JNI_BASE+161)
#define VXSYSCALL_JNI_SetStaticDoubleField	(VXSYSCALL_JNI_BASE+162)

#define VXSYSCALL_JNI_NewString			(VXSYSCALL_JNI_BASE+163)
#define VXSYSCALL_JNI_GetStringLength		(VXSYSCALL_JNI_BASE+164)
#define VXSYSCALL_JNI_GetStringChars		(VXSYSCALL_JNI_BASE+165)
#define VXSYSCALL_JNI_ReleaseStringChars	(VXSYSCALL_JNI_BASE+166)
  
#define VXSYSCALL_JNI_NewStringUTF		(VXSYSCALL_JNI_BASE+167)
#define VXSYSCALL_JNI_GetStringUTFLength	(VXSYSCALL_JNI_BASE+168)
#define VXSYSCALL_JNI_GetStringUTFChars		(VXSYSCALL_JNI_BASE+169)
#define VXSYSCALL_JNI_ReleaseStringUTFChars	(VXSYSCALL_JNI_BASE+170)
  
#define VXSYSCALL_JNI_GetArrayLength		(VXSYSCALL_JNI_BASE+171)
#define VXSYSCALL_JNI_NewObjectArray		(VXSYSCALL_JNI_BASE+172)
#define VXSYSCALL_JNI_GetObjectArrayElement	(VXSYSCALL_JNI_BASE+173)
#define VXSYSCALL_JNI_SetObjectArrayElement	(VXSYSCALL_JNI_BASE+174)

#define VXSYSCALL_JNI_NewBooleanArray		(VXSYSCALL_JNI_BASE+175)
#define VXSYSCALL_JNI_NewByteArray		(VXSYSCALL_JNI_BASE+176)
#define VXSYSCALL_JNI_NewCharArray		(VXSYSCALL_JNI_BASE+177)
#define VXSYSCALL_JNI_NewShortArray		(VXSYSCALL_JNI_BASE+178)
#define VXSYSCALL_JNI_NewIntArray		(VXSYSCALL_JNI_BASE+179)
#define VXSYSCALL_JNI_NewLongArray		(VXSYSCALL_JNI_BASE+180)
#define VXSYSCALL_JNI_NewFloatArray		(VXSYSCALL_JNI_BASE+181)
#define VXSYSCALL_JNI_NewDoubleArray		(VXSYSCALL_JNI_BASE+182)

#define VXSYSCALL_JNI_GetBooleanArrayElements	(VXSYSCALL_JNI_BASE+183)
#define VXSYSCALL_JNI_GetByteArrayElements	(VXSYSCALL_JNI_BASE+184)
#define VXSYSCALL_JNI_GetCharArrayElements	(VXSYSCALL_JNI_BASE+185)
#define VXSYSCALL_JNI_GetShortArrayElements	(VXSYSCALL_JNI_BASE+186)
#define VXSYSCALL_JNI_GetIntArrayElements	(VXSYSCALL_JNI_BASE+187)
#define VXSYSCALL_JNI_GetLongArrayElements	(VXSYSCALL_JNI_BASE+188)
#define VXSYSCALL_JNI_GetFloatArrayElements	(VXSYSCALL_JNI_BASE+189)
#define VXSYSCALL_JNI_GetDoubleArrayElements	(VXSYSCALL_JNI_BASE+190)

#define VXSYSCALL_JNI_ReleaseBooleanArrayElements	(VXSYSCALL_JNI_BASE+191)
#define VXSYSCALL_JNI_ReleaseByteArrayElements		(VXSYSCALL_JNI_BASE+192)
#define VXSYSCALL_JNI_ReleaseCharArrayElements		(VXSYSCALL_JNI_BASE+193)
#define VXSYSCALL_JNI_ReleaseShortArrayElements		(VXSYSCALL_JNI_BASE+194)
#define VXSYSCALL_JNI_ReleaseIntArrayElements		(VXSYSCALL_JNI_BASE+195)
#define VXSYSCALL_JNI_ReleaseLongArrayElements		(VXSYSCALL_JNI_BASE+196)
#define VXSYSCALL_JNI_ReleaseFloatArrayElements		(VXSYSCALL_JNI_BASE+197)
#define VXSYSCALL_JNI_ReleaseDoubleArrayElements	(VXSYSCALL_JNI_BASE+198)

#define VXSYSCALL_JNI_GetBooleanArrayRegion	(VXSYSCALL_JNI_BASE+199)
#define VXSYSCALL_JNI_GetByteArrayRegion	(VXSYSCALL_JNI_BASE+200)
#define VXSYSCALL_JNI_GetCharArrayRegion	(VXSYSCALL_JNI_BASE+201)
#define VXSYSCALL_JNI_GetShortArrayRegion	(VXSYSCALL_JNI_BASE+202)
#define VXSYSCALL_JNI_GetIntArrayRegion		(VXSYSCALL_JNI_BASE+203)
#define VXSYSCALL_JNI_GetLongArrayRegion	(VXSYSCALL_JNI_BASE+204)
#define VXSYSCALL_JNI_GetFloatArrayRegion	(VXSYSCALL_JNI_BASE+205)
#define VXSYSCALL_JNI_GetDoubleArrayRegion	(VXSYSCALL_JNI_BASE+206)

#define VXSYSCALL_JNI_SetBooleanArrayRegion	(VXSYSCALL_JNI_BASE+207)
#define VXSYSCALL_JNI_SetByteArrayRegion	(VXSYSCALL_JNI_BASE+208)
#define VXSYSCALL_JNI_SetCharArrayRegion	(VXSYSCALL_JNI_BASE+209)
#define VXSYSCALL_JNI_SetShortArrayRegion	(VXSYSCALL_JNI_BASE+210)
#define VXSYSCALL_JNI_SetIntArrayRegion		(VXSYSCALL_JNI_BASE+211)
#define VXSYSCALL_JNI_SetLongArrayRegion	(VXSYSCALL_JNI_BASE+212)
#define VXSYSCALL_JNI_SetFloatArrayRegion	(VXSYSCALL_JNI_BASE+213)
#define VXSYSCALL_JNI_SetDoubleArrayRegion	(VXSYSCALL_JNI_BASE+214)

#define VXSYSCALL_JNI_RegisterNatives		(VXSYSCALL_JNI_BASE+215)
#define VXSYSCALL_JNI_UnregisterNatives		(VXSYSCALL_JNI_BASE+216)

#define VXSYSCALL_JNI_MonitorEnter		(VXSYSCALL_JNI_BASE+217)
#define VXSYSCALL_JNI_MonitorExit		(VXSYSCALL_JNI_BASE+218)
 
#define VXSYSCALL_JNI_GetJavaVM			(VXSYSCALL_JNI_BASE+219)

#define VXSYSCALL_JNI_GetStringRegion		(VXSYSCALL_JNI_BASE+220)
#define VXSYSCALL_JNI_GetStringUTFRegion	(VXSYSCALL_JNI_BASE+221)

#define VXSYSCALL_JNI_GetPrimitiveArrayCritical		(VXSYSCALL_JNI_BASE+222)
#define VXSYSCALL_JNI_ReleasePrimitiveArrayCritical	(VXSYSCALL_JNI_BASE+223)

#define VXSYSCALL_JNI_GetStringCritical		(VXSYSCALL_JNI_BASE+224)
#define VXSYSCALL_JNI_ReleaseStringCritical	(VXSYSCALL_JNI_BASE+225)
#define VXSYSCALL_JNI_NewWeakGlobalRef		(VXSYSCALL_JNI_BASE+226)
#define VXSYSCALL_JNI_DeleteWeakGlobalRef	(VXSYSCALL_JNI_BASE+227)
#define VXSYSCALL_JNI_ExceptionCheck		(VXSYSCALL_JNI_BASE+228)
#define VXSYSCALL_JNI_NewDirectByteBuffer	(VXSYSCALL_JNI_BASE+229)
#define VXSYSCALL_JNI_GetDirectBufferAddress	(VXSYSCALL_JNI_BASE+230)
#define VXSYSCALL_JNI_GetDirectBufferCapacity	(VXSYSCALL_JNI_BASE+231)


#endif
