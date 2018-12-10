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

#include "build-pre.h"
#include "eb.h"
#include "error.h"
#include "build-post.h"

/*
 * Examine whether the current subbook in `book' supports `MENU SEARCH'
 * or not.
 */
int
eb_have_menu(EB_Book *book)
{
  eb_lock(&book->lock);
  LOG(("in: eb_have_menu(book=%d)", (int)book->code));

  /*
   * Current subbook must have been set.
   */
  if (book->subbook_current == NULL)
    goto failed;

  /*
   * Check for the index page of menu search.
   */
  if (book->subbook_current->menu.start_page == 0)
    goto failed;

  LOG(("out: eb_have_menu() = %d", 1));
  eb_unlock(&book->lock);

  return 1;

  /*
   * An error occurs...
   */
  failed:
  LOG(("out: eb_have_menu() = %d", 0));
  eb_unlock(&book->lock);
  return 0;
}


/*
 * Menu.
 */
EB_Error_Code
eb_menu(EB_Book *book, EB_Position *position)
{
  EB_Error_Code error_code;
  int page;

  eb_lock(&book->lock);
  LOG(("in: eb_menu(book=%d)", (int)book->code));

  /*
   * Current subbook must have been set.
   */
  if (book->subbook_current == NULL) {
    error_code = EB_ERR_NO_CUR_SUB;
    goto failed;
  }

  /*
   * Check for the page number of menu search.
   */
  page = book->subbook_current->menu.start_page;
  if (page == 0) {
    error_code = EB_ERR_NO_SUCH_SEARCH;
    goto failed;
  }

  /*
   * Copy the position to `position'.
   */
  position->page = page;
  position->offset = 0;

  LOG(("out: eb_menu(position={%d,%d}) = %s",
    position->page, position->offset, eb_error_string(EB_SUCCESS)));
  eb_unlock(&book->lock);

  return EB_SUCCESS;

  /*
   * An error occurs...
   */
  failed:
  LOG(("out: eb_menu() = %s", eb_error_string(error_code)));
  eb_unlock(&book->lock);
  return error_code;
}

/*
 * Examine whether the current subbook in `book' supports `GRAPHIC MENU SEARCH'
 * or not.
 */
int
eb_have_image_menu(EB_Book *book)
{
  eb_lock(&book->lock);
  LOG(("in: eb_have_image_menu(book=%d)", (int)book->code));

  /*
   * Current subbook must have been set.
   */
  if (book->subbook_current == NULL)
    goto failed;

  /*
   * Check for the index page of graphic menu search.
   */
  if (book->subbook_current->image_menu.start_page == 0)
    goto failed;

  LOG(("out: eb_have_image_menu() = %d", 1));
  eb_unlock(&book->lock);

  return 1;

  /*
   * An error occurs...
   */
  failed:
  LOG(("out: eb_have_image_menu() = %d", 0));
  eb_unlock(&book->lock);
  return 0;
}


/*
 * Graphic Menu.
 */
EB_Error_Code
eb_image_menu(EB_Book *book, EB_Position *position)
{
  EB_Error_Code error_code;
  int page;

  eb_lock(&book->lock);
  LOG(("in: eb_image_menu(book=%d)", (int)book->code));

  /*
   * Current subbook must have been set.
   */
  if (book->subbook_current == NULL) {
    error_code = EB_ERR_NO_CUR_SUB;
    goto failed;
  }

  /*
   * Check for the page number of graphic menu search.
   */
  page = book->subbook_current->image_menu.start_page;
  if (page == 0) {
    error_code = EB_ERR_NO_SUCH_SEARCH;
    goto failed;
  }

  /*
   * Copy the position to `position'.
   */
  position->page = page;
  position->offset = 0;

  LOG(("out: eb_image_menu(position={%d,%d}) = %s",
    position->page, position->offset, eb_error_string(EB_SUCCESS)));
  eb_unlock(&book->lock);

  return EB_SUCCESS;

  /*
   * An error occurs...
   */
  failed:
  LOG(("out: eb_image_menu() = %s", eb_error_string(error_code)));
  eb_unlock(&book->lock);
  return error_code;
}
