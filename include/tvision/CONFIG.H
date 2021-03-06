/* ------------------------------------------------------------------------*/
/*                                                                         */
/*   CONFIG.H                                                              */
/*                                                                         */
/*   miscellaneous system-wide configuration parameters                    */
/*   FOR INTERNAL USE ONLY                                                 */
/*                                                                         */
/* ------------------------------------------------------------------------*/
/*
 *      Turbo Vision - Version 2.0
 *
 *      Copyright (c) 1994 by Borland International
 *      All Rights Reserved.
 *
 */

 #define __FLAT__

#if !defined( __CONFIG_H )
#define __CONFIG_H

#include <limits.h>


const int eventQSize = 16;
const int maxCollectionSize = (int)(( (long) UINT_MAX - 16)/sizeof( void * ));

const int maxViewWidth = MAX_VIEW_WIDTH;

const int maxFindStrLen    = 80;
const int maxReplaceStrLen = 80;

#endif  // __CONFIG_H
