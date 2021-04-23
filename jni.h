/*
 * Copyright (C) 2006 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * JNI specification, as defined by Sun:
 * http://java.sun.com/javase/6/docs/technotes/guides/jni/spec/jniTOC.html
 *
 * Everything here is expected to be VM-neutral.
 */

/* Primitive types that match up with Java equivalents. */
typedef uint8_t  jboolean; /* unsigned 8 bits */
typedef int8_t   jbyte;    /* signed 8 bits */
typedef uint16_t jchar;    /* unsigned 16 bits */
typedef int16_t  jshort;   /* signed 16 bits */
typedef int32_t  jint;     /* signed 32 bits */
typedef int64_t  jlong;    /* signed 64 bits */
typedef float    jfloat;   /* 32-bit IEEE 754 */
typedef double   jdouble;  /* 64-bit IEEE 754 */

/* "cardinal indices and sizes" */
typedef jint     jsize;

/*
 * Reference types, in C.
 */
typedef void*           jobject;
typedef jobject         jclass;
typedef jobject         jstring;
typedef jobject         jarray;
typedef jarray          jobjectArray;
typedef jarray          jbooleanArray;
typedef jarray          jbyteArray;
typedef jarray          jcharArray;
typedef jarray          jshortArray;
typedef jarray          jintArray;
typedef jarray          jlongArray;
typedef jarray          jfloatArray;
typedef jarray          jdoubleArray;
typedef jobject         jthrowable;
typedef jobject         jweak;

struct _jfieldID;                       /* opaque structure */
typedef struct _jfieldID* jfieldID;     /* field IDs */

struct _jmethodID;                      /* opaque structure */
typedef struct _jmethodID* jmethodID;   /* method IDs */

struct JNIInvokeInterface;

typedef union {
    jboolean    z;
    jbyte       b;
    jchar       c;
    jshort      s;
    jint        i;
    jlong       j;
    jfloat      f;
    jdouble     d;
    jobject     l;
} jvalue;

typedef enum {
    JNIInvalidRefType = 0,
    JNILocalRefType = 1,
    JNIGlobalRefType = 2,
    JNIWeakGlobalRefType = 3
} jobjectRefType;

typedef struct {
    const char* name;
    const char* signature;
    void*       fnPtr;
} JNINativeMethod;

struct _JNIEnv;
struct _JavaVM;
/*
 * Table of interface function pointers.
 */
struct JNINativeInterface {
    void*       reserved0;
    void*       reserved1;
    void*       reserved2;
    void*       reserved3;

    jint        (*GetVersion)(JNINativeInterface* *);

    jclass      (*DefineClass)(JNINativeInterface**, const char*, jobject, const jbyte*,
                        jsize);
    jclass      (*FindClass)(JNINativeInterface**, const char*);

    jmethodID   (*FromReflectedMethod)(JNINativeInterface**, jobject);
    jfieldID    (*FromReflectedField)(JNINativeInterface**, jobject);
    /* spec doesn't show jboolean parameter */
    jobject     (*ToReflectedMethod)(JNINativeInterface**, jclass, jmethodID, jboolean);

    jclass      (*GetSuperclass)(JNINativeInterface**, jclass);
    jboolean    (*IsAssignableFrom)(JNINativeInterface**, jclass, jclass);

    /* spec doesn't show jboolean parameter */
    jobject     (*ToReflectedField)(JNINativeInterface**, jclass, jfieldID, jboolean);

    jint        (*Throw)(JNINativeInterface**, jthrowable);
    jint        (*ThrowNew)(JNINativeInterface* *, jclass, const char *);
    jthrowable  (*ExceptionOccurred)(JNINativeInterface**);
    void        (*ExceptionDescribe)(JNINativeInterface**);
    void        (*ExceptionClear)(JNINativeInterface**);
    void        (*FatalError)(JNINativeInterface**, const char*);

    jint        (*PushLocalFrame)(JNINativeInterface**, jint);
    jobject     (*PopLocalFrame)(JNINativeInterface**, jobject);

