/*
  ProntoHex by Wesley Tuzza

  Based on information from
  https://github.com/probonopd/ProntoHex
*/

#include "Arduino.h"
#include "ProntoHex.h"

/**
   Convert a Pronto Hex String to raw timings, Frequency and length
*/
void ProntoHex::convert(String prontoDataStr) {
  // Define reused variables
  int i = 0;
  uint16_t j = 0;
  uint16_t hexVal;
  char hexchararray[5];

  // Make sure pronto hex is formatted correctly by removing spaces and making sure the hex is equal 2 byte groups
  prontoDataStr.replace(" ", "");
  while ((prontoDataStr.length() & 4) != 0) {
    prontoDataStr = prontoDataStr + "0";
  }

  // Convert string to char*, it's easier to deal with
  const char* prontoDataChar = prontoDataStr.c_str();

  // Calculate buffer size and init buffer
  int bufferSize = strlen(prontoDataChar) / 4;
  uint16_t buffer[bufferSize];

  // Get every 2 bytes e.g. 0000 or FFFF
  for (i = 0; i < strlen(prontoDataChar); i += 4) {
    strncpy(hexchararray, prontoDataChar + i, 4);
    hexchararray[4] = '\0'; // Null terminate

    // Convert HEX to int
    hexVal = strtol(hexchararray, NULL, 16);

    // Add hex to buffer array
    buffer[j] = hexVal;
    j++;
  }

  // Calculate Frequancy
  float carrierFrequency = 1000000 / (buffer[1] * 0.241246);

  int codeLength = buffer[2];
  if (codeLength == 0) codeLength = buffer[3]; // If Sequence #1 is missing (array[2]=0000), then Burst Sequence #2 starts at array[4]

  length = codeLength * 2;

  int index = 0;
  unsigned int convertedToMicrosec;
  convertedRaw = new uint16_t[length]; // Need to make sure convertedRaw is the same length as "length"

  for (i = 4; i < (length + 4); i++ ) {
    convertedToMicrosec = (1000000 * (buffer[i] / carrierFrequency) + 0.5);

    if (convertedToMicrosec > 20000) { // ESP8266 has a recommended limit of 20 milliseconds delay. IR Shouldn't be more than that anyway
      convertedToMicrosec = 0;
    }
    convertedRaw[index++] = convertedToMicrosec;
  }

  frequency = (uint16_t)(carrierFrequency / 1000);
}

/**
   Take an array of ints and return a string, containing + and - prefixes for displaying raw timings
*/
String ProntoHex::join(uint16_t *strs, uint16_t len) {
  String result = "";
  String prefix = "";

  for (int i = 0; i < len; i++) {
    if (i % 2 == 0) { // Check if number is even or uneven
      prefix = "+";
    } else {
      prefix = "-";
    }
    result += prefix + String(strs[i]) + " ";
  }

  return result.substring(0, result.length() - 1);
}
