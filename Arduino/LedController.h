#ifndef __LEDCONTROLLER_H__
#define __LEDCONTROLLER_H__

#include <stdint.h>

/**
 * Interface for a controller
 */
class LedController
{
  public:
    virtual ~LedController(){}
    virtual void setPixel( uint16_t index, uint8_t r, uint8_t g, uint8_t b ) {}
    virtual void updateLength( uint16_t len ) {}
    virtual uint16_t getLength() { return 0; }
    virtual void show() {}
    
    static uint8_t getIndexFromName( const char *chipset );
    static uint8_t getNumberOfSupportedControllers();
    static const char *getNameFromIndex( uint8_t index );
    static LedController *create( const char *chipset, uint16_t numLeds );
};

#endif
