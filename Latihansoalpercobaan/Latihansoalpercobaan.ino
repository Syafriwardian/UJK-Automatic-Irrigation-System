//code serial pada web Bylnk
#define BLYNK_TEMPLATE_ID "TMPL6d8Vk8YhD"
#define BLYNK_TEMPLATE_NAME "SENSOR DHT DAN SILINOID"
#define BLYNK_AUTH_TOKEN "-z9NmhIVWtP2Posjm7jy9X1rhM-zpqCP"

//library untuk koneksi ke wifi dan blynk
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//library untuk LCD
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Set password to "" for open networks.
char ssid[] = "Ws-Eletronika";
char pass[] = "@bpvppdg25";

//lib sensor DHT
#include "DHT.h"
DHT dht(5, DHT11);

//fungsion untuk pin sensor selenoid dan led
const int sensorPin = 36;
int nilaiADC;
int persen;
int ledPin = 2;
float hum = dht.readHumidity();
float tem = dht.readTemperature();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  pinMode(ledPin, OUTPUT);
  //console ke kode blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  // put your main code here, to run repeatedly:

  float nilaiADC = analogRead(sensorPin);
  persen = map(nilaiADC, 2600, 1089, 0, 100);
  persen = constrain(persen, 0, 100);

  //serial sorcode untuk code alat sensor
  Blynk.run();
  Serial.println(analogRead(sensorPin));

  Blynk.virtualWrite(V0, persen);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, tem);

  //untuk mengatur kondisi LED saat kondisi tanah lebih lembab
  if (persen > 25) {
    Blynk.virtualWrite(V3, 1);
    digitalWrite(ledPin, HIGH);
  } else {
    Blynk.virtualWrite(V3, 0);
    digitalWrite(ledPin, LOW);
  }

  Serial.print("Kelembaban Udara: ");
  Serial.println(hum);
  Serial.print("%");
  Serial.print("Suhu Udara");
  Serial.print(tem);
  Serial.print("°C");
  delay(1000);

  //sorcode untuk menanpilkanya ke dalam LCD
  lcd.setCursor(0, 0);
  lcd.print("Kl Udara:");
  lcd.print(hum);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Suhu:");
  lcd.print(tem);
  lcd.print(" C");
  delay(2000);
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Kl Tanah:");
  lcd.print(persen);
  lcd.print("%");
  delay(2000);
  lcd.clear();
}