    jobject     (*NewGlobalRef)(JNINativeInterface**, jobject);
    void        (*DeleteGlobalRef)(JNINativeInterface**, jobject);
    void        (*DeleteLocalRef)(JNINativeInterface**, jobject);
    jboolean    (*IsSameObject)(JNINativeInterface**, jobject, jobject);

    jobject     (*NewLocalRef)(JNINativeInterface**, jobject);
    jint        (*EnsureLocalCapacity)(JNINativeInterface**, jint);

    jobject     (*AllocObject)(JNINativeInterface**, jclass);
    jobject     (*NewObject)(JNINativeInterface**, jclass, jmethodID, ...);
    jobject     (*NewObjectV)(JNINativeInterface**, jclass, jmethodID, va_list);
    jobject     (*NewObjectA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);

    jclass      (*GetObjectClass)(JNINativeInterface**, jobject);
    jboolean    (*IsInstanceOf)(JNINativeInterface**, jobject, jclass);
    jmethodID   (*GetMethodID)(JNINativeInterface**, jclass, const char*, const char*);

    jobject     (*CallObjectMethod)(JNINativeInterface**, jobject, jmethodID, ...);
    jobject     (*CallObjectMethodV)(JNINativeInterface**, jobject, jmethodID, va_list);
    jobject     (*CallObjectMethodA)(JNINativeInterface**, jobject, jmethodID, const jvalue*);
    jboolean    (*CallBooleanMethod)(JNINativeInterface**, jobject, jmethodID, ...);
    jboolean    (*CallBooleanMethodV)(JNINativeInterface**, jobject, jmethodID, va_list);
    jboolean    (*CallBooleanMethodA)(JNINativeInterface**, jobject, jmethodID, const jvalue*);
    jbyte       (*CallByteMethod)(JNINativeInterface**, jobject, jmethodID, ...);
    jbyte       (*CallByteMethodV)(JNINativeInterface**, jobject, jmethodID, va_list);
    jbyte       (*CallByteMethodA)(JNINativeInterface**, jobject, jmethodID, const jvalue*);
    jchar       (*CallCharMethod)(JNINativeInterface**, jobject, jmethodID, ...);
    jchar       (*CallCharMethodV)(JNINativeInterface**, jobject, jmethodID, va_list);
    jchar       (*CallCharMethodA)(JNINativeInterface**, jobject, jmethodID, const jvalue*);
    jshort      (*CallShortMethod)(JNINativeInterface**, jobject, jmethodID, ...);
    jshort      (*CallShortMethodV)(JNINativeInterface**, jobject, jmethodID, va_list);
    jshort      (*CallShortMethodA)(JNINativeInterface**, jobject, jmethodID, const jvalue*);
    jint        (*CallIntMethod)(JNINativeInterface**, jobject, jmethodID, ...);
    jint        (*CallIntMethodV)(JNINativeInterface**, jobject, jmethodID, va_list);
    jint        (*CallIntMethodA)(JNINativeInterface**, jobject, jmethodID, const jvalue*);
    jlong       (*CallLongMethod)(JNINativeInterface**, jobject, jmethodID, ...);
    jlong       (*CallLongMethodV)(JNINativeInterface**, jobject, jmethodID, va_list);
    jlong       (*CallLongMethodA)(JNINativeInterface**, jobject, jmethodID, const jvalue*);
    jfloat      (*CallFloatMethod)(JNINativeInterface**, jobject, jmethodID, ...);
    jfloat      (*CallFloatMethodV)(JNINativeInterface**, jobject, jmethodID, va_list);
    jfloat      (*CallFloatMethodA)(JNINativeInterface**, jobject, jmethodID, const jvalue*);
    jdouble     (*CallDoubleMethod)(JNINativeInterface**, jobject, jmethodID, ...);
    jdouble     (*CallDoubleMethodV)(JNINativeInterface**, jobject, jmethodID, va_list);
    jdouble     (*CallDoubleMethodA)(JNINativeInterface**, jobject, jmethodID, const jvalue*);
    void        (*CallVoidMethod)(JNINativeInterface**, jobject, jmethodID, ...);
    void        (*CallVoidMethodV)(JNINativeInterface**, jobject, jmethodID, va_list);
    void        (*CallVoidMethodA)(JNINativeInterface**, jobject, jmethodID, const jvalue*);

