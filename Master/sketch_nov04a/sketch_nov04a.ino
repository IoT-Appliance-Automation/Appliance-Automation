/***************************************************
  Adafruit ESP8266 Lamp Controller Module

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino
  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Tony DiCola for Adafruit Industries.
  Adafruit IO example additions by Todd Treece.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

// Libraries
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Lamp pin
const int lamp_pin = LED_BUILTIN;

// WiFi parameters
#define WLAN_SSID       "Bloodraven"
#define WLAN_PASS       "bloodraven"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "Cham3l30n"
#define AIO_KEY         "24f139eecdbb448fafa8f6cd49551a53"

// Functions
void connect();

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
//
//// Store the MQTT server, client ID, username, and password in flash memory.
//// This is required for using the Adafruit MQTT library.
//const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
//// Set a unique MQTT client ID using the AIO key + the date and time the sketch
//// was compiled (so this should be unique across multiple devices for a user,
//// alternatively you can manually set this to a GUID or other random value).
//const char MQTT_CLIENTID[] PROGMEM  = AIO_KEY __DATE__ __TIME__;
//const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
//const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
//Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


/****************************** Feeds ***************************************/

// Setup a feed called 'lamp' for subscribing to changes.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
//const char LAMP_FEED[] PROGMEM = AIO_USERNAME "/feeds/data";
Adafruit_MQTT_Subscribe lamp = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/data");

/*************************** Sketch Code ************************************/

void setup() {

  Serial.println("Hatana 0");
  // Set lamp pin to output
  pinMode(lamp_pin, OUTPUT);

  Serial.begin(57600);

  Serial.println("Hatana 1");

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // listen for events on the lamp feed
  mqtt.subscribe(&lamp);

  // connect to adafruit io
  connect();

}

void loop() {

  Adafruit_MQTT_Subscribe *subscription;

  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }

  // this is our 'wait for incoming subscription packets' busy subloop
  while (subscription = mqtt.readSubscription(100)) {

    // we only care about the lamp events
    if (subscription == &lamp) {

      // convert mqtt ascii payload to int
      char *value = (char *)lamp.lastread;
      Serial.print("Received: ");
      Serial.println(value);
      int val = atoi(value);
      if (val == 1)
        digitalWrite(lamp_pin, LOW);
      else if (val == 0)
        digitalWrite(lamp_pin, HIGH);
      
//      if (value[0] == 'O'){
//        if (value[1] == 'N'){
//          digitalWrite(lamp_pin, LOW);
//        }
//      }
//      else if (value[0] == '0'){
//          digitalWrite(lamp_pin, HIGH);
//      }
//      
      // Apply message to lamp
//      String message = String(value);
//      message.trim();
//      if (message.charAt(0) == 'O'){
//        if (message.charAt(1) == 'N'){
//          digitalWrite(lamp_pin, LOW);
//        }
//        else if (message.charAt(1) == 'F'){
//          digitalWrite(lamp_pin, LOW);
//        }
//      }
//      if (message.startsWith("ON")) {digitalWrite(lamp_pin, HIGH);}
//      else {digitalWrite(lamp_pin, LOW);} //(message == "OFF")

    }

  }

}

// connect to adafruit io via MQTT
//void connect() {
//
//  Serial.print("Connecting to Adafruit IO... ");
//
//  int8_t ret;
//
//  while ((ret = mqtt.connect()) != 0) {
//
////    switch (ret) {
////      case 1: Serial.println("Wrong protocol"); break;
////      case 2: Serial.println("ID rejected"); break;
////      case 3: Serial.println("Server unavail"); break;
////      case 4: Serial.println("Bad user/pass"); break;
////      case 5: Serial.println("Not authed"); break;
////      case 6: Serial.println("Failed to subscribe"); break;
////      default: Serial.println("Connection failed"); break;
////    }
////
//    if(ret >= 0)
//      mqtt.disconnect();
////
//    Serial.println("Retrying connection...");
//    delay(5000);
//
//  }

void connect()
{
     int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("");
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");

  Serial.println("Adafruit IO Connected!");

}
