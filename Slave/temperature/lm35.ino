#include <ESP8266WiFi.h>
#define WLAN_SSID       "Bloodraven"
#define WLAN_PASS       "bloodraven"


const int lamp_pin = LED_BUILTIN;

WiFiServer server(80);

void setup() {
  Serial.begin(57600);
  WiFi.begin("Bloodraven", "bloodraven");
  //pinMode(lamp_pin, OUTPUT);

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

//WiFiClient client;
void loop() 
{
//    if (!client.connected()) {
//        // try to connect to a new client
//        
//    } 

      WiFiClient client = server.available();
//      else {
        // read data from the connected client
        //if (client.available() > 0) {
      if (client){
        float celsius = get_temp();
        if (client.available()){  
          Serial.print(client.read());
        }  
        client.println(celsius);
        Serial.println("Sent ");
        client.stop();
      }            
        //}
    //}

}

float get_temp_dummy() //main loop
{
  return 20.0;
}

float get_temp() //main loop
{
  int analogValue = analogRead(A0);
  float millivolts = (analogValue/1024.0) * 3.3; //3300 is the voltage provided by NodeMCU
  float celsius = (millivolts-0.5)*100;
  Serial.print("in DegreeC=   ");
  Serial.println(celsius);
  return celsius;
}
