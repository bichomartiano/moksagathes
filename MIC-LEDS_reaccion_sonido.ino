
#define FASTLED_INTERNAL
#include <FastLED.h>
#define LED_PIN 8
#define NUM_LEDS 150
#define MAX_LEDS 150
#define AV_LEDS 150
#define BRIGHTNESS  20

// Define hardware connections
#define PIN_GATE_IN 12 //gate
#define PIN_GATE2_IN 2 //gate
#define IRQ_GATE_IN  0 //pin 0 not used, not sure what this is for?
#define IRQ_GATE2_IN  1 //pin 1 not used, not sure what this is for?
#define PIN_ANALOG_IN A0 // envelope
#define PIN_ANALOG2_IN A1 //envelope

#define INIT_LED 1

CRGB leds[NUM_LEDS];

#define BUTTONPIN 2

int buttonState = LOW;
int BASE = 0;

int cycle = 0;
int cycleValue = 6;

int progressiveTime = 40;

int mode = 0;
int volume = 0;
int volume2 = 0;
int change = 0;
// soundISR()
// This function is installed as an interrupt service routine for the pin
// change interrupt.  When digital input 2 changes state, this routine
// is called.
// ItS queries the state of that pin, and sets the onboard LED to reflect that
// pin's state.
void soundISR()
{
  int pin_val;
  int pin_val2;
  pin_val = digitalRead(PIN_GATE_IN);
  pin_val2 = digitalRead(PIN_GATE2_IN);
  //digitalWrite(PIN_LED_OUT, pin_val);
}





void setup() {
  Serial.begin(9600);
  pinMode(PIN_GATE_IN, INPUT);
  pinMode(PIN_GATE2_IN, INPUT);
  attachInterrupt(IRQ_GATE_IN, soundISR, CHANGE);
  attachInterrupt(IRQ_GATE2_IN, soundISR, CHANGE);

  // Display status
  Serial.print("Initialized");


  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  set_max_power_in_volts_and_milliamps(5, 20000);

  pinMode(BUTTONPIN, INPUT_PULLUP);


}
void loop() {

  // Check the envelope input
  volume = analogRead(PIN_ANALOG_IN);
  volume2 = analogRead(PIN_ANALOG2_IN);
  Serial.print(volume);
  Serial.print("\t");

  // Serial.println(volume2);
  for (int i = 0; i <= MAX_LEDS; i++) {
    leds[i] = CRGB (0, 0, 0);
  }
  FastLED.show();

  fadingMode();
 
  //remap the values coming from the mic
  /* volume=map(volume,20,420,0,255);
     Serial.println(volume);
  */

  /* 


  */

  buttonState = digitalRead(BUTTONPIN);

}

void fadingMode () {

  int randR = random(0, 255);
  int randG = random(0, 255);
  int randB = random(0, 255);
  int sign;
  int j = random(-50, 50);


  pintaleds(randR, randG, randB);
  Serial.println(j);

  if (j == 0) {
    j = 1;
  } //just to avoid 0/0
  //parameter to define 'direction' of colours
  sign = j / -j;
  sign = 1;
  Serial.println("\t");
  Serial.println(sign);
  

 
  while (change == 0)   {

    if (volume > 100) {
    volume = analogRead(PIN_ANALOG_IN);
randR=map(volume,20,420,0,255);
    }

    pintaleds(randR, randG, randB);

    randR = randR + sign; randG -= sign; randB += sign;

    delay(10);

    //limits filters
    if ( randR == 0 || randG == 0 || randB == 0 || randR == 255 || randG == 255 || randB == 255)
    {
      sign = -sign;
    }

    volume = analogRead(PIN_ANALOG_IN);
    if (volume >= 175) {
      change = 1;
    }

  }

 change = 0;

}


void pintaleds(int R, int G, int B) {
  for (int i = 0; i <= AV_LEDS; i++) {
    leds[i] = CRGB (R, G, B);
  }
  FastLED.show();
}

void cycleMode(int R, int G, int B) {
  int inc = 2;
  if (cycle >= 0 && cycle < cycleValue) {

    progresiveMode(R, G, B);
    cycle = cycle + 1;
  } else if (cycle >= cycleValue && cycle < cycleValue * 3) {
    glitchMode();
    cycle = cycle + 1;
    inc = inc + 1;
  } else if (cycle >= cycleValue * 3 && cycle < cycleValue * 4) {
    glitchModeFast();
    cycle = cycle + 1;
    inc = inc + 1;
  } else if (cycle >= cycleValue * 4 && cycle < cycleValue * 5) {
    progresiveModeFarmacy(R, G, B);
    cycle = cycle + 1;
    inc = inc + 1;
  } else if (cycle >= cycleValue * 5 && cycle < cycleValue * 8) {
    randomColorMode();
    cycle = cycle + 1;
    inc = inc + 1;
  } else if (cycle >= cycleValue * 8 && cycle < cycleValue * 9) {
    glitchMode();
    jumpsMode(R, G, B, 5);
    cycle = cycle + 1;
    inc = inc + 1;
  } else if (cycle >= cycleValue * 9 && cycle < cycleValue * 10) {
    progresiveModeExp(R, G, B, 5);
    cycle = cycle + 1;
    inc = inc + 1;
  } else {
    cycle = 0;
  }
}

void switchOffLoop() {
  for (int i = 0; i <= AV_LEDS; i++) {
    leds[i] = CRGB ( 0, 0, 0);
  }
  FastLED.show();
}

