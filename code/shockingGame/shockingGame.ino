#include <math.h>
#include <FastLED.h>
#include "pitches.h"

#define MAX_PLAYERS 5

// LED stuff
#define NUM_LEDS 20
#define TWINKLE_SPEED 6
#define TWINKLE_DENSITY 6
CRGBArray<NUM_LEDS> leds;
CRGBPalette16 gCurrentPalette = *&RainbowColors_p;
CRGB gBackgroundColor = CRGB::Black;
int LEDSequenceNumber = -1; // tracks where in an LED sequence we are so that it is non-blocking

// Global play options
int numberOfPlayers = 5;
const int numberOfLEDsPerPlayer = 4;
const int shockTime = 100; // SHOCK DURATION IN MS
const int maxDelayBetweenResponses = 5000;
const int debounceTime = 50;

// Pins
const int buttonStartPin = 3;
const int buttonNumberOfPlayersPin = 12;
const int LEDPin = A0;
const int modePins[3] = {9, 10, 11}; // Reaction, Roulette, Recall
const int buttonPins[MAX_PLAYERS] = {4, 5, 6, 7, 8};
const int shockPins[MAX_PLAYERS] = {A5, A4, A3, A2, A1};
const int buzzer = 13; 

enum State {
  WAITING,
  PLAYING,
  FINISHING
};
State currentState = WAITING;

enum Mode {
  REACTION,
  ROULETTE,
  RECALL
};
Mode currentMode = RECALL;

// Reaction mode settings
int pressed[MAX_PLAYERS] = {0, 0, 0, 0, 0};
int pressedTotal = 0;
int startTime;

// Recall mode settings
const CRGB playerColors[MAX_PLAYERS] = {CRGB::Orange, CRGB::Blue, CRGB::Yellow, CRGB::Green, CRGB::Cyan};
const int playerTones[MAX_PLAYERS] = {NOTE_G3, NOTE_C4, NOTE_E4, NOTE_G4, NOTE_F4};
int betweenRoundsDelay = 1500;
int sequenceDelay = 750;
int sequenceDecrement = 25;
int sequence[1000];
int sequenceLength = 0;

// ============================================================================
// State Management
// ============================================================================
void reset() {
  Serial.println (F("Resetting."));
  FastLED.clear(true);
  currentState = WAITING;
  for(int player = 0; player < MAX_PLAYERS; player++) {
    pressed[player] = 0;
  }
  pressedTotal = 0;
  sequenceLength = 0;
  LEDSequenceNumber = -1;
  sequenceDelay = 750;
  LEDsNumberOfPlayers();
  delay(1000);
}

bool settingsChanged() {
  bool changed = false;
  if(digitalRead(buttonNumberOfPlayersPin) == LOW) {
    numberOfPlayers++;
    if(numberOfPlayers > MAX_PLAYERS) {
      numberOfPlayers = 2;
    }
    Serial.print(F("number of players changed to "));
    Serial.println(numberOfPlayers);
    changed = true;
    delay(50); // to prevent bounce
  }
  for(int pin = 0; pin < 3; pin++) {
    if(digitalRead(modePins[pin]) == LOW) {
      if(pin != static_cast<int>(currentMode)) {
        Serial.print(F("game mode changed to "));
        Serial.println(pin);
        changed = true;
        currentMode = static_cast<Mode>(pin); // 0 = Reaction, 1 = Roulette, 2 = recall
      }
    }
  }
  if(changed) {
    Serial.println(F("Settings changed"));
  }
  return changed;
}
// ============================================================================

// ============================================================================
// Game Play
// ============================================================================
void waitForRouletteButton() {
  Serial.println(F("waiting for a roulette button press..."));
  int targetPlayer = random(0, numberOfPlayers);
  Serial.print(F("target player is "));
  Serial.println(targetPlayer);
  while (pressedTotal == 0) {
    for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
      if (digitalRead(buttonPins[playerNum]) == LOW) {
        FastLED.clear(true);
        pressedTotal = 1;
        Serial.print(F("Button "));
        Serial.print(playerNum);
        Serial.println(F(" was pressed."));
        if(playerNum == targetPlayer) {
          shockPlayer(playerNum);
        }
        else {
          LEDsPlayerOneColor(playerNum, CRGB::Green);
          playSuccess();
          delay(2000);
        }
      }
    }
  }
}

