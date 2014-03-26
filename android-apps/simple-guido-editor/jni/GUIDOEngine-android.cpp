#include "GUIDOEngine.h"
#include "GUIDOParse.h"
#include "SVGSystem.h"

#include "guido2.h"

#include <string>
#include <sstream>

#include <jni.h>

extern "C" {
  JNIEXPORT jint JNICALL Java_com_grame_simpleguidoeditor_SimpleGuidoEditor_init(JNIEnv *env, jclass kls);
  JNIEXPORT jint JNICALL Java_com_grame_simpleguidoeditor_SimpleGuidoEditor_shutdown(JNIEnv *env, jclass kls);
  JNIEXPORT jstring JNICALL Java_com_grame_simpleguidoeditor_SimpleGuidoEditor_gmntosvg(JNIEnv *env, jclass kls, jstring java_gmn);
};

JNIEXPORT
jint JNICALL
Java_com_grame_simpleguidoeditor_SimpleGuidoEditor_init(JNIEnv *env, jclass kls)
{
   SVGSystem *sys = new SVGSystem(0, reinterpret_cast<char *>(______src_guido2_svg));
   VGDevice *dev = sys->CreateMemoryDevice(10,10);

   GuidoInitDesc desc;
   desc.graphicDevice = dev;
   desc.musicFont = "guido2";
   desc.textFont  = "Times";
   GuidoErrCode err = GuidoInit (&desc);
   if (err != guidoNoErr) {std::cerr << "INIT ERR" << std::endl;}
   std::cerr << "SUCCESS GUIDO INIT" << std::endl;
   return err;
}

JNIEXPORT
jint JNICALL
Java_com_grame_simpleguidoeditor_SimpleGuidoEditor_shutdown(JNIEnv *env, jclass kls)
{
   //GuidoShutdown();
   return 0;
}

JNIEXPORT
jstring JNICALL
Java_com_grame_simpleguidoeditor_SimpleGuidoEditor_gmntosvg(JNIEnv *env, jclass kls, jstring java_gmn)
{
   const char *gmn = env->GetStringUTFChars(java_gmn, NULL);
   if (NULL == gmn) {std::cerr << "CONVERSION_ERROR at GETTING GMN" << std::endl;
     return NULL;
   }
   
    GuidoParser *parser = GuidoOpenParser();
    ARHandler arh = 0;
    GRHandler grh = 0;

    arh = GuidoString2AR (parser, gmn);
    if (!arh) {std::cerr << "CONVERSION_ERROR at AR" << std::endl;
      GuidoCloseParser(parser);
      env->ReleaseStringUTFChars(java_gmn, gmn);  // release resources
      if (grh) GuidoFreeGR(grh);
      if (arh) GuidoFreeAR(arh);
      return NULL;
    }

    GuidoCloseParser(parser);
    GuidoErrCode err;

    err = GuidoAR2GR (arh, 0, &grh);
    if (err != guidoNoErr) {std::cerr << "CONVERSION_ERROR at AR2GH" << std::endl;
      env->ReleaseStringUTFChars(java_gmn, gmn);  // release resources
      if (grh) GuidoFreeGR(grh);
      if (arh) GuidoFreeAR(arh);
      return NULL;
    }

    err = GuidoResizePageToMusic (grh);
    if (err != guidoNoErr) {std::cerr << "CONVERSION_ERROR at RESIZE" << std::endl;
      env->ReleaseStringUTFChars(java_gmn, gmn);  // release resources
      if (grh) GuidoFreeGR(grh);
      if (arh) GuidoFreeAR(arh);
      return NULL;
    }

    std::stringstream out;

    err = GuidoSVGExportWithFontSpec (grh, 1, out, 0, reinterpret_cast<char *>(______src_guido2_svg));
    if (err != guidoNoErr) {
std::cerr << "CONVERSION_ERROR at SVG EXPORT" << std::endl;
      env->ReleaseStringUTFChars(java_gmn, gmn);  // release resources
      if (grh) GuidoFreeGR(grh);
      if (arh) GuidoFreeAR(arh);
      return NULL;
    }
std::cerr << "SUCCESS_GUIDO SVG" << std::endl <<  out.str() << std::endl;
    if (grh) GuidoFreeGR(grh);
    if (arh) GuidoFreeAR(arh);
    return env->NewStringUTF(out.str().c_str());
}
