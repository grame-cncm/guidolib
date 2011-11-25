
--------------------------------------------------------------

This is the place when every mac-os specific code and projects must stay.

--------------------------------------------------------------
 Quick Note about CFM and Mach-O target:

- CFM (Code Fragment Manager) is the old binary format of code objects, 
it's the Mac-OS 9 format. It is supported by codewarrior, but not by Project 
Builder. Carbon applications that mut run under both OS-9 and OS-X must be 
build as CFM.

- Mach-O is the native binary format of MacOS X. For Guido apps and libs, 
please use Mach-O whenever possible. OS-9 is no longer supported by Apple.