void repeatSequenceItem(int itemNumber) {
  Serial.print("Repeating Item ");
  Serial.print(itemNumber);
  Serial.print(" (");
  Serial.print(sequence[itemNumber]);
  Serial.println(")");
  LEDsPlayerOneColor(sequence[itemNumber], playerColors[sequence[itemNumber]]);
  tone(buzzer, playerTones[sequence[itemNumber]], 300);
  delay(sequenceDelay);
  FastLED.clear(true);
}

void repeatSequence() {
  logSequence(0);
  FastLED.clear(true);
  for(int seq = 0; seq < sequenceLength; seq++) {
    repeatSequenceItem(seq);
    if(settingsChanged()) {
      reset();
    }
  }
}

void addItemToSequence() {
  FastLED.clear(true);
  int nextPlayerInSequence = random(numberOfPlayers);
  Serial.print(F("player "));
  Serial.print(nextPlayerInSequence);
  Serial.println(F(" is next in the sequence"));
  sequence[sequenceLength] = nextPlayerInSequence;
  repeatSequenceItem(sequenceLength);
  sequenceLength++;
  if(sequenceDelay > 200) { sequenceDelay = sequenceDelay - sequenceDecrement; } // Speed up as we go.  To a point.
}

void logSequence(int sequenceNumber) {
  Serial.print("Sequence: [");
  for(int i = 0; i < sequenceLength; i++) {
    Serial.print(sequence[i]);
    Serial.print(", ");
  }
  Serial.println("]");

  Serial.print("At position ");
  Serial.print(sequenceNumber);
  Serial.print(" expecting ");
  Serial.println(sequence[sequenceNumber]);
}

void waitForSequenceRepeat() {
  FastLED.clear(true);
  int sequenceNumber = 0;
  int lastButtonPress = millis();
  logSequence(sequenceNumber);

  while(currentState == PLAYING && sequenceNumber < sequenceLength) {
    for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
      if(digitalRead(buttonPins[playerNum]) == LOW) {
        // Only debounce if same player as last press
        if(millis() - lastButtonPress > debounceTime || sequenceNumber == 0 || sequence[sequenceNumber-1] != playerNum) { 
          if(playerNum == sequence[sequenceNumber]) {
            Serial.print(F("Player "));
            Serial.print(playerNum);
            Serial.print(F(" correctly pressed their button for sequence item "));
            Serial.println(sequenceNumber);
            repeatSequenceItem(sequenceNumber);
            sequenceNumber++;
            logSequence(sequenceNumber);
          }
          else {
            Serial.print(F("player "));
            Serial.print(playerNum);
            Serial.print(F(" pressed their button when we were expecting player "));
            Serial.println(sequence[sequenceNumber]);
            shockPlayer(playerNum);
            currentState = FINISHING;
          }
        }
        lastButtonPress = millis();
      }
  
      if(settingsChanged()) {
        currentState = FINISHING;
      }
    }

    if(millis() - lastButtonPress > maxDelayBetweenResponses) {
      Serial.print(F("player "));
      Serial.print(sequence[sequenceNumber]);
      Serial.println(F(" failed to press their button in time "));
      shockPlayer(sequence[sequenceNumber]);
      currentState = FINISHING;
    }

    if(settingsChanged()) {
      currentState = FINISHING;
    }
  }
}

bool checkCheater() {
  bool cheater = false;
  for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
    // A cheater has their button pressed when the game is about to start
    // Only the first cheater is punished 
    if (!cheater && digitalRead(buttonPins[playerNum]) == LOW) {
      cheater = true;
      shockCheater(playerNum);
    }
  }
  return cheater;
}

