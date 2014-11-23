#include "LedManager.h"
#include "Arduino.h"

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