    jobject     (*CallNonvirtualObjectMethod)(JNINativeInterface**, jobject, jclass,
                        jmethodID, ...);
    jobject     (*CallNonvirtualObjectMethodV)(JNINativeInterface**, jobject, jclass,
                        jmethodID, va_list);
    jobject     (*CallNonvirtualObjectMethodA)(JNINativeInterface**, jobject, jclass,
                        jmethodID, const jvalue*);
    jboolean    (*CallNonvirtualBooleanMethod)(JNINativeInterface**, jobject, jclass,
                        jmethodID, ...);
    jboolean    (*CallNonvirtualBooleanMethodV)(JNINativeInterface**, jobject, jclass,
                         jmethodID, va_list);
    jboolean    (*CallNonvirtualBooleanMethodA)(JNINativeInterface**, jobject, jclass,
                         jmethodID, const jvalue*);
    jbyte       (*CallNonvirtualByteMethod)(JNINativeInterface**, jobject, jclass,
                        jmethodID, ...);
    jbyte       (*CallNonvirtualByteMethodV)(JNINativeInterface**, jobject, jclass,
                        jmethodID, va_list);
    jbyte       (*CallNonvirtualByteMethodA)(JNINativeInterface**, jobject, jclass,
                        jmethodID, const jvalue*);
    jchar       (*CallNonvirtualCharMethod)(JNINativeInterface**, jobject, jclass,
                        jmethodID, ...);
    jchar       (*CallNonvirtualCharMethodV)(JNINativeInterface**, jobject, jclass,
                        jmethodID, va_list);
    jchar       (*CallNonvirtualCharMethodA)(JNINativeInterface**, jobject, jclass,
                        jmethodID, const jvalue*);
    jshort      (*CallNonvirtualShortMethod)(JNINativeInterface**, jobject, jclass,
                        jmethodID, ...);
    jshort      (*CallNonvirtualShortMethodV)(JNINativeInterface**, jobject, jclass,
                        jmethodID, va_list);
    jshort      (*CallNonvirtualShortMethodA)(JNINativeInterface**, jobject, jclass,
                        jmethodID, const jvalue*);
    jint        (*CallNonvirtualIntMethod)(JNINativeInterface**, jobject, jclass,
                        jmethodID, ...);
    jint        (*CallNonvirtualIntMethodV)(JNINativeInterface**, jobject, jclass,
                        jmethodID, va_list);
    jint        (*CallNonvirtualIntMethodA)(JNINativeInterface**, jobject, jclass,
                        jmethodID, const jvalue*);
    jlong       (*CallNonvirtualLongMethod)(JNINativeInterface**, jobject, jclass,
                        jmethodID, ...);
    jlong       (*CallNonvirtualLongMethodV)(JNINativeInterface**, jobject, jclass,
                        jmethodID, va_list);
    jlong       (*CallNonvirtualLongMethodA)(JNINativeInterface**, jobject, jclass,
                        jmethodID, const jvalue*);
    jfloat      (*CallNonvirtualFloatMethod)(JNINativeInterface**, jobject, jclass,
                        jmethodID, ...);
    jfloat      (*CallNonvirtualFloatMethodV)(JNINativeInterface**, jobject, jclass,
                        jmethodID, va_list);
    jfloat      (*CallNonvirtualFloatMethodA)(JNINativeInterface**, jobject, jclass,
                        jmethodID, const jvalue*);
    jdouble     (*CallNonvirtualDoubleMethod)(JNINativeInterface**, jobject, jclass,
                        jmethodID, ...);
    jdouble     (*CallNonvirtualDoubleMethodV)(JNINativeInterface**, jobject, jclass,
                        jmethodID, va_list);
    jdouble     (*CallNonvirtualDoubleMethodA)(JNINativeInterface**, jobject, jclass,
                        jmethodID, const jvalue*);
    void        (*CallNonvirtualVoidMethod)(JNINativeInterface**, jobject, jclass,
                        jmethodID, ...);
    void        (*CallNonvirtualVoidMethodV)(JNINativeInterface**, jobject, jclass,
                        jmethodID, va_list);
    void        (*CallNonvirtualVoidMethodA)(JNINativeInterface**, jobject, jclass,
                        jmethodID, const jvalue*);

