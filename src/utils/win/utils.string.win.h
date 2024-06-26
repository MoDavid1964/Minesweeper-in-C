/**
 * @ Author: MMMM
 * @ Create Time: 2024-02-05 11:18:06
 * @ Modified time: 2024-03-30 15:12:22
 * @ Description:
 * 
 * A utility library for implementing some string related functionality
 *    on Windows.
 * 
 * The funny thing is I kinda only need this for printing strings, because
 *    for some damn reason puts() is more reliable on Windows and printf() is
 *    more reliable on Linux (at least based from my tests).
 */

#ifndef UTILS_STRING_WIN_
#define UTILS_STRING_WIN_

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/**
 * Prints a string.
 * 
 * @param   { char * }  string  The string we want to print.
*/
void String_print(char *string) {
  puts(string);
}

/**
 * Converts a string to upper case.
 * THIS MUTATES THE ORIGINAL STRING.
 * 
 * @param   { char * }  string  The string we want to convert.
 * @return  { char * }          The uppercased string.
*/
char *String_toUpper(char *string) {
  // Originally just this, but i dont trust it
  // return strupr(string);
  char *origString = string;

  while(*string) {
    *string = toupper(*string);
    string++;
  }

  return origString;
}

#endif