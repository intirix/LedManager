#ifndef __LEDMANAGER_H__
#define __LEDMANAGER_H__

#include <stdint.h>
#include "LedController.h"
#include "Program.h"

/**
 * Manager for making LED lighting displays
 *
 * EEPROM - LedManager requires 16 bytes of EEPROM
 */
class LedManager
{
  public:
    ~LedManager();
  
  
    /**
     * Set the offset inside of the EEPROM to store the LED configuration.  This allows
     * the library to be used with other code that uses the EEPROM
     * Parameters:
     *  offset - offset in the EEPROM to start storing configuration
     */
    void setEepromOffset( uint16_t offset );
    
    /**
     * Call from the Arduino setup() function
     */
    void init();
    
    /**
     * Call from the Arduino loop() function
     */
    void loop();
    
  private:
    uint16_t eepromOffset = 0;
    bool initialized = false;
    
    char *cmd;

    uint16_t numLeds;
    uint8_t ledSkip;
    uint8_t ledChip;
    uint8_t lastProgramIndex;
    
    LedController *controller = new LedController();

    /**
     * The current program that is executing
     */    
    Program *currentProgram = 0L;
    
    /**
     * Dump the configuration to the Serial for debugging
     */
    void dumpConfig();
    
    /**
     * Read EEPROM data
     */
    void readEeprom();
    
    /**
     * Display the generic help info
     */
    void displayHelp();
    
    /**
     * Handler for the help command
     */
    void handleHelp( const char *helpcmd );

    /**
     * Handler for the setNumLeds command
     */
    void handleNumLeds( const char *str );

    /**
     * Handler for the setLedSkip command
     */
    void handleLedSkip( const char *str );

    /**
     * Handler for the setLedType command
     */
    void handleLedType( const char *str );
    
    /**
     * Handler for the startProgram command
     */
    void handleStartProgram( const char *str );
    
    /**
     * Initialized the LED controller
     */
    void initLEDs();
    
    /**
     * Turn off all LEDs
     */
    void clearLEDs();

};





#endif
