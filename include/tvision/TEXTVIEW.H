/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   TEXTVIEW.H                                                            */
/*                                                                         */
/*   defines the classes TTextDevice and TTerminal                         */
/*                                                                         */
/* ------------------------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */


#if defined( __BCOPT__ ) && !defined (__FLAT__)
#pragma option -po-
#endif

#if defined( Uses_TTextDevice ) && !defined( __TTextDevice )
#define __TTextDevice

#include <iostream>
#include <stdio.h> //for EOF constant
using namespace std;

#if defined( __BCOPT__ ) && !defined (__FLAT__)
#pragma option -po-
#endif

class _FAR TRect;
class _FAR TScrollBar;

class TTextDevice : public TScroller, public streambuf
{

public:

    TTextDevice( const TRect& bounds,
                 TScrollBar *aHScrollBar,
                 TScrollBar *aVScrollBar
               );

    virtual int do_sputn( const char *s, int count ) = 0;
    virtual int overflow( int = EOF );

};

#endif  // Uses_TTextDevice

#if defined( Uses_TTerminal ) && !defined( __TTerminal )
#define __TTerminal

class _FAR TRect;
class _FAR TScrollBar;

class TTerminal: public TTextDevice
{

public:


    TTerminal( const TRect& bounds,
               TScrollBar *aHScrollBar,
               TScrollBar *aVScrollBar,
               ushort aBufSize
             );
    ~TTerminal();

    virtual int do_sputn( const char *s, int count );

    void bufInc( ushort& val );
    Boolean canInsert( ushort amount );
    short calcWidth();
    virtual void draw();
    ushort nextLine( ushort pos );
    ushort prevLines( ushort pos, ushort lines );
    Boolean queEmpty();

protected:

    ushort bufSize;
    char *buffer;
    ushort queFront, queBack;
    void bufDec(ushort& val);
// modification #TV2N-02.01 -------------------------------
    ushort curLineWidth;   // Added horizontal cursor tracking
//  end of bug-fix. ---------------------------------------
};

#endif  // Uses_TTerminal

#if defined( Uses_otstream ) && !defined( __otstream )
#define __otstream

#include <iostream>
using namespace std;


#if defined( __BCOPT__ ) && !defined (__FLAT__)
#pragma option -po-
#endif


class otstream : public ostream
{

public:

    otstream( TTerminal * );

};


#endif


#if defined( __BCOPT__ ) && !defined (__FLAT__)
#pragma option -po.
#endif
