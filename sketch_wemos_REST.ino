#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>

#include <ESP8266WiFi.h>
#include "restServer.h"

const char* ssid = "CableBox-AA38";
const char* password = "czn4yjy3tm";
char light1_status[8] = "OFF";

WiFiServer server(80);
void process(WiFiClient);

struct light lights[10];

/////// RF //////
//int msg[1];
//RF24 radio(9,10);
//const uint64_t pipe = 0xE8E8F0F0E1LL;
//int SW1 = 7;


void setup() {
  Serial.begin(115200);
  Serial.println();
  delay(10);
   
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.println(WiFi.localIP());


// Populate lightList with artificial active lights
  lights[0].id = 1;
  lights[0].onStatus = true;
  lights[1].id = 2;
  lights[1].onStatus = false;
  lights[2].id = 3;
  lights[2].onStatus = true;
}

void loop() {

WiFiClient client = server.available();

if(client){
  Serial.println("Client connected");
 
  process(client);

  client.flush();
  Serial.println("connection stopped");
  client.stop();
}


delay(100);
}


