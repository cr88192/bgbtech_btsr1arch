/*
legal status uncertain, derived in large part from Apache's jni.h
 */

#ifndef JNIWRAP_H
#define JNIWRAP_H

#include <jni.h>

#ifdef __cplusplus
#define JNIENV env
#else
#define JNIENV (*env)
#endif

static inline jint jGetVersion(JNIEnv *env)
	{ return JNIENV->GetVersion(env); }
static inline jclass jDefineClass(JNIEnv *env, const char *name, 
		jobject loader, const jbyte *buf, jsize len)
	{ return JNIENV->DefineClass(env, name, loader, buf, len); }
static inline jclass jFindClass(JNIEnv *env, const char *name)
	{ return JNIENV->FindClass(env, name); }
static inline jmethodID jFromReflectedMethod(JNIEnv *env, jobject mth)
	{ return JNIENV->FromReflectedMethod(env, mth); }
static inline jfieldID jFromReflectedField(JNIEnv *env, jobject fld)
	{ return JNIENV->FromReflectedField(env, fld); }
static inline jobject jToReflectedMethod(JNIEnv *env, 
		jclass cls, jmethodID mth, jboolean staticP)
	{ return JNIENV->ToReflectedMethod(env, cls, mth, staticP); }
static inline jclass jGetSuperclass(JNIEnv *env, jclass sub)
	{ return JNIENV->GetSuperclass(env, sub); }
static inline jboolean jIsAssignableFrom(JNIEnv *env, 
		jclass sub, jclass sup)
	{ return JNIENV->IsAssignableFrom(env, sub, sup); }
static inline jobject jToReflectedField(JNIEnv *env, 
		jclass cls, jfieldID id, jboolean staticP)
	{ return JNIENV->ToReflectedField(env, cls, id, staticP); }

static inline jint jThrow(JNIEnv *env, jthrowable obj)
	{ return JNIENV->Throw(env, obj); }
static inline jint jThrowNew(JNIEnv *env, jclass cls, const char *msg)
	{ return JNIENV->ThrowNew(env, cls, msg); }
static inline jthrowable jExceptionOccurred(JNIEnv *env)
	{ return JNIENV->ExceptionOccurred(env); }
static inline void jExceptionDescribe(JNIEnv *env)
	{ JNIENV->ExceptionDescribe(env); }
static inline void jExceptionClear(JNIEnv *env)
	{ JNIENV->ExceptionClear(env); }
static inline void jFatalError(JNIEnv *env, const char *msg)
	{ JNIENV->FatalError(env, msg); }

static inline jint jPushLocalFrame(JNIEnv *env, jint cap)
	{ return JNIENV->PushLocalFrame(env, cap); }
static inline jobject jPopLocalFrame(JNIEnv *env, jobject res)
	{ return JNIENV->PopLocalFrame(env, res); }

static inline jobject jNewGlobalRef(JNIEnv *env, jobject lobj)
	{ return JNIENV->NewGlobalRef(env, lobj); }
static inline void jDeleteGlobalRef(JNIEnv *env, jobject gref)
	{ JNIENV->DeleteGlobalRef(env, gref); }
static inline void jDeleteLocalRef(JNIEnv *env, jobject obj)
	{ JNIENV->DeleteLocalRef(env, obj); }
static inline jboolean jIsSameObject(JNIEnv *env, jobject obj1, jobject obj2)
	{ return JNIENV->IsSameObject(env, obj1, obj2); }
static inline jobject jNewLocalRef(JNIEnv *env, jobject ref)
	{ return JNIENV->NewLocalRef(env, ref); }

static inline jint jEnsureLocalCapacity(JNIEnv *env, jint cap)
	{ return JNIENV->EnsureLocalCapacity(env, cap); }

static inline jobject jAllocObject(JNIEnv *env, jclass cls)
	{ return JNIENV->AllocObject(env, cls); }