    jfieldID    (*GetFieldID)(JNINativeInterface**, jclass, const char*, const char*);

    jobject     (*GetObjectField)(JNINativeInterface**, jobject, jfieldID);
    jboolean    (*GetBooleanField)(JNINativeInterface**, jobject, jfieldID);
    jbyte       (*GetByteField)(JNINativeInterface**, jobject, jfieldID);
    jchar       (*GetCharField)(JNINativeInterface**, jobject, jfieldID);
    jshort      (*GetShortField)(JNINativeInterface**, jobject, jfieldID);
    jint        (*GetIntField)(JNINativeInterface**, jobject, jfieldID);
    jlong       (*GetLongField)(JNINativeInterface**, jobject, jfieldID);
    jfloat      (*GetFloatField)(JNINativeInterface**, jobject, jfieldID);
    jdouble     (*GetDoubleField)(JNINativeInterface**, jobject, jfieldID);

    void        (*SetObjectField)(JNINativeInterface**, jobject, jfieldID, jobject);
    void        (*SetBooleanField)(JNINativeInterface**, jobject, jfieldID, jboolean);
    void        (*SetByteField)(JNINativeInterface**, jobject, jfieldID, jbyte);
    void        (*SetCharField)(JNINativeInterface**, jobject, jfieldID, jchar);
    void        (*SetShortField)(JNINativeInterface**, jobject, jfieldID, jshort);
    void        (*SetIntField)(JNINativeInterface**, jobject, jfieldID, jint);
    void        (*SetLongField)(JNINativeInterface**, jobject, jfieldID, jlong);
    void        (*SetFloatField)(JNINativeInterface**, jobject, jfieldID, jfloat);
    void        (*SetDoubleField)(JNINativeInterface**, jobject, jfieldID, jdouble);

    jmethodID   (*GetStaticMethodID)(JNINativeInterface**, jclass, const char*, const char*);

    jobject     (*CallStaticObjectMethod)(JNINativeInterface**, jclass, jmethodID, ...);
    jobject     (*CallStaticObjectMethodV)(JNINativeInterface**, jclass, jmethodID, va_list);
    jobject     (*CallStaticObjectMethodA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);
    jboolean    (*CallStaticBooleanMethod)(JNINativeInterface**, jclass, jmethodID, ...);
    jboolean    (*CallStaticBooleanMethodV)(JNINativeInterface**, jclass, jmethodID,
                        va_list);
    jboolean    (*CallStaticBooleanMethodA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);
    jbyte       (*CallStaticByteMethod)(JNINativeInterface**, jclass, jmethodID, ...);
    jbyte       (*CallStaticByteMethodV)(JNINativeInterface**, jclass, jmethodID, va_list);
    jbyte       (*CallStaticByteMethodA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);
    jchar       (*CallStaticCharMethod)(JNINativeInterface**, jclass, jmethodID, ...);
    jchar       (*CallStaticCharMethodV)(JNINativeInterface**, jclass, jmethodID, va_list);
    jchar       (*CallStaticCharMethodA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);
    jshort      (*CallStaticShortMethod)(JNINativeInterface**, jclass, jmethodID, ...);
    jshort      (*CallStaticShortMethodV)(JNINativeInterface**, jclass, jmethodID, va_list);
    jshort      (*CallStaticShortMethodA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);
    jint        (*CallStaticIntMethod)(JNINativeInterface**, jclass, jmethodID, ...);
    jint        (*CallStaticIntMethodV)(JNINativeInterface**, jclass, jmethodID, va_list);
    jint        (*CallStaticIntMethodA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);
    jlong       (*CallStaticLongMethod)(JNINativeInterface**, jclass, jmethodID, ...);
    jlong       (*CallStaticLongMethodV)(JNINativeInterface**, jclass, jmethodID, va_list);
    jlong       (*CallStaticLongMethodA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);
    jfloat      (*CallStaticFloatMethod)(JNINativeInterface**, jclass, jmethodID, ...);
    jfloat      (*CallStaticFloatMethodV)(JNINativeInterface**, jclass, jmethodID, va_list);
    jfloat      (*CallStaticFloatMethodA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);
    jdouble     (*CallStaticDoubleMethod)(JNINativeInterface**, jclass, jmethodID, ...);
    jdouble     (*CallStaticDoubleMethodV)(JNINativeInterface**, jclass, jmethodID, va_list);
    jdouble     (*CallStaticDoubleMethodA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);
    void        (*CallStaticVoidMethod)(JNINativeInterface**, jclass, jmethodID, ...);
    void        (*CallStaticVoidMethodV)(JNINativeInterface**, jclass, jmethodID, va_list);
    void        (*CallStaticVoidMethodA)(JNINativeInterface**, jclass, jmethodID, const jvalue*);

