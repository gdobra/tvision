/*--------------------------------------------------------------*/
/* filename -           trescoll.cpp                            */
/*                                                              */
/* function(s)                                                  */
/*                      TResourceCollection member functions    */
/*--------------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

#define Uses_TResourceCollection
#define Uses_TResourceItem
#define Uses_TStreamableClass
#include <tvision\tv.h>

TResourceCollection::TResourceCollection( short aLimit, short aDelta) :
    TStringCollection( aLimit, aDelta)
{
}

void TResourceCollection::freeItem( void* item )
{
    delete ((TResourceItem*)item)->key;
    delete (TResourceItem*)item;
}

void* TResourceCollection::keyOf( void* item )
{
    return ((TResourceItem *)item)->key;
}

#if !defined(NO_STREAMABLE)

TStreamable *TResourceCollection::build()
{
    return new TResourceCollection( streamableInit );
}

void TResourceCollection::writeItem( void *obj, opstream& os )
{

   os << ((TResourceItem *)obj)->pos;
   os << ((TResourceItem *)obj)->size;
   os.writeString(((TResourceItem *)obj)->key);

}

void *TResourceCollection::readItem( ipstream& is )
{
    TResourceItem *obj;

    obj = new TResourceItem;
    is >> obj->pos;
    is >> obj->size;
    obj->key = is.readString();
    return (void *)obj;
}

static
TStreamableClass RResourceCollection( TResourceCollection::name,
                                      TResourceCollection::build,
                                      __DELTA(TResourceCollection)
                                    );

const char * const _NEAR TResourceCollection::name = "TResourceCollection";

#endif