void waitForReactions() {
  Serial.println(F("Waiting for reactions..."));
  while (currentState == PLAYING  && pressedTotal < numberOfPlayers - 1) {
    for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
      if(digitalRead(buttonPins[playerNum]) == LOW) {
        if(pressed[playerNum] == 0) {
          pressed[playerNum] = 1;
          pressedTotal = pressedTotal + 1;
          LEDsPlayerPosition(playerNum, pressedTotal-1);
          Serial.print(F("Button "));
          Serial.print(playerNum);
          Serial.print(F(" was pressed in "));
          Serial.print(millis() - startTime);
          Serial.println(F(" millseconds"));
        }
      }
    }

    if(millis() - startTime > maxDelayBetweenResponses) {
      Serial.println(F("Time expired without a press"));
      for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
        if(pressed[playerNum] == 0) {
          Serial.print(F("player "));
          Serial.print(playerNum);
          Serial.println(F(" failed to press their button in time "));
        }
      }
      currentState = FINISHING;
    }

    if(settingsChanged()) {
      currentState = FINISHING;
    }
  }
}

void shockingTime() {
  noTone(buzzer);

  for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
    // Shock any player who deserves it (might be many if they all failed to press their button)
    if (pressed[playerNum] == 0) {
      int shockPin = shockPins[playerNum];

      Serial.print(F("Shocking player "));
      Serial.println(playerNum);
      digitalWrite (shockPin, HIGH);
      LEDsPlayerOneColor(playerNum, CRGB::Red);
    }
  }

  playFail();
  delay(shockTime); // Wait for all

  // Turn them all off
  for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
    if (pressed[playerNum] == 0) {
      int shockPin = shockPins[playerNum]; 
      digitalWrite (shockPin, LOW);
    }
  }
}

void shockCheater(int playerNum) {
  int shockPin = shockPins[playerNum];

  Serial.print(F("Player "));
  Serial.print(playerNum);
  Serial.println(F(" cheated."));

  noTone(buzzer);
  LEDsPlayerOneColor(playerNum, CRGB::Red);

  digitalWrite (shockPin, HIGH);
  delay(1000); // cheaters get a full second
  digitalWrite (shockPin, LOW);

  playBoom();
  LEDsPlayerFlashOneColor(playerNum, CRGB::Red);

  currentState = FINISHING;
}

void shockPlayer(int playerNum) {
  int shockPin = shockPins[playerNum];

  Serial.print(F("Shocking player "));
  Serial.println(playerNum);

  noTone(buzzer);  
  digitalWrite (shockPin, HIGH);
  playFail();
  LEDsPlayerOneColor(playerNum, CRGB::Red);
  delay(shockTime);
  digitalWrite (shockPin, LOW);
  LEDsPlayerFlashOneColor(playerNum, CRGB::Red);
}
// ============================================================================

// ============================================================================
// LED Stuff
// ============================================================================
void LEDsTest(CRGB color) {
  for(int i = 0; i < MAX_PLAYERS * numberOfLEDsPerPlayer; i++) {
    leds[i] = color;
    FastLED.show();
    delay(50);
  }
}

void LEDsNumberOfPlayers() {
  FastLED.clear();
  for(int player = 0; player < numberOfPlayers; player++) {
    leds[player*numberOfLEDsPerPlayer] = CRGB::Green;
  }
  FastLED.show();
}

void LEDsPlayerPosition(int player, int position) {
  for(int led = 0; led < numberOfLEDsPerPlayer; led++) {
    if(led <= position) {
      leds[player*numberOfLEDsPerPlayer+led] = CRGB::Blue;
    }
    else {
      leds[player*numberOfLEDsPerPlayer+led] = CRGB::Black;
    }
  }
  FastLED.show();
}

void LEDsPlayerOneColor(int player, CRGB color) {
  for(int led = 0; led < numberOfLEDsPerPlayer; led++) {
    leds[player*numberOfLEDsPerPlayer+led] = color;
  }
  FastLED.show();
}

void LEDsPlayerFlashOneColor(int player, CRGB color) {
  for (int x = 0; x < 10; x++) {
    LEDsPlayerOneColor(player, color);
    delay(150);
    
    LEDsPlayerOneColor(player, CRGB::Black);
    delay(150);
  }
  LEDsPlayerOneColor(player, color);
}

void LEDsAllOneColor(CRGB color) {
  for(int player = 0; player < numberOfPlayers; player++) {
    for(int led = 0; led < numberOfLEDsPerPlayer; led++) {
      leds[player*numberOfLEDsPerPlayer+led] = color;
    }
  }
  FastLED.show();
}

