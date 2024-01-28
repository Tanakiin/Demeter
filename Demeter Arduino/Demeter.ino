#include "DHT.h"
#include <Arduino.h>
#include <U8x8lib.h>
#include "Wire.h"


long duration;
int distance;
const int trigPin = 5;
const int echoPin = 6;
const int standardLEDPin = 3;
const int buzzPin = 2;
const int buttonPin = 4; 

int buttonState = 0; 

#define DHTTYPE DHT20   // DHT 20
DHT dht(6, DHTTYPE);
#if defined(ARDUINO_ARCH_AVR)
    #define debug  Serial
 
#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
    #define debug  SerialUSB
#else
    #define debug  Serial
#endif
 


int menustate = 0;

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
 
void setup(void) {
    pinMode(buttonPin, INPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    debug.begin(115200);
    delay(1000);
    Wire.begin();
    delay(1000);
    pinMode(standardLEDPin, OUTPUT);
    pinMode(buzzPin, OUTPUT);
 
    dht.begin();
    delay(1000);
    u8x8.begin();
    delay(1000);
    u8x8.setPowerSave(0);  
    u8x8.setFlipMode(1);
    u8x8.drawString(0, 0, "Setup Complete");
    delay(1000);
    u8x8.clearDisplay();
    
}

#define SCROLL_DELAY 250 // Adjust the scrolling speed

// void scrollText(const char* text, int line) {
  
//     int len = strlen(text);
//     if (len <= 16) { // If text fits the screen, no need to scroll
//         u8x8.drawString(0, line, text);
//         delay(SCROLL_DELAY * 4); // Wait for a while
//     } else {
//         for (int i = 0; i < len - 16 + 1; i++) {
//             char buffer[17]; // Create a buffer for the visible part of the text
//             strncpy(buffer, text + i, 16);
//             buffer[16] = '\0'; // Null-terminate the string
//             u8x8.drawString(0, line, buffer);
//             delay(SCROLL_DELAY);
//         }
//         delay(SCROLL_DELAY * 4); // Wait at the end of the scroll
//     }
// }
 
void loop(void) {
  
  float temp, humi;
  temp = dht.readTemperature();
  humi = dht.readHumidity();
  int light = analogRead(A0);

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 33);

  

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH && menustate == 0)
  {
    u8x8.clearDisplay();
    menustate = 1;
  }
  else if (buttonState == HIGH && menustate == 1)
  {
    u8x8.clearDisplay();
    menustate = 0;
  }

  if (menustate == 0)
  {
    if (temp >= 27 && humi <= 50) {
        u8x8.drawString(0, 0, "It's way too");
        u8x8.drawString(0, 1, "hot for me!");
    } 
    else if (temp >= 27 && humi >= 50) {
        u8x8.drawString(0, 0, "Hot & humid,");
        u8x8.drawString(0, 1, "move me!");
    } 
    else if (temp < 27 && humi > 50) {
        u8x8.drawString(0, 0, "Too humid!");
        u8x8.drawString(0, 1, "");
    } 
    else {
        u8x8.drawString(0, 0, "All good!");
        u8x8.drawString(0, 1, "");
    }
    if ( light < 9)
    {
      u8x8.drawString(0, 3, "Its too dark!");
      u8x8.drawString(0, 4, "I need light!");
    }
    else if (light >= 9)
    {
      u8x8.drawString(0, 3, "");
      u8x8.drawString(0, 4, "");
    }
  }
  else
  {
    
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  

  if (distance < 10) {
        digitalWrite(standardLEDPin, LOW); // LED on
        digitalWrite(buzzPin, HIGH); // Buzzer on
    } else {
        digitalWrite(standardLEDPin, HIGH); // LED off
        digitalWrite(buzzPin, LOW); // Buzzer off
    }
    

    char tempStr[10]; 
    char humiStr[10];
    char distStr[10];
    char lightStr[10];

    dtostrf(temp, 6, 2, tempStr);
    dtostrf(humi, 6, 2, humiStr);
    dtostrf(distance, 6, 2, distStr);
    dtostrf(light, 6, 2, lightStr);

    char tempDisplay[20];
    char humiDisplay[20];
    char distDisplay[20];
    char lightDisplay[20];

    sprintf(tempDisplay, "Temp:%s C", tempStr);
    sprintf(humiDisplay, "Humid:%s%", humiStr);
    sprintf(distDisplay, "Dist:%s%", distStr);
    sprintf(lightDisplay, "LightLvl:%s", lightStr);

    u8x8.drawString(0, 0, tempDisplay);
    u8x8.drawString(0, 1, humiDisplay);
    u8x8.drawString(0, 2, distDisplay);
    u8x8.drawString(0, 3, lightDisplay);
  }
    
    u8x8.refreshDisplay(); 
    debug.print("Temperature: ");
    debug.print(temp);
    debug.print(" C, ");
    debug.print("Humidity: ");
    debug.print(humi);
    debug.print("%, ");
    
    
    Serial.print("Light Level: ");
    Serial.print(light);// get a detailed calculating expression for UV index in schematic files.
    Serial.print("\n");

    delay(20); // Adjust delay as needed
}

