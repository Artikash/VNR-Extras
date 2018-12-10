/*                                                            -*- C -*-
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

#ifndef EB_BUILD_POST_H
#define EB_BUILD_POST_H

#include "defs.h"

/*
 * Text domain name.
 */
#define EB_TEXT_DOMAIN_NAME  "eb"

/*
 * Locale directory.
 */
#ifndef WIN32
#define EB_LOCALEDIR    "@localedir@"
#else
#define EB_LOCALEDIR    localedir()
#endif

/*
 * Data size of a book entry in a catalog file.
 */
#define EB_SIZE_EB_CATALOG    40
#define EB_SIZE_EPWING_CATALOG    164

/*
 * Maximum number of search titles.
 */
#define EB_MAX_SEARCH_TITLES    14

/*
 * File names.
 */
#define EB_FILE_NAME_START    "start"
#define EB_FILE_NAME_HONMON    "honmon"
#define EB_FILE_NAME_FUROKU    "furoku"
#define EB_FILE_NAME_APPENDIX    "appendix"

/*
 * Directory names.
 */
#define EB_DIRECTORY_NAME_DATA    "data"
#define EB_DIRECTORY_NAME_GAIJI    "gaiji"
#define EB_DIRECTORY_NAME_STREAM  "stream"
#define EB_DIRECTORY_NAME_MOVIE    "movie"

/*
 * Search word types.
 */
#define EB_WORD_ALPHABET    0
#define EB_WORD_KANA      1
#define EB_WORD_OTHER      2
#define EB_WORD_INVALID      -1

/*
 * Index Style flags.
 */
#define EB_INDEX_STYLE_CONVERT    0
#define EB_INDEX_STYLE_ASIS    1
#define EB_INDEX_STYLE_REVERSED_CONVERT  2
#define EB_INDEX_STYLE_DELETE    2

/*
 * Text content currently read.
 */
#define EB_TEXT_MAIN_TEXT    1
#define EB_TEXT_HEADING      2
#define EB_TEXT_RAWTEXT      3
#define EB_TEXT_OPTIONAL_TEXT    4
#define EB_TEXT_SEEKED      0
#define EB_TEXT_INVALID      -1

/*
 * Search method currently processed.
 */
#define EB_SEARCH_EXACTWORD    0
#define EB_SEARCH_WORD      1
#define EB_SEARCH_ENDWORD    2
#define EB_SEARCH_KEYWORD    3
#define EB_SEARCH_MULTI      4
#define EB_SEARCH_CROSS      5
#define EB_SEARCH_NONE      -1

/*
 * Arrangement style of entries in a search index page.
 */
#define EB_ARRANGE_FIXED    0
#define EB_ARRANGE_VARIABLE    1
#define EB_ARRANGE_INVALID    -1

/*
 * Binary data types.
 */
#define EB_BINARY_MONO_GRAPHIC    0
#define EB_BINARY_COLOR_GRAPHIC    1
#define EB_BINARY_WAVE      2
#define EB_BINARY_MPEG      3
#define EB_BINARY_GRAY_GRAPHIC    4
#define EB_BINARY_INVALID    -1

/*
 * Text-stop status.
 */
#define EB_TEXT_STATUS_CONTINUED  0
#define EB_TEXT_STATUS_SOFT_STOP  1
#define EB_TEXT_STATUS_HARD_STOP  2

/*
 * The maximum index depth of search indexes.
 */
#define EB_MAX_INDEX_DEPTH    6

/*
 * The maximum length of path name relative to top directory of a CD-ROM
 * book.  An example of the longest relative path is:
 *
 *     "subdir01/subdir02/filename.ebz;1"
 */
#define EB_MAX_RELATIVE_PATH_LENGTH \
  (EB_MAX_DIRECTORY_NAME_LENGTH + 1 \
  + EB_MAX_DIRECTORY_NAME_LENGTH + 1 \
  + EB_MAX_FILE_NAME_LENGTH)

/*
 * The environment variable name to enable/disable debug messages.
 */
#define EB_DEBUG_ENVIRONMENT_VARIABLE  "EB_DEBUG"

/*
 * Trace log macro.
 */
#define LOG(x) do {if (eb_log_flag) eb_log x;} while (0)

/*
 * Get an unsigned value from an octet stream buffer.
 */
#define eb_uint1(p) (*(const unsigned char *)(p))

