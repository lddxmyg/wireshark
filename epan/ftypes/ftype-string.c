/*
 * $Id: ftype-string.c,v 1.11 2003/07/30 22:20:04 guy Exp $
 *
 * Ethereal - Network traffic analyzer
 * By Gerald Combs <gerald@ethereal.com>
 * Copyright 2001 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ftypes-int.h>
#include <string.h>

static void
string_fvalue_new(fvalue_t *fv)
{
	fv->value.string = NULL;
}

static void
string_fvalue_free(fvalue_t *fv)
{
	if (fv->value.string) {
		g_free(fv->value.string);
	}
}

static void
string_fvalue_set(fvalue_t *fv, gpointer value, gboolean already_copied)
{
	g_assert(value != NULL);
	if (already_copied) {
		fv->value.string = value;
	}
	else {
		fv->value.string = g_strdup(value);
	}
}

static int
string_repr_len(fvalue_t *fv, ftrepr_t rtype)
{
	gchar *p, c;
	int repr_len;

	switch (rtype) {
		case FTREPR_DISPLAY:
			return strlen(fv->value.string);
		case FTREPR_DFILTER:
			repr_len = 0;
			for (p = fv->value.string; (c = *p) != '\0'; p++) {
				if (c == '\\') {
					/* Backslashes must be escaped. */
					repr_len++;
				}
				repr_len++;
			}
			return repr_len + 2;	/* string plus leading and trailing quotes */
	}
	g_assert_not_reached();
	return -1;
}

static void
string_to_repr(fvalue_t *fv, ftrepr_t rtype, char *buf)
{
	gchar *p, c;
	char *bufp;

	if (rtype == FTREPR_DFILTER) {
		bufp = buf;
		*bufp++ = '"';
		for (p = fv->value.string; (c = *p) != '\0'; p++) {
			if (c == '\\') {
				/* Backslashes must be escaped. */
				*bufp++ = '\\';
			}
			*bufp++ = c;
		}
		*bufp++ = '"';
		*bufp = '\0';
	}
	else {
		strcpy(buf, fv->value.string);
	}
}


static gpointer
value_get(fvalue_t *fv)
{
	return fv->value.string;
}

static gboolean
val_from_string(fvalue_t *fv, char *s, LogFunc logfunc _U_)
{
	fv->value.string = g_strdup(s);
	return TRUE;
}

static guint
len(fvalue_t *fv)
{
	return strlen(fv->value.string);
}

static void
slice(fvalue_t *fv, GByteArray *bytes, guint offset, guint length)
{
	guint8* data;

	data = fv->value.string + offset;

	g_byte_array_append(bytes, data, length);
}


static gboolean
cmp_eq(fvalue_t *a, fvalue_t *b)
{
	return (strcmp(a->value.string, b->value.string) == 0);
}

static gboolean
cmp_ne(fvalue_t *a, fvalue_t *b)
{
	return (strcmp(a->value.string, b->value.string) != 0);
}

static gboolean
cmp_gt(fvalue_t *a, fvalue_t *b)
{
	return (strcmp(a->value.string, b->value.string) > 0);
}

static gboolean
cmp_ge(fvalue_t *a, fvalue_t *b)
{
	return (strcmp(a->value.string, b->value.string) >= 0);
}

static gboolean
cmp_lt(fvalue_t *a, fvalue_t *b)
{
	return (strcmp(a->value.string, b->value.string) < 0);
}

static gboolean
cmp_le(fvalue_t *a, fvalue_t *b)
{
	return (strcmp(a->value.string, b->value.string) <= 0);
}

void
ftype_register_string(void)
{

	static ftype_t string_type = {
		"FT_STRING",			/* name */
		"character string",		/* pretty_name */
		0,				/* wire_size */
		string_fvalue_new,		/* new_value */
		string_fvalue_free,		/* free_value */
		val_from_string,		/* val_from_unparsed */
		val_from_string,		/* val_from_string */
		string_to_repr,			/* val_to_string_repr */
		string_repr_len,		/* len_string_repr */

		string_fvalue_set,		/* set_value */
		NULL,				/* set_value_integer */
		NULL,				/* set_value_floating */

		value_get,			/* get_value */
		NULL,				/* get_value_integer */
		NULL,				/* get_value_floating */

		cmp_eq,
		cmp_ne,
		cmp_gt,
		cmp_ge,
		cmp_lt,
		cmp_le,

		len,
		slice,
	};
	static ftype_t stringz_type = {
		"FT_STRINGZ",
		"character string",
		0,
		string_fvalue_new,
		string_fvalue_free,
		val_from_string,		/* val_from_unparsed */
		val_from_string,		/* val_from_string */
		NULL,				/* val_to_string_repr */
		NULL,				/* len_string_repr */

		string_fvalue_set,
		NULL,
		NULL,

		value_get,
		NULL,
		NULL,

		cmp_eq,
		cmp_ne,
		cmp_gt,
		cmp_ge,
		cmp_lt,
		cmp_le,

		len,
		slice,
	};
	static ftype_t uint_string_type = {
		"FT_UINT_STRING",
		"character string",
		0,
		string_fvalue_new,
		string_fvalue_free,
		val_from_string,		/* val_from_unparsed */
		val_from_string,		/* val_from_string */
		NULL,				/* val_to_string_repr */
		NULL,				/* len_string_repr */

		string_fvalue_set,
		NULL,
		NULL,

		value_get,
		NULL,
		NULL,

		cmp_eq,
		cmp_ne,
		cmp_gt,
		cmp_ge,
		cmp_lt,
		cmp_le,

		len,
		slice,
	};

	ftype_register(FT_STRING, &string_type);
	ftype_register(FT_STRINGZ, &stringz_type);
	ftype_register(FT_UINT_STRING, &uint_string_type);
}
