#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <FirebaseJson.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "tttttrunggggg"
#define WIFI_PASSWORD "ductrung"
#define API_KEY "AIzaSyCH7NojcKtJIG98LKan1WxfFsDIn1bNe9A"
#define DATABASE_URL "https://smart-home-d65f6-default-rtdb.firebaseio.com" //<databaseName>.firebaseio.com or <databaseName>.<region>.firebasedatabase.app
#define USER_EMAIL "dctrung0108@gmail.com"
#define USER_PASSWORD "dctrung0108@gmail.com"

FirebaseData fbdo;
FirebaseJson json;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
uint32_t idleTimeForStream = 15000;

void setup() {

  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);
  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback; 
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);
  Firebase.begin(&config, &auth);
  fbdo.keepAlive(5, 5, 1);
  if (!Firebase.beginStream(fbdo, "/data"))
    Serial.printf("sream begin error, %s\n\n", fbdo.errorReason().c_str());
}

void loop() {
  if (Firebase.ready() && (millis() - sendDataPrevMillis > idleTimeForStream || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    count++;
  }

  if (Firebase.ready())
  {

    if (!Firebase.readStream(fbdo))
      Serial.printf("sream read error, %s\n\n", fbdo.errorReason().c_str());

    if (fbdo.streamTimeout())
    {
      Serial.println("stream timed out, resuming...\n");

      if (!fbdo.httpConnected())
        Serial.printf("error code: %d, reason: %s\n\n", fbdo.httpCode(), fbdo.errorReason().c_str());
    }

    if (fbdo.streamAvailable())
    {
      Serial.printf("sream path, %s\nevent path, %s\ndata type, %s\nevent type, %s\nvalue, %s\n\n",
                    fbdo.streamPath().c_str(),
                    fbdo.dataPath().c_str(),
                    fbdo.dataType().c_str(),
                    fbdo.eventType().c_str(),
                    fbdo.stringData().c_str());
      if (fbdo.eventType().c_str() == 'put') {
        Serial.println("Initial data");
      } else if (fbdo.dataPath().c_str() == '/light') {

      } else if (fbdo.dataPath().c_str() == '/luminosity') {

      } else if (fbdo.dataPath().c_str() == '/window') {
        
      } else if (fbdo.dataPath().c_str() == '/lock') {
        
      } else if (fbdo.dataPath().c_str() == '/waterspray') {
        
      }
    }
  }

  // After calling stream.keepAlive, now we can track the server connecting status
  if (!fbdo.httpConnected())
  {
    // Server was disconnected!
  }
}