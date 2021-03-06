//
//    FILE: dht.h
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.09
// PURPOSE: DHT Temperature & Humidity Sensor library for Arduino
//     URL: http://arduino.cc/playground/Main/DHTLib
//
// HISTORY:
// see dht.cpp file
//

#ifndef dht_h
#define dht_h

#if ARDUINO < 100
#include <Arduino.h>
#else
#include <Arduino.h>
#endif

#define DHT_LIB_VERSION "0.1.09"

#define DHTLIB_OK				0
#define DHTLIB_ERROR_CHECKSUM	-1
#define DHTLIB_ERROR_TIMEOUT	-2
#define DHTLIB_INVALID_VALUE	-999

class dht
{
public:
    int read11(uint8_t pin);
    int read21(uint8_t pin);
    int read22(uint8_t pin);

    double humidity;
    double temperature;

private:
    uint8_t bits[5];  // buffer to receive data
    int read(uint8_t pin);
	int toWord(uint8_t value1, uint8_t value2);
};
#endif
//
// END OF FILE
//