
// Following code was previously in GUIDOEngine.cpp

#ifdef JAVADLL

#include "creategif.h"
#include <jni.h>

	JNIEXPORT jint JNICALL Java_NotationRenderer_parse__Ljava_lang_String_2
		(JNIEnv *env,
		jclass obj,
		jstring filename)
	{
	
		const char * str = env->GetStringUTFChars(filename,0);
		int i = GuidoParse2(str,0);
		env->ReleaseStringUTFChars(filename,str);
		return i;
	}

	JNIEXPORT void JNICALL Java_NotationRenderer_free__I
		( JNIEnv *env,
		jclass obj,
		jint id)
	{
		GuidoFreeHandle((int) id);	
	}

	JNIEXPORT int JNICALL Java_NotationRenderer_GuidoGetNumPages__I
		(JNIEnv *env,
		jclass obj,
		jint id)
	{
		return GuidoGetNumPages((int) id);
	}

	JNIEXPORT jbyteArray JNICALL 
		Java_NotationRenderer_getGIF__IFII
		(JNIEnv *env,
		jclass obj,
		jint id,
		jfloat zoom,
		jint pagenum,
		jint adjustpagesize)
	{
		// now we need to call the equivalent GMN2GIF-Function ...

		// create a temporary filename ... -> date + random ...

		char *name=tmpnam(NULL);

		FILE *mylog = fopen("mylog","a");

		fprintf(mylog,"Tempname : %s\n",name);

		fflush(mylog);

		if (name == NULL)
		{
			return NULL;
		}

		int ret = CreateGIFPicture(id,
			(double) zoom,0,0,adjustpagesize,
			pagenum,0,NULL,
			name);

		// now we read in the file that was 
		// just created and save it in the byte-Array ...
		FILE *myfile = fopen(name,"rb");
	
		// read binary ...

		if (!myfile)
		{
			fprintf(mylog,"File %s could not be opened\n",name);
			fflush(mylog);
			return NULL;
		}
		
		fseek(myfile,0,SEEK_END);
		long length = ftell(myfile);
		fseek(myfile,0,SEEK_SET);

		fprintf(mylog,"length %ld\n",length);
		fflush(mylog);

		// how to we declare a new byteArray ?
		jbyteArray mybytearray = env->NewByteArray(length);

		if (mybytearray == NULL)
		{
			fprintf(mylog,"Array could not be constructed\n");
			fflush(mylog);
			return NULL;
		}
		// now we set the elements ...
		jbyte *mybytes = env->GetByteArrayElements(mybytearray,NULL);

		if (mybytes == NULL)
		{
			fprintf(mylog,"Could not get a contiguos memory ...\n");
			fflush(mylog);
			return NULL;
		}

		fprintf(mylog,"Now reading %ld bytes\n",length);

		fflush(mylog);

		fread(mybytes,length,1,myfile);

		fclose(myfile);

		remove(name);

		// copy back changes ...
		env->ReleaseByteArrayElements(mybytearray,mybytes,0);

		return mybytearray;
	}

jmethodID ggsoswrite = NULL;
JNIEnv    *jnienv = NULL;
jclass    ggsos;

	// This routine is being called from the
    // GuidoGraphicStreamOutput-routines of the classlibrary
    // it justs writes the information on
    // the given Stream ...
	void AddGGSOutput(const char *s)
	{
		// this routine is called by the classes
		// when they have to produce GGS-Output ...
		if (jnienv == NULL || 
			ggsoswrite == NULL ||
			ggsos == NULL) return;


		jstring mystring = jnienv->NewStringUTF(s);
		jnienv->CallVoidMethod(ggsos,ggsoswrite,mystring);
	}


	JNIEXPORT void JNICALL 
		Java_NotationRenderer_GuidoGetGuidoGraphicStream__IILjava_io_PrintStream_2
		(JNIEnv *env,
		jclass obj,
		jint id,
		jint numpage,
		jclass os)
	{
		// this is called to get the GuidoGraphicStream ...
		// it gets an outputstream that can be written ...


		jnienv = env;
		ggsos  = os;


		jclass myclass = env->GetObjectClass(os);


		// ggsoswrite =
		// 	env->GetMethodID(myclass,"write","([B)V");
		ggsoswrite =
		 	env->GetMethodID(myclass,"print","(Ljava/lang/String;)V");
		
		if (ggsoswrite == NULL)
		{
			FILE *mylog = fopen("mylog","a");
			fprintf(mylog,"Could not find method ID\n");
			fflush(mylog);
			return;
		}

		// now we call the function to retrieve the
		// data ...

		GuidoGraphicStreamOutput(id,numpage);

		// AddGGSOutput("done\n");


		jnienv = NULL;
		ggsos = NULL;
		ggsoswrite = NULL;
	}



