/*
 * OS/2 Compatibility Header for GCC 9.x
 *
 * Must be included FIRST in every source file that uses OS/2 headers.
 * Neutralizes OS/2 memory model keywords and provides compatible errno.
 */

#ifndef OS2_COMPAT_H
#define OS2_COMPAT_H

/* Neutralize OS/2 memory model keywords */
#undef _near    /* Near memory model */
#undef _far     /* Far memory model */
#undef _cdecl   /* Default calling convention */
#undef _huge    /* Huge memory model */
#undef _FAR_    /* Far pointer in function declarations */

/* Define as empty tokens to make them disappear in code */
#define _near
#define _far
#define _cdecl
#define _huge
#define _FAR_

/* Handle errno/_errno conflict */
extern int errno_storage;    /* Single global variable for all modules */
#define errno errno_storage        /* Replace 'errno' */
#define _errno errno_storage       /* Replace '_errno' */

/* Data path for the game */
#define SRC_DIR ""

/* OS/2 event/message types */
typedef struct {
    long msg_id;
    char *msg_text;
} Message;

typedef struct {
    int event_type;
    char *file_path;
} FileEvent;

#endif /* OS2_COMPAT_H */