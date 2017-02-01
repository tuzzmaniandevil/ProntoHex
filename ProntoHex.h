/*
  ProntoHex by Wesley Tuzza
*/

#ifndef ProntoHex_h
#define ProntoHex_h

#include "Arduino.h"

class ProntoHex
{
  public:
    int frequency;
    int length;
    unsigned int *convertedRaw;
    void convert(String hexcode);
    String join(unsigned int *strs, int len);
};

#endif
