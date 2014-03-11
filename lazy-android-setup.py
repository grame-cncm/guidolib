import subprocess
import os
import sys

print '''Welcome to GUIDO Lazy Android setup, the Android setup tool for the
Gaston Lagaffes of Guido Development.

Before running this, you need to make sure that you are connected to the
internet and that you have done the four PATH exports required in
readme.txt involving the android NDK and SDK.
'''

RI = raw_input('Are you connected to the internet and have you done the path exports [N/y]:')

if RI != 'y' :
  print "Goodbye"
  sys.exit(1)

# init all submodules and checkout master branch

def subprocess_shortcut(command, cwd = ".", rc = 0, msg = "") :
  print "Step:", msg
  p = subprocess.Popen(command, shell=True, cwd=cwd, stdout = subprocess.PIPE, stderr = subprocess.PIPE)
  p.wait()
  if p.returncode == rc :
    print "  success with exit code", p.returncode
  else :
    print "  failed with exit code", p.returncode
    if p.stderr != '' :
      print "  and the following message was printed to stderr:"
      print p.stderr
      print ""
      sys.exit(1)

subprocess_shortcut('git submodule init', ".", 0, "Initializing git submodules.")

subprocess_shortcut('git submodule update', ".", 0, "Updating submodules")

subprocess_shortcut('git checkout master', 'android-apps/android-cmake', 0, "Checking out the master branch of android-cmake")

subprocess_shortcut('git checkout master', 'android-apps/android-cmake', 0, "Checking out the master branch of android-cmake")

subprocess_shortcut('git submodule init', 'android-apps/android-cairo', 0, "Initializing submodules of android-cairo")

subprocess_shortcut('git submodule update', 'android-apps/android-cairo', 0, "Updating submodules of android-cairo")

subprocess_shortcut('git checkout master', 'android-apps/android-cairo', 0, "Checking out the master branch of android-cairo")

try :
  os.makedirs('build/android')
  print "Successfully created build/android"
except OSError :
  print "build/android already exists, not remaking"

subprocess_shortcut('cp jni/pixman.mk jni/Android.mk', 'android-apps/android-cairo', 0, 'Creating pixman Android makefile')

subprocess_shortcut('ndk-build', 'android-apps/android-cairo', 0, 'Building pixman for android')

subprocess_shortcut('cp jni/cairo.mk jni/Android.mk', 'android-apps/android-cairo', 0, 'Creating cairo Android makefile')

subprocess_shortcut('ndk-build', 'android-apps/android-cairo', 0, 'Building cairo for android')

subprocess_shortcut('cmake -DANDROID=1 ../../cmake', 'build/android', 0, "Running cmake for android.")

subprocess_shortcut('make', 'build/android', 0, "Making C++ library compiled with android compiler.")

print '''
Congrats, you made it!
To compile a test android project, go to :

  android-apps/simple-guido-editor

and follow the instructions in the readme.

If this doesn't succeed, e-mail mike@mikesolomon.org.

Happy hacking!
'''