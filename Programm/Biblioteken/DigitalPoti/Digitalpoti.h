#ifndef DIGITALPOTI_H
#define DIGITALPOTI_H

#include "Arduino.h"

class DigitalPoti
{
  public:
    DigitalPoti(int CS, int SHDN, int RS);

    void setValue(byte value, int potentiometer); 

    void shutdown (boolean data);
    void reset ();

  private:
    int  _CS;
    int  _SHDN;
    int  _RS;
    byte spi_Uebertragung(byte data);
};

#endif  /* DIGITALPOTI_H */