    jfieldID    (*GetStaticFieldID)(JNINativeInterface**, jclass, const char*,
                        const char*);

    jobject     (*GetStaticObjectField)(JNINativeInterface**, jclass, jfieldID);
    jboolean    (*GetStaticBooleanField)(JNINativeInterface**, jclass, jfieldID);
    jbyte       (*GetStaticByteField)(JNINativeInterface**, jclass, jfieldID);
    jchar       (*GetStaticCharField)(JNINativeInterface**, jclass, jfieldID);
    jshort      (*GetStaticShortField)(JNINativeInterface**, jclass, jfieldID);
    jint        (*GetStaticIntField)(JNINativeInterface**, jclass, jfieldID);
    jlong       (*GetStaticLongField)(JNINativeInterface**, jclass, jfieldID);
    jfloat      (*GetStaticFloatField)(JNINativeInterface**, jclass, jfieldID);
    jdouble     (*GetStaticDoubleField)(JNINativeInterface**, jclass, jfieldID);

    void        (*SetStaticObjectField)(JNINativeInterface**, jclass, jfieldID, jobject);
    void        (*SetStaticBooleanField)(JNINativeInterface**, jclass, jfieldID, jboolean);
    void        (*SetStaticByteField)(JNINativeInterface**, jclass, jfieldID, jbyte);
    void        (*SetStaticCharField)(JNINativeInterface**, jclass, jfieldID, jchar);
    void        (*SetStaticShortField)(JNINativeInterface**, jclass, jfieldID, jshort);
    void        (*SetStaticIntField)(JNINativeInterface**, jclass, jfieldID, jint);
    void        (*SetStaticLongField)(JNINativeInterface**, jclass, jfieldID, jlong);
    void        (*SetStaticFloatField)(JNINativeInterface**, jclass, jfieldID, jfloat);
    void        (*SetStaticDoubleField)(JNINativeInterface**, jclass, jfieldID, jdouble);

    jstring     (*NewString)(JNINativeInterface**, const jchar*, jsize);
    jsize       (*GetStringLength)(JNINativeInterface**, jstring);
    const jchar* (*GetStringChars)(JNINativeInterface**, jstring, jboolean*);
    void        (*ReleaseStringChars)(JNINativeInterface**, jstring, const jchar*);
    jstring     (*NewStringUTF)(JNINativeInterface**, const char*);
    jsize       (*GetStringUTFLength)(JNINativeInterface**, jstring);
    /* JNI spec says this returns const jbyte*, but that's inconsistent */
    const char* (*GetStringUTFChars)(JNINativeInterface**, jstring, jboolean*);
    void        (*ReleaseStringUTFChars)(JNINativeInterface**, jstring, const char*);
    jsize       (*GetArrayLength)(JNINativeInterface**, jarray);
    jobjectArray (*NewObjectArray)(JNINativeInterface**, jsize, jclass, jobject);
    jobject     (*GetObjectArrayElement)(JNINativeInterface**, jobjectArray, jsize);
    void        (*SetObjectArrayElement)(JNINativeInterface**, jobjectArray, jsize, jobject);

