#include <Adafruit_NeoPixel.h>
#define PIN   D4
#define LED_NUM 7
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WEMOS_SHT3X.h>
#define ON_Board_LED 2
SHT3X sht30(0x45);
#include <Wire.h>
#include <SPI.h>


Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, PIN, NEO_GRB + NEO_KHZ800);

const int SHTPin = 5; 
 

const char* ssid = "wifi-name"; 
const char* password = "psswrd"; 


const char* host = "script.google.com";
const int httpsPort = 443;


WiFiClientSecure client; 

String GAS_ID = "ID-Google"; 

void setup() {

  leds.begin();

  Serial.begin(115200);
  delay(500);
  
  delay(500);
  
  WiFi.begin(ssid, password); 
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT); 
  digitalWrite(ON_Board_LED, HIGH); 


  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
 
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
  }

  digitalWrite(ON_Board_LED, HIGH);  
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  client.setInsecure();
}

void led_set(uint8 R, uint8 G, uint8 B) {
  for (int i = 0; i < LED_NUM; i++) {
    leds.setPixelColor(i, leds.Color(R, G, B));
    leds.show();
    delay(50);
  }
}

void loop() {

  sht30.get();
  int h = sht30.humidity;
  
  float t = sht30.cTemp;
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from SHT sensor !");
    delay(500);
    return;
  }
  String Temp = "Temperature : " + String(t) + " °C";
  String Humi = "Humidity : " + String(h) + " %";
  Serial.println(Temp);
  Serial.println(Humi);
  
  sendData(t, h);

  delay(300000);
}

void sendData(float tem, int hum) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  String string_temperature =  String(tem);
  String string_humidity =  String(hum, DEC); 
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity;
  Serial.print("requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");
  Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
    led_set(0,10,0);
  } else {
    Serial.println("esp8266/Arduino CI has failed");
    led_set(10,0,0);
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();  
} 