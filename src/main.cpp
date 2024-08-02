#include <Arduino.h>
#include <WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>

#include <ArduinoJson.h>

//WiFi credentials
#define WIFI_SSID "yourwifissidhere"
#define WIFI_PASSWORD "yourpasswordhere"

//Firebase credentials
#define API_KEY "AIzaSyAXyI_4THjGKLasAEWKPFKulh_W5rkqxzM"
#define DATABASE_URL "https://esp32ledcontrol-46eac-default-rtdb.firebaseio.com/"

//Define user credentials
#define USER_EMAIL "aizhar.quindoza@gmail.com"
#define USER_PASSWORD "11@XBlastbreed1"

//define firebase data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

const int ledPin1 = 17;
const int ledPin2 = 16;
const int ledPin3 = 4;
const int ledPin4 = 2;



void setup() {
  
  pinMode(ledPin1, OUTPUT);
  digitalWrite(ledPin1, LOW);

  pinMode(ledPin2, OUTPUT);
  digitalWrite(ledPin2, LOW);

  pinMode(ledPin3, OUTPUT);
  digitalWrite(ledPin3, LOW);

  pinMode(ledPin4, OUTPUT);
  digitalWrite(ledPin4, LOW);

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

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  // Comment or pass false value when WiFi reconnection will control by your code or third party library e.g. WiFiManager
  Firebase.reconnectNetwork(true);

  // Since v4.4.x, BearSSL engine was used, the SSL buffer need to be set.
  // Large data transmission may require larger RX buffer, otherwise connection issue or data read time out can be occurred.
  fbdo.setBSSLBufferSize(4096 /* Rx buffer size in bytes from 512 - 16384 */, 1024 /* Tx buffer size in bytes from 512 - 16384 */);

  // Limit the size of response payload to be collected in FirebaseData
  fbdo.setResponseSize(2048);

  Firebase.begin(&config, &auth);

  Firebase.setDoubleDigits(5);

  config.timeout.serverResponse = 10 * 1000;
}


void loop() {
  
// Firebase.ready() should be called repeatedly to handle authentication tasks.
  if (Firebase.ready() && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();

  int ledState;
   if(Firebase.RTDB.getInt(&fbdo, "/Led1Status", &ledState)){
    digitalWrite(ledPin1, ledState);
   }

   if(Firebase.RTDB.getInt(&fbdo, "/Led2Status", &ledState)){
    digitalWrite(ledPin2, ledState);
   }

   if(Firebase.RTDB.getInt(&fbdo, "/Led3Status", &ledState)){
    digitalWrite(ledPin3, ledState);
   }

   if(Firebase.RTDB.getInt(&fbdo, "/Led4Status", &ledState)){
    digitalWrite(ledPin4, ledState);
   }
   
   else{
    Serial.println(fbdo.errorReason().c_str());
   }
  }

}