    jbooleanArray (*NewBooleanArray)(JNINativeInterface**, jsize);
    jbyteArray    (*NewByteArray)(JNINativeInterface**, jsize);
    jcharArray    (*NewCharArray)(JNINativeInterface**, jsize);
    jshortArray   (*NewShortArray)(JNINativeInterface**, jsize);
    jintArray     (*NewIntArray)(JNINativeInterface**, jsize);
    jlongArray    (*NewLongArray)(JNINativeInterface**, jsize);
    jfloatArray   (*NewFloatArray)(JNINativeInterface**, jsize);
    jdoubleArray  (*NewDoubleArray)(JNINativeInterface**, jsize);

    jboolean*   (*GetBooleanArrayElements)(JNINativeInterface**, jbooleanArray, jboolean*);
    jbyte*      (*GetByteArrayElements)(JNINativeInterface**, jbyteArray, jboolean*);
    jchar*      (*GetCharArrayElements)(JNINativeInterface**, jcharArray, jboolean*);
    jshort*     (*GetShortArrayElements)(JNINativeInterface**, jshortArray, jboolean*);
    jint*       (*GetIntArrayElements)(JNINativeInterface**, jintArray, jboolean*);
    jlong*      (*GetLongArrayElements)(JNINativeInterface**, jlongArray, jboolean*);
    jfloat*     (*GetFloatArrayElements)(JNINativeInterface**, jfloatArray, jboolean*);
    jdouble*    (*GetDoubleArrayElements)(JNINativeInterface**, jdoubleArray, jboolean*);

    void        (*ReleaseBooleanArrayElements)(JNINativeInterface**, jbooleanArray,
                        jboolean*, jint);
    void        (*ReleaseByteArrayElements)(JNINativeInterface**, jbyteArray,
                        jbyte*, jint);
    void        (*ReleaseCharArrayElements)(JNINativeInterface**, jcharArray,
                        jchar*, jint);
    void        (*ReleaseShortArrayElements)(JNINativeInterface**, jshortArray,
                        jshort*, jint);
    void        (*ReleaseIntArrayElements)(JNINativeInterface**, jintArray,
                        jint*, jint);
    void        (*ReleaseLongArrayElements)(JNINativeInterface**, jlongArray,
                        jlong*, jint);
    void        (*ReleaseFloatArrayElements)(JNINativeInterface**, jfloatArray,
                        jfloat*, jint);
    void        (*ReleaseDoubleArrayElements)(JNINativeInterface**, jdoubleArray,
                        jdouble*, jint);

    void        (*GetBooleanArrayRegion)(JNINativeInterface**, jbooleanArray,
                        jsize, jsize, jboolean*);
    void        (*GetByteArrayRegion)(JNINativeInterface**, jbyteArray,
                        jsize, jsize, jbyte*);
    void        (*GetCharArrayRegion)(JNINativeInterface**, jcharArray,
                        jsize, jsize, jchar*);
    void        (*GetShortArrayRegion)(JNINativeInterface**, jshortArray,
                        jsize, jsize, jshort*);
    void        (*GetIntArrayRegion)(JNINativeInterface**, jintArray,
                        jsize, jsize, jint*);
    void        (*GetLongArrayRegion)(JNINativeInterface**, jlongArray,
                        jsize, jsize, jlong*);
    void        (*GetFloatArrayRegion)(JNINativeInterface**, jfloatArray,
                        jsize, jsize, jfloat*);
    void        (*GetDoubleArrayRegion)(JNINativeInterface**, jdoubleArray,
                        jsize, jsize, jdouble*);

