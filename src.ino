#include <Wire.h> 
#include <hd44780.h> 
#include <hd44780ioClass/hd44780_I2Cexp.h> // I2C LCD library 
#include <SoftwareSerial.h> 
#include <TinyGPS++.h> 
// Pins for Ultrasonic Sensor 
const int trigPin = 12; // Trigger Pin 
const int echoPin = 13; // Echo Pin 
// GPS Module 
SoftwareSerial gpsSerial(8, 9); 
TinyGPSPlus gps; 
float latitude, longitude;
// LCD Display 
hd44780_I2Cexp lcd; 
// Variables long 
duration; int 
distance; 
bool freezeLCD = false; // Flag to freeze LCD display when level is high 
void setup() { 
// Ultrasonic Sensor Setup 
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT); 
// GPS Module Setup 
gpsSerial.begin(9600); 
// LCD Setup 
lcd.begin(20, 4); // Initialize 20x4 LCD 
lcd.print("System Ready..."); 
// Serial Monitor 
Serial.begin(115200); 
delay(2000);
lcd.clear(); 
} 
void loop() { 
// Clear Trigger Pin 
digitalWrite(trigPin, LOW); 
delayMicroseconds(2); 
// Send 10-microsecond pulse 
digitalWrite(trigPin, HIGH); 
delayMicroseconds(10); 
digitalWrite(trigPin, LOW); 
// Measure Echo Pulse Duration 
duration = pulseIn(echoPin, HIGH, 30000); 
distance = duration * 0.034 / 2; 
// Print distance to Serial Monitor 
Serial.print("Distance: "); 
Serial.print(distance); Serial.println(" 
cm"); 
// Check Distance and Control LCD & GPS 
if (distance >= 0 && distance <= 4) { 
  // GPS Module Starts if 
(!freezeLCD) { 
lcd.clear(); 
lcd.print("Level: HIGH"); 
lcd.setCursor(0, 1); lcd.print("GPS 
Active..."); freezeLCD = true; // 
Freeze LCD 
} 
startGPS(); 
} else if (distance >= 5 && distance <= 12) { 
// Level Medium if 
(!freezeLCD) { 
lcd.clear(); 
lcd.print("Level: MEDIUM"); 
} 
} else if (distance >= 13 && distance <= 18) { 
// Level Low 
if (!freezeLCD) { 
lcd.clear(); 
lcd.print("Level: LOW"); 
} 
} else { 
// Out of range, clear LCD 
if (!freezeLCD) {
  lcd.clear(); 
lcd.print("No valid echo"); 
} 
} 
// Reset freezeLCD when distance goes outside high level range if 
(distance > 6 && freezeLCD) { 
freezeLCD = false; 
} 
delay(1000); // Wait for stability 
} 
// Function to Start GPS Module void 
startGPS() { 
while (gpsSerial.available()) { 
int data = gpsSerial.read(); 
if (gps.encode(data)) { latitude 
= gps.location.lat(); 
longitude = gps.location.lng(); 
if (gps.location.isValid()) { 
Serial.print("Latitude: 
"); 
Serial.println(latitude, 6);
Serial.print("Longitude: "); 
Serial.println(longitude, 6); 
// Print Google Maps link to Serial Monitor Serial.print("Google 
Maps: "); Serial.print("https://www.google.com/maps?q="); 
Serial.print(latitude, 6); 
Serial.print(","); 
Serial.println(longitude, 6); 
} else { 
Serial.println("Unable to connect to satellite."); 
} 
} 
}
}
