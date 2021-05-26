#define BLYNK_PRINT Serial
#include <Adafruit_NeoPixel.h>
#include "BlynkSimpleEsp8266.h"
#include <ESP8266WiFi.h>
#define RELAY_PIN D1
#define RGB_PIN D8
#define LED_NUM 7
#define reed_pin1 D2
#define reed_pin2 D3
char auth[] = "_K_nA3w4S83QyjY0iXG63E6PRE1g6KdT";
char ssid[] = "vallauri.docenti";
char pass[] = "fossano-vallauri";
int valPin1;
int valPin2;
int t2 = 1000;
unsigned long tempo = 0;
int t1 = 1000;
int strokes; // n corse

Adafruit_NeoPixel leds =
    Adafruit_NeoPixel(LED_NUM, RGB_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  // Imposta pin digitale D1 come output
  pinMode(RELAY_PIN, OUTPUT);
  leds.begin(); // inizializza la  libreria del led rgb
}
void led_set(uint8 R, uint8 G, uint8 B) {
  for (int i = 0; i < LED_NUM; i++) {
    leds.setPixelColor(i, leds.Color(R, G, B));
    leds.show();
    delay(50);
  }
}
void loop() {

  Blynk.run();


  if (valPin1 == 0 && valPin2 == 0) {
      led_set(10, 0, 0); // rosso
      digitalWrite(RELAY_PIN, LOW);
  } else if (valPin1 == 1 && valPin2 == 0 ) {
      tempo = millis();
      led_set(10, 10, 0); // led giallo
      delay(t2);
      led_set(0, 10, 0); // verde
      digitalWrite(RELAY_PIN, HIGH);
      strokes = strokes + 1;
  } else if (valPin1 == 1 && valPin2 == 1) {
      led_set(0, 10, 0); // verde
      digitalWrite(RELAY_PIN, LOW);
      strokes = strokes +1;
  } 
  }


BLYNK_WRITE(V0) // V0 E' il numero del pin virtuale
{
  // Inserisce all'interno della variabile valPin il valore ricevuto dal pin
  // virtuale V0
  valPin1 = param.asInt();
}
BLYNK_READ(V1)
{
  Blynk.virtualWrite(V1,tempo);
}
BLYNK_READ(V2)
{
  Blynk.virtualWrite(V2,t1);
}
BLYNK_READ(V3)
{
  Blynk.virtualWrite(V3,t2);
}
BLYNK_READ(V4)
{
  Blynk.virtualWrite(V4,strokes);
}
BLYNK_WRITE(V5) // V5 E' il numero del pin virtuale
{
  // Inserisce all'interno della variabile valPin il valore ricevuto dal pin
  // virtuale V5
  valPin2 = param.asInt();
}