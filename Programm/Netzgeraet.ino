#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Digitalpoti.h>
#include <Netzteil.h>

/*************************************************************************************/

// Keypad Variablen
const byte ROWS1 = 4; //4 Zeilen
const byte COLS1 = 3; //3 Spalten
char keys1[ROWS1][COLS1] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
}; // Tastenübersicht
byte rowPins1[ROWS1] = {9, 8, 7, 6}; //Pins der Zeilen
byte colPins1[COLS1] = {5, 4, 3}; //Pins der Spalten

/***********************************************************************************/

//Zeitmessung Variable
long prevMillis = 0;  //Aktuelle Zeit wird gespeichert
byte Activ = 1;       //Welche Anzeige Aktuelle angezeigt werden soll.

/***********************************************************************************/

//Gemmessene Spannug und Strom Variable
double Spannung = 0.00;
double Strom = 0.00;

/***********************************************************************************/

//Einstellungs Variablen
double Spannung_Eingestellt = 0.00; //Hier wird die Aktuelle Spannung Einstellung hinterlegt.
double Strom_Eingestellt = 0.00;    //Hier wird die Aktuelle Strom Einstellung hinterlegt.

/***********************************************************************************/

//Objekt erstellung
Keypad keypad1 = Keypad( makeKeymap(keys1), rowPins1, colPins1, ROWS1, COLS1 ); //Keypad Objekt erstellen
LiquidCrystal_I2C lcd(0x27, 16, 2); // LC-Display Objekt erstellen und Konfigurieren
DigitalPoti Poti(53, 50, 49); //Digitalpotenziometer Objekt erstellt
Netzteil Supply;  //Netzteil Objekt erstellt

/***********************************************************************************/
//Konfiguration
void setup()
{
  /******************/
  //Betriebsspannung für das LC-Display
  pinMode(22,OUTPUT);
  digitalWrite(22,HIGH);
  /******************/
  //Initialisieren der Klassen
  Supply.begin();
  lcd.begin();
  /*****************/
  //Willkommen Text auf dem LC-Display ausgeben
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("** Netzgeraet **");
  lcd.setCursor(0, 1);
  lcd.print("** Hochfahren **");
  delay(2000);
  /****************/
}
/***********************************************************************************/
// Main schleife
void loop()
{
  /********************************************************************/
  //Messung von Strom und Spannung an A0 und A1 danach umwandlung in 2500V und 100A danach die umgewandelte zahl durch 100 teile und von einen Int in eine double Zahl convertieren
  Spannung = ((double)map(analogRead(A0), 0, 775, 0, 2500)) / 100;
  Strom = ((double)map(analogRead(A1), 0, 15, 0, 100)) / 100;
  /********************************************************************/
  //Anzeige auf dem LC-Display
  if (millis() - prevMillis > 1000) //Jede Sekunde wird das LC-Display einmal verändert
  {
    prevMillis = millis(); //Aktuelle zeit wird in prevMillis übertragen
    if (Activ == 1) //Überprüfen ob diese Anzeige angezeigt werden soll
    {
      //LCD-Ausgabe
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Eingestellt");

    } else if (Activ == 2)//Überprüfen ob diese Anzeige angezeigt werden soll
    {
      //LCD-Ausgabe
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Spannung:      V");
      lcd.setCursor(10, 0);
      lcd.print(Spannung_Eingestellt);
      lcd.setCursor(0, 1);
      lcd.print("Strom:      A");
      lcd.setCursor(7, 1);
      lcd.print(Strom_Eingestellt);
      
    } else if (Activ == 3)//Überprüfen ob diese Anzeige angezeigt werden soll
    {
      //LCD-Ausgabe
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Gemessen");
    } else if (Activ == 4)//Überprüfen ob diese Anzeige angezeigt werden soll
    {
      //LCD-Ausgabe
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Spannung:      V");

      lcd.setCursor(10, 0);
      lcd.print(Spannung);
      lcd.setCursor(0, 1);
      lcd.print("Strom:     A");
      lcd.setCursor(7, 1);
      lcd.print(Strom);

      Activ = 0; //Activ wird auf Null gesetzt um die erste Anzeige wieder zu zeigen
    }
    Activ++; //Activ wir um ein erhöht
  }
  /********************************************************************/
  char key = keypad1.getKey();// Einlesen der Gedrückten Tasten auf dem Keypad

  if (key == '#') { //Überprüfen ob die Taste # Gerückt worden ist
    delay(1000);    // 1000ms Warten
    key = keypad1.getKey(); //Erneutes Einlesen der gedrückten Tasten auf dem Keypad
    Spannung_Eingestellt = Supply.setNewVoltage(Spannung); //Funktion der Spannungsseite Programmieren
    Strom_Eingestellt = Supply.setNewCurrent(Strom);
    Poti.setValue(Supply.ResistorTranslate(Spannung_Eingestellt, 2500), 1);
    Poti.setValue(Supply.ResistorTranslate(Strom_Eingestellt, 100), 0);
  }
}

