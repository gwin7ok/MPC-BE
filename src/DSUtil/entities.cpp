/*	Copyright 2012, 2016 Christoph Gärtner
	Distributed under the Boost Software License, Version 1.0
*/

#include "stdafx.h"

#include "entities.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define UNICODE_MAX 0x10FFFFul

static const char *const NAMED_ENTITIES[][2] = {
	{ "AElig;", "\u00C6" },
	{ "Aacute;", "\u00C1" },
	{ "Acirc;", "\u00C2" },
	{ "Agrave;", "\u00C0" },
	{ "Alpha;", "\u0391" },
	{ "Aring;", "\u00C5" },
	{ "Atilde;", "\u00C3" },
	{ "Auml;", "\u00C4" },
	{ "Beta;", "\u0392" },
	{ "Ccedil;", "\u00C7" },
	{ "Chi;", "\u03A7" },
	{ "Dagger;", "\u2021" },
	{ "Delta;", "\u0394" },
	{ "ETH;", "\u00D0" },
	{ "Eacute;", "\u00C9" },
	{ "Ecirc;", "\u00CA" },
	{ "Egrave;", "\u00C8" },
	{ "Epsilon;", "\u0395" },
	{ "Eta;", "\u0397" },
	{ "Euml;", "\u00CB" },
	{ "Gamma;", "\u0393" },
	{ "Iacute;", "\u00CD" },
	{ "Icirc;", "\u00CE" },
	{ "Igrave;", "\u00CC" },
	{ "Iota;", "\u0399" },
	{ "Iuml;", "\u00CF" },
	{ "Kappa;", "\u039A" },
	{ "Lambda;", "\u039B" },
	{ "Mu;", "\u039C" },
	{ "Ntilde;", "\u00D1" },
	{ "Nu;", "\u039D" },
	{ "OElig;", "\u0152" },
	{ "Oacute;", "\u00D3" },
	{ "Ocirc;", "\u00D4" },
	{ "Ograve;", "\u00D2" },
	{ "Omega;", "\u03A9" },
	{ "Omicron;", "\u039F" },
	{ "Oslash;", "\u00D8" },
	{ "Otilde;", "\u00D5" },
	{ "Ouml;", "\u00D6" },
	{ "Phi;", "\u03A6" },
	{ "Pi;", "\u03A0" },
	{ "Prime;", "\u2033" },
	{ "Psi;", "\u03A8" },
	{ "Rho;", "\u03A1" },
	{ "Scaron;", "\u0160" },
	{ "Sigma;", "\u03A3" },
	{ "THORN;", "\u00DE" },
	{ "Tau;", "\u03A4" },
	{ "Theta;", "\u0398" },
	{ "Uacute;", "\u00DA" },
	{ "Ucirc;", "\u00DB" },
	{ "Ugrave;", "\u00D9" },
	{ "Upsilon;", "\u03A5" },
	{ "Uuml;", "\u00DC" },
	{ "Xi;", "\u039E" },
	{ "Yacute;", "\u00DD" },
	{ "Yuml;", "\u0178" },
	{ "Zeta;", "\u0396" },
	{ "aacute;", "\u00E1" },
	{ "acirc;", "\u00E2" },
	{ "acute;", "\u00B4" },
	{ "aelig;", "\u00E6" },
	{ "agrave;", "\u00E0" },
	{ "alefsym;", "\u2135" },
	{ "alpha;", "\u03B1" },
	{ "amp;", "\u0026" },
	{ "and;", "\u2227" },
	{ "ang;", "\u2220" },
	{ "apos;", "\u0027" },
	{ "aring;", "\u00E5" },
	{ "asymp;", "\u2248" },
	{ "atilde;", "\u00E3" },
	{ "auml;", "\u00E4" },
	{ "bdquo;", "\u201E" },
	{ "beta;", "\u03B2" },
	{ "brvbar;", "\u00A6" },
	{ "bull;", "\u2022" },
	{ "cap;", "\u2229" },
	{ "ccedil;", "\u00E7" },
	{ "cedil;", "\u00B8" },
	{ "cent;", "\u00A2" },
	{ "chi;", "\u03C7" },
	{ "circ;", "\u02C6" },
	{ "clubs;", "\u2663" },
	{ "cong;", "\u2245" },
	{ "copy;", "\u00A9" },
	{ "crarr;", "\u21B5" },
	{ "cup;", "\u222A" },
	{ "curren;", "\u00A4" },
	{ "dArr;", "\u21D3" },
	{ "dagger;", "\u2020" },
	{ "darr;", "\u2193" },
	{ "deg;", "\u00B0" },
	{ "delta;", "\u03B4" },
	{ "diams;", "\u2666" },
	{ "divide;", "\u00F7" },
	{ "eacute;", "\u00E9" },
	{ "ecirc;", "\u00EA" },
	{ "egrave;", "\u00E8" },
	{ "empty;", "\u2205" },
	{ "emsp;", "\u2003" },
	{ "ensp;", "\u2002" },
	{ "epsilon;", "\u03B5" },
	{ "equiv;", "\u2261" },
	{ "eta;", "\u03B7" },
	{ "eth;", "\u00F0" },
	{ "euml;", "\u00EB" },
	{ "euro;", "\u20AC" },
	{ "exist;", "\u2203" },
	{ "fnof;", "\u0192" },
	{ "forall;", "\u2200" },
	{ "frac12;", "\u00BD" },
	{ "frac14;", "\u00BC" },
	{ "frac34;", "\u00BE" },
	{ "frasl;", "\u2044" },
	{ "gamma;", "\u03B3" },
	{ "ge;", "\u2265" },
	{ "gt;", "\u003E" },
	{ "hArr;", "\u21D4" },
	{ "harr;", "\u2194" },
	{ "hearts;", "\u2665" },
	{ "hellip;", "\u2026" },
	{ "iacute;", "\u00ED" },
	{ "icirc;", "\u00EE" },
	{ "iexcl;", "\u00A1" },
	{ "igrave;", "\u00EC" },
	{ "image;", "\u2111" },
	{ "infin;", "\u221E" },
	{ "int;", "\u222B" },
	{ "iota;", "\u03B9" },
	{ "iquest;", "\u00BF" },
	{ "isin;", "\u2208" },
	{ "iuml;", "\u00EF" },
	{ "kappa;", "\u03BA" },
	{ "lArr;", "\u21D0" },
	{ "lambda;", "\u03BB" },
	{ "lang;", "\u2329" },
	{ "laquo;", "\u00AB" },
	{ "larr;", "\u2190" },
	{ "lceil;", "\u2308" },
	{ "ldquo;", "\u201C" },
	{ "le;", "\u2264" },
	{ "lfloor;", "\u230A" },
	{ "lowast;", "\u2217" },
	{ "loz;", "\u25CA" },
	{ "lrm;", "\u200E" },
	{ "lsaquo;", "\u2039" },
	{ "lsquo;", "\u2018" },
	{ "lt;", "\u003C" },
	{ "macr;", "\u00AF" },
	{ "mdash;", "\u2014" },
	{ "micro;", "\u00B5" },
	{ "middot;", "\u00B7" },
	{ "minus;", "\u2212" },
	{ "mu;", "\u03BC" },
	{ "nabla;", "\u2207" },
	{ "nbsp;", "\u00A0" },
	{ "ndash;", "\u2013" },
	{ "ne;", "\u2260" },
	{ "ni;", "\u220B" },
	{ "not;", "\u00AC" },
	{ "notin;", "\u2209" },
	{ "nsub;", "\u2284" },
	{ "ntilde;", "\u00F1" },
	{ "nu;", "\u03BD" },
	{ "oacute;", "\u00F3" },
	{ "ocirc;", "\u00F4" },
	{ "oelig;", "\u0153" },
	{ "ograve;", "\u00F2" },
	{ "oline;", "\u203E" },
	{ "omega;", "\u03C9" },
	{ "omicron;", "\u03BF" },
	{ "oplus;", "\u2295" },
	{ "or;", "\u2228" },
	{ "ordf;", "\u00AA" },
	{ "ordm;", "\u00BA" },
	{ "oslash;", "\u00F8" },
	{ "otilde;", "\u00F5" },
	{ "otimes;", "\u2297" },
	{ "ouml;", "\u00F6" },
	{ "para;", "\u00B6" },
	{ "part;", "\u2202" },
	{ "permil;", "\u2030" },
	{ "perp;", "\u22A5" },
	{ "phi;", "\u03C6" },
	{ "pi;", "\u03C0" },
	{ "piv;", "\u03D6" },
	{ "plusmn;", "\u00B1" },
	{ "pound;", "\u00A3" },
	{ "prime;", "\u2032" },
	{ "prod;", "\u220F" },
	{ "prop;", "\u221D" },
	{ "psi;", "\u03C8" },
	{ "quot;", "\u0022" },
	{ "rArr;", "\u21D2" },
	{ "radic;", "\u221A" },
	{ "rang;", "\u232A" },
	{ "raquo;", "\u00BB" },
	{ "rarr;", "\u2192" },
	{ "rceil;", "\u2309" },
	{ "rdquo;", "\u201D" },
	{ "real;", "\u211C" },
	{ "reg;", "\u00AE" },
	{ "rfloor;", "\u230B" },
	{ "rho;", "\u03C1" },
	{ "rlm;", "\u200F" },
	{ "rsaquo;", "\u203A" },
	{ "rsquo;", "\u2019" },
	{ "sbquo;", "\u201A" },
	{ "scaron;", "\u0161" },
	{ "sdot;", "\u22C5" },
	{ "sect;", "\u00A7" },
	{ "shy;", "\u00AD" },
	{ "sigma;", "\u03C3" },
	{ "sigmaf;", "\u03C2" },
	{ "sim;", "\u223C" },
	{ "spades;", "\u2660" },
	{ "sub;", "\u2282" },
	{ "sube;", "\u2286" },
	{ "sum;", "\u2211" },
	{ "sup1;", "\u00B9" },
	{ "sup2;", "\u00B2" },
	{ "sup3;", "\u00B3" },
	{ "sup;", "\u2283" },
	{ "supe;", "\u2287" },
	{ "szlig;", "\u00DF" },
	{ "tau;", "\u03C4" },
	{ "there4;", "\u2234" },
	{ "theta;", "\u03B8" },
	{ "thetasym;", "\u03D1" },
	{ "thinsp;", "\u2009" },
	{ "thorn;", "\u00FE" },
	{ "tilde;", "\u02DC" },
	{ "times;", "\u00D7" },
	{ "trade;", "\u2122" },
	{ "uArr;", "\u21D1" },
	{ "uacute;", "\u00FA" },
	{ "uarr;", "\u2191" },
	{ "ucirc;", "\u00FB" },
	{ "ugrave;", "\u00F9" },
	{ "uml;", "\u00A8" },
	{ "upsih;", "\u03D2" },
	{ "upsilon;", "\u03C5" },
	{ "uuml;", "\u00FC" },
	{ "weierp;", "\u2118" },
	{ "xi;", "\u03BE" },
	{ "yacute;", "\u00FD" },
	{ "yen;", "\u00A5" },
	{ "yuml;", "\u00FF" },
	{ "zeta;", "\u03B6" },
	{ "zwj;", "\u200D" },
	{ "zwnj;", "\u200C" }
};

