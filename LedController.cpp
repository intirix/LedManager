#include "LedController.h"

#include "LPD8806.h"
#include "Adafruit_WS2801.h"


#define LED_TYPE_COUNT       2
const char LED_TYPE_NAME_LIST[][20] = { "", "WS2801", "LPD8806" };






/**
 * Controller for the WS2801 LED light strip
 */
class WS2801Controller : public LedController
{
  public:
    WS2801Controller( uint16_t numLeds )
    {
      strip = new Adafruit_WS2801( numLeds );
      strip->begin();
    }
    
    ~WS2801Controller()
    {
      delete strip;
    }
    
    virtual void setPixel( uint16_t index, uint8_t r, uint8_t g, uint8_t b )
    {
      strip->setPixelColor( index, r, g, b );
    }

    virtual void updateLength( uint16_t len )
    {
      strip->updateLength( len );
    }

    virtual uint16_t getLength()
    {
      return strip->numPixels();
    }
    
    virtual void show()
    {
      strip->show();
    }
            
  private:
    Adafruit_WS2801 *strip;
};

/**
 * Controller for the LPD8806 LED strip
 */
class LPD8806Controller : public LedController
{
  public:
    LPD8806Controller( uint16_t numLeds )
    {
      strip = new LPD8806( numLeds );
      strip->begin();
    }
    
    ~LPD8806Controller()
    {
       delete strip;
    }

    virtual void setPixel( uint16_t index, uint8_t r, uint8_t g, uint8_t b )
    {
      strip->setPixelColor( index, r, g, b );
    }
    
    virtual void updateLength( uint16_t len )
    {
      strip->updateLength( len );
    }
    
    virtual uint16_t getLength()
    {
      return strip->numPixels();
    }
    
    virtual void show()
    {
      strip->show();
    }
    
  private:
    LPD8806 *strip;
};








LedController *LedController::create( const char *chipset, uint16_t numLeds )
{
  uint8_t ledChip = getIndexFromName( chipset );
  LedController *controller = new LedController();
  if ( ledChip == 1 )
  {
    controller = new WS2801Controller( numLeds );
  }
  else if ( ledChip == 2 )
  {
    controller = new LPD8806Controller( numLeds );
  }
  return controller;
}

uint8_t LedController::getNumberOfSupportedControllers()
{
  return LED_TYPE_COUNT;
}

uint8_t LedController::getIndexFromName( const char *chipset )
{
  for ( uint8_t i = 0; i <= LED_TYPE_COUNT; i++ )
  {
    if ( strcmp( chipset, LED_TYPE_NAME_LIST[ i ] ) == 0 )
    {
      return i;
    }
  }
  return 0;
}

const char *LedController::getNameFromIndex( uint8_t index )
{
  return LED_TYPE_NAME_LIST[ index ];
}

