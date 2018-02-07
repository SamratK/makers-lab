#include <ESP8266WiFi.h> 

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(D4, INPUT_PULLUP);
	Serial.begin(115200);
}

void loop()
{
	int pushValue = digitalRead(D4);
	Serial.print("Button value : ");
	Serial.println(pushValue);

	if(!pushValue) {
		while(!digitalRead(D4)) {
			Serial.println("Button pressed ...");
			delay(10);
		}
		Serial.println("Button released ...");
	}

	delay(20);
}