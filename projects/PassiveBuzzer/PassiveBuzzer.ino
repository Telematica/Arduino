#include "src/pitches.h"

#define BUZZER_PIN 4 // GPIO14
#define MAX_FREQUENCY 2100 // Maximum frequency for the buzzer (2.1 kHz)

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  Serial.println("test");
  Serial.printf("Core Version: %s\n", ESP.getCoreVersion());
  alarmSiren();  
  // Wait before repeating the melody
  // delay(2000); 
}

void nokiaRingtone() {
  int noteDurations[] = { 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 600 };

  // Nokia Ringtone (E5 D5 F#4 G#4 - C#5 B4 D4 E4 - B4 A4 C#4 E4 A4)
  int nokiaRingtone[] = { 
    NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
    NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
    NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4, NOTE_A4 
  };
  
  for (int i = 0; i < 13; i++) {
    tone(BUZZER_PIN, nokiaRingtone[i], noteDurations[i]);
    
    // Pause between notes
    delay(noteDurations[i] * 1.30); 
    noTone(BUZZER_PIN);
  }
}

void alarmSiren() {
  for (int i = 500; i <= MAX_FREQUENCY; i++) {
    tone(BUZZER_PIN, i);
    delay(2);
  }
  
  // Ramp down the frequency from 2100 Hz to 500 Hz
  for (int i = MAX_FREQUENCY; i >= 500; i--) {
    tone(BUZZER_PIN, i);
    delay(2);
  }
}