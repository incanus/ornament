// power routine based on http://www.technoblogy.com/show?VOO
// tone on ATtiny25 via https://github.com/SpenceKonde/ATTinyCore

#include <avr/sleep.h>

int Power __attribute__ ((section (".noinit")));

const uint16_t scale[] = {
  262,294,330,349,392
};
const uint8_t n[] = {
  2,2,2,2,2,2,2,4,0,1,2,
  3,3,3,3,3,2,2,2,2,4,4,3,1,0
};
const uint8_t ds[] = {
  5,10,20,40
};
const uint8_t d[] = {
  1,1,2,1,1,2,1,1,1,1,3,1,
  1,1,1,1,1,1,0,0,1,1,1,1,3
};

const uint8_t red = 0;
const uint8_t beep = 1;
const uint8_t green = 2;
const uint8_t yellow = 3;
const uint8_t nc = 4;

int active = 0;
int timer = 0;

void PowerDown() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  ADCSRA &= ~(1<<ADEN);
  sleep_enable();
  sleep_cpu();
}

void setup() {
  Power = !Power;
  if (!Power) PowerDown();
  pinMode(red, OUTPUT);
  pinMode(beep, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(nc, INPUT_PULLUP);
}

void loop() {
  digitalWrite(yellow, HIGH);
  timer = millis();
  for (int i=0; i<25; i++) {
    digitalWrite(active, HIGH);
    tone(beep, scale[n[i]]);
    unsigned long dur = (unsigned long)ds[d[i]] * (unsigned long)20;
    int frag = dur / 10;
    for (int j=0; j<dur; j=j+frag) {
      if (millis() > timer + 500) {
        digitalWrite(active, LOW);
        active = (active == red) ? green : red;
        digitalWrite(active, HIGH);
        timer = millis();
      }
      delay(frag);
    }
    noTone(beep);
    delay(100);
  }
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  digitalWrite(yellow, LOW);
  delay(5000);
}
