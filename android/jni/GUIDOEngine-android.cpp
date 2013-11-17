#include "GUIDOEngine.h"
#include "GUIDOParse.h"
#include "CairoSystem.h"

#include "guido2.h"

#include <string>
#include <sstream>

#include <jni.h>

extern "C" {
  JNIEXPORT jint JNICALL Java_com_grame_guidoandroid_GuidoAndroid_init(JNIEnv *env, jobject thisObj);
  JNIEXPORT jint JNICALL Java_com_grame_guidoandroid_GuidoAndroid_shutdown(JNIEnv *env, jobject thisObj);
  JNIEXPORT jstring JNICALL Java_com_grame_guidoandroid_GuidoAndroid_gmntosvg(JNIEnv *env, jobject thisObj, jstring java_gmn);
};

JNIEXPORT
jint JNICALL
Java_com_grame_guidoandroid_GuidoAndroid_init(JNIEnv *env, jobject thisObj)
{
   CairoSystem *sys = new CairoSystem(0);
   VGDevice * dev = sys->CreateMemoryDevice(10,10);

   GuidoInitDesc desc;
   desc.graphicDevice = dev;
   desc.musicFont = "guido2";
   desc.textFont  = "Times";
   return GuidoInit (&desc);
   return 0;
}

JNIEXPORT
jint JNICALL
Java_com_grame_guidoandroid_GuidoAndroid_shutdown(JNIEnv *env, jobject thisObj)
{
   //GuidoShutdown();
   return 0;
}

JNIEXPORT
jstring JNICALL
Java_com_grame_guidoandroid_GuidoAndroid_gmntosvg(JNIEnv *env, jobject thisObj, jstring java_gmn)
{
   const char *gmn = env->GetStringUTFChars(java_gmn, NULL);
   if (NULL == gmn) {
     return NULL;
   }
   
    GuidoParser *parser = GuidoOpenParser();
    ARHandler arh = 0;
    GRHandler grh = 0;

    arh = GuidoString2AR (parser, gmn);
    if (!arh) {
      GuidoCloseParser(parser);
      env->ReleaseStringUTFChars(java_gmn, gmn);  // release resources
      return NULL;
    }

    GuidoCloseParser(parser);
    GuidoErrCode err;

    err = GuidoAR2GR (arh, 0, &grh);
    if (err != guidoNoErr) {
        env->ReleaseStringUTFChars(java_gmn, gmn);  // release resources
        return NULL;
    }

    err = GuidoResizePageToMusic (grh);
    if (err != guidoNoErr) {
      env->ReleaseStringUTFChars(java_gmn, gmn);  // release resources
      return NULL;
    }

    std::stringstream out;

    err = GuidoSVGExport (grh, 1, out, 0, guido2);
    if (err != guidoNoErr) {
      env->ReleaseStringUTFChars(java_gmn, gmn);  // release resources
      return NULL;
    }

    return env->NewStringUTF(out.str().c_str());
    return NULL;
}