static int cmp(const void *key, const void *value)
{
	return strncmp((const char *)key, *(const char *const *)value,
		strlen(*(const char *const *)value));
}

static const char *get_named_entity(const char *name)
{
	const char *const *entity = (const char *const *)bsearch(name,
		NAMED_ENTITIES, sizeof NAMED_ENTITIES / sizeof *NAMED_ENTITIES,
		sizeof *NAMED_ENTITIES, cmp);

	return entity ? entity[1] : NULL;
}

static size_t putc_utf8(unsigned long cp, char *buffer)
{
	unsigned char *bytes = (unsigned char *)buffer;

	if(cp <= 0x007Ful)
	{
		bytes[0] = (unsigned char)cp;
		return 1;
	}

	if(cp <= 0x07FFul)
	{
		bytes[1] = (unsigned char)((2 << 6) | (cp & 0x3F));
		bytes[0] = (unsigned char)((6 << 5) | (cp >> 6));
		return 2;
	}

	if(cp <= 0xFFFFul)
	{
		bytes[2] = (unsigned char)(( 2 << 6) | ( cp       & 0x3F));
		bytes[1] = (unsigned char)(( 2 << 6) | ((cp >> 6) & 0x3F));
		bytes[0] = (unsigned char)((14 << 4) |  (cp >> 12));
		return 3;
	}

	if(cp <= 0x10FFFFul)
	{
		bytes[3] = (unsigned char)(( 2 << 6) | ( cp        & 0x3F));
		bytes[2] = (unsigned char)(( 2 << 6) | ((cp >>  6) & 0x3F));
		bytes[1] = (unsigned char)(( 2 << 6) | ((cp >> 12) & 0x3F));
		bytes[0] = (unsigned char)((30 << 3) |  (cp >> 18));
		return 4;
	}

	return 0;
}

