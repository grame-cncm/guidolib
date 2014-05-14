
/******************************************************************************
 PORTABLE ROUTINES FOR WRITING PRIVATE PROFILE STRINGS --  by Joseph J. Graf
 Header file containing prototypes and compile-time configuration.
 
 [09/05/02] D. Fober - Windows definitions added
******************************************************************************/

#ifndef __profport__
#define __profport__

#define MAX_LINE_LENGTH    1024

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#define get_private_profile_int        GetPrivateProfileInt
#define get_private_profile_string     GetPrivateProfileString
#define write_private_profile_string   WritePrivateProfileString
#define write_private_profile_int      WritePrivateProfileInt
#else
int get_private_profile_int      (const char * section, const char * entry, int def, const char * file_name);
int get_private_profile_string   (const char * section, const char * entry, const char * def, char * buffer, int buffer_len, const char * file_name);
int write_private_profile_string (const char * section, const char * entry, const char * buffer, const char * file_name);
int write_private_profile_int    (const char * section, const char * entry, int val, const char * file_name);
#endif

#ifdef __cplusplus
}
#endif

#endif