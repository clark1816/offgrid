#include <SoftwareSerial.h>
#include <DHT.h>

// Pins
#define DHTPIN 2
#define DHTTYPE DHT11
#define SOIL_PIN A0

// Soil calibration
const int dryValue = 853;
const int wetValue = 350;

// LoRa address settings
#define MY_ADDRESS 1
#define DEST_ADDRESS 2
#define NETWORK_ID 18

SoftwareSerial lora(4, 3); // RX=4, TX=3
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  //Serial.println(analogRead(SOIL_PIN));
  Serial.begin(9600);
  lora.begin(115200);
  dht.begin();
  delay(2000);

  lora.print("AT+BAND=915000000\r\n");     delay(500);
  Serial.print("BAND: ");
  while(lora.available()) Serial.write(lora.read());
  Serial.println();

  lora.print("AT+PARAMETER=9,7,1,12\r\n"); delay(500);
  Serial.print("PARAM: ");
  while(lora.available()) Serial.write(lora.read());
  Serial.println();

  lora.print("AT+ADDRESS=1\r\n");           delay(500);
  Serial.print("ADDR: ");
  while(lora.available()) Serial.write(lora.read());
  Serial.println();

  lora.print("AT+NETWORKID=18\r\n");        delay(500);
  Serial.print("NET: ");
  while(lora.available()) Serial.write(lora.read());
  Serial.println();

  Serial.println("Transmitter ready!");
}

void loop() {
  float temp = dht.readTemperature(true);
  float humidity = dht.readHumidity();

  int raw = analogRead(SOIL_PIN);
  int moisture = map(raw, dryValue, wetValue, 0, 100);
  moisture = constrain(moisture, 0, 100);

  if (isnan(temp) || isnan(humidity)) {
    Serial.println("DHT11 read failed!");
  } else {
    String message = "T:" + String(temp, 1) +
                     "F H:" + String(humidity, 1) +
                     "% S:" + String(moisture) + "%";

    lora.print("AT+SEND=");
    lora.print(DEST_ADDRESS);
    lora.print(",");
    lora.print(message.length());
    lora.print(",");
    lora.print(message);
    lora.print("\r\n");

    Serial.println("Sent: " + message);
  }

  delay(3600000); // 36 seconds for testing (change to 3600000 for 1 hour)
}
