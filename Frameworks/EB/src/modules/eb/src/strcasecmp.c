/*
 * Copyright (c) 1997-2006  Motoyuki Kasahara
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/*
 * This program requires the following Autoconf macros:
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#else

/* Define to empty if the keyword `const' does not work.  */
/* #define const */

/* Define if `size_t' is not defined.  */
/* #define size_t unsigned */

#endif /* not HAVE_CONFIG_H */

#include <sys/types.h>

/*
 * Compare strings.
 * Cases in the strings are insensitive.
 */
int
eb_strcasecmp(const char *string1, const char *string2)
{
  const unsigned char *string1_p = (const unsigned char *)string1;
  const unsigned char *string2_p = (const unsigned char *)string2;
  int c1, c2;

  while (*string1_p != '\0') {
    if ('a' <= *string1_p && *string1_p <= 'z')
      c1 = *string1_p - ('a' - 'A');
    else
      c1 = *string1_p;

    if ('a' <= *string2_p && *string2_p <= 'z')
      c2 = *string2_p - ('a' - 'A');
    else
      c2 = *string2_p;

    if (c1 != c2)
      return c1 - c2;

    string1_p++;
    string2_p++;
  }

  return -(*string2_p);
}


/*
 * Compare strings within `n' characters.
 * Cases in the strings are insensitive.
 */
int
eb_strncasecmp(const char *string1, const char *string2, size_t n)
{
  const unsigned char *string1_p = (const unsigned char *)string1;
  const unsigned char *string2_p = (const unsigned char *)string2;
  size_t i = n;
  int c1, c2;

  if (i <= 0)
    return 0;

  while (*string1_p != '\0') {
    if ('a' <= *string1_p && *string1_p <= 'z')
      c1 = *string1_p - ('a' - 'A');
    else
      c1 = *string1_p;

    if ('a' <= *string2_p && *string2_p <= 'z')
      c2 = *string2_p - ('a' - 'A');
    else
      c2 = *string2_p;

    if (c1 != c2)
      return c1 - c2;

    string1_p++;
    string2_p++;
    i--;
    if (i <= 0)
      return 0;
  }

  return -(*string2_p);
}
