#include <Arduino.h>
#include <WiFi.h>

#define WIFI_SSID "tttttrunggggg"                        
#define WIFI_PASSWORD "ductrung"  

int led = 39;  // Number of GPIO that is connected to LED

void setup() 
{
  Serial.begin(115200);
  delay(1000);    
  pinMode(led, OUTPUT);    
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);            
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
}
 
void loop() {
  digitalWrite(led, HIGH);     
}
