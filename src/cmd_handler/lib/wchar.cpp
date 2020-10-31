#include <stdlib.h>
#include <wchar.h>

#include "wchar.hpp"

static bool is_comb(wchar_t c)
{
  if (c >= 0x300  && c <= 0x36f ) return true;
  if (c >= 0x1dc0 && c <= 0x1dff) return true;
  if (c >= 0x20d0 && c <= 0x20ff) return true;
  if (c >= 0xfe20 && c <= 0xfe2f) return true;
  return 0;
}

wchar_t* to_wchar(const char* s) {
  size_t len =
    mbstowcs(
      0,  // wide char
      s,  // const char
      0   // length
    ) + 1;
  if (not len) {
    return 0;
  }
  wchar_t* wide = (wchar_t*)malloc(sizeof (wchar_t) * len);
  mbstowcs(
    wide, // wide char
    s,    // const char
    len   // length
  );
  return wide;
}

static wchar_t* wchar_reverse(const wchar_t* wide) {
  size_t len, i, j;
  wchar_t* out;

  for (len = 0; wide[len]; len++);

  out = (wchar_t*)malloc(sizeof (wchar_t) * (len + 1));
  out[len] = 0;
  j        = 0;
  while (len) {
    for (i = len - 1; i && is_comb(wide[i]); i--);
    wcsncpy(
      out  + j,
      wide + i,
      len  - i
    );
    j += len - i;
    len = i;
  }
  return out;
}

char* reverse(const char* in) {
  size_t   len;
  char*    out;
  wchar_t* u = to_wchar(in);
  wchar_t* r = wchar_reverse(u);
  len        = wcstombs(0, r, 0) + 1;
  out        = (char*)malloc(len);
  wcstombs(out, r, len);
  free(u);
  free(r);
  return out;
}
