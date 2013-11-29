import subprocess
import os

# first, we need to create the Application.mk file
APPLICATION_MK = file('android-cairo/jni/Application.mk','w')
APPLICATION_MK.write('APP_ABI := armeabi-v7a\n')
APPLICATION_MK.close()

# then, we need to build the Android.mk file for pixman
ANDROID_MK = file('android-cairo/jni/Android.mk','w')
PIXMAN_MK = file('android-cairo/jni/pixman.mk','r')
PIXMAN = PIXMAN_MK.read()
PIXMAN_MK.close()
ANDROID_MK.write(PIXMAN+'\n')
ANDROID_MK.close()
subprocess.call('ndk-build', shell=True, cwd='android-cairo/')

# then, we need to build the Android.mk file for cairo
ANDROID_MK = file('android-cairo/jni/Android.mk','w')
CAIRO_MK = file('android-cairo/jni/cairo.mk','r')
CAIRO = CAIRO_MK.read()
CAIRO_MK.close()
ANDROID_MK.write(CAIRO+'\n')
ANDROID_MK.close()
subprocess.call('ndk-build', shell=True, cwd='android-cairo/')
