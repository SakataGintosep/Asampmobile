/*
 * Copyright (c) 2006, 2010, 2013 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

#ifndef _SYSTEM_SIMPLE_H_
#define _SYSTEM_SIMPLE_H_

#include <sys/cdefs.h>
#include <stdarg.h>

#include <Availability.h>

typedef void *_SIMPLE_STRING;
typedef const char *_esc_func(unsigned char);

__BEGIN_DECLS
/*
 * A simplified vfprintf variant.  The format string is interpreted with
 * arguments from the va_list, and the results are written to the given
 * file descriptor.
 */
void _simple_vdprintf(int __fd, const char *__fmt, va_list __ap) __printflike(2, 0);

/*
 * A simplified fprintf variant.  The format string is interpreted with
 * arguments from the variable argument list, and the results are written
 * to the given file descriptor.
 */
void _simple_dprintf(int __fd, const char *__fmt, ...) __printflike(2, 3);

/*
 * A simplified string allocate routine.  Pass the opaque pointer to structure
 * to _simple_*sprintf() routines.  Use _simple_string() to retrieve the
 * current string (the string is guaranteed to be null terminated only on
 * the call to _simple_string()).  Use _simple_sfree() to free the structure
 * and string memory.
 */
_SIMPLE_STRING _simple_salloc(void);

/*
 * The format string is interpreted with arguments from the va_list, and the
 * results are appended to the string maintained by the opaque structure, as
 * returned by a previous call to _simple_salloc().  Non-zero is returned on
 * out-of-memory error.
 */
int _simple_vsprintf(_SIMPLE_STRING __b, const char *__fmt, va_list __ap) __printflike(2, 0);

/*
 * The format string is interpreted with arguments from the variable argument
 * list, and the results are appended to the string maintained by the opaque
 * structure, as returned by a previous call to _simple_salloc().  Non-zero is
 * returned on out-of-memory error.
 */
int _simple_sprintf(_SIMPLE_STRING __b, const char *__fmt, ...) __printflike(2, 3);

/*
 * Like _simple_vsprintf(), except __esc is a function to call on each
 * character; the function returns NULL if the character should be passed
 * as is, otherwise, the returned character string is used instead.
 */
int _simple_vesprintf(_SIMPLE_STRING __b, _esc_func __esc, const char *__fmt, va_list __ap) __printflike(3, 0);

/*
 * Like _simple_sprintf(), except __esc is a function to call on each
 * character; the function returns NULL if the character should be passed
 * as is, otherwise, the returned character string is used instead.
 */
int _simple_esprintf(_SIMPLE_STRING __b, _esc_func __esc, const char *__fmt, ...) __printflike(3, 4);

/*
 * Return the null terminated string from the opaque structure, as returned
 * by a previous call to _simple_salloc().
 */
char *_simple_string(_SIMPLE_STRING __b);

/*
 * Reposition the pointer to the first null in the buffer.  After a call to
 * _simple_string, the buffer can be modified, and shrunk.
 */
void _simple_sresize(_SIMPLE_STRING __b);

/*
 * Append the null-terminated string to the string associated with the opaque
 * structure.  Non-zero is returned on out-of-memory error.
 */
int _simple_sappend(_SIMPLE_STRING __b, const char *__str);

/*
 * Like _simple_sappend(), except __esc is a function to call on each
 * character; the function returns NULL if the character should be passed
 * as is, otherwise, the returned character string is used instead.
 */
int _simple_esappend(_SIMPLE_STRING __b, _esc_func __esc, const char *__str);

/*
 * Write the string associated with the opaque structure to the file descriptor.
 */
void _simple_put(_SIMPLE_STRING __b, int __fd);

/*
 * Write the string associated with the opaque structure and a trailing newline,
 * to the file descriptor.
 */
void _simple_putline(_SIMPLE_STRING __b, int __fd);

/*
 * Free the opaque structure, and the associated string.
 */
void _simple_sfree(_SIMPLE_STRING __b);

/*
 * Simplified ASL log interface; does not use malloc.  Unfortunately, this
 * requires knowledge of the format used by ASL.
 */
#ifndef ASL_LEVEL_DEBUG
#define ASL_LEVEL_EMERG   0
#define ASL_LEVEL_ALERT   1
#define ASL_LEVEL_CRIT    2
#define ASL_LEVEL_ERR     3
#define ASL_LEVEL_WARNING 4
#define ASL_LEVEL_NOTICE  5
#define ASL_LEVEL_INFO    6
#define ASL_LEVEL_DEBUG   7
#endif

void _simple_asl_log(int __level, const char *__facility, const char *__message);
void _simple_asl_log_prog(int level, const char *facility, const char *message, const char *progname);

__OSX_AVAILABLE_STARTING(__MAC_10_9,__IPHONE_7_0)
_SIMPLE_STRING _simple_asl_msg_new(void);

__OSX_AVAILABLE_STARTING(__MAC_10_9,__IPHONE_7_0)
void _simple_asl_msg_set(_SIMPLE_STRING __b, const char *__key, const char *__val);

__OSX_AVAILABLE_STARTING(__MAC_10_9,__IPHONE_7_0)
void _simple_asl_send(_SIMPLE_STRING __b);

__OSX_AVAILABLE_STARTING(__MAC_10_9,__IPHONE_7_0)
const char *_simple_getenv(const char *envp[], const char *var);

__END_DECLS

#endif /* _SYSTEM_SIMPLE_H_ */