void progresiveMode(int R, int G, int B) {
  for (int i = 0; i <= MAX_LEDS; i++) {
    leds[i] = CRGB (0, 0, 0);

    volume = analogRead(PIN_ANALOG_IN);
    volume2 = analogRead(PIN_ANALOG2_IN);
    Serial.print(volume);
    Serial.print("\t");
    Serial.println(volume2);

  }
  FastLED.show();

  for (int i = INIT_LED; i <= AV_LEDS; i++) {
    leds[i] = CRGB (R, G, B);
    FastLED.show();
    delay(progressiveTime);
  }

  for (int i = AV_LEDS; i > 0; i--) {
    leds[i] = CRGB (0, 0, 0);
    FastLED.show();
    delay(progressiveTime);
  }
}


void progresiveModeFarmacy(int R, int G, int B) {
  for (int i = 0; i <= MAX_LEDS; i++) {
    leds[i] = CRGB (0, 0, 0);
  }
  FastLED.show();

  for (int i = INIT_LED; i <= AV_LEDS; i++) {
    leds[i] = CRGB (R, G, B);
    FastLED.show();
    delay(progressiveTime);
  }

  for (int i = INIT_LED; i <= AV_LEDS; i++) {
    leds[i] = CRGB (0, 0, 0);
    FastLED.show();
    delay(progressiveTime);
  }
}

void progresiveModeExp(int R, int G, int B, int amount) {

  int cycles = 10;
  int total = MAX_LEDS / amount - amount;
  for (int a = 0; a <= cycles; a++) {

    for (int i = 0; i <= total; i++) {

      for (int o = 0; o <= MAX_LEDS; o++) {
        leds[o] = CRGB (0, 0, 0);
      }

      int initLed = INIT_LED + amount * i;
      int endLed = INIT_LED + (amount * (i + 1));
      for (int o = initLed; o <= endLed; o++) {
        leds[o] = CRGB (R, G, B);
      }
      FastLED.show();
      delay(400);

    }
  }
}


void sidesMode(int R, int G, int B) {
  int longSide = 57;
  int shortSide = 17;
  int value = 10;
  for (int i = 0; i <= value; i++) {

    for (int i = 0; i <= MAX_LEDS; i++) {
      leds[i] = CRGB (0, 0, 0);
    }
    for (int i = INIT_LED; i <= INIT_LED + longSide * 1 + shortSide * 0 ; i++) {
      leds[i] = CRGB (R, G, B);
    }
    FastLED.show();
    delay(400);

    for (int i = 0; i <= MAX_LEDS; i++) {
      leds[i] = CRGB (0, 0, 0);
    }
    FastLED.show();
    for (int i = INIT_LED + longSide * 1 + shortSide * 0; i <= INIT_LED + longSide * 1 + shortSide * 1; i++) {
      leds[i] = CRGB (R, G, B);
    }
    FastLED.show();
    delay(400);


  }
}

void jumpsMode(int R, int G, int B, int amount) {

  int cycles = 10;

  for (int i = 0; i <= MAX_LEDS; i++) {

    for (int o = 0; o <= MAX_LEDS; o++) {
      leds[o] = CRGB (0, 0, 0);
    }

    int initLed = INIT_LED + amount * i;
    int endLed = INIT_LED + (amount * (i + 1));
    for (int o = initLed; o <= endLed; o++) {
      leds[o] = CRGB (R, G, B);
    }
    FastLED.show();
    delay(400);

  }

}

void randomColorMode() {
  int value = 10;
  for (int i = 0; i <= value; i++) {
    for (int i = INIT_LED; i <= AV_LEDS; i++) {
      int rnd = random(0, 255);
      leds[i] = CRGB (rnd, rnd, rnd);
    }
    FastLED.show();
    delay(40);
  }
}

void glitchMode() {
  switchOn(255, 255, 255);
  delay(1000);
  switchOff(1000);

  switchOn(255, 255, 255);
  delay(100);
  switchOff(100);

  switchOn(255, 255, 255);
  delay(2000);
  switchOff(1000);

  switchOn(255, 255, 255);
  delay(100);
  switchOff(1000);

  switchOn(255, 255, 255);
  delay(50);
  switchOff(100);

  switchOn(255, 255, 255);
  delay(100);
  switchOff(100);

  switchOn(255, 255, 255);
  delay(50);
  switchOff(1000);

  switchOn(255, 255, 255);
  delay(500);
  switchOff(100);

  switchOn(255, 255, 255);
  delay(50);
  switchOff(2000);

  switchOn(255, 255, 255);
  delay(500);
  switchOff(100);
}


void glitchModeFast() {
  switchOn(255, 255, 255);
  delay(500);
  switchOff(500);

  switchOn(255, 255, 255);
  delay(100);
  switchOff(100);

  switchOn(255, 255, 255);
  delay(200);
  switchOff(200);

  switchOn(255, 255, 255);
  delay(100);
  switchOff(500);

  switchOn(255, 255, 255);
  delay(50);
  switchOff(100);

  switchOn(255, 255, 255);
  delay(100);
  switchOff(100);

  switchOn(255, 255, 255);
  delay(50);
  switchOff(500);

  switchOn(255, 255, 255);
  delay(100);
  switchOff(100);

  switchOn(255, 255, 255);
  delay(50);
  switchOff(1000);

  switchOn(255, 255, 255);
  delay(200);
  switchOff(100);
}


void switchOn(int R, int G, int B) {
  for (int i = INIT_LED; i <= AV_LEDS; i++) {
    leds[i] = CRGB (R, G, B);
  }
  FastLED.show();
}


void switchOff(int time) {
  for (int i = AV_LEDS; i > 0; i--) {
    leds[i] = CRGB (0, 0, 0);
  }
  FastLED.show();
  delay(time);
}
