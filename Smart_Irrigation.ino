
//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <DHT.h>    // Install DHT11 Library and Adafruit Unified Sensor Library


#define FIREBASE_HOST "smart-irrigation-system-ce584-default-rtdb.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "ThaOUwAB3kuFC6imqLa3eGvMYQSXTaSNjeKHZ9x0"
#define WIFI_SSID "Souhila"
#define WIFI_PASSWORD "Badra#1999"
#define DHTTYPE DHT11
#include <string.h>

const int DHT1 = 2;     //D4 
const int RainSensor= 14;  //D5
const int LevelWater= 12;  //D6
const int Pump = 13;       //D7


String PumpStatus="false";

float temp;
float hum;
float humSoil;
float level;
float rain;

//Define FirebaseESP8266 data object
FirebaseData firebaseData;

DHT dht(DHT1, DHTTYPE);
void setup()
{

  Serial.begin(115200);
 digitalWrite(RainSensor,LOW);
  digitalWrite(LevelWater,LOW);
  pinMode(DHT1 , INPUT); 
  //pinMode(HumSoil, INPUT);
  pinMode(RainSensor, INPUT);
  pinMode(LevelWater, INPUT);
  pinMode(Pump , OUTPUT); 
  
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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void loop() {
 temp=dht.readHumidity();
 hum=dht.readTemperature();
 if (isnan(temp)) temp=0;
 if (isnan(hum)) hum=0;
 rain=digitalRead(RainSensor);
 level=digitalRead(LevelWater);
 Serial.print(rain);
 humSoil= ( 100.00 - ( (analogRead(A0)/1023.00) * 100.00 ) );

 
   if (Firebase.setFloat(firebaseData, "/Temperature", temp))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
   if (Firebase.setFloat(firebaseData, "/Humidity", temp))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  
  if (Firebase.setFloat(firebaseData, "/SoilHumidity", humSoil))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
if (Firebase.setFloat(firebaseData, "/Raining", rain))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  if (Firebase.setFloat(firebaseData, "/WaterLevel", level))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  
   PumpStatus=Firebase.getString(firebaseData,"/Pump");

   if (PumpStatus=="true")
   {
    if (rain==HIGH || humSoil==80 || hum==80 || level==LOW)
    {
      PumpStatus="false";
      digitalWrite(Pump,LOW);
        if (Firebase.setString(firebaseData, "/Pump", PumpStatus))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
      
    }
    else digitalWrite(Pump,HIGH);
   }
   else {
    if (rain==LOW and (humSoil<30 || (temp>30 and hum<30)))
    {
      PumpStatus="true";
      digitalWrite(Pump,HIGH);
        if (Firebase.setString(firebaseData, "/Pump", PumpStatus))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
      
    }
    else digitalWrite(Pump,LOW);
   }
   

}