#define eb_uint2(p) ((*(const unsigned char *)(p) << 8) \
        + (*(const unsigned char *)((p) + 1)))

#define eb_uint3(p) ((*(const unsigned char *)(p) << 16) \
        + (*(const unsigned char *)((p) + 1) << 8) \
        + (*(const unsigned char *)((p) + 2)))

#define eb_uint4(p) ((*(const unsigned char *)(p) << 24) \
        + (*(const unsigned char *)((p) + 1) << 16) \
        + (*(const unsigned char *)((p) + 2) << 8) \
        + (*(const unsigned char *)((p) + 3)))

#define eb_uint4_le(p) ((*(const unsigned char *)(p)) \
        + (*(const unsigned char *)((p) + 1) << 8) \
        + (*(const unsigned char *)((p) + 2) << 16) \
        + (*(const unsigned char *)((p) + 3) << 24))

/*
 * Test whether the path is URL with the `ebnet' scheme.
 */
#define is_ebnet_url(p) \
  (   ((p)[0] == 'E' || (p)[0] == 'e') \
   && ((p)[1] == 'B' || (p)[1] == 'b') \
   && ((p)[2] == 'N' || (p)[2] == 'n') \
   && ((p)[3] == 'E' || (p)[3] == 'e') \
   && ((p)[4] == 'T' || (p)[4] == 't') \
   && (p)[5] == ':' && (p)[6] == '/' && (p)[7] == '/')

/*
 * Test whether `off_t' represents a large integer.
 */
#define off_t_is_large \
  ((((off_t) 1 << 41) + ((off_t) 1 << 40) + 1) % 9999991 == 7852006)

/*
 * External variable declarations.
 */
/* log.c */
extern int eb_log_flag;

/* hook.c */
extern EB_Hookset eb_default_hookset;

/*
 * Function declarations.
 */
/* appendix.c */
void eb_initialize_alt_caches(EB_Appendix *appendix);
void eb_finalize_alt_caches(EB_Appendix *appendix);

/* appsub.c */
void eb_initialize_appendix_subbooks(EB_Appendix *appendix);
void eb_finalize_appendix_subbooks(EB_Appendix *appendix);

/* bcd.c */
unsigned eb_bcd2(const char *stream);
unsigned eb_bcd4(const char *stream);
unsigned eb_bcd6(const char *stream);

/* binary.c */
void eb_initialize_binary_context(EB_Book *book);
void eb_reset_binary_context(EB_Book *book);
void eb_finalize_binary_context(EB_Book *book);

/* booklist.c */
EB_Error_Code eb_booklist_add_book(EB_BookList *booklist, const char *name,
    const char *title);

/* filename.c */
EB_Error_Code eb_canonicalize_path_name(char *path_name);
void eb_canonicalize_file_name(char *file_name);
EB_Error_Code eb_fix_directory_name(const char *path, char *directory_name);
EB_Error_Code eb_fix_directory_name2(const char *path,
    const char *directory_name, char *sub_directory_name);
void eb_fix_path_name_suffix(char *path_name, const char *suffix);
EB_Error_Code eb_find_file_name(const char *path_name,
    const char *target_file_name, char *found_file_name);
EB_Error_Code eb_find_file_name2(const char *path_name,
    const char *sub_directory_name, const char *target_file_name,
    char *found_file_name);
EB_Error_Code eb_find_file_name3(const char *path_name,
    const char *sub_directory_name, const char *sub2_directory_name,
    const char *target_file_name, char *found_file_name);
void eb_compose_path_name(const char *path_name, const char *file_name,
    char *composed_path_name);
void eb_compose_path_name2(const char *path_name,
    const char *sub_directory_name, const char *file_name,
    char *composed_path_name);
void eb_compose_path_name3(const char *path_name,
    const char *sub_directory_name, const char *sub2_directory_name,
    const char *file_name, char *composed_path_name);
void eb_path_name_zio_code(const char *path_name, Zio_Code default_zio_code,
    Zio_Code *zio_code);

/* font.c */
void eb_initialize_fonts(EB_Book *book);
void eb_load_font_headers(EB_Book *book);
void eb_finalize_fonts(EB_Book *book);

/* hook.c */
void eb_initialize_default_hookset(void);

/* jacode.c */
void eb_jisx0208_to_euc(char *out_string, const char *in_string);
void eb_sjis_to_euc(char *out_string, const char *in_string);

