#include "Program.h"
#include <string.h>


uint16_t Program::getVirtualLength()
{
  const uint16_t realLength = controller->getLength();
  if ( realLength <= 1 )
  {
    return realLength;
  }
  return 1 + ( ( controller->getLength() - 1 ) / ( ledSkip + 1 ) );
}
    
void Program::setVirtualPixel( uint16_t index, uint8_t r, uint8_t g, uint8_t b )
{
  setRawPixel( ( ledSkip + 1 ) * index, r, g, b );
}
    
void Program::setRawPixel( uint16_t index, uint8_t r, uint8_t g, uint8_t b )
{
  controller->setPixel( index, r, g, b );
}


///////////////////////////////////////////////////////////////////////////////////
//
// Custom Programs
//
///////////////////////////////////////////////////////////////////////////////////



class BlankProgram : public Program
{
  public:
    void loop()
    {
      for ( int i = 0; i < getLength(); i++ )
      {
        setRawPixel( i, 0, 0, 0 );
      }
      controller->show();
    }
};



class BlueGreenProgram : public Program
{
  public:
    void loop()
    {
      const uint16_t len = getVirtualLength();
      for ( int i = 0; i < len; i += 2 )
      {
        setVirtualPixel( i + 0, 0x00, 0x00, 0xFF );
        setVirtualPixel( i + 1, 0x00, 0xFF, 0x00 );
      }
      controller->show();
    }
};


class RedGreenProgram : public Program
{
  public:
    void loop()
    {
      const uint16_t len = getVirtualLength();
      for ( int i = 0; i < len; i += 2 )
      {
        setVirtualPixel( i + 0, 0xFF, 0x00, 0x00 );
        setVirtualPixel( i + 1, 0x00, 0xFF, 0x00 );
      }
      controller->show();
    }
};




class MulticolorProgram : public Program
{
  public:
    void loop()
    {
      const uint16_t len = getVirtualLength();
      for ( int i = 0; i < len; i += 5 )
      {
        setVirtualPixel( i + 0, 0x00, 0x00, 0xFF ); // Blue
        setVirtualPixel( i + 1, 0xFF, 0xFF, 0x00 ); // Yellow
        setVirtualPixel( i + 2, 0x00, 0xFF, 0x00 ); // Green
        setVirtualPixel( i + 3, 0xCC, 0x00, 0xFF ); // Purple
        setVirtualPixel( i + 4, 0xFF, 0x00, 0x00 ); // Red
      }
      controller->show();
    }
};




class BlueProgram : public Program
{
  public:
    void loop()
    {
      const uint16_t len = getVirtualLength();
      for ( int i = 0; i < len; i++ )
      {
        setVirtualPixel( i, 0x00, 0x00, 0xFF );
      }
      controller->show();
    }
};



class WhiteProgram : public Program
{
  public:
    void loop()
    {
      const uint16_t len = getVirtualLength();
      for ( int i = 0; i < len; i++ )
      {
        setVirtualPixel( i, 0xFF, 0xFF, 0xFF );
      }
      controller->show();
    }
};




class RedProgram : public Program
{
  public:
    void loop()
    {
      const uint16_t len = getVirtualLength();
      for ( int i = 0; i < len; i++ )
      {
        setVirtualPixel( i, 0xFF, 0x00, 0x00 );
      }
      controller->show();
    }
};



#define  NUM_PROGRAMS 7
const char PROGRAM_NAMES[][20] = { "Blank", "BlueGreen", "RedGreen", "Multicolor", "Blue", "White", "Red", NULL };




Program *Program::createProgram( const char *name )
{
  Program *program = NULL;
  if ( strncmp( name, "BlueGreen", 20 ) == 0 )
  {
    program = new BlueGreenProgram();
  }
  else if ( strncmp( name, "RedGreen", 20 ) == 0 )
  {
    program = new RedGreenProgram();
  }
  else if ( strncmp( name, "Multicolor", 20 ) == 0 )
  {
    program = new MulticolorProgram();
  }
  else if ( strncmp( name, "Blue", 20 ) == 0 )
  {
    program = new BlueProgram();
  }
  else if ( strncmp( name, "White", 20 ) == 0 )
  {
    program = new WhiteProgram();
  }
  else if ( strncmp( name, "Red", 20 ) == 0 )
  {
    program = new RedProgram();
  }
  else
  {
    program = new BlankProgram();
  }
  
  for ( uint8_t i = 0; i < NUM_PROGRAMS; i++ )
  {
    if ( strncmp( name, PROGRAM_NAMES[ i ], 20 ) == 0 )
    {
      program->programIndex = i;
      break;
    }
  }

/*  
  uint8_t index = 0;
  do
  {
    if ( strncmp( name, PROGRAM_NAMES[ index ], 20 ) == 0 )
    {
      program->programIndex = index;
    }
    
    index++;
  }
  while ( PROGRAM_NAMES[ index ] != NULL );
  */
  
  return program;
}

const char *Program::getProgramByIndex( uint8_t index )
{
  return PROGRAM_NAMES[ index ];
}

const char *Program::getName()
{
  return PROGRAM_NAMES[ programIndex ];
}
