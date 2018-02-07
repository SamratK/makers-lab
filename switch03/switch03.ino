#include <ESP8266WiFi.h> 
#define MAX_CONNECTION_ATTEMPTS 30

bool isConnected = false;

const char* host = "";
const int httpsPort = 443;

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
			bool result = sendToSN("Aditya IOT Device", String(millis()));
			if(result) {
				Serial.println("Posted data successfully ... ");
			} else {
				Serial.println("Failed to post data to servicenow ... ");
			}
		}
	}

	delay(20);
}

bool connectToWiFi() {
	Serial.println();
	Serial.println();
	Serial.print("Connecting ...");

	int count = 0;

	WiFi.begin("", "");

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

bool sendToSN(String device, String message) {

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  if (!client.connect(host, httpsPort) || (WiFi.status() != WL_CONNECTED)) {
    return false;
  }

  String url = "/api/now/table/x_snc_pushbutton01_table";
  String jsonContent = "{'device':'" + device + "','message':'"+ message +"'}\r\n";

  Serial.print("Sending value to service now : ");
  Serial.println(jsonContent);
  
  client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: ESP8266\r\n" +
               "Content-Type: application/json\r\n" +
               "Accept: application/json\r\n" +
               "Authorization: \r\n" +
               "Content-Length: " + jsonContent.length() + "\r\n" +
               "Connection: close\r\n\r\n" +
               jsonContent);

  //  bypass HTTP headers
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println( "Header: " + line );
    if (line == "\r") {
      break;
    }
  }
  //  read body 
  int bodyLength = 0;
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println( "Header: " + line );
    if (line == "\r") {
      break;
    }
  }

  Serial.println("Successfully posted values to ServiceNow instance ... ");

  return true;
}