/* lock.c */
#ifdef ENABLE_PTHREAD
void eb_initialize_lock(EB_Lock *lock);
void eb_finalize_lock(EB_Lock *lock);
void eb_lock(EB_Lock *lock);
void eb_unlock(EB_Lock *lock);
#else /* not ENABLE_PTHREAD */
#define eb_lock(x)
#define eb_unlock(x)
#define eb_initialize_lock(x)
#define eb_finalize_lock(x)
#endif /* not ENABLE_PTHREAD */

/* log.c */
void eb_initialize_log(void);
const char *eb_quoted_stream(const char *stream, size_t stream_length);
const char *eb_quoted_string(const char *string);

/* match.c */
int eb_match_word(const char *word, const char *pattern, size_t length);
int eb_pre_match_word(const char *word, const char *pattern, size_t length);
int eb_exact_match_word_jis(const char *word, const char *pattern,
    size_t length);
int eb_exact_pre_match_word_jis(const char *word, const char *pattern,
    size_t length);
int eb_exact_match_word_latin(const char *word, const char *pattern,
    size_t length);
int eb_exact_pre_match_word_latin(const char *word, const char *pattern,
    size_t);
int eb_match_word_kana_single(const char *word, const char *pattern,
    size_t length);
int eb_match_word_kana_group(const char *word, const char *pattern,
    size_t length);
int eb_exact_match_word_kana_single(const char *word, const char *pattern,
    size_t length);
int eb_exact_match_word_kana_group(const char *word, const char *pattern,
    size_t length);

/* message.c */
EB_Error_Code eb_initialize_messages(EB_Book *book);

/* multi.c */
EB_Error_Code eb_load_multi_searches(EB_Book *book);
EB_Error_Code eb_load_multi_titles(EB_Book *book);

/* narwfont.c */
EB_Error_Code eb_open_narrow_font_file(EB_Book *book, EB_Font_Code font_code);
EB_Error_Code eb_load_narrow_font_header(EB_Book *book,
    EB_Font_Code font_code);
EB_Error_Code eb_load_narrow_font_glyphs(EB_Book *book,
    EB_Font_Code font_code);

/* search.c */
void eb_initialize_search_contexts(EB_Book *book);
void eb_finalize_search_contexts(EB_Book *book);
void eb_reset_search_contexts(EB_Book *book);
void eb_initialize_search(EB_Search *search);
void eb_finalize_search(EB_Search *search);
void eb_initialize_searches(EB_Book *book);
void eb_finalize_searches(EB_Book *book);
EB_Error_Code eb_presearch_word(EB_Book *book, EB_Search_Context *context);

/* setword.c */
EB_Error_Code eb_set_word(EB_Book *book, const char *input_word, char *word,
    char *canonicalized_word, EB_Word_Code *word_code);
EB_Error_Code eb_set_endword(EB_Book *book, const char *input_word, char *word,
    char *canonicalized_word, EB_Word_Code *word_code);
EB_Error_Code eb_set_keyword(EB_Book *book, const char *input_word, char *word,
    char *canonicalized_word, EB_Word_Code *word_code);
EB_Error_Code eb_set_multiword(EB_Book *book, EB_Multi_Search_Code multi_id,
    EB_Multi_Entry_Code entry_id, const char *input_word, char *word,
    char *canonicalized_word, EB_Word_Code *word_code);

/* subbook.c */
void eb_initialize_subbooks(EB_Book *book);
void eb_finalize_subbooks(EB_Book *book);

/* text.c */
void eb_initialize_text_context(EB_Book *book);
void eb_finalize_text_context(EB_Book *book);
void eb_reset_text_context(EB_Book *book);
void eb_invalidate_text_context(EB_Book *book);
EB_Error_Code eb_forward_heading(EB_Book *book);

/* widefont.c */
EB_Error_Code eb_open_wide_font_file(EB_Book *book, EB_Font_Code font_code);
EB_Error_Code eb_load_wide_font_header(EB_Book *book, EB_Font_Code font_code);
EB_Error_Code eb_load_wide_font_glyphs(EB_Book *book, EB_Font_Code font_code);

/* strcasecmp.c */
int eb_strcasecmp(const char *string1, const char *string2);
int eb_strncasecmp(const char *string1, const char *string2, size_t n);

#endif /* not EB_BUILD_POST_H */
