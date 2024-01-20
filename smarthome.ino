#include <Arduino.h>
#if defined(ESP32) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#elif __has_include(<WiFiNINA.h>)
#include <WiFiNINA.h>
#elif __has_include(<WiFi101.h>)
#include <WiFi101.h>
#elif __has_include(<WiFiS3.h>)
#include <WiFiS3.h>
#endif
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

int led = 22;  

const char* ssid = "tttttrunggggg";
const char* password = "ductrung";

#define API_KEY "AIzaSyCH7NojcKtJIG98LKan1WxfFsDIn1bNe9A"
#define DATABASE_URL "https://smart-home-d65f6-default-rtdb.firebaseio.com" 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long dataMillis = 0;
int count = 0;
bool signupOK = false;

void setup() {
  Serial.begin(115200);
  pinMode(led, OUTPUT);    

  
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print(WiFi.localIP());


  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  if (Firebase.signUp(&config, &auth, "", ""))
  {
    Serial.println("ok");
    signupOK = true;
  }
  else
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  config.token_status_callback = tokenStatusCallback; 
  Firebase.begin(&config, &auth);
}
 
void loop() {
  if (Firebase.ready() && (millis() - dataMillis > 15000 || dataMillis == 0))
  {
    dataMillis = millis();
    bool bVal;
    Firebase.RTDB.getBool(&fbdo, F("light"), &bVal) ? bVal ? digitalWrite(led, HIGH) : digitalWrite(led, LOW) : fbdo.errorReason().c_str();
    count++;
  }
}
