/****************************************************************************
  Titel  :   Netzteil Library für das Projekt Netzteil
  Autor:    Marcel Kühl <Marcel.Kuehl@web.de>  http://github.com/KMarcel
  Datei:     Netzteil.cpp
  Software:  Arduino IDE

*****************************************************************************/
#include <Netzteil.h> // Einbinden des Netzgerätes

/************************************************************************************************************************/
/** Keyboard Variablen **/

const byte ZEILEN = 4;        //Wie viele Zeilen das Keypad hat
const byte SPALTEN = 3;       //Wie viele Spalten das Keypad hat
char Keyarray[ZEILEN][SPALTEN] = {      //Erstellung einer Keymatrix map die anzeigt, wo welcher Buchstabe ist
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte Zeilenpins[ZEILEN] = {9, 8, 7, 6}; //Die Pins der Key Zeilen
byte Spaltenpins[SPALTEN] = {5, 4, 3}; //Die Pins der Key Spalten

/************************************************************************************************************************/
// Objecte Erzeugen

Keypad Keyboard( makeKeymap(Keyarray), Zeilenpins, Spaltenpins, ZEILEN, SPALTEN); // Keypad objekt Erzeugen

LiquidCrystal_I2C LC_Display(0x27, 16, 2);  // LC_Display Object Erzeugen

/************************************************************************************************************************/
// Netzteil Initialisieren
void Netzteil::begin()
{
  LC_Display.begin();   // LC_Display Initialisieren
}

/************************************************************************************************************************/
//Spannung umwandeln für den Digitalwiderstand
int Netzteil::ResistorTranslate(double Value, int Max)
{
  int Text = ((int)Value) * 100;	//Den Übergebenen Parameter Value in einen Int verwandeln und die nachkommastellen mit mal 100 in Ganzzahlen verwandeln
  Text = map(Text, 0, Max, 0, 255); //mit dem Befehl map eine neue Größenortnung vergeben
  return Text;
}

/************************************************************************************************************************/
//Neue Spannung einstellen
double Netzteil::setNewVoltage(double Value)
{
  /********************************************************************/
  LC_Display.clear();
  LC_Display.setCursor(0, 0);
  LC_Display.print("Spannung: 00,00V");
  LC_Display.setCursor(0, 1);
  LC_Display.print("Strom: 0,00A");
  /********************************************************************/
  int E1_1 = 0, E1_2 = 0, E1_3 = 0;
  double Ausgabe;
  /********************************************************************/
  E1_1 = Position(10, MaxVoltage, E1_1, 0, 10);
  LC_Display.setCursor(10, 0);
  LC_Display.print(E1_1);
  E1_2 = Position(1, MaxVoltage, E1_2, 0, 11);
  LC_Display.setCursor(11, 0);
  LC_Display.print(E1_2);
  E1_3 = Position(0.1, MaxVoltage, E1_3, 0, 13);
  LC_Display.setCursor(13, 0);
  LC_Display.print(E1_3);
  /********************************************************************/
  Ausgabe = (((double)E1_1) * 10) + ((double)E1_2) + (((double)E1_3) / 10); //Ein Eingestellten Zahlen in eine Double Variable umwandeln und speichern
  /********************************************************************/
  if (Ausgabe > 25.00)	// Wenn Ausgabe größer ist als die maximal Spannung dann soll diese auf die Maxmal größe beschrenkt werden
  {
    Ausgabe = 25.00;
  }
  /********************************************************************/
  return Ausgabe; //Gibt die eingestelle Spannung zurück
}

/************************************************************************************************************************/
//Neue Strombegrenzu einstellen
double Netzteil::setNewCurrent(double Value)
{
  /********************************************************************/
  int E1_1 = 0, E1_2 = 0, E1_3 = 0;
  double Ausgabe;
  /********************************************************************/
  E1_1 = Position(10, MaxVoltage, E1_1, 1, 7); //funktion Position wir aufgerufen
  LC_Display.setCursor(7, 1);
  LC_Display.print(E1_1);
  E1_2 = Position(1, MaxVoltage, E1_2, 1, 9);
  LC_Display.setCursor(9, 1);
  LC_Display.print(E1_2);
  E1_3 = Position(0.1, MaxVoltage, E1_3, 1, 10);
  LC_Display.setCursor(10, 1);
  LC_Display.print(E1_3);
  /********************************************************************/
  Ausgabe = ((double)E1_1) + (((double)E1_2) / 10) + (((double)E1_3) / 100);//Ein Eingestellten Zahlen in eine Double Variable umwandeln und speichern
  /********************************************************************/
  if (Ausgabe > 1.00) // Wenn Ausgabe größer ist als die maximal Strombegrenzung dann soll diese auf die Maxmal größe beschrenkt werden
  {
    Ausgabe = 1.00;
  }
  /********************************************************************/
  return Ausgabe;
}

/************************************************************************************************************************/

double Netzteil::Position(double Skall, int Max, int Value, bool Zeile, int Spalte)
{
  /********************************************************************/
  
  char Eingabe; //Tastenfeld Eingabe Variable
  long dev = millis(); //Aktuelle Zeit wird hier gespeichert
  int Interval = 500; //Interval des LC-Display Blinkens auf 500ms setzten
  int temp = Value; //Aktueller Wert wird in temp geladen 
  bool ready = 0; //Auswahl der Aktuellen Ausgabewert
  /********************************************************************/
  //Endlosschleife
  while (true)
  {
    Eingabe = Keyboard.getKey(); //Erneutes Einlesen der gedrückten Tasten auf dem Keypad
    /********************************************************************/
	// Bei Tastendruck wird einer der cases ausgeführt und der wert in die Variable temp gespeichert
    switch (Eingabe)
    {
      case '1':
        temp = 1;
        break;
      case '2':
        temp = 2;
        break;
      case '3':
        temp = 3;
        break;
      case '4':
        temp = 4;
        break;
      case '5':
        temp = 5;
        break;
      case '6':
        temp = 6;
        break;
      case '7':
        temp = 7;
        break;
      case '8':
        temp = 8;
        break;
      case '9':
        temp = 9;
        break;
      case '0':
        temp = 0;
        break;
    }
	/********************************************************************/
	// In die nächste Einstellung gebracht (Navigation oder auch Select button)
    if (Eingabe == '#')
    {
      break;
    }
	/********************************************************************/
    if (millis() - dev > Interval && ready == 1)  //Wenn eine Ausgabe erfolgen soll ist ready auf 1 und es ist eine zeit von 500ms vergangen
    {
      dev = millis();	//Aktuelle Zeit wird in dev gespeichert
	  //lCD Ausgabe
      LC_Display.setCursor(Spalte, Zeile);
      LC_Display.print(temp);
	  //Ready wird auf 0 gesetzt
      ready = 0;
    }
    else if (millis() - dev > Interval && ready == 0) //Wenn eine Ausgabe erfolgen soll ist ready auf 0 und es ist eine zeit von 500ms vergangen
    {
      dev = millis(); //Aktuelle Zeit wird in dev gespeichert
	  //LCD stelle wird gelöscht
      LC_Display.setCursor(Spalte, Zeile);
      LC_Display.print(" ");
	  //Ready wird auf 1 gesetzt
      ready = 1;
    }
	/********************************************************************/
  }
  //Endlosschleife END
  return temp; //Rückgabe der Einzelen Stelle
}

/************************************************************************************************************************/
