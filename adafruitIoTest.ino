#include <SPI.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFi101.h>

// WiFi parameters 
char ssid[] = "1312acab"; 
char pass[] = "Interiorcrocodilealligaytor"; 
int keyIndex = 27; 
int status = WL_IDLE_STATUS;

// Adafruit IO 
#define AIO_SERVER      "io.adafruit.com" 
#define AIO_SERVERPORT  1883 
#define AIO_USERNAME    "mayviolet" 
#define AIO_KEY         "aio_xwud57NtYpIbHOrN4gmpDOIAKekk" 
//#define DHTPIN 6       // what digital pin we're connected to 
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
 
//Set up the wifi client
WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

#define halt(s) { Serial.println(F( s )); while(1);  }

// Setup a feed called 'photocell' for publishing.
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");

Adafruit_MQTT_Publish doorSTatus = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/doorSTatus");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/onoff");

const int DOOR_SENSOR_PIN = 7; 
int doorState;

void setup() {
  while (!Serial);
  Serial.begin(9600);                     // initialize serial

  Serial.println(F("Adafruit MQTT demo for WINC1500"));

  // Initialise the Client
  Serial.print(F("\nInit the WiFi module..."));
  // check for the presence of the breakout
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WINC1500 not present");
    // don't continue:
    while (true);
  }
  Serial.println("ATWINC OK!");
  
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP); // set arduino pin to input pull-up mode
  mqtt.subscribe(&onoffbutton);
}
uint32_t x=0;

void loop() {
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  doorState = digitalRead(DOOR_SENSOR_PIN); // read state
  /*while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoffbutton) {
      Serial.print(F("Got: "));
      Serial.println((char *)onoffbutton.lastread);

      if (doorState == HIGH) {
        Serial.println("The door is open");
        strcmp((char *)onoffbutton.lastread, "OPEN");
      } else {
        Serial.println("The door is closed");
        strcmp((char *)onoffbutton.lastread, "COSED");
      }
    }
  }*/
  if (doorState == HIGH) {
    Serial.println("The door is open");
    doorSTatus.publish("OPEN");
  } else {
    Serial.println("The door is closed");
    doorSTatus.publish("CLOSED");
  }
  // Now we can publish stuff!
  Serial.print(F("\nSending photocell val "));
  Serial.print(x);
  Serial.print("...");
  /*if (! photocell.publish(x++)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }*/
  delay(3000);
}

void MQTT_connect() {
  int8_t ret;

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    uint8_t timeout = 10;
    while (timeout && (WiFi.status() != WL_CONNECTED)) {
      timeout--;
      delay(1000);
    }
  }
  
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}
