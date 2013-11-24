#include <Arduino.h>

extern HardwareSerial Serial;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
	delay(1000);
	Serial.print("Uptime: ");
	Serial.print(millis());
	Serial.println(" miliseconds.");
	digitalWrite(13, HIGH);
	delay(1000);
	digitalWrite(13, LOW);
}