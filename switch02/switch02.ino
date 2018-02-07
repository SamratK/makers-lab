#include <ESP8266WiFi.h> 
#define MAX_CONNECTION_ATTEMPTS 30

bool isConnected = false;


void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(D4, INPUT_PULLUP);
	Serial.begin(115200);

	isConnected = connectToWiFi();
}

void loop()
{
	int pushValue = digitalRead(D4);

	if(!pushValue) {
		while(!digitalRead(D4)) {
			delay(10);
		}
		Serial.println("Button released ...");

		if(isConnected) {
			Serial.println("Will push row to ServiceNow here..");
		}
	}

	delay(20);
}

bool connectToWiFi() {
	Serial.println();
	Serial.println();
	Serial.print("Connecting ...");

	int count = 0;

	WiFi.begin("arriotgateway", "9885104058");

	while (WiFi.status() != WL_CONNECTED) {
		if(count++ > MAX_CONNECTION_ATTEMPTS) 
			return false; 
	
		Serial.print(".");
		delay(600);
	}

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP().toString());

	return true;
}