    /* spec shows these without const; some jni.h do, some don't */
    void        (*SetBooleanArrayRegion)(JNINativeInterface**, jbooleanArray,
                        jsize, jsize, const jboolean*);
    void        (*SetByteArrayRegion)(JNINativeInterface**, jbyteArray,
                        jsize, jsize, const jbyte*);
    void        (*SetCharArrayRegion)(JNINativeInterface**, jcharArray,
                        jsize, jsize, const jchar*);
    void        (*SetShortArrayRegion)(JNINativeInterface**, jshortArray,
                        jsize, jsize, const jshort*);
    void        (*SetIntArrayRegion)(JNINativeInterface**, jintArray,
                        jsize, jsize, const jint*);
    void        (*SetLongArrayRegion)(JNINativeInterface**, jlongArray,
                        jsize, jsize, const jlong*);
    void        (*SetFloatArrayRegion)(JNINativeInterface**, jfloatArray,
                        jsize, jsize, const jfloat*);
    void        (*SetDoubleArrayRegion)(JNINativeInterface**, jdoubleArray,
                        jsize, jsize, const jdouble*);

    jint        (*RegisterNatives)(JNINativeInterface**, jclass, const JNINativeMethod*,
                        jint);
    jint        (*UnregisterNatives)(JNINativeInterface**, jclass);
    jint        (*MonitorEnter)(JNINativeInterface**, jobject);
    jint        (*MonitorExit)(JNINativeInterface**, jobject);
    jint        (*GetJavaVM)(JNINativeInterface**, JNIInvokeInterface***);

    void        (*GetStringRegion)(JNINativeInterface**, jstring, jsize, jsize, jchar*);
    void        (*GetStringUTFRegion)(JNINativeInterface**, jstring, jsize, jsize, char*);

    void*       (*GetPrimitiveArrayCritical)(JNINativeInterface**, jarray, jboolean*);
    void        (*ReleasePrimitiveArrayCritical)(JNINativeInterface**, jarray, void*, jint);

    const jchar* (*GetStringCritical)(JNINativeInterface**, jstring, jboolean*);
    void        (*ReleaseStringCritical)(JNINativeInterface**, jstring, const jchar*);

    jweak       (*NewWeakGlobalRef)(JNINativeInterface**, jobject);
    void        (*DeleteWeakGlobalRef)(JNINativeInterface**, jweak);

    jboolean    (*ExceptionCheck)(JNINativeInterface**);

    jobject     (*NewDirectByteBuffer)(JNINativeInterface**, void*, jlong);
    void*       (*GetDirectBufferAddress)(JNINativeInterface**, jobject);
    jlong       (*GetDirectBufferCapacity)(JNINativeInterface**, jobject);

    /* added in JNI 1.6 */
    jobjectRefType (*GetObjectRefType)(JNINativeInterface**, jobject);
};


typedef const struct JNINativeInterface* JNIEnv;
typedef const struct JNIInvokeInterface* JavaVM;

/*
 * JNI invocation interface.
 */
struct JNIInvokeInterface {
    void*       reserved0;
    void*       reserved1;
    void*       reserved2;

    jint        (*DestroyJavaVM)(JavaVM*);
    jint        (*AttachCurrentThread)(JavaVM*, JNIEnv**, void*);
    jint        (*DetachCurrentThread)(JavaVM*);
    jint        (*GetEnv)(JavaVM*, void**, jint);
    jint        (*AttachCurrentThreadAsDaemon)(JavaVM*, JNIEnv**, void*);
};

typedef struct {
    jint        version;    /* must be >= JNI_VERSION_1_2 */
    const char* name;       /* NULL or name of thread as modified UTF-8 str */
    jobject     group;      /* global ref of a ThreadGroup object, or NULL */
} JavaVMAttachArgs;

/*
 * JNI 1.2+ initialization.  (As of 1.6, the pre-1.2 structures are no
 * longer supported.)
 */
typedef struct {
    const char* optionString;
    void*       extraInfo;
} JavaVMOption;

typedef struct {
    jint        version;    /* use JNI_VERSION_1_2 or later */

    jint        nOptions;
    JavaVMOption* options;
    jboolean    ignoreUnrecognized;
} JavaVMInitArgs;
