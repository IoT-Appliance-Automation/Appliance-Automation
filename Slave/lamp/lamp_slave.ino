#include <ESP8266WiFi.h>
#define WLAN_SSID       "Bloodraven"
#define WLAN_PASS       "bloodraven"


const int lamp_pin = LED_BUILTIN;

WiFiServer server(80);

void setup() {
   Serial.begin(57600);
  WiFi.begin("Bloodraven", "bloodraven");
  pinMode(lamp_pin, OUTPUT);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

WiFiClient client;
void loop() 
{
    if (!client.connected()) {
        // try to connect to a new client
        client = server.available();
    } else {
        // read data from the connected client
        if (client.available() > 0) {
            int ret = client.read();
            Serial.print("Received ");
            Serial.println(ret);
            if(ret==0){
              digitalWrite(lamp_pin, HIGH);
           
            }
            else if(ret==1){
              digitalWrite(lamp_pin,LOW);
            }
            
        }
    }
}
