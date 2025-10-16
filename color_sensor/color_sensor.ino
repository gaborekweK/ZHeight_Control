/*#include <Wire.h>
#include <Adafruit_TCS34725.h>

#define SDA_PIN 5
#define SCL_PIN 4

// Fastest integration (≈2.4 ms) + modest gain keeps latency low for LED detection
Adafruit_TCS34725 tcs(
  TCS34725_INTEGRATIONTIME_2_4MS,
  TCS34725_GAIN_4X
);

// Helpers
static void rgb_to_hsv(float r, float g, float b, float &h, float &s, float &v) {
  float maxc = max(r, max(g, b));
  float minc = min(r, min(g, b));
  v = maxc;
  float d = maxc - minc;
  s = (maxc == 0.0f) ? 0.0f : d / maxc;

  if (d == 0.0f) { h = 0.0f; return; } // gray
  if (maxc == r)      h = fmodf(((g - b) / d), 6.0f);
  else if (maxc == g) h = ((b - r) / d) + 2.0f;
  else                h = ((r - g) / d) + 4.0f;
  h *= 60.0f;
  if (h < 0) h += 360.0f;
}

static const char* color_name_from_hsv(float h, float s, float v) {
  // Simple, readable palette; tweak thresholds for your environment/LED
  if (v < 0.10f) return "black";
  if (s < 0.10f) return (v > 0.80f) ? "white" : "gray";

  if      ((h >= 345 || h < 15))  return "red";
  else if (h < 45)                return "orange";
  else if (h < 70)                return "yellow";
  else if (h < 170)               return "green";
  else if (h < 200)               return "cyan";
  else if (h < 255)               return "blue";
  else if (h < 290)               return "indigo";
  else if (h < 345)               return "magenta";
  return "unknown";
}

void setup() {
  Serial.begin(115200);
  delay(100);
  Wire.begin(SDA_PIN, SCL_PIN); 

  if (!tcs.begin()) {
    Serial.println("TCS3472 not found. Check wiring.");
    while (true) delay(1000);
  }

  // Confirm settings (already set in constructor, but explicit is nice)
  tcs.setIntegrationTime(TCS34725_INTEGRATIONTIME_2_4MS);
  tcs.setGain(TCS34725_GAIN_4X);

  Serial.println("TCS3472 ready (addr 0x29). Reading colors...");
}

void loop() {
  uint16_t r_raw, g_raw, b_raw, c_raw;
  tcs.getRawData(&r_raw, &g_raw, &b_raw, &c_raw);

  // Guard against divide-by-zero
  float sum = (float)max<uint16_t>(1, c_raw);

  // Normalize to 0..1 (approx “chromaticity”)
  float rn = r_raw / sum;
  float gn = g_raw / sum;
  float bn = b_raw / sum;

  // Also scale to 0..1 range by dynamic gain (optional)
  // Here we use normalized chroma (rn,gn,bn) for hue; use c_raw for brightness (v)
  float h, s, v;
  rgb_to_hsv(rn, gn, bn, h, s, v);

  // Use clear channel to drive value (v) more meaningfully if desired
  // Map c_raw roughly into 0..1 (simple soft clip)
  float v_clear = min(1.0f, c_raw / 65535.0f);
  if (v < v_clear) v = v_clear;

  const char* name = color_name_from_hsv(h, s, v);

  Serial.print("RGBC: ");
  Serial.print(r_raw); Serial.print(", ");
  Serial.print(g_raw); Serial.print(", ");
  Serial.print(b_raw); Serial.print(", C=");
  Serial.print(c_raw);

  Serial.print("  |  HSV: ");
  Serial.print(h, 1); Serial.print("°, ");
  Serial.print(s, 2); Serial.print(", ");
  Serial.print(v, 2);

  Serial.print("  ->  ");
  Serial.println(name);

  delay(10); // ~100 Hz; integration time is 2.4ms so this is safe
}
*/