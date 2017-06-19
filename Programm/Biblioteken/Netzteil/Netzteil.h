#ifndef NETZTEIL_H_
#define NETZTEIL_H_
#define __Arduino_Mega
#define MaxVoltage 5
#define MaxCurrent 1
#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <Keypad.h>				//Keypad library init
#include <Wire.h>	
#include <LiquidCrystal_I2C.h>			//LCD library eingefügt
			//Leitungs library eingefügt

#define POTIOMETER_0 0				//für die Festlegung des POTENTIOMETER 0
#define POTIOMETER_1 1				//für die Festlegung des POTENTIOMETER 1
#define SCALE	5/1023				//Skalierung für die Spannungs- und Strom errechnung

#ifdef __Arduino_Mega
	#define CS 		53					//für den Chipselect pin des MCP42010
	#define SHDN	50					//für den shutdown pin des MCP42010
	#define RS		49					//für den Reset Pin des MCP42010 
#else  
#endif  




/*************************************************************************
 Title	:   Arduino Library für das Netzgerät
 Author:    Marrcel Kühl <Marcel.Kuehl@web.de>  https://github.com/KMarcel
 File:	    Netzteil.h und Netzteil.cpp
 Software:  Arduino IDE
 Hardware:  Arduino Serie 
***************************************************************************/

class Netzteil
{
public:
void begin();
int ResistorTranslate(double Value, int Max);
double setNewVoltage(double Value);
double setNewCurrent(double Value);

private:
double Position(double Skall, int Max, int Value, bool Zeile, int Spalte);
int Einstell_Spannung;				//für die digitale Einstellung der Spannung
int Einstell_Strom;				//für die digitale Einstellung des Stromes
unsigned long Timer;				//Init die Timer Variable;

};


#endif
