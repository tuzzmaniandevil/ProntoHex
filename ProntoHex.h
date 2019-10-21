/*
  ProntoHex by Wesley Tuzza
*/

#ifndef ProntoHex_h
#define ProntoHex_h

#include "Arduino.h"

class ProntoHex
{
  public:
    uint16_t frequency;
    uint16_t length;
    uint16_t * convertedRaw;
    void convert(String hexcode);
    String join(uint16_t *strs, uint16_t len);
};

#endif
