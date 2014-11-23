
#include "LedManager.h"
#include <stdlib.h>
#include "Arduino.h"
#include "/home/jeff/Dev/arduino-1.0.6/libraries/EEPROM/EEPROM.h"


// Configuration indicies
#define EEPROM_HEADER       42
#define HEADER_ADDR          0
#define NUM_LEDS_ADDR        1
#define LED_SKIP_ADDR        3
#define LED_CHIP_TYPE_ADDR   4
#define LAST_PROGRAM_ADDR    5
#define EEPROM_LAST_INDEX   15

#define COMMAND_ARRAY_SIZE 40
#define COMMAND_MAX_SIZE COMMAND_ARRAY_SIZE - 1








LedManager::~LedManager()
{
  delete controller;
}



void LedManager::setEepromOffset( uint16_t offset )
{
  eepromOffset = offset;
}

void LedManager::readEeprom()
{
  if ( EEPROM.read( eepromOffset + HEADER_ADDR ) == EEPROM_HEADER )
  {
    initialized = true;
    numLeds = ( EEPROM.read( eepromOffset + NUM_LEDS_ADDR ) << 8 ) + EEPROM.read( eepromOffset + NUM_LEDS_ADDR + 1 );
    ledSkip = EEPROM.read( eepromOffset + LED_SKIP_ADDR );
    ledChip = EEPROM.read( eepromOffset + LED_CHIP_TYPE_ADDR );
    lastProgramIndex = EEPROM.read( eepromOffset + LAST_PROGRAM_ADDR );
  }
  else
  {
    initialized = false;
  }
}

void LedManager::dumpConfig()
{
  Serial.print( "Number of LEDs: " );
  char ibuf[12];
  Serial.println( itoa( numLeds, ibuf, 10 ) );
  
  Serial.print( "Number of LEDs to skip: " );
  Serial.println( itoa( ledSkip, ibuf, 10 ) );
  
  Serial.print( "LED chipset: " );
  if ( ledChip == 0 )
  {
    Serial.println( "none" );
  }
  else
  {
    Serial.println( LedController::getNameFromIndex( ledChip ) );
  }
  
  Serial.print( "Display Program: " );
  Serial.print( itoa( lastProgramIndex, ibuf, 10 ) );
  Serial.print( ": " );
  Serial.println( currentProgram->getName() );
}

void LedManager::init()
{
  cmd = new char[ COMMAND_ARRAY_SIZE ];
  cmd[ 0 ] = 0;
  
  if ( EEPROM.read( eepromOffset + HEADER_ADDR ) != EEPROM_HEADER )
  {
    EEPROM.write( eepromOffset + HEADER_ADDR, EEPROM_HEADER );
  
    // reset all the bytes
    for ( int i = 1; i < EEPROM_LAST_INDEX; i++ )
    {
      EEPROM.write( eepromOffset + i, 0 );
    }
  }
  
  readEeprom();
  
  // create the blank program
  currentProgram = Program::createProgram( Program::getProgramByIndex( lastProgramIndex ) );

  initLEDs();
  clearLEDs();
  
  currentProgram->setController( controller );
  currentProgram->init();
  
  Serial.begin( 9600 ); // USB is always 12 Mbit/sec

}

void LedManager::initLEDs()
{
  if ( controller != NULL )
  {
    delete controller;
    controller = NULL;
  }
  
  // verify we have something to do
  if ( ledChip > 0 && numLeds > 0 )
  {
    const char *chipset = LedController::getNameFromIndex( ledChip );
    controller = LedController::create( chipset, numLeds );
  }
  
  if ( controller == NULL )
  {
    Serial.println( "LEDs not activited" );
    controller = new LedController();
  }
  
  currentProgram->setController( controller );
}

void LedManager::clearLEDs()
{
  for ( int i = 0; i < numLeds; i++ )
  {
    controller->setPixel( i, 0, 0, 0 );
  }
  controller->show();
}

void LedManager::displayHelp()
{
  Serial.println( "LedManager commands:" );
  Serial.println( "" );
  Serial.println( "help" );
  Serial.println( "  Display this command list reference" );
  Serial.println( "" );
  Serial.println( "help <command>" );
  Serial.println( "  Display usage information for the given command" );
  Serial.println( "" );
  Serial.println( "reboot" );
  Serial.println( "  Reboot into the bootloader to load a new program" );
  Serial.println( "" );
  Serial.println( "dumpConfig" );
  Serial.println( "  Print out the current configuration" );
  Serial.println( "" );
  Serial.println( "setNumLeds <num>" );
  Serial.println( "  Save the number of LEDs in your strip into the EEPROM" );
  Serial.println( "" );
  Serial.println( "setLedSkip <num>" );
  Serial.println( "  Set the number of LEDs to always leave blank between active LEDs" );
  Serial.println( "" );
  Serial.println( "setLedType <chipset>" );
  Serial.println( "  Set the type of LED strip that you are using" );
  Serial.println( "" );
  Serial.println( "startProgram <name>" );
  Serial.println( "  Start one of the pre-written programs" );
  Serial.println( "" );
  Serial.println( "" );
  Serial.println( "" );
}

