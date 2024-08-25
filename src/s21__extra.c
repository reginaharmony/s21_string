#include <stdlib.h>

#include "s21_string.h"

#define TO_WHAT(start, end, sign)                                             \
  if (!str) return NULL;                                                      \
  s21_size_t length = s21_strlen(str);                                        \
  char *result = (char *)calloc(length + 1, sizeof(char));                    \
  FST_FORS(length)                                                            \
  result[i] = (str[i] >= start && str[i] <= end) ? (str[i] sign 32) : str[i]; \
  return result;

void *s21_to_lower(const char *str) { TO_WHAT('A', 'Z', +) }
void *s21_to_upper(const char *str) { TO_WHAT('a', 'z', -) }

void *s21_trim(const char *src, const char *trim_chars) {
  char *new_str = 0;

  if (trim_chars == s21_NULL) {
    s21_size_t lenSrc = s21_strlen(src) + 1;
    new_str = (char *)calloc(lenSrc, sizeof(char));

    FST_FORS(lenSrc) new_str[i] = src[i];
  } else if (src) {
    int start = 0;
    int end = s21_strlen(src) - 1;

    while (src[start] && s21_strchr(trim_chars, src[start])) start++;
    while (end >= start && s21_strchr(trim_chars, src[end])) end--;
    int length = end - start + 1;
    new_str = (char *)calloc((length + 1), sizeof(char));
    int j = 0;
    for (; start <= end; j++) new_str[j] = src[start], start++;
    new_str[j] = '\0';
  }
  return new_str;
}

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *buff = s21_NULL;
  int src_len = s21_strlen(src);
  int str_len = s21_strlen(str);

  if (src && str && start_index <= s21_strlen(src))
    buff = (char *)calloc((src_len + str_len + 1), sizeof(char)),
    s21_strncpy(buff, src, start_index),
    s21_strncpy(buff + start_index, str, str_len),
    s21_strncpy(buff + start_index + str_len, src + start_index,
                src_len - start_index),
    buff[src_len + str_len] = '\0';
  return buff;
}