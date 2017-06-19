#include "Arduino.h"

#include "Digitalpoti.h"
/************************************************************************/

DigitalPoti::DigitalPoti(int CS, int SHDN, int RS)
{
/************************************************************************/
  //Übergebene Parameter in Globale Variabelen speichern
  _CS = CS; 
  _SHDN = SHDN;
  _RS = RS;
/************************************************************************/
  // Pinmods auf Ausgang Konfigurieren
  pinMode(_CS, OUTPUT); 
  pinMode(_SHDN, OUTPUT); 
  pinMode(_RS, OUTPUT); 

  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);

/************************************************************************/
  digitalWrite(SCK, LOW); //Clock auf Low setzen
  digitalWrite(MOSI, LOW); // MOSI auf Low setzen

  digitalWrite(_CS,HIGH); //Das Ausgeschaltete Gerät (DigitalPoti) einschalten
  digitalWrite(_SHDN,HIGH); //Herrunterfahren oder Shutdown ist Low activ und muss daher auf High gestellt werden
  digitalWrite(_RS,HIGH); //Reset Pin Low Aktiv muss auf HIGH gestellt werden

  //SPCR = 01010000
  //interrupt Abschalten, SPI Anschalten, Die Daten werde voran mit dem MSB übertragen, Es ist auf Master gesetzt,
  //einfache ansteigende flanke auf dem SCK, Systen Uhr /4
  SPCR = (1<<SPE)|(1<<MSTR);   //SPI Konfiguriert
  delay(10); //10 ms warten
}

//************************************************************************
void DigitalPoti::setValue(byte value, int potentiometer)
{
  byte settings; //Variabele Setting wird erzeugt

  digitalWrite(_CS,LOW); //Chip select wird ausgeschaltet da es bei der Übertragung keine Spannung über das Digitalpotenziometer beim einstellen laufen darf

    if (potentiometer == 0) //Entscheiden ob das erste poti oder das 2 Poti ausgewählt worden ist
       {settings = B00010001;} //Konfiguration für das Poti 0 setzen
    else 
     {
           if (potentiometer == 1) 
              {settings = B00010010;}//Konfiguration für das Poti 1 setzen
           else 
              {settings = B00010011;}//Konfiguration für das Poti 0 und 1 setzen
     }
    
    spi_Uebertragung(settings);     //Schreibe Config
    spi_Uebertragung(value);        //Schreibe Value

  digitalWrite(_CS,HIGH); //DIgitalpoti Aktivieren

} 

//************************************************************************
byte DigitalPoti::spi_Uebertragung(volatile byte data)
{
   SPDR = data; // in das SPI Data Regiser um es auf dem SPI weg zu verschicken
   while (!(SPSR & (1<<SPIF))) //übertragung beginnen
   { 
   };
   return SPSR; //rückgabe ob die Übertragung erfolgreich war. Wenn ja dann 1 wenn nein das 0
}

