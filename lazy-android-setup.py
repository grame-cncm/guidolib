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

subprocess.call('git submodule init', shell=True, cwd=".")
subprocess.call('git submodule update', shell=True, cwd=".")
subprocess.call('git checkout master', shell=True, cwd='android-apps/android-cmake')
subprocess.call('git submodule init', shell=True, cwd='android-apps/android-cairo')
subprocess.call('git submodule update', shell=True, cwd='android-apps/android-cairo')
subprocess.call('git checkout master', shell=True, cwd='android-apps/android-cairo')

try :
  os.makedirs('build/android')
  print "successfully created build/android"
except OSError :
  print "build/android already exists"

subprocess.call('./autogen.sh', shell=True, cwd='android-apps/android-cairo')

print "pixman and cairo for android built"

subprocess.call('cmake -DANDROID=1 ../../cmake', shell=True, cwd='build/android')
subprocess.call('make', shell=True, cwd='build/android')

print '''If this succeeded, to compile a test android project, go to :

  android-apps/simple-guido-editor

and follow the instructions in the readme.

If this didn't succeed, e-mail mike@mikesolomon.org.

Happy hacking!
'''