void LEDsAllSequential(CRGB color) {
  for(int player = 0; player < numberOfPlayers; player++) {
    if(LEDSequenceNumber < 0) {
      FastLED.clear(true);
    }
    else {
      for(int led = 0; led < LEDSequenceNumber+1; led++) {
        leds[player*numberOfLEDsPerPlayer+led] = color;
      }
    }
  }
  LEDSequenceNumber++;
  if(LEDSequenceNumber > numberOfLEDsPerPlayer-1) {
    LEDSequenceNumber = -1;
  }
  FastLED.show();
  delay(150);
}

void LEDsAllRadial(CRGB color) {
  if(LEDSequenceNumber < 0) {
    LEDSequenceNumber = 0;
  }
  int player = LEDSequenceNumber;
  FastLED.clear(true);
  for(int led = 0; led < numberOfLEDsPerPlayer; led++) {
    leds[player*numberOfLEDsPerPlayer+led] = color;
  }
  LEDSequenceNumber++;
  if(LEDSequenceNumber > numberOfPlayers-1) {
    LEDSequenceNumber = 0;
  }
  FastLED.show();
  delay(150);
}

//  This function loops over each pixel, calculates the adjusted 'clock' that this pixel should use, and calls 
//  "CalculateOneTwinkle" on each pixel.  It then displays either the twinkle color of the background color, whichever is brighter.
void LEDsTwinkle( CRGBSet& L) {
  uint16_t PRNG16 = 11337; 
  uint32_t clock32 = millis();
 
  CRGB bg = gBackgroundColor;
  uint8_t backgroundBrightness = bg.getAverageLight();
  
  for(CRGB& pixel: L) {
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    uint16_t myclockoffset16= PRNG16; // use that number as clock offset
    PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
    // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
    uint8_t myspeedmultiplierQ5_3 =  ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
    uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
    uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel
 
    // We now have the adjusted 'clock' for this pixel, now we call the function that computes what color the pixel should be based
    // on the "brightness = f( time )" idea.
    CRGB c = computeOneTwinkle( myclock30, myunique8);
 
    uint8_t cbright = c.getAverageLight();
    int16_t deltabright = cbright - backgroundBrightness;
    if( deltabright >= 32 || (!bg)) {
      // If the new pixel is significantly brighter than the background color, use the new color.
      pixel = c;
    } else if( deltabright > 0 ) {
      // If the new pixel is just slightly brighter than the background color, mix a blend of the new color and the background color
      pixel = blend( bg, c, deltabright * 8);
    } else { 
      // if the new pixel is not at all brighter than the background color, just use the background color.
      pixel = bg;
    }
  }

  FastLED.show();
}

