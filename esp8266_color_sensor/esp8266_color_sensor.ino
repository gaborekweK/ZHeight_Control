/*#include <Wire.h>
#include <Adafruit_TCS34725.h>

#define SDA_PIN 5
#define SCL_PIN 4

Adafruit_TCS34725 tcs(
  TCS34725_INTEGRATIONTIME_2_4MS,
  TCS34725_GAIN_4X
);

bool latched = false;

bool greenSeen() {
  uint16_t r,g,b,c;
  tcs.getRawData(&r,&g,&b,&c);
  float ratio = (float)g / (float)(r + b + 1);
  
  // Adjusted for your green readings: R=4, G=6, B=3, ratio=0.75
  return (g >= 5) && (ratio >= 0.6f) && (c >= 10);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!tcs.begin()) { Serial.println("ERR:TCS34725"); while(1) delay(1000); }
  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_2_4MS);
  tcs.setGain(TCS34725_GAIN_4X);
  Serial.println("OK:READY");
}

void loop() {
  // allow host to re-arm
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n'); s.trim();
    if (s == "RST") { latched = false; Serial.println("OK:RST"); }
  }
  if (!latched) {
    // 3-of-5 confirmation to avoid flicker
    int hits = 0;
    for (int i=0;i<5;i++){ if(greenSeen()) hits++; delayMicroseconds(300); }
    if (hits >= 3) { latched = true; Serial.println("TOUCH"); }
  }
  delay(2);
}*/