jmethodID guidooswrite = NULL;
jclass    guidoos;

	// This routine is being called from the
    // Guido-output-routine of the classlibrary
    // it justs writes the information on
    // the given Stream ...
	void AddGuidoOutput(const char *s)
	{
		// this routine is called by the classes
		// when they have to produce Guido-Output ...
		if (jnienv == NULL || 
			guidooswrite == NULL ||
			guidoos == NULL) return;
	

		// change this .... to the 
		// new NEWUTF-String thing ...

		jstring mystring = jnienv->NewStringUTF(s);
		jnienv->CallVoidMethod(guidoos,guidooswrite,mystring);

	}


	JNIEXPORT void JNICALL 
		Java_NotationRenderer_GuidoGetGuido__ILjava_io_PrintStream_2
		(JNIEnv *env,
		jclass obj,
		jint id,
		jclass os)
	{
		// this is called to get the GuidoGraphicStream ...
		// it gets an outputstream that can be written ...


		jnienv = env;
		guidoos  = os;


		jclass myclass = env->GetObjectClass(os);

		// guidooswrite =
		//	env->GetMethodID(myclass,"write","([B)V");
		guidooswrite = 
			env->GetMethodID(myclass,"print","(Ljava/lang/String;)V");

		if (guidooswrite == NULL)
		{
			FILE *mylog = fopen("mylog","a");
			fprintf(mylog,"Could not find method ID\n");
			fflush(mylog);
			return;
		}

		// now we call the function to retrieve the
		// data ...

		GuidoGetGuido(id);

		jnienv = NULL;
		guidoos = NULL;
		guidooswrite = NULL;
	}

	JNIEXPORT jint JNICALL 
		Java_NotationRenderer_putGuidoGraphicStream__IILjava_lang_String_2
		(JNIEnv *env,
		jclass obj,
		jint id,
		 jint page,
		jstring ggsstring)
	{
		// this is called to get the GuidoGraphicStream ...
		// it gets an outputstream that can be written ...

		const char * str = env->GetStringUTFChars(ggsstring,0);
		// now we need to do something with this string...
		
		// get it to the GRMusic ... or what?
		int ret = GuidoGraphicStreamInput(id,page,str);

		// last step: we release the JAVA string.
		env->ReleaseStringUTFChars(ggsstring,str);
		
		return ret;
	}

	JNIEXPORT jint JNICALL Java_NotationRenderer_getTestGID__I
		(JNIEnv *env,
		jclass obj,
		jint id)
	{
			return (jint) GuidoGraphicStreamTest(id);
	}

	// this routine returns info about
	// a graphics-object ...
	JNIEXPORT jstring JNICALL Java_NotationRenderer_getGraphicInfo__III
		(JNIEnv *env,
		 jclass obj,
		 jint id,
		 jint gid,
		 jint manipulation)
	{

		if (gid == 0) return NULL;
		// gid points directly into memory ...

		GObject * o = dynamic_cast<GObject *>((GObject *) gid);
		if (!o) return NULL;

		char *buf = o->getGGSInfo(manipulation);
		if (buf)
		{
			jstring mystr = env->NewStringUTF(buf);
			delete [] buf;
			return mystr;
		}
		else
			return NULL;


		//const type_info &tinfo = typeid(*o);
		// const char *name = tinfo.name();
		// jstring mystr = env->NewStringUTF(name);
		// return mystr;
		

	}

#else // JAVADLL
	// This routine is being called from the
    // GuidoGraphicStreamOutput-routines of the classlibrary
    // it justs writes the information on
    // the given Stream ...
	void AddGGSOutput(const char *s)
	{
		// we do nothing ....
	}


void AddGuidoOutput(const char *s)
{
}

#endif // JAVADLL