//  This function takes a time in pseudo-milliseconds, figures out brightness = f( time ), and also hue = f( time )
//  The 'low digits' of the millisecond time are used as input to the brightness wave function.  
//  The 'high digits' are used to select a color, so that the color does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel should light at all during this cycle, based on the TWINKLE_DENSITY.
CRGB computeOneTwinkle( uint32_t ms, uint8_t salt) {
  uint16_t ticks = ms >> (8-TWINKLE_SPEED);
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8( slowcycle16);
  slowcycle16 =  (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  uint8_t bright = 0;
  if( ((slowcycle8 & 0x0E)/2) < TWINKLE_DENSITY) {
    bright = attackDecayWave8( fastcycle8);
  }
 
  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if( bright > 0) {
    c = ColorFromPalette( gCurrentPalette, hue, bright, NOBLEND);
  } else {
    c = CRGB::Black;
  }
  return c;
}
 
uint8_t attackDecayWave8( uint8_t i) {
  if( i < 86) {
    return i * 3;
  } else {
    i -= 86;
    return 255 - (i + (i/2));
  }
}
// ============================================================================

// ============================================================================
// Sound Stuff
// ============================================================================
void playBoom() {
  tone(buzzer, 31, 1500);
}

void playFail() {
  tone(buzzer, 392, 450);
  // delay(450);
  tone(buzzer, 262, 900);
  // delay(900);
  // noTone(buzzer);
}

void playSuccess() {
  const uint8_t NOTE_SUSTAIN = 50; 
  for(uint8_t nLoop = 0; nLoop < 2; nLoop ++) {
    tone(buzzer,NOTE_A5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_B5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_C5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_B5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_C5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_D5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_C5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_D5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_E5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_D5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_E5);
    delay(NOTE_SUSTAIN);
    tone(buzzer,NOTE_E5);
    delay(NOTE_SUSTAIN);
  }
  noTone(buzzer);
}

void playShortBeep() {
  tone(buzzer, 700, 250);
}

void playLongBeep() {
  tone(buzzer, 700, 750);
}
// ============================================================================

// Main program
void setup() {
  Serial.begin (9600);
  randomSeed(analogRead(A1));
  FastLED.addLeds<WS2812, LEDPin, GRB>(leds, NUM_LEDS);
  pinMode(buttonStartPin, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  for(int modePin = 0 ; modePin < sizeof(modePins); modePin++) {
    pinMode(modePins[modePin], INPUT_PULLUP);
  }
  pinMode(buttonNumberOfPlayersPin, INPUT_PULLUP);
  for(int buttonPin = 0 ; buttonPin < sizeof(buttonPins); buttonPin++) {
    pinMode(buttonPins[buttonPin], INPUT_PULLUP);
  }
  for(int shockPin = 0 ; shockPin < sizeof(shockPins); shockPin++) {
    pinMode(shockPins[shockPin], OUTPUT);
  }
  LEDsTest(CRGB::Green);
  FastLED.clear(true);
  Serial.println(F("Setup complete"));
}

void loop() {
  Serial.println (F("Stand-by waiting for start button..."));

  if(settingsChanged()) {
    reset();
  }

  while (digitalRead(buttonStartPin) == HIGH) {
    if(currentMode == RECALL) {
      LEDsTwinkle(leds);
    }
    if(currentMode == REACTION) {
      LEDsAllSequential(CRGB::Green);
    }
    if(currentMode == ROULETTE) {      
      LEDsAllRadial(CRGB::Blue);
    }
    if(settingsChanged()) {
      reset();
    }
  }

  currentState = PLAYING;

  if(currentMode == ROULETTE) {
    Serial.println (F("Starting Russian roulette mode..."));
    LEDsAllOneColor(CRGB::White);
    waitForRouletteButton();
    reset();
  }

  if(currentMode == REACTION) {
    Serial.println (F("Starting reaction game countdown..."));
    FastLED.clear(true);
    for (int i = 20; i < 255; i++) { // GAME START BUZZER SOUND
      tone(buzzer, i * 7);
      // LEDsAllSequential(CRGB::Green);
      delay(15);
    }
    noTone(buzzer);
    FastLED.clear(true);

    delay(random(2000, 10000));  // Wait 2-10 seconds
    Serial.println(F("Random wait over"));

    if(currentState == PLAYING) {
      Serial.println (F("Checking for cheaters..."));
      if(!checkCheater()) {
        Serial.println (F("Game start!"));
        startTime = millis();
        LEDsAllOneColor(CRGB::White);
        tone(buzzer, 3000);
      
        waitForReactions();

        Serial.print(F("Total buttons pressed: "));
        Serial.println(pressedTotal);

        for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
          Serial.print(F("Player "));
          Serial.print(playerNum);
          Serial.print(F(" pressed? - "));
          Serial.println(pressed[playerNum]);
        }
        
        shockingTime();
      }

      delay(5000);
      reset();
    }
  }

  if(currentMode == RECALL) {
    Serial.println (F("Starting recall game..."));
    FastLED.clear(true);
    LEDsAllOneColor(CRGB::Blue);
    playSuccess();
    delay(betweenRoundsDelay);
    FastLED.clear(true);
    while(currentState == PLAYING) {
      Serial.println(F("Another round of recall..."));
      repeatSequence();
      addItemToSequence();
      waitForSequenceRepeat();
      if(currentState == PLAYING) {
        FastLED.clear(true);
        playSuccess();
        delay(betweenRoundsDelay);
      }
    }
  }

  if(currentState == FINISHING) {
    reset();
  }
}
