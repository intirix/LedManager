#ifndef __PROGRAM_H__
#define __PROGRAM_H__

#include "LedController.h"

/**
 * Base class for a program that executes
 */
class Program
{
  public:

    virtual ~Program() {}
    
    /**
     * Set the LedController to use
     *
     * Parameters:
     *  controller - the controller to use
     */
    void setController( LedController *controller )
    {
      this->controller = controller;
    }
    
    virtual void init() {}
    
    virtual void loop() {}
    
    uint16_t getLength()
    {
      return controller->getLength();
    }
    
    /**
     * Get the length of the strip, taking into account the skipped pixels
     */
    uint16_t getVirtualLength();
    
    /**
     * Set a pixel on the LED strip, taking into account the skipped pixels
     */
    void setVirtualPixel( uint16_t index, uint8_t r, uint8_t g, uint8_t b );
    
    /**
     * Set a pixel on the LED strip
     */
    void setRawPixel( uint16_t index, uint8_t r, uint8_t g, uint8_t b );
    
    /**
     * Get the index of this program
     */
    uint8_t getProgramIndex() const
    {
      return programIndex;
    }
    
    /**
     * Get the name of the program
     */
    const char *getName();
    
    /**
     * Create a new instance of a program with the given name
     *
     * Parameters:
     *  name - name of the program
     */
    static Program *createProgram( const char *name );
    
    /**
     * Get a program name by the index
     */
    static const char *getProgramByIndex( uint8_t index );

    void incrementRuntime() {
      runtime++;
    }
    uint32_t getRuntime() {
      return runtime;
    }
  
  protected:
    /**
     * The LED controller to use
     */
    LedController *controller;
    
    /**
     * Number of leds to skip between active leds
     */
    uint8_t ledSkip = 0;
    
  private:
    uint8_t programIndex = 0;
    uint32_t runtime = 0;
};

#endif
