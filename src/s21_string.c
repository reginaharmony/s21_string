#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *str_p1 = (const unsigned char *)str1;
  const unsigned char *str_p2 = (const unsigned char *)str2;

  FST_FORS(n) if (str_p1[i] != str_p2[i]) return str_p1[i] - str_p2[i];
  return 0; 
}

void *s21_memchr(const void *str, int c, s21_size_t n) {
  char *ptr = s21_NULL;
  int len = n / sizeof(char);

  if (n != 0 && str != s21_NULL) FST_FOR(len)
  if (*((char *)str + i) == (char)c && ptr == s21_NULL) ptr = (char *)str + i;
  return ptr;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *dest_p = (unsigned char *)dest;
  unsigned char *src_p = (unsigned char *)src;

  FST_FORS(n) dest_p[i] = src_p[i];
  return dest;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *buff = (unsigned char *)str;

  FST_FORS(n) buff[i] = c;
  return str;
}

char *s21_strchr(const char *str, int c) {
  while (*str && *str != c) str++;
  return (*str == c || c == '\0') ? (char *)str : s21_NULL;
}

#define SPN(s)                                                 \
  s21_size_t i = 0;                                            \
  while (str1[i] && s21_strchr(str2, str1[i]) s s21_NULL) i++; \
  return i;

s21_size_t s21_strspn(const char *str1, const char *str2){SPN(!=)}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  SPN(==)
}

char *s21_strerror(int errnum) {
  const char *list[] = LIST_ERRORS;
  static char str[69] = {0};

  s21_sprintf(str, "Unknown error: %d", errnum);
  return (errnum <= N && errnum >= 0) ? (char *)list[errnum] : str;
}

s21_size_t s21_strlen(const char *str) {
  s21_size_t i = 0;
  if (str)
    while (str[i]) i++;
  return i;
}

char* s21_strncat(char* dest, const char* src, s21_size_t n) {
  char* end = dest + s21_strlen(dest);
  for (; *src != '\0' && n > 0; src++, end++, n--) *end = *src;
  return dest;
}

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  if (n == 0 || str1 == s21_NULL || str2 == s21_NULL) return 0;
  for (; *str1 == *str2 && n > 1; str1++, str2++, n--)
    ;
  return *str1 - *str2;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  FST_FORS(n) dest[i] = src[i];
  return dest;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  for (; *str1; str1++)
    for (s21_size_t i = 0; str2[i]; i++)
      if (*str1 == str2[i]) return (char *)str1;
  return s21_NULL;
}

char *s21_strrchr(const char *str, int c) {
  const char *start = s21_NULL;
  if (c == '\0') start = (str + s21_strlen(str));
  for (; *str; str++)
    if (*str == c) start = str;
  return (char *)start;
}


char *s21_strstr(const char *haystack, const char *needle) {
  s21_size_t len = s21_strlen(needle);

  for (s21_size_t n; *haystack; haystack++) {
    for (n = 0; n < len && *(haystack + n) == needle[n]; n++)
      ;
    if (n == len) return (char *)haystack;
  }
  return s21_NULL;
}

char* s21_strtok(char* str, const char* delim) {
  static char* ptr = s21_NULL;
  if (!str) str = ptr;
  if (!str || !(*str)) return s21_NULL;
  str += s21_strspn(str, delim);
  if (!*str) return ptr = str, s21_NULL;

  char* tok = str + s21_strcspn(str, delim);
  if (*tok) *tok++ = '\0', ptr = tok;
  return str;
}