/*  Smart Fan Project
 * 
 *  Gets current room temperature and calculates the fan speed to meet the desired temperature
 *  Desired temperature must be hard-coded. Also "windchill" (fan power) needs to be tested more
 *  
 *  Board used: Arduino Uno Rev 2
 *  Parts used: LCD1602, Potentiometer 10K, DHT11, 9V Battery, Fan
 *  
 *  This was done as a project for ELET 3403 (Sensor Applications)
 * 
 * Setup
 *    Wire up the board correctly (see diagram.png)
 *    Power Arduino and upload code
 */


// LCD display modules
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// DHT11 3 pin Humidity sensor module
#include <dht11.h>

#define DHT11PIN 7
dht11 DHT11;

// Set the pin number for fanPower
#define fanPower 9

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


void setup(){
  delay(1000);//Wait before accessing Sensor
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
  analogWrite(fanPower, 255);
}

// Read room temperature from DHT11 and do calculations
void loop(){
    int chk = DHT11.read(DHT11PIN);
    double roomTempF = (double)((((float)DHT11.temperature) * 9 / 5) + 32);
    double idealTempF = 69.00;
    //float tempdiff = abs(idealTempF - roomTempF);

    float windChill = (float)(pow((idealTempF - 35.74 - 0.6215*roomTempF)/(0.425*roomTempF - 35.75), 0.16));

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("temp: ");
    lcd.print(roomTempF);
    lcd.print(char(223)); 
    lcd.print("F");
    lcd.setCursor(0, 1);
    
    if(windChill < 1.11) {
      analogWrite(fanPower, 0);
      lcd.print("Fan OFF            ");
      delay(100);
    } else if(windChill < 1.2) {
      analogWrite(fanPower, 51);
      lcd.print("Fan Speed: 20%   ");
      delay(100);
    } else if(windChill < 1.3) {
      analogWrite(fanPower, 102);
      lcd.print("Fan Speed: 40%   ");
      delay(100);
    } else if(windChill < 1.4) {
      analogWrite(fanPower, 153);
      lcd.print("Fan Speed: 60%   ");
      delay(100);
    } else if(windChill < 1.5) {
      analogWrite(fanPower, 204);
      lcd.print("Fan Speed: 80%    ");
      delay(100);
    } else {
      analogWrite(windChill, 255);
      lcd.print("Fan Speed: 100%   ");
      delay(100);
    }
    
    delay(5000);//Wait 5 seconds before accessing sensor again.
}
