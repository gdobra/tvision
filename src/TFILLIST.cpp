/*------------------------------------------------------------*/
/* filename -       tfillist.cpp                              */
/*                                                            */
/* function(s)                                                */
/*                  TFileList member functions                */
/*------------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#define Uses_TVMemMgr
#define Uses_MsgBox
#define Uses_TFileList
#define Uses_TRect
#define Uses_TSearchRec
#define Uses_TEvent
#define Uses_TGroup
#define Uses_TKeys
#define Uses_TStreamableClass
#include <tvision\tv.h>

#if !defined( __ERRNO_H )
#include <Errno.h>
#endif  // __ERRNO_H

#if !defined( __STDIO_H )
#include <Stdio.h>
#endif  // __STDIO_H

#if !defined( __CTYPE_H )
#include <ctype.h>
#endif  // __CTYPE_H

#if !defined( __ASSERT_H )
#include <Assert.h>
#endif  // __ASSERT_H

#if !defined( __STRING_H )
#include <String.h>
#endif  // __STRING_H

#include <tvision\fileops.h>

void fexpand( char * );


TFileList::TFileList( const TRect& bounds,
                      TScrollBar *aScrollBar) :
    TSortedListBox( bounds, 2, aScrollBar )
{
}

TFileList::~TFileList()
{
   if ( list() )
      destroy ( list() );
}

void TFileList::focusItem( short item )
{
    TSortedListBox::focusItem( item );
    message( owner, evBroadcast, cmFileFocused, list()->at(item) );
}

void TFileList::selectItem( short item )
{
    message( owner, evBroadcast, cmFileDoubleClicked, list()->at(item) );
}

void TFileList::getData( void * )
{
}

void TFileList::setData( void * )
{
}

ushort TFileList::dataSize()
{
    return 0;
}

void* TFileList::getKey( const char *s )
{
static TSearchRec sR;

    if( (shiftState & kbShift) != 0 || *s == '.' )
        sR.attr = FA_DIREC;
    else
        sR.attr = 0;
    strcpy( sR.name, s );
    strupr( sR.name );
    return &sR;
}

void TFileList::getText( char *dest, short item, short maxChars )
{
    TSearchRec *f = (TSearchRec *)(list()->at(item));

    strncpy( dest, f->name, maxChars );
    dest[maxChars] = '\0';
    if( f->attr & FA_DIREC )
        strcat( dest, "\\" );
}


void TFileList::readDirectory( const char *dir, const char *wildCard )
{
    char path[MAXPATH];
    strcpy( path, dir );
    strcat( path, wildCard );
    readDirectory( path );
}

struct DirSearchRec : public TSearchRec
{
    void readFf_blk(finddata *f)
    {
        attr = (char)f->attrib;
    //TODO: clarify file time usage
    //time = (((long)(unsigned)f->ff_fdate)<<16) | f->ff_ftime;
    time = f->time_write;
    size = f->size;
    memcpy(name, f->name, sizeof(name));
    }

    void *operator new( size_t );

};

void *DirSearchRec::operator new( size_t sz )
{
    void *temp = ::operator new( sz );
    if( TVMemMgr::safetyPoolExhausted() )
        {
        delete temp;
        temp = 0;
        }
    return temp;
}

void TFileList::readDirectory( const char *aWildCard )
{
    finddata s;
    intptr_t fh;
    char path[MAXPATH];
    char drive[MAXDRIVE];
    char dir[MAXDIR];
    char file[MAXFILE];
    char ext[MAXEXT];
    const unsigned findAttr = FA_RDONLY | FA_ARCH;
    memset(&s, 0, sizeof(s));
    strcpy( path, aWildCard );
    fexpand( path );
    fnsplit( path, drive, dir, file, ext );

    TFileCollection *fileList = new TFileCollection( 5, 5 );

    fh = findfirst( aWildCard, &s );
    int res = fh == -1 ? 1 : 0;
    DirSearchRec *p = (DirSearchRec *)&p;
    while( p != 0 && res == 0 )
        {
        if( ((s.attrib & FA_DIREC) == 0) && ((s.attrib & findAttr) != 0) )
            {
            p = new DirSearchRec;
            if( p != 0 )
                {
        p->readFf_blk(&s);
                fileList->insert( p );
                }
            }
        res = findnext( fh, &s );
        }
    findclose(fh);
    fnmerge( path, drive, dir, "*", ".*" );

    fh = findfirst( path, &s );
    res = fh == -1 ? 1 : 0;
    while( p != 0 && res == 0 )
        {
        if( (s.attrib & FA_DIREC) != 0 && s.name[0] != '.' )
            {
            p = new DirSearchRec;
            if( p != 0 )
                {
        p->readFf_blk(&s);
                fileList->insert( p );
                }
            }
        res = findnext( fh, &s );
        }

    if( strlen( dir ) > 1 )
        {
        p = new DirSearchRec;
        if( p != 0 )
            {
            if( (fh = findfirst( path, &s )) != -1 &&
                findnext( fh, &s ) == 0 &&
                ((s.attrib & FA_DIREC) != 0) &&
                strcmp( s.name, ".." ) == 0
            )
            p->readFf_blk(&s);
            else
                {
                strcpy( p->name, ".." );
                p->size = 0;
                p->time = 0x210000uL;
                p->attr = FA_DIREC;
                }
            fileList->insert( p );
            findclose(fh);
            }
        }

    if( p == 0 )
        messageBox( tooManyFiles, mfOKButton | mfWarning );
    newList(fileList);
    if( list()->getCount() > 0 )
        message( owner, evBroadcast, cmFileFocused, list()->at(0) );
    else
        {
        static DirSearchRec noFile;
        message( owner, evBroadcast, cmFileFocused, &noFile );
        }
}

/*
    fexpand:    reimplementation of pascal's FExpand routine.  Takes a
                relative DOS path and makes an absolute path of the form

                    drive:\[subdir\ ...]filename.ext

                works with '/' or '\' as the subdir separator on input;
                changes all to '\' on output.

*/

