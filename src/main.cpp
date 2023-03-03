#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h" // remember to add this file to the .gitignore

#define switch_1 21
#define switch_2 22
#define switch_3 23

bool data [8][3]={
  {false,false,false},
  {false,false,true},
  {false,true,false},
  {false,true,true},
  {true,false,false},
  {true,false,true},
  {true,true,false},
  {true,true,true}
};

void setup() {
  pinMode(switch_1, OUTPUT);
  pinMode(switch_2, OUTPUT);
  pinMode(switch_3, OUTPUT);
  
  Serial.begin(9600);
	
	// WiFi_SSID and WIFI_PASS should be stored in the env.h
  WiFi.begin(WIFI_SSID, WIFI_PASS);

	// Connect to wifi
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
  
    // Establish a connection to the server
    http.begin(endpoint);
    
    // Specify content-type header
   
    for(int i=0; i<8; i++){

      http.addHeader("Content-Type", "application/json");
      http.addHeader("X-API-Key", key);

      // Serialise JSON object into a string to be sent to the API
      StaticJsonDocument<72> doc;
      String httpRequestData;

      digitalWrite(switch_1,data[i][0]);
      digitalWrite(switch_2,data[i][1]);
      digitalWrite(switch_3,data[i][2]);


      doc["light_switch_1"] = data[i][0];
      doc["light_switch_2"] = data[i][1];
      doc["light_switch_3"] = data[i][2];

      serializeJson(doc, httpRequestData);


      // Send HTTP POST request
      int httpResponseCode = http.PUT(httpRequestData);
      String http_response;

      // check reuslt of POST request. negative response code means server wasn't reached
     
   
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      http_response = http.getString();
      Serial.println(http_response);
      
      // Free resources
      http.end();

      delay(2000);

    }
    
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}