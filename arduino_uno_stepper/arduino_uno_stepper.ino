/*
// Uno + CNC Shield V3 quick runner (no GRBL)
// Z steps slowly until 'STOP' hits on Serial. 'GO' starts, 'RST' re-arms.

const int PIN_Z_STEP = 4;
const int PIN_Z_DIR  = 7;
const int PIN_EN     = 8;   // LOW=enable, HIGH=disable

// step timing - INCREASED SPEED
const uint32_t STEP_INTERVAL_US = 500;  // ~2kHz (was 1500us = 667Hz)
uint32_t lastStepUs = 0;
bool stepping = false;
bool stepLevel = false;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_Z_STEP, OUTPUT);
  pinMode(PIN_Z_DIR,  OUTPUT);
  pinMode(PIN_EN,     OUTPUT);

  digitalWrite(PIN_Z_DIR, HIGH); // set direction (flip if needed)
  digitalWrite(PIN_EN, HIGH);    // keep disabled until GO
  Serial.println("UNO:READY (commands: GO, STOP, RST)");
}

void loop() {
  // command parser
  if (Serial.available()) {
    String s = Serial.readStringUntil('\n'); s.trim();
    if (s == "GO") {
      stepping = true;
      digitalWrite(PIN_EN, LOW);    // enable driver
      lastStepUs = micros();
      Serial.println("OK:GO");
    } else if (s == "STOP") {
      // HARD STOP immediately
      digitalWrite(PIN_EN, HIGH);   // disable outputs
      stepping = false;
      Serial.println("OK:STOP");
    } else if (s == "RST") {
      stepping = false;
      digitalWrite(PIN_EN, HIGH);
      Serial.println("OK:RST");
    }
  }

  // step generator
  if (stepping) {
    uint32_t now = micros();
    if (now - lastStepUs >= (STEP_INTERVAL_US/2)) {
      lastStepUs = now;
      stepLevel = !stepLevel;
      digitalWrite(PIN_Z_STEP, stepLevel ? HIGH : LOW);
    }
  }
}*/