void LedManager::handleHelp( const char *helpcmd )
{
  if ( strcmp( helpcmd, "setNumLeds" ) == 0 )
  {
    Serial.print( helpcmd );
    Serial.println( " <count>" );
    Serial.println( "  Set the number of LEDs in the strip" );
  }
  else if ( strcmp( helpcmd, "setLedSkip" ) == 0 )
  {
    Serial.print( helpcmd );
    Serial.println( " <count>" );
    Serial.println( "  Set the number of LEDs to always leave blank between active LEDs" );
  }
  else if ( strcmp( helpcmd, "reboot" ) == 0 )
  {
    Serial.println( helpcmd );
    Serial.println( "  Reboot into the bootloader to load a new program" );
  }
  else if ( strcmp( helpcmd, "dumpConfig" ) == 0 )
  {
    Serial.println( helpcmd );
    Serial.println( "  Print out the current configuration" );
  }
  else if ( strcmp( helpcmd, "setLedType" ) == 0 )
  {
    Serial.print( helpcmd );
    Serial.println( " <chipset>" );
    Serial.println( "  Set the type of LED strip that you are using" );
    Serial.println( "" );
    Serial.println( "  Availble types:" );
    for ( int i = 1; i <= LedController::getNumberOfSupportedControllers(); i++ )
    {
      Serial.println( LedController::getNameFromIndex( i ) );
    }
  }
  else if ( strcmp( helpcmd, "startProgram" ) == 0 )
  {
    Serial.print( helpcmd );
    Serial.println( " <name>" );
    Serial.println( "  Start one of the pre-written programs" );
  }
  else
  {
     Serial.print( "No help available for command: " );
     Serial.println( helpcmd );
  }
}


void LedManager::handleNumLeds( const char *str )
{
  clearLEDs();
  
  char ibuf[12];
  int num = atoi( str );
  EEPROM.write( eepromOffset + NUM_LEDS_ADDR, num >> 8 );
  EEPROM.write( eepromOffset + NUM_LEDS_ADDR + 1, num & 0xFF );
  
  // reread from EEPROM
  readEeprom();
  
  // update the controller
  controller->updateLength( numLeds );

  clearLEDs();
  
  Serial.print( "Saved number of LEDs: " );
  Serial.println( itoa( numLeds, ibuf, 10 ) );
}

void LedManager::handleLedSkip( const char *str )
{
  int num = atoi( str );
  EEPROM.write( eepromOffset + LED_SKIP_ADDR, num );
  readEeprom();
  Serial.print( "Saved LED Skip: " );
  char ibuf[12];
  Serial.println( itoa( ledSkip, ibuf, 10 ) );
}

void LedManager::handleLedType( const char *str )
{
  clearLEDs();
  
  int type = LedController::getIndexFromName( str );

  if ( type >= 0 )
  {
    EEPROM.write( eepromOffset + LED_CHIP_TYPE_ADDR, type );
    
    // reread from EEPROM
    readEeprom();
    
    // reinitialize the controller
    initLEDs();
    
    Serial.print( "Saved LED Type: " );
    Serial.println( str );
  }
  else
  {
    Serial.print( "Unknown LED type: " );
    Serial.println( str );
  }
}


void LedManager::handleStartProgram( const char *str )
{
  clearLEDs();
  
  Serial.print( "Stopping program " );
  Serial.print( currentProgram->getProgramIndex() );
  Serial.print( ": " );
  Serial.println( currentProgram->getName() );
  
  delete currentProgram;
  currentProgram = Program::createProgram( str );
  
  Serial.print( "Starting program " );
  Serial.print( currentProgram->getProgramIndex() );
  Serial.print( ": " );
  Serial.println( currentProgram->getName() );
  
  currentProgram->setController( controller );
  currentProgram->init();
  
  // wait for init to run before saving the program
  lastProgramIndex = currentProgram->getProgramIndex();
  EEPROM.write( eepromOffset + LAST_PROGRAM_ADDR, lastProgramIndex );

}

void LedManager::loop()
{
  while ( Serial.available() > 0 )
  {
    char c = Serial.read();
    int currentCmdLength = strlen( cmd );
    
    // we do detect the end of a command
    if ( c == '\n' || c == '\r' || currentCmdLength >= COMMAND_MAX_SIZE )
    {
      // prevent buffer overflows
      cmd[ COMMAND_MAX_SIZE ] = 0;
      Serial.print( "> " );
      Serial.println( cmd );
      
      // check for the various commands
      if ( strncmp( "reboot", cmd, COMMAND_MAX_SIZE ) == 0 )
      {
        Serial.println( "Rebooting to bootloader..." );
        delay( 1000 );  // do not print too fast!
        _reboot_Teensyduino_();
      }
      else if ( strncmp( "dumpConfig" ,cmd, COMMAND_MAX_SIZE ) == 0 )
      {
        dumpConfig();
      }
      else if ( strncmp( "help" ,cmd, COMMAND_MAX_SIZE ) == 0 )
      {
        displayHelp();
      }
      else if ( strncmp( "help ", cmd, 5 ) == 0 )
      {
        handleHelp( cmd + 5 );
      }
      else if ( strncmp( "setNumLeds ", cmd, 11 ) == 0 )
      {
        handleNumLeds( cmd + 11 );
      }
      else if ( strncmp( "setLedSkip ", cmd, 11 ) == 0 )
      {
        handleLedSkip( cmd + 11 );
      }
      else if ( strncmp( "setLedType ", cmd, 11 ) == 0 )
      {
        handleLedType( cmd + 11 );
      }
      else if ( strncmp( "startProgram ", cmd, 13 ) == 0 )
      {
        handleStartProgram( cmd + 13 );
      }
      else
      {
        Serial.print( "Unknown command: " );
        Serial.println( cmd );
      }
      // reset the command
      cmd[ 0 ] = 0;
    } else {
      // append the char to the command
      cmd[ currentCmdLength ] = c;
      cmd[ currentCmdLength + 1 ] = 0;
    }
  }
  
  currentProgram->loop();
  
  delay( 10 );
}
