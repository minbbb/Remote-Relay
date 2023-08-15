#include <RCSwitch.h>

#define BEEP_PIN 3
#define SHOT_PIN 4

#define BUTTON_PRESS_DELAY 500
#define AFTER_SHOT_DELAY 2000
#define BEEP_TONE 5000
#define BEEP_DELAY 100  //max 1000
#define FINAL_BEEP_DELAY 600

RCSwitch radio = RCSwitch();
byte key = 0b110011;
struct {
  unsigned soundEnable : 1;
  unsigned shotDelay : 5;
  unsigned key : 6;
} radioData;

void setup() {
  radio.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
  pinMode(BEEP_PIN, OUTPUT);
  pinMode(SHOT_PIN, OUTPUT);
}

void loop() {
  if (radio.available()) {
    if (radio.getReceivedBitlength() == 12) {
      int value = radio.getReceivedValue();
      radioData.soundEnable = value & 0x01;
      radioData.shotDelay = (value >> 1) & 0x1F;
      radioData.key = (value >> 6) & 0x3F;

      if (radioData.key == key) {
        shot(radioData.shotDelay, radioData.soundEnable);
        delay(AFTER_SHOT_DELAY);
      }
    }
    radio.resetAvailable();
  }
}

void shot(byte shotDelay, bool soundEnable) {
  if (soundEnable) {
    for (byte i = 0; i < shotDelay; i++) {
      beep(BEEP_DELAY);
      delay(1000 - BEEP_DELAY);
    }
    beep(FINAL_BEEP_DELAY);
  } else {
    delay(1000 * shotDelay);
  }
  digitalWrite(SHOT_PIN, HIGH);
  delay(BUTTON_PRESS_DELAY);
  digitalWrite(SHOT_PIN, LOW);
}

void beep(unsigned int delayMs) {
  tone(BEEP_PIN, BEEP_TONE);
  delay(delayMs);
  noTone(BEEP_PIN);
}
