#include <stdarg.h>

#include "s21_string.h"

extern double pow(double x, double y);
extern double round(double x);
extern double modf(double value, double *iptr);

#define FL struct flags fl
struct flags {
  int plus;
  int minus;
  int space;
};

//=============================================================================

void read_flag(const char **c, struct flags *fl) {
  while (*++(*c) && (**c == '+' || **c == '-' || **c == ' '))
    (**c == '+')   ? fl->plus = 1
    : (**c == '-') ? fl->minus = 1
    : (**c == ' ') ? fl->space = 1
                   : 0;
  (*c)--;
}

int sup_findDigit(char c) { return (c >= '0' && c <= '9'); }
int sup_convertDigit(const char **c, int i) {
  return ((int)*--(*c) - '0') * pow(10, i);
}
int read_width(const char **c) {
  int cnt_digits = 0;
  int width = cnt_digits;

  while (sup_findDigit(*++(*c))) cnt_digits++;
  if (cnt_digits) FST_FOR(cnt_digits) width += sup_convertDigit(c, i);
  *c += cnt_digits;
  (*c)--;

  return width;
}

int read_point(const char **c) {
  return (*++(*c) == '.') ? read_width(c) : ((*c)--, -1);
}

char read_length(const char **c) {
  return (*++(*c) == 'h' || *(*c) == 'l') ? **c : ((*c)--, 'n');
}

//----------------------------------------------------------

void cast_int(long int *d, char length) {
  if (length == 'h')
    *d = (short int)*d;
  else if (length != 'l')
    *d = (int)*d;
}

void sup_reverseString(char *str, int length) {
  int start = 0;
  int end = length - 1;
  char temp;

  while (start < end)
    temp = str[start], str[start++] = str[end], str[end--] = temp;
}

void cst_itoa(long int num, char *str, FL, int point, char int_dec,
              char format) {
  int indxStr = 0;
  int isNeg = indxStr;

  num < 0 ? num = -num, isNeg++ : isNeg;
  if (num == 0 && (int_dec == 'd' || point == -1)) str[indxStr++] = '0';
  while (num) str[indxStr++] = (num % 10) + '0', num /= 10;
  while (int_dec == 'i' && indxStr < point) str[indxStr++] = '0';
  if (format != 'u') {
    if (isNeg)
      str[indxStr++] = '-';
    else if (fl.plus == 1)
      str[indxStr++] = '+';
    else if (fl.space == 1)
      str[indxStr++] = ' ';
  }
  sup_reverseString(str, indxStr);
  str[indxStr] = '\0';
}

//----------------------------------------------------------

double sup_roundn(double num, int n) {
  double scale = pow(10, n);
  return round(num * scale) / scale;
}

void cst_ftoa(double num, char *str, FL, int point, char format) {
  if (point == -1) point = 6;

  num = sup_roundn(num, point);
  double decPart = 0;
  double pointPart = modf(num, &decPart);
  long int int_decPart = decPart;

  cst_itoa(int_decPart, str, fl, point, 'd', format);
  if (point) {
    int length = s21_strlen(str);
    int lastDigit;
    str[length++] = '.';

    FST_FOR(point) pointPart *= 10;
    long int int_pointPart = (long int)round(pointPart);
    int numDec[point];
    FST_FOR(point) numDec[i] = int_pointPart % 10, int_pointPart /= 10;
    FST_FOR(point)
    lastDigit = point - 1 - i, str[length++] = numDec[lastDigit] + '0';
    str[length] = '\0';
  }
}

void cst_atoa(char *str1, char *str2, int point) {
  int length = s21_strlen(str1);
  int end = (point != -1 && point < length) ? point : length;

  s21_strncpy(str2, str1, end);
  str2[end] = '\0';
}

//----------------------------------------------------------

void out_strFormat(char format, va_list *factor, FL, int point, char length,
                   char *formatStr) {
  long int d;
  double f;
  int c;
  char *s;

  if (format == 'd' || format == 'u')
    d = va_arg(*factor, long), cast_int(&d, length),
    cst_itoa(d, formatStr, fl, point, 'i', format);
  else if (format == 'f')
    f = va_arg(*factor, double), cst_ftoa(f, formatStr, fl, point, format);
  else if (format == 'c')
    c = va_arg(*factor, int), formatStr[0] = c, formatStr[1] = '\0';
  else if (format == 's')
    s = va_arg(*factor, char *), cst_atoa(s, formatStr, point);
  else if (format == '%')
    formatStr[0] = '%', formatStr[1] = '\0';
}

void out_strWidth(char **str, int width, FL, char *formatStr) {
  int len_formatStr = s21_strlen(formatStr);
  int len_space = width - len_formatStr;

  if (width > len_formatStr) {
    char arrSpace[100] = "";
    s21_memset(arrSpace, ' ', len_space);
    if (fl.minus == 1)
      s21_strncpy(*str, formatStr, len_formatStr), *str += len_formatStr,
          s21_strncpy(*str, arrSpace, len_space), *str += len_space;
    else
      s21_strncpy(*str, arrSpace, len_space), *str += len_space,
          s21_strncpy(*str, formatStr, len_formatStr), *str += len_formatStr;
  } else {
    s21_strncpy(*str, formatStr, len_formatStr);
    *str += len_formatStr;
  }
}

//----------------------------------------------------------

int s21_sprintf(char *str, const char *format, ...) {
  char *start = str;  //указатель на начало строки
  va_list factor;
  va_start(factor, format);

  for (const char *c = format; *c; c++) {
    if (*c != '%') {
      *str++ = *c;
      continue;
    }
    FL = {0, 0, 0};

    read_flag(&c, &fl);
    int width = read_width(&c);
    int point = read_point(&c);
    char length = read_length(&c);

    char fmt = *++c;
    char formatStr[100] = "";
    if (fmt == 'd' || fmt == 'u' || fmt == 'f' || fmt == 's' || fmt == 'c' ||
        fmt == '%')
      out_strFormat(fmt, &factor, fl, point, length, formatStr),
          out_strWidth(&str, width, fl, formatStr);
  }
  *str = '\0';
  va_end(factor);
  return s21_strlen(start);
}
//=============================================================================