static inline jobject jNewObject(JNIEnv *env, jclass cls, jmethodID mth, ...)
{
	va_list args;
	jobject val;
	va_start(args, mth);
	val=JNIENV->NewObjectV(env, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jobject jNewObjectV(JNIEnv *env, 
		jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->NewObjectV(env, cls, mth, args); }
static inline jobject jNewObjectA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->NewObjectA(env, cls, mth, args); }

static inline jclass jGetObjectClass(JNIEnv *env, jobject obj)
	{ return JNIENV->GetObjectClass(env, obj); }
static inline jboolean jIsInstanceOf(JNIEnv *env, jobject obj, jclass cls)
	{ return JNIENV->IsInstanceOf(env, obj, cls); }

static inline jmethodID jGetMethodID(JNIEnv *env, 
		jclass cls, const char *name, const char *sig)
	{ return JNIENV->GetMethodID(env, cls, name, sig); }

static inline jobject jCallObjectMethod(JNIEnv *env, 
	jobject obj, jmethodID mth, ...)
{
	va_list args;
	jobject val;
	va_start(args, mth);
	val=JNIENV->CallObjectMethodV(env, obj, mth, args);
	va_end(args);
	return(val);
}

static inline jobject jCallObjectMethodV(JNIEnv *env, 
		jobject obj, jmethodID mth, va_list args)
	{ return JNIENV->CallObjectMethodV(env, obj, mth, args); }
static inline jobject jCallObjectMethodA(JNIEnv *env, 
		jobject obj, jmethodID mth, jvalue *args)
	{ return JNIENV->CallObjectMethodA(env, obj, mth, args); }

static inline jboolean jCallBooleanMethod(JNIEnv *env, 
	jobject obj, jmethodID mth, ...)
{
	va_list args;
	jboolean val;
	va_start(args, mth);
	val=JNIENV->CallBooleanMethodV(env, obj, mth, args);
	va_end(args);
	return(val);
}

static inline jboolean jCallBooleanMethodV(JNIEnv *env, 
		jobject obj, jmethodID mth, va_list args)
	{ return JNIENV->CallBooleanMethodV(env, obj, mth, args); }
static inline jboolean jCallBooleanMethodA(JNIEnv *env, 
		jobject obj, jmethodID mth, jvalue *args)
	{ return JNIENV->CallBooleanMethodA(env, obj, mth, args); }

static inline jbyte jCallByteMethod(JNIEnv *env, 
	jobject obj, jmethodID mth, ...)
{
	va_list args;
	jbyte val;
	va_start(args, mth);
	val=JNIENV->CallByteMethodV(env, obj, mth, args);
	va_end(args);
	return(val);
}

static inline jbyte jCallByteMethodV(JNIEnv *env, 
		jobject obj, jmethodID mth, va_list args)
	{ return JNIENV->CallByteMethodV(env, obj, mth, args); }
static inline jbyte jCallByteMethodA(JNIEnv *env, 
		jobject obj, jmethodID mth, jvalue *args)
	{ return JNIENV->CallByteMethodA(env, obj, mth, args); }

static inline jchar jCallCharMethod(JNIEnv *env, 
	jobject obj, jmethodID mth, ...)
{
	va_list args;
	jchar val;
	va_start(args, mth);
	val=JNIENV->CallCharMethodV(env, obj, mth, args);
	va_end(args);
	return(val);
}

static inline jchar jCallCharMethodV(JNIEnv *env, 
		jobject obj, jmethodID mth, va_list args)
	{ return JNIENV->CallCharMethodV(env, obj, mth, args); }
static inline jchar jCallCharMethodA(JNIEnv *env, 
		jobject obj, jmethodID mth, jvalue *args)
	{ return JNIENV->CallCharMethodA(env, obj, mth, args); }

static inline jshort jCallShortMethod(JNIEnv *env, 
	jobject obj, jmethodID mth, ...)
{
	va_list args;
	jshort val;
	va_start(args, mth);
	val=JNIENV->CallShortMethodV(env, obj, mth, args);
	va_end(args);
	return(val);
}

static inline jshort jCallShortMethodV(JNIEnv *env, 
		jobject obj, jmethodID mth, va_list args)
	{ return JNIENV->CallShortMethodV(env, obj, mth, args); }
static inline jshort jCallShortMethodA(JNIEnv *env, 
		jobject obj, jmethodID mth, jvalue *args)
	{ return JNIENV->CallShortMethodA(env, obj, mth, args); }

static inline jint jCallIntMethod(JNIEnv *env, 
	jobject obj, jmethodID mth, ...)
{
	va_list args;
	jint val;
	va_start(args, mth);
	val=JNIENV->CallIntMethodV(env, obj, mth, args);
	va_end(args);
	return(val);
}

static inline jint jCallIntMethodV(JNIEnv *env, 
		jobject obj, jmethodID mth, va_list args)
	{ return JNIENV->CallIntMethodV(env, obj, mth, args); }
static inline jint jCallIntMethodA(JNIEnv *env, 
		jobject obj, jmethodID mth, jvalue *args)
	{ return JNIENV->CallIntMethodA(env, obj, mth, args); }

static inline jlong jCallLongMethod(JNIEnv *env, 
	jobject obj, jmethodID mth, ...)
{
	va_list args;
	jlong val;
	va_start(args, mth);
	val=JNIENV->CallLongMethodV(env, obj, mth, args);
	va_end(args);
	return(val);
}
static inline jlong jCallLongMethodV(JNIEnv *env, 
		jobject obj, jmethodID mth, va_list args)
	{ return JNIENV->CallLongMethodV(env, obj, mth, args); }
static inline jlong jCallLongMethodA(JNIEnv *env, 
		jobject obj, jmethodID mth, jvalue *args)
	{ return JNIENV->CallLongMethodA(env, obj, mth, args); }

static inline jfloat jCallFloatMethod(JNIEnv *env, 
	jobject obj, jmethodID mth, ...)
{
	va_list args;
	jfloat val;
	va_start(args, mth);
	val=JNIENV->CallFloatMethodV(env, obj, mth, args);
	va_end(args);
	return(val);
}

static inline jfloat jCallFloatMethodV(JNIEnv *env, 
		jobject obj, jmethodID mth, va_list args)
	{ return JNIENV->CallFloatMethodV(env, obj, mth, args); }
static inline jfloat jCallFloatMethodA(JNIEnv *env, 
		jobject obj, jmethodID mth, jvalue *args)
	{ return JNIENV->CallFloatMethodA(env, obj, mth, args); }

static inline jdouble jCallDoubleMethod(JNIEnv *env, 
	jobject obj, jmethodID mth, ...)
{
	va_list args;
	jdouble val;
	va_start(args, mth);
	val=JNIENV->CallDoubleMethodV(env, obj, mth, args);
	va_end(args);
	return(val);
}

static inline jdouble jCallDoubleMethodV(JNIEnv *env, 
		jobject obj, jmethodID mth, va_list args)
	{ return JNIENV->CallDoubleMethodV(env, obj, mth, args); }
static inline jdouble jCallDoubleMethodA(JNIEnv *env, 
		jobject obj, jmethodID mth, jvalue *args)
	{ return JNIENV->CallDoubleMethodA(env, obj, mth, args); }

static inline void jCallVoidMethod(JNIEnv *env, 
	jobject obj, jmethodID mth, ...)
{
	va_list args;
	va_start(args, mth);
	JNIENV->CallVoidMethodV(env, obj, mth, args);
	va_end(args);
}

static inline void jCallVoidMethodV(JNIEnv *env, 
		jobject obj, jmethodID mth, va_list args)
	{ JNIENV->CallVoidMethodV(env, obj, mth, args); }
static inline void jCallVoidMethodA(JNIEnv *env, 
		jobject obj, jmethodID mth, jvalue *args)
	{ JNIENV->CallVoidMethodA(env, obj, mth, args); }

static inline jobject jCallNonvirtualObjectMethod(JNIEnv *env, 
	jobject obj, jclass cls, jmethodID mth, ...)
{
	va_list args;
	jobject val;
	va_start(args, mth);
	val=JNIENV->CallNonvirtualObjectMethodV(env, obj, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jobject jCallNonvirtualObjectMethodV(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallNonvirtualObjectMethodV(env, obj, cls, mth, args); }
static inline jobject jCallNonvirtualObjectMethodA(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallNonvirtualObjectMethodA(env, obj, cls, mth, args); }

static inline jboolean jCallNonvirtualBooleanMethod(JNIEnv *env, 
	jobject obj, jclass cls, jmethodID mth, ...)
{
	va_list args;
	jboolean val;
	va_start(args, mth);
	val=JNIENV->CallNonvirtualBooleanMethodV(env, obj, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jboolean jCallNonvirtualBooleanMethodV(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallNonvirtualBooleanMethodV(env, obj, cls, mth, args); }
static inline jboolean jCallNonvirtualBooleanMethodA(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallNonvirtualBooleanMethodA(env, obj, cls, mth, args); }

static inline jbyte jCallNonvirtualByteMethod(JNIEnv *env, 
	jobject obj, jclass cls, jmethodID mth, ...)
{
	va_list args;
	jbyte val;
	va_start(args, mth);
	val=JNIENV->CallNonvirtualByteMethodV(env, obj, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jbyte jCallNonvirtualByteMethodV(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallNonvirtualByteMethodV(env, obj, cls, mth, args); }
static inline jbyte jCallNonvirtualByteMethodA(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallNonvirtualByteMethodA(env, obj, cls, mth, args); }

static inline jchar jCallNonvirtualCharMethod(JNIEnv *env, 
	jobject obj, jclass cls, jmethodID mth, ...)
{
	va_list args;
	jchar val;
	va_start(args, mth);
	val=JNIENV->CallNonvirtualCharMethodV(env, obj, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jchar jCallNonvirtualCharMethodV(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallNonvirtualCharMethodV(env, obj, cls, mth, args); }
static inline jchar jCallNonvirtualCharMethodA(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallNonvirtualCharMethodA(env, obj, cls, mth, args); }

static inline jshort jCallNonvirtualShortMethod(JNIEnv *env, 
	jobject obj, jclass cls, jmethodID mth, ...)
{
	va_list args;
	jshort val;
	va_start(args, mth);
	val=JNIENV->CallNonvirtualShortMethodV(env, obj, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jshort jCallNonvirtualShortMethodV(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallNonvirtualShortMethodV(env, obj, cls, mth, args); }
static inline jshort jCallNonvirtualShortMethodA(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallNonvirtualShortMethodA(env, obj, cls, mth, args); }

static inline jint jCallNonvirtualIntMethod(JNIEnv *env, 
	jobject obj, jclass cls, jmethodID mth, ...)
{
	va_list args;
	jint val;
	va_start(args, mth);
	val=JNIENV->CallNonvirtualIntMethodV(env, obj, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jint jCallNonvirtualIntMethodV(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallNonvirtualIntMethodV(env, obj, cls, mth, args); }
static inline jint jCallNonvirtualIntMethodA(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallNonvirtualIntMethodA(env, obj, cls, mth, args); }

static inline jlong jCallNonvirtualLongMethod(JNIEnv *env, 
	jobject obj, jclass cls, jmethodID mth, ...)
{
	va_list args;
	jlong val;
	va_start(args, mth);
	val=JNIENV->CallNonvirtualLongMethodV(env, obj, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jlong jCallNonvirtualLongMethodV(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallNonvirtualLongMethodV(env, obj, cls, mth, args); }
static inline jlong jCallNonvirtualLongMethodA(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallNonvirtualLongMethodA(env, obj, cls, mth, args); }

static inline jfloat jCallNonvirtualFloatMethod(JNIEnv *env, 
	jobject obj, jclass cls, jmethodID mth, ...)
{
	va_list args;
	jfloat val;
	va_start(args, mth);
	val=JNIENV->CallNonvirtualFloatMethodV(env, obj, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jfloat jCallNonvirtualFloatMethodV(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallNonvirtualFloatMethodV(env, obj, cls, mth, args); }
static inline jfloat jCallNonvirtualFloatMethodA(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallNonvirtualFloatMethodA(env, obj, cls, mth, args); }

static inline jdouble jCallNonvirtualDoubleMethod(JNIEnv *env, 
	jobject obj, jclass cls, jmethodID mth, ...)
{
	va_list args;
	jdouble val;
	va_start(args, mth);
	val=JNIENV->CallNonvirtualDoubleMethodV(env, obj, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jdouble jCallNonvirtualDoubleMethodV(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallNonvirtualDoubleMethodV(env, obj, cls, mth, args); }
static inline jdouble jCallNonvirtualDoubleMethodA(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallNonvirtualDoubleMethodA(env, obj, cls, mth, args); }

static inline void jCallNonvirtualVoidMethod(JNIEnv *env, 
	jobject obj, jclass cls, jmethodID mth, ...)
{
	va_list args;
	va_start(args, mth);
	JNIENV->CallNonvirtualVoidMethodV(env, obj, cls, mth, args);
	va_end(args);
}

static inline void jCallNonvirtualVoidMethodV(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, va_list args)
	{ JNIENV->CallNonvirtualVoidMethodV(env, obj, cls, mth, args); }
static inline void jCallNonvirtualVoidMethodA(JNIEnv *env, 
		jobject obj, jclass cls, jmethodID mth, jvalue *args)
	{ JNIENV->CallNonvirtualVoidMethodA(env, obj, cls, mth, args); }


static inline jfieldID jGetFieldID(JNIEnv *env, 
		jclass cls, const char *name, const char *sig)
	{ return JNIENV->GetFieldID(env, cls, name, sig); }

static inline jobject jGetObjectField(JNIEnv *env, jobject obj, jfieldID id)
	{ return JNIENV->GetObjectField(env, obj, id); }
static inline jboolean jGetBooleanField(JNIEnv *env, jobject obj, jfieldID id)
	{ return JNIENV->GetBooleanField(env, obj, id); }
static inline jbyte jGetByteField(JNIEnv *env, jobject obj, jfieldID id)
	{ return JNIENV->GetByteField(env, obj, id); }
static inline jchar jGetCharField(JNIEnv *env, jobject obj, jfieldID id)
	{ return JNIENV->GetCharField(env, obj, id); }
static inline jshort jGetShortField(JNIEnv *env, jobject obj, jfieldID id)
	{ return JNIENV->GetShortField(env, obj, id); }
static inline jint jGetIntField(JNIEnv *env, jobject obj, jfieldID id)
	{ return JNIENV->GetIntField(env, obj, id); }
static inline jlong jGetLongField(JNIEnv *env, jobject obj, jfieldID id)
	{ return JNIENV->GetLongField(env, obj, id); }
static inline jfloat jGetFloatField(JNIEnv *env, jobject obj, jfieldID id)
	{ return JNIENV->GetFloatField(env, obj, id); }
static inline jdouble jGetDoubleField(JNIEnv *env, jobject obj, jfieldID id)
	{ return JNIENV->GetDoubleField(env, obj, id); }

static inline void jSetObjectField(JNIEnv *env, 
		jobject obj, jfieldID id, jobject val)
	{ JNIENV->SetObjectField(env, obj, id, val); }
static inline void jSetBooleanField(JNIEnv *env, 
		jobject obj, jfieldID id, jboolean val)
	{ JNIENV->SetBooleanField(env, obj, id, val); }
static inline void jSetByteField(JNIEnv *env, 
		jobject obj, jfieldID id, jbyte val)
	{ JNIENV->SetByteField(env, obj, id, val); }
static inline void jSetCharField(JNIEnv *env, 
		jobject obj, jfieldID id, jchar val)
	{ JNIENV->SetCharField(env, obj, id, val); }
static inline void jSetShortField(JNIEnv *env, 
		jobject obj, jfieldID id, jshort val)
	{ JNIENV->SetShortField(env, obj, id, val); }
static inline void jSetIntField(JNIEnv *env, 
		jobject obj, jfieldID id, jint val)
	{ JNIENV->SetIntField(env, obj, id, val); }
static inline void jSetLongField(JNIEnv *env, 
		jobject obj, jfieldID id, jlong val)
	{ JNIENV->SetLongField(env, obj, id, val); }
static inline void jSetFloatField(JNIEnv *env, 
		jobject obj, jfieldID id, jfloat val)
	{ JNIENV->SetFloatField(env, obj, id, val); }
static inline void jSetDoubleField(JNIEnv *env, 
		jobject obj, jfieldID id, jdouble val)
	{ JNIENV->SetDoubleField(env, obj, id, val); }

static inline jmethodID jGetStaticMethodID(JNIEnv *env, 
		jclass cls, const char *name, const char *sig)
	{ return JNIENV->GetStaticMethodID(env, cls, name, sig); }

static inline jobject jCallStaticObjectMethod(JNIEnv *env, 
	jclass cls, jmethodID mth, ...)
{
	va_list args;
	jobject val;
	va_start(args, mth);
	val=JNIENV->CallStaticObjectMethodV(env, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jobject jCallStaticObjectMethodV(JNIEnv *env, 
		jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallStaticObjectMethodV(env, cls, mth, args); }
static inline jobject jCallStaticObjectMethodA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallStaticObjectMethodA(env, cls, mth, args); }

static inline jboolean jCallStaticBooleanMethod(JNIEnv *env, 
	jclass cls, jmethodID mth, ...)
{
	va_list args;
	jboolean val;
	va_start(args, mth);
	val=JNIENV->CallStaticBooleanMethodV(env, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jboolean jCallStaticBooleanMethodV(JNIEnv *env, 
		jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallStaticBooleanMethodV(env, cls, mth, args); }
static inline jboolean jCallStaticBooleanMethodA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallStaticBooleanMethodA(env, cls, mth, args); }

static inline jbyte jCallStaticByteMethod(JNIEnv *env, 
	jclass cls, jmethodID mth, ...)
{
	va_list args;
	jbyte val;
	va_start(args, mth);
	val=JNIENV->CallStaticByteMethodV(env, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jbyte jCallStaticByteMethodV(JNIEnv *env, 
		jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallStaticByteMethodV(env, cls, mth, args); }
static inline jbyte jCallStaticByteMethodA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallStaticByteMethodA(env, cls, mth, args); }

static inline jchar jCallStaticCharMethod(JNIEnv *env, 
	jclass cls, jmethodID mth, ...)
{
	va_list args;
	jchar val;
	va_start(args, mth);
	val=JNIENV->CallStaticCharMethodV(env, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jchar jCallStaticCharMethodV(JNIEnv *env, 
		jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallStaticCharMethodV(env, cls, mth, args); }
static inline jchar jCallStaticCharMethodA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallStaticCharMethodA(env, cls, mth, args); }

static inline jshort jCallStaticShortMethod(JNIEnv *env, 
	jclass cls, jmethodID mth, ...)
{
	va_list args;
	jshort val;
	va_start(args, mth);
	val=JNIENV->CallStaticShortMethodV(env, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jshort jCallStaticShortMethodV(JNIEnv *env, 
		jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallStaticShortMethodV(env, cls, mth, args); }
static inline jshort jCallStaticShortMethodA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallStaticShortMethodA(env, cls, mth, args); }

static inline jint jCallStaticIntMethod(JNIEnv *env, 
	jclass cls, jmethodID mth, ...)
{
	va_list args;
	jint val;
	va_start(args, mth);
	val=JNIENV->CallStaticIntMethodV(env, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jint jCallStaticIntMethodV(JNIEnv *env, 
	jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallStaticIntMethodV(env, cls, mth, args); }
static inline jint jCallStaticIntMethodA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallStaticIntMethodA(env, cls, mth, args); }

static inline jlong jCallStaticLongMethod(JNIEnv *env, 
	jclass cls, jmethodID mth, ...)
{
	va_list args;
	jlong val;
	va_start(args, mth);
	val=JNIENV->CallStaticLongMethodV(env, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jlong jCallStaticLongMethodV(JNIEnv *env, 
		jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallStaticLongMethodV(env, cls, mth, args); }
static inline jlong jCallStaticLongMethodA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallStaticLongMethodA(env, cls, mth, args); }

static inline jfloat jCallStaticFloatMethod(JNIEnv *env, 
	jclass cls, jmethodID mth, ...)
{
	va_list args;
	jfloat val;
	va_start(args, mth);
	val=JNIENV->CallStaticFloatMethodV(env, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jfloat jCallStaticFloatMethodV(JNIEnv *env, 
		jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallStaticFloatMethodV(env, cls, mth, args); }
static inline jfloat jCallStaticFloatMethodA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallStaticFloatMethodA(env, cls, mth, args); }

static inline jdouble jCallStaticDoubleMethod(JNIEnv *env, 
	jclass cls, jmethodID mth, ...)
{
	va_list args;
	jdouble val;
	va_start(args, mth);
	val=JNIENV->CallStaticDoubleMethodV(env, cls, mth, args);
	va_end(args);
	return(val);
}

static inline jdouble jCallStaticDoubleMethodV(JNIEnv *env, 
		jclass cls, jmethodID mth, va_list args)
	{ return JNIENV->CallStaticDoubleMethodV(env, cls, mth, args); }
static inline jdouble jCallStaticDoubleMethodA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ return JNIENV->CallStaticDoubleMethodA(env, cls, mth, args); }

static inline void jCallStaticVoidMethod(JNIEnv *env, 
	jclass cls, jmethodID mth, ...)
{
	va_list args;
	va_start(args, mth);
	JNIENV->CallStaticVoidMethodV(env, cls, mth, args);
	va_end(args);
}

static inline void jCallStaticVoidMethodV(JNIEnv *env, 
		jclass cls, jmethodID mth, va_list args)
	{ JNIENV->CallStaticVoidMethodV(env, cls, mth, args); }
static inline void jCallStaticVoidMethodA(JNIEnv *env, 
		jclass cls, jmethodID mth, jvalue *args)
	{ JNIENV->CallStaticVoidMethodA(env, cls, mth, args); }

static inline jfieldID jGetStaticFieldID(JNIEnv *env, 
		jclass cls, const char *name, const char *sig)
	{ return JNIENV->GetStaticFieldID(env, cls, name, sig); }

static inline jobject jGetStaticObjectField(JNIEnv *env, 
		jclass cls, jfieldID id)
	{ return JNIENV->GetStaticObjectField(env, cls, id); }
static inline jboolean jGetStaticBooleanField(JNIEnv *env, 
		jclass cls, jfieldID id)
	{ return JNIENV->GetStaticBooleanField(env, cls, id); }
static inline jbyte jGetStaticByteField(JNIEnv *env, 
		jclass cls, jfieldID id)
	{ return JNIENV->GetStaticByteField(env, cls, id); }
static inline jchar jGetStaticCharField(JNIEnv *env, 
		jclass cls, jfieldID id)
	{ return JNIENV->GetStaticCharField(env, cls, id); }
static inline jshort jGetStaticShortField(JNIEnv *env, 
		jclass cls, jfieldID id)
	{ return JNIENV->GetStaticShortField(env, cls, id); }
static inline jint jGetStaticIntField(JNIEnv *env, 
		jclass cls, jfieldID id)
	{ return JNIENV->GetStaticIntField(env, cls, id); }
static inline jlong jGetStaticLongField(JNIEnv *env, 
		jclass cls, jfieldID id)
	{ return JNIENV->GetStaticLongField(env, cls, id); }
static inline jfloat jGetStaticFloatField(JNIEnv *env, 
		jclass cls, jfieldID id)
	{ return JNIENV->GetStaticFloatField(env, cls, id); }
static inline jdouble jGetStaticDoubleField(JNIEnv *env, 
		jclass cls, jfieldID id)
	{ return JNIENV->GetStaticDoubleField(env, cls, id); }

static inline void jSetStaticObjectField(JNIEnv *env, 
		jclass cls, jfieldID id, jobject value)
	{ JNIENV->SetStaticObjectField(env, cls, id, value); }
static inline void jSetStaticBooleanField(JNIEnv *env, 
		jclass cls, jfieldID id, jboolean value)
	{ JNIENV->SetStaticBooleanField(env, cls, id, value); }
static inline void jSetStaticByteField(JNIEnv *env, 
		jclass cls, jfieldID id, jbyte value)
	{ JNIENV->SetStaticByteField(env, cls, id, value); }
static inline void jSetStaticCharField(JNIEnv *env, 
		jclass cls, jfieldID id, jchar value)
	{ JNIENV->SetStaticCharField(env, cls, id, value); }
static inline void jSetStaticShortField(JNIEnv *env, 
		jclass cls, jfieldID id, jshort value)
	{ JNIENV->SetStaticShortField(env, cls, id, value); }
static inline void jSetStaticIntField(JNIEnv *env, 
		jclass cls, jfieldID id, jint value) 
	{ JNIENV->SetStaticIntField(env, cls, id, value); }
static inline void jSetStaticLongField(JNIEnv *env, 
		jclass cls, jfieldID id, jlong value) 
	{ JNIENV->SetStaticLongField(env, cls, id, value); }
static inline void jSetStaticFloatField(JNIEnv *env, 
		jclass cls, jfieldID id, jfloat value) 
	{ JNIENV->SetStaticFloatField(env, cls, id, value); }
static inline void jSetStaticDoubleField(JNIEnv *env, 
		jclass cls, jfieldID id, jdouble value) 
	{ JNIENV->SetStaticDoubleField(env, cls, id, value); }

static inline jstring jNewString(JNIEnv *env, 
		const jchar *unicode, jsize len) 
	{ return JNIENV->NewString(env, unicode, len); }
static inline jsize jGetStringLength(JNIEnv *env, jstring str) 
	{ return JNIENV->GetStringLength(env, str); }
static inline const jchar *GetStringChars(JNIEnv *env, 
		jstring str, jboolean *copy) 
	{ return JNIENV->GetStringChars(env, str, copy); }
static inline void jReleaseStringChars(JNIEnv *env, 
		jstring str, const jchar *chars) 
	{ JNIENV->ReleaseStringChars(env, str, chars); }

static inline jstring jNewStringUTF(JNIEnv *env, const char *utf) 
	{ return JNIENV->NewStringUTF(env, utf); }
static inline jsize jGetStringUTFLength(JNIEnv *env, jstring str) 
	{ return JNIENV->GetStringUTFLength(env, str); }
static inline const char *jGetStringUTFChars(JNIEnv *env, 
		jstring str, jboolean *copy) 
	{ return JNIENV->GetStringUTFChars(env, str, copy); }
static inline void jReleaseStringUTFChars(JNIEnv *env, 
		jstring str, const char *chars)
	{ JNIENV->ReleaseStringUTFChars(env, str, chars); }

static inline jsize jGetArrayLength(JNIEnv *env, jarray array) 
	{ return JNIENV->GetArrayLength(env, array); }

static inline jobjectArray jNewObjectArray(JNIEnv *env, 
		jsize len, jclass cls, jobject init) 
	{ return JNIENV->NewObjectArray(env, len, cls, init); }
static inline jobject jGetObjectArrayElement(JNIEnv *env, 
		jobjectArray array, jsize index) 
	{ return JNIENV->GetObjectArrayElement(env, array, index); }
static inline void jSetObjectArrayElement(JNIEnv *env, 
		jobjectArray array, jsize index, jobject val) 
	{ JNIENV->SetObjectArrayElement(env, array, index, val); }

static inline jbooleanArray jNewBooleanArray(JNIEnv *env, jsize len) 
	{ return JNIENV->NewBooleanArray(env, len); }
static inline jbyteArray jNewByteArray(JNIEnv *env, jsize len) 
	{ return JNIENV->NewByteArray(env, len); }
static inline jcharArray jNewCharArray(JNIEnv *env, jsize len)
	{ return JNIENV->NewCharArray(env, len); }
static inline jshortArray jNewShortArray(JNIEnv *env, jsize len)
	{ return JNIENV->NewShortArray(env, len); }
static inline jintArray jNewIntArray(JNIEnv *env, jsize len) 
	{ return JNIENV->NewIntArray(env, len); }
static inline jlongArray jNewLongArray(JNIEnv *env, jsize len) 
	{ return JNIENV->NewLongArray(env, len); }
static inline jfloatArray jNewFloatArray(JNIEnv *env, jsize len) 
	{ return JNIENV->NewFloatArray(env, len); }
static inline jdoubleArray jNewDoubleArray(JNIEnv *env, jsize len) 
	{ return JNIENV->NewDoubleArray(env, len); }

static inline jboolean *jGetBooleanArrayElements(JNIEnv *env, 
		jbooleanArray array, jboolean *copy) 
	{ return JNIENV->GetBooleanArrayElements(env, array, copy); }
static inline jbyte *jGetByteArrayElements(JNIEnv *env, 
		jbyteArray array, jboolean *copy) 
	{ return JNIENV->GetByteArrayElements(env, array, copy); }
static inline jchar *jGetCharArrayElements(JNIEnv *env, 
		jcharArray array, jboolean *copy) 
	{ return JNIENV->GetCharArrayElements(env, array, copy); }
static inline jshort *jGetShortArrayElements(JNIEnv *env, 
		jshortArray array, jboolean *copy) 
	{ return JNIENV->GetShortArrayElements(env, array, copy); }
static inline jint *jGetIntArrayElements(JNIEnv *env, 
		jintArray array, jboolean *copy) 
	{ return JNIENV->GetIntArrayElements(env, array, copy); }
static inline jlong *jGetLongArrayElements(JNIEnv *env, 
		jlongArray array, jboolean *copy) 
	{ return JNIENV->GetLongArrayElements(env, array, copy); }
static inline jfloat *jGetFloatArrayElements(JNIEnv *env, 
		jfloatArray array, jboolean *copy) 
	{ return JNIENV->GetFloatArrayElements(env, array, copy); }
static inline jdouble *jGetDoubleArrayElements(JNIEnv *env, 
		jdoubleArray array, jboolean *copy) 
	{ return JNIENV->GetDoubleArrayElements(env, array, copy); }

static inline void jReleaseBooleanArrayElements(JNIEnv *env, 
		jbooleanArray array, jboolean *elems, jint mode) 
	{ JNIENV->ReleaseBooleanArrayElements(env, array, elems, mode); }
static inline void jReleaseByteArrayElements(JNIEnv *env, 
		jbyteArray array, jbyte *elems, jint mode) 
	{ JNIENV->ReleaseByteArrayElements(env, array, elems, mode); }
static inline void jReleaseCharArrayElements(JNIEnv *env, 
		jcharArray array, jchar *elems, jint mode) 
	{ JNIENV->ReleaseCharArrayElements(env, array, elems, mode); }
static inline void jReleaseShortArrayElements(JNIEnv *env, 
		jshortArray array, jshort *elems, jint mode) 
	{ JNIENV->ReleaseShortArrayElements(env, array, elems, mode); }
static inline void jReleaseIntArrayElements(JNIEnv *env, 
		jintArray array, jint *elems, jint mode) 
	{ JNIENV->ReleaseIntArrayElements(env, array, elems, mode); }
static inline void jReleaseLongArrayElements(JNIEnv *env, 
		jlongArray array, jlong *elems, jint mode) 
	{ JNIENV->ReleaseLongArrayElements(env, array, elems, mode); }
static inline void jReleaseFloatArrayElements(JNIEnv *env, 
		jfloatArray array, jfloat *elems, jint mode) 
	{ JNIENV->ReleaseFloatArrayElements(env, array, elems, mode); }
static inline void jReleaseDoubleArrayElements(JNIEnv *env, 
		jdoubleArray array, jdouble *elems, jint mode) 
	{ JNIENV->ReleaseDoubleArrayElements(env, array, elems, mode); }

static inline void jGetBooleanArrayRegion(JNIEnv *env, 
		jbooleanArray array, jsize start, jsize len, jboolean *buf) 
	{ JNIENV->GetBooleanArrayRegion(env, array, start, len, buf); }
static inline void jGetByteArrayRegion(JNIEnv *env, 
		jbyteArray array, jsize start, jsize len, jbyte *buf) 
	{ JNIENV->GetByteArrayRegion(env, array, start, len, buf); }
static inline void jGetCharArrayRegion(JNIEnv *env, 
		jcharArray array, jsize start, jsize len, jchar *buf) 
	{ JNIENV->GetCharArrayRegion(env, array, start, len, buf); }
static inline void jGetShortArrayRegion(JNIEnv *env, 
		jshortArray array, jsize start, jsize len, jshort *buf) 
	{ JNIENV->GetShortArrayRegion(env, array, start, len, buf); }
static inline void jGetIntArrayRegion(JNIEnv *env, 
		jintArray array, jsize start, jsize len, jint *buf) 
	{ JNIENV->GetIntArrayRegion(env, array, start, len, buf); }
static inline void jGetLongArrayRegion(JNIEnv *env, 
		jlongArray array, jsize start, jsize len, jlong *buf) 
	{ JNIENV->GetLongArrayRegion(env, array, start, len, buf); }
static inline void jGetFloatArrayRegion(JNIEnv *env, 
		jfloatArray array, jsize start, jsize len, jfloat *buf) 
	{ JNIENV->GetFloatArrayRegion(env, array, start, len, buf); }
static inline void jGetDoubleArrayRegion(JNIEnv *env, 
		jdoubleArray array, jsize start, jsize len, jdouble *buf) 
	{ JNIENV->GetDoubleArrayRegion(env, array, start, len, buf); }

static inline void jSetBooleanArrayRegion(JNIEnv *env, 
		jbooleanArray array, jsize start, jsize len, jboolean *buf) 
	{ JNIENV->SetBooleanArrayRegion(env, array, start, len, buf); }
static inline void jSetByteArrayRegion(JNIEnv *env, 
		jbyteArray array, jsize start, jsize len, jbyte *buf) 
	{ JNIENV->SetByteArrayRegion(env, array, start, len, buf); }
static inline void jSetCharArrayRegion(JNIEnv *env, 
		jcharArray array, jsize start, jsize len, jchar *buf) 
	{ JNIENV->SetCharArrayRegion(env, array, start, len, buf); }
static inline void jSetShortArrayRegion(JNIEnv *env, 
		jshortArray array, jsize start, jsize len, jshort *buf) 
	{ JNIENV->SetShortArrayRegion(env, array, start, len, buf); }
static inline void jSetIntArrayRegion(JNIEnv *env, 
		jintArray array, jsize start, jsize len, jint *buf) 
	{ JNIENV->SetIntArrayRegion(env, array, start, len, buf); }
static inline void jSetLongArrayRegion(JNIEnv *env, 
		jlongArray array, jsize start, jsize len, jlong *buf) 
	{ JNIENV->SetLongArrayRegion(env, array, start, len, buf); }
static inline void jSetFloatArrayRegion(JNIEnv *env, 
		jfloatArray array, jsize start, jsize len, jfloat *buf) 
	{ JNIENV->SetFloatArrayRegion(env, array, start, len, buf); }
static inline void jSetDoubleArrayRegion(JNIEnv *env, 
		jdoubleArray array, jsize start, jsize len, jdouble *buf)
	{ JNIENV->SetDoubleArrayRegion(env, array, start, len, buf); }

static inline jint jRegisterNatives(JNIEnv *env, 
		jclass cls, const JNINativeMethod *methods, jint n_method)
	{ return JNIENV->RegisterNatives(env, cls, methods, n_method); }
static inline jint jUnregisterNatives(JNIEnv *env, jclass cls)
	{ return JNIENV->UnregisterNatives(env, cls); }

static inline jint jMonitorEnter(JNIEnv *env, jobject obj)
	{ return JNIENV->MonitorEnter(env, obj); }
static inline jint jMonitorExit(JNIEnv *env, jobject obj)
	{ return JNIENV->MonitorExit(env, obj); }

static inline jint jGetJavaVM(JNIEnv *env, JavaVM **vm)
	{ return JNIENV->GetJavaVM(env, vm); }

static inline void jGetStringRegion(JNIEnv *env, 
		jstring str, jsize off, jsize len, jchar *buf)
	{ JNIENV->GetStringRegion(env, str, off, len, buf); }
static inline void jGetStringUTFRegion(JNIEnv *env, 
		jstring str, jsize off, jsize len, char *buf)
	{ JNIENV->GetStringUTFRegion(env, str, off, len, buf); }

static inline void *jGetPrimitiveArrayCritical(JNIEnv *env, 
		jarray array, jboolean *copy)
	{ return JNIENV->GetPrimitiveArrayCritical(env, array, copy); }

static inline void jReleasePrimitiveArrayCritical(JNIEnv *env, 
		jarray array, void *carray, jint mode)
	{ JNIENV->ReleasePrimitiveArrayCritical(env, array, carray, mode); }

static inline const jchar *jGetStringCritical(JNIEnv *env, 
		jstring str, jboolean *copy)
	{ return JNIENV->GetStringCritical(env, str, copy); }
static inline void jReleaseStringCritical(JNIEnv *env, 
		jstring str, const jchar *cstr)
	{ JNIENV->ReleaseStringCritical(env, str, cstr); }

static inline jweak jNewWeakGlobalRef(JNIEnv *env, jobject obj)
	{ return JNIENV->NewWeakGlobalRef(env, obj); }

static inline void jDeleteWeakGlobalRef(JNIEnv *env, jweak obj)
	{ JNIENV->DeleteWeakGlobalRef(env, obj); }

static inline jboolean jExceptionCheck(JNIEnv *env)
	{ return JNIENV->ExceptionCheck(env); }

static inline jobject jNewDirectByteBuffer(JNIEnv *env,
		void *address, jlong capacity)
	{ return JNIENV->NewDirectByteBuffer(env, address, capacity); }
static inline void *jGetDirectBufferAddress(JNIEnv *env, jobject buf)
	{ return JNIENV->GetDirectBufferAddress(env, buf); }
static inline jlong jGetDirectBufferCapacity(JNIEnv *env, jobject buf)
	{ return JNIENV->GetDirectBufferCapacity(env, buf); }

#undef JNIENV

#endif