static bool parse_entity(
	const char *current, char **to, const char **from)
{
	const char *end = strchr(current, ';');
	if(!end) return 0;

	if(current[1] == '#')
	{
		char *tail = NULL;
		int errno_save = errno;
		bool hex = current[2] == 'x' || current[2] == 'X';

		errno = 0;
		unsigned long cp = strtoul(
			current + (hex ? 3 : 2), &tail, hex ? 16 : 10);

		bool fail = errno || tail != end || cp > UNICODE_MAX;
		errno = errno_save;
		if(fail) return 0;

		*to += putc_utf8(cp, *to);
		*from = end + 1;

		return 1;
	}
	else
	{
		const char *entity = get_named_entity(&current[1]);
		if(!entity) return 0;

		size_t len = strlen(entity);
		memcpy(*to, entity, len);

		*to += len;
		*from = end + 1;

		return 1;
	}
}

size_t decode_html_entities_utf8(char *dest, const char *src)
{
	if(!src) src = dest;

	char *to = dest;
	const char *from = src;

	for(const char *current; (current = strchr(from, '&'));)
	{
		memmove(to, from, (size_t)(current - from));
		to += current - from;

		if(parse_entity(current, &to, &from))
			continue;

		from = current;
		*to++ = *from++;
	}

	size_t remaining = strlen(from);

	memmove(to, from, remaining);
	to += remaining;
	*to = 0;

	return (size_t)(to - dest);
}
