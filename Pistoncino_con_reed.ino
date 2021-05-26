#define BLYNK_PRINT Serial
#include <Adafruit_NeoPixel.h>
#include "BlynkSimpleEsp8266.h"
#include <ESP8266WiFi.h>
#define RELAY_PIN D1
#define RGB_PIN D4
#define LED_NUM 7
#define reed_pin1 D2
#define reed_pin2 D3
char auth[] = "p0UQFbBDlNG24kxmPqOzZbda09WSOfOF";
char ssid[] = "vallauri.docenti";
char pass[] = "fossano-vallauri";
int valPin;
int t2 = 1000;
unsigned long tempo = 0;
int t1 = 1000;
int strokes;

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
  digitalRead(reed_pin1);
  digitalRead(reed_pin2);

  if (valPin == 0) {
      led_set(10, 0, 0); // rosso
      digitalWrite(RELAY_PIN, LOW);
  } else if (valPin == 1 && reed_pin1 == HIGH && reed_pin2 == LOW) {
      tempo = millis();
      led_set(10, 10, 0); // led giallo
      delay(t2);
      led_set(0, 10, 0); // verde
      digitalWrite(RELAY_PIN, HIGH);
  } else if (valPin == 1 && reed_pin1 == LOW && reed_pin2 == LOW) {
      led_set(0, 10, 0); // verde
  } else if (valPin == 1 && reed_pin1 == LOW && reed_pin2 == HIGH) {
      strokes = strokes + 1;
      digitalWrite(RELAY_PIN, LOW);
  } else if (valPin == 1 && reed_pin1 == HIGH && reed_pin2 == LOW && strokes >= 1) {
      strokes = strokes + 1;
      delay(t1);
      digitalWrite(RELAY_PIN, HIGH);
      led_set(0, 10, 0);
  }
}

BLYNK_WRITE(V0) // V0 E' il numero del pin virtuale
{
  // Inserisce all'interno della variabile valPin il valore ricevuto dal pin
  // virtuale V0
  valPin = param.asInt();
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
