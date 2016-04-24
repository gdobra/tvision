#ifndef FILEOPS_H
#define FILEOPS_H

#include <windows.h>
#include <io.h>

//TODO: move driveValid() from 'tdircoll.cpp' here

#define PATH_SEPARATOR_C  '\\'
#define PATH_SEPARATOR_S  "\\"
#define EXTENS_SEPARTOR_C '.'
#define EXTENS_SEPARATOR_S "."

#define FA_NORMAL   _A_NORMAL
#define FA_RDONLY   _A_RDONLY
#define FA_HIDDEN   _A_HIDDEN
#define FA_SYSTEM   _A_SYSTEM
#define FA_VOLID    _A_VOLID
#define FA_DIREC    _A_SUBDIR
#define FA_ARCH     _A_ARCH

#define finddata _finddata_t

#define findfirst   _findfirst
#define findnext    _findnext
#define findclose   _findclose


/*definitions for fnsplit return flags*/
#define DRIVE       0x01
#define DIRECTORY   0x02
#define FILENAME    0x04
#define EXTENSION   0x08
#define WILDCARDS   0x10

#ifdef __cplusplus
extern "C" {
#endif

extern __cdecl int getcurdir(char *directory);
extern __cdecl void fnmerge (char *path, const char *drive, const char *dir, const char *name, const char *ext);
extern __cdecl int fnsplit (const char *path, char *drive, char *dir, char *name, char *ext);

#ifdef __cplusplus
}
#endif



#define MAXDRIVE    4
#define MAXDIR      MAX_PATH
#define MAXPATH     MAX_PATH
#define MAXFILE     MAX_PATH
#define MAXEXT      MAX_PATH

#endif
