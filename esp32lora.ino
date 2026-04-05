HardwareSerial lora(2);

void sendAT(String cmd) {
  lora.print(cmd);
  delay(500);
  String resp = "";
  while (lora.available()) resp += (char)lora.read();
  resp.trim();
  Serial.println(cmd.substring(0, cmd.length()-2) + " -> " + resp);
}

void setup() {
  Serial.begin(115200);
  lora.begin(9600, SERIAL_8N1, 16, 17);
  delay(2000);

  sendAT("AT+BAND=915000000\r\n");
  sendAT("AT+PARAMETER=9,7,1,12\r\n");
  sendAT("AT+ADDRESS=2\r\n");
  sendAT("AT+NETWORKID=18\r\n");

  Serial.println("Ready - waiting for data...");
}

void loop() {
  if (lora.available()) {
    String msg = lora.readStringUntil('\n');
    msg.trim();
    if (msg.length() > 0) {
      Serial.println(msg);
    }
  }
}