void squeeze( char *path )
{
    char *dest = path;
    char *src = path;
    while( *src != 0 )
        {
        if( *src != '.' )
            *dest++ = *src++;   // just copy it...
        else
            {
            src++;
            if( *src == '.' )
                {               // have a '..'
                src += 2;       // skip the following '\'
                dest--;         // back up to the previous '\'
                while( dest > path && *--dest != '\\' ) // back up to the previous '\'
                    ;
                dest++;         // move to the next position
                }
            else
                src++;          // skip the following '\'
            }
        }
    *dest = EOS;                // zero terminator
}

void fexpand( char *rpath )
{
    char path[MAXPATH];
    char drive[MAXDRIVE];
    char dir[MAXDIR];
    char file[MAXFILE];
    char ext[MAXEXT];

    int flags = fnsplit( rpath, drive, dir, file, ext );
    drive[0] = toupper(drive[0]);
    if( (flags & DIRECTORY) == 0 || (dir[0] != '\\' && dir[0] != '/') )
        {
        char curdir[MAXDIR];
        getcurdir( curdir );
        strcat( curdir, dir );
        if( *curdir != '\\' && *curdir != '/' )
            {
            *dir = '\\';
            strcpy( dir+1, curdir );
            }
        else
            strcpy( dir, curdir );
        }

    squeeze( dir );
    char *p = dir;
    while( (p = strchr( p, '/' )) != 0 )
        *p = '\\';
    fnmerge( path, drive, dir, file, ext );
    strupr( path );
    strcpy( rpath, path );
}

#if !defined(NO_STREAMABLE)

TStreamable *TFileList::build()
{
    return new TFileList( streamableInit );
}

static
TStreamableClass RFileList( TFileList::name,
                            TFileList::build,
                            __DELTA(TFileList)
                          );

const char * const _NEAR TFileList::name = "TFileList";

#endif
