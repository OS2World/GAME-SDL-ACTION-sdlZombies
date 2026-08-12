/*
 * SDL2 OS/2 GCC Compatibility Header
 *
 * This header ensures that OS/2 x86 memory model keywords (_near, _far,
 * _cdecl, _huge, _FAR_) are properly neutralized when compiling with
 * GCC 9.x on OS/2. These keywords are used by OS/2 system headers but
 * GCC cannot parse them as type specifiers. This header must be included
 * FIRST in every source file that uses OS/2 headers.
 */

#ifndef SDL_OS2_FIX_H
#define SDL_OS2_FIX_H

/*==========================================================================*/
/*  1. The key to resolving OS/2 compatibility issues is to neutralize     */
/*      memory model keywords BEFORE any system headers are processed.   */
/*==========================================================================*/

/* Save any existing macro definitions in case they're needed later */
#pragma push_macro("_near")
#pragma push_macro("_far")
#pragma push_macro("_cdecl")
#pragma push_macro("_huge")
#pragma push_macro("_FAR_")

/* Undefine the OS/2 memory model keywords so they can be redefined */
#undef _near
#undef _far
#undef _cdecl
#undef _huge
#undef _FAR_

/* Redefine them as empty tokens - this makes GCC ignore them        */
/* while still allowing OS/2 headers to compile                      */
#define _near   /* empty - GCC will treat this as 'nothing' */
#define _far    /* empty - GCC will treat this as 'nothing' */
#define _cdecl  /* empty - GCC will treat this as 'nothing' */
#define _huge   /* empty - GCC will treat this as 'nothing' */
#define _FAR_   /* empty - GCC will treat this as 'nothing' */

/*==========================================================================*/
/*  2. Handle the errno/_errno conflict                                            */
/*==========================================================================*/

/*
 * GCC defines _errno() as a function returning int*:
 *   extern int * _errno(void);
 *
 * But OS/2 headers declare errno as:
 *   extern int _near _cdecl volatile errno;
 *
 * After we neuter the keywords, the OS/2 declaration becomes:
 *   extern int volatile errno;
 *
 * We need to provide our own errno variable to avoid conflicts.
 */

#pragma push_macro("errno")

/* Remove any existing errno definition */
#ifdef errno
#   undef errno
#endif

/* Provide our own errno variable */
static int errno_storage = 0;
#define errno errno_storage

#pragma pop_macro("errno")

/*==========================================================================*/
/*  3. Restore the original macro definitions for later use               */
/*==========================================================================*/
#pragma pop_macro("_near")
#pragma pop_macro("_far")
#pragma pop_macro("_cdecl")
#pragma pop_macro("_huge")
#pragma pop_macro("_FAR_")

#endif /* SDL_OS2_FIX_H */