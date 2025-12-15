#include <math.h>
#include <FastLED.h>

#define NUM_LEDS 20
#define LED_PIN 1
#define TWINKLE_SPEED 6
#define TWINKLE_DENSITY 6

CRGBArray<NUM_LEDS> leds;
CRGBPalette16 gCurrentPalette = *&RainbowColors_p;
CRGB gBackgroundColor = CRGB::Black;

int numberOfPlayers = 5;
const int numberOfLEDSPerPlayer = 4;
const int shockTime = 100; // SHOCK DURATION IN MS
const int maxDelayBetweenResponses = 3000;

const int buttonStart = A0;
const int modePins[3] = {10, 11, 12};
const int numberOfPlayerPins[4] = {A2, A3, A4, A5};
const int buttonPins[4] = {9, 8, 7, 6};
const int shockPins[4] = {5, 4, 3, 2}; //SHOCK PINS ARE CONNECTED TO THE BASE OF A BC547 TRANSISTOR
const int buzzer = A5; 

enum State {
  WAITING,
  PLAYING,
  FINISHING
};
State currentState = WAITING;

enum Mode {
  REACTION,
  TEST,
  MEMORY
};
Mode currentMode = REACTION;

// Reaction mode settings
int pressed[4] = {0, 0, 0, 0};
int pressedTotal = 0;
int startTime;

// Memory mode settings
int sequenceDelay = 1500;
int sequence[1000];
int sequenceLength = 0;

void setup() {
  Serial.begin (9600);
  randomSeed(analogRead(A1));
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  pinMode(buttonStart, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  for(int modePin = 0 ; modePin < sizeof(modePins); modePin++) {
    pinMode(modePins[modePin], INPUT_PULLUP);
  }
  for(int numberOfPlayersPin = 0 ; numberOfPlayersPin < sizeof(numberOfPlayerPins); numberOfPlayersPin++) {
    pinMode(numberOfPlayerPins[numberOfPlayersPin], INPUT_PULLUP);
  }
  for(int buttonPin = 0 ; buttonPin < sizeof(buttonPins); buttonPin++) {
    pinMode(buttonPins[buttonPin], INPUT_PULLUP);
  }
  for(int shockPin = 0 ; shockPin < sizeof(shockPins); shockPin++) {
    pinMode(shockPins[shockPin], OUTPUT);
  }
  setAllPlayerLEDsToOneColor(CRGB::Black);
}

void loop() {
  Serial.println ("Stand-by waiting for start button...");
  if(settingsChanged()) {
    reset();
  }

  while (digitalRead(buttonStart) == HIGH) {
    if(currentMode == MEMORY) {
      drawTwinkles(leds);
    }
    if(currentMode == REACTION) {
      sequentialLEDsForAllPlayers(CRGB::Blue);
    }
  }

  currentState = PLAYING;

  if(currentMode == TEST) {
    waitForTestButton();
    reset();
  }

  if(currentMode == REACTION) {
    Serial.println ("Starting reaction game countdown...");
    FastLED.clear(true);
    for (int i = 20; i < 255; i++) { // GAME START BUZZER SOUND
      tone(buzzer, i * 7);
      //setAllLEDs(i);
      sequentialLEDsForAllPlayers(CRGB::Green);
      delay(15);
    }
    noTone(buzzer);
    FastLED.clear(true);

    delay(random(2000, 10000));  //WAIT RANDOM 2 - 10 SEC

    Serial.println ("Checking for cheaters...");
    checkCheater(); //CHECK IF CHEATER IS HOLDING A BUTTON EARLY AND PUNISH HIM/HER
      if(currentState == PLAYING) {
      Serial.println ("Game start!");
      startTime = millis();
      setAllPlayerLEDsToOneColor(CRGB::White);
      tone(buzzer, 2500);
    
      waitForReactions();

      Serial.print("Total buttons pressed: ");
      Serial.println(pressedTotal);

      for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
        Serial.print("Player ");
        Serial.print(playerNum + 1);
        Serial.print(" pressed? - ");
        Serial.println(pressed[playerNum]);
      }
      
      shockingTime();

      delay(5000);
    }
  }

  if(currentMode == MEMORY) {
    Serial.println ("Starting memory game...");
    playSequence();
    addItemToSequence();
    waitForSequenceRepeat();
  }

  if(currentState == FINISHING) {
    reset();
  }
}

void reset() {
  Serial.println ("Resetting.");
  FastLED.clear(true);
  currentState = WAITING;
  for(int player = 0; player < numberOfPlayers; player++) {
    pressed[player] = 0;
  }
  pressedTotal = 0;
  sequenceLength = 0;
}

bool settingsChanged() {
  bool changed = false;
  for(int pin = 0; pin < sizeof(numberOfPlayerPins); pin++) {
    if(digitalRead(numberOfPlayerPins[pin]) == LOW) {
      if(pin+2 != numberOfPlayers) {
        changed = true;
        numberOfPlayers = pin+2; // knob pin 0 = 2 players
      }
    }
  }
  for(int pin = 0; pin < sizeof(modePins); pin++) {
    if(digitalRead(modePins[pin]) == LOW) {
      if(pin != currentMode) {
        changed = true;
        currentMode = static_cast<Mode>(pin); // 0 = Reaction, 1 = Test, 2 = Memory
      }
    }
  }
  return changed;
}

void waitForTestButton() {
  Serial.println("waiting for a button test press...");
  while (pressedTotal == 0) {
    for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
      if (digitalRead(buttonPins[playerNum]) == LOW) {
        pressedTotal = 1;
        Serial.print("Button ");
        Serial.print(playerNum + 1);
        Serial.println(" was pressed.");
        shockPlayer(playerNum);
      }
    }
  }
}

void playSequenceItem(int player) {
  setPlayerLEDsToOneColor(player, CRGB::White);
  tone(buzzer, 2500, 100);
  delay(sequenceDelay);
}

void playSequence() {
  Serial.print("playing memory sequence of length ");
  Serial.println(sequenceLength);
  FastLED.clear(true);
  for(int seq = 0; seq < sequenceLength; seq++) {
    playSequenceItem(seq);
  }
}

void addItemToSequence() {
  FastLED.clear(true);
  int nextPlayerInSequence = random(numberOfPlayers);
  Serial.print("player ");
  Serial.print(nextPlayerInSequence);
  Serial.println(" is next in the sequence");
  sequence[sequenceLength] = nextPlayerInSequence;
  sequenceLength++;
  playSequenceItem(nextPlayerInSequence);
  sequenceDelay = sequenceDelay - 10;
}

void waitForSequenceRepeat() {
  int sequenceNumber = 0;
  int lastButtonPress = millis();
  Serial.println("waiting for sequence to be repeated...");

  while(currentState == PLAYING && sequenceNumber <= sequenceLength) {
    for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
      if(digitalRead(buttonPins[playerNum]) == LOW) {
        if(playerNum == sequence[sequenceNumber]) {
          playSequenceItem(playerNum);
          sequenceNumber++;
        }
        else {
          Serial.print("player ");
          Serial.print(playerNum);
          Serial.print(" pressed their button when we were expecting player ");
          Serial.println(sequence[sequenceNumber]);
          shockPlayer(playerNum);
          currentState = FINISHING;
        }
      }
    }

    if(millis() - lastButtonPress > maxDelayBetweenResponses) {
      Serial.print("player ");
      Serial.print(sequence[sequenceNumber]);
      Serial.println(" failed to press their button in time ");
      shockPlayer(sequence[sequenceNumber]);
      currentState = FINISHING;
    }

    if(settingsChanged) {
      currentState = FINISHING;
    }
  }
}

void checkCheater() {
  for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
    if (digitalRead(buttonPins[playerNum]) == LOW) {
      shockCheater(playerNum);
    }
  }
}

void waitForReactions() {
  while (currentState == PLAYING  && pressedTotal < numberOfPlayers - 1) {
    for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
      if(digitalRead(buttonPins[playerNum]) == LOW) {
        if(pressed[playerNum] == 0) {
          pressed[playerNum] = 1;
          pressedTotal = pressedTotal + 1;
          setPlayerLEDPosition(playerNum, pressedTotal-1);
          Serial.print("Button ");
          Serial.print(playerNum + 1);
          Serial.print(" was pressed in ");
          Serial.print(millis() - startTime);
          Serial.println(" millseconds");
        }
      }
    }

    if(millis() - startTime > maxDelayBetweenResponses) {
      for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
        if(pressed[playerNum] == 0) {
          Serial.print("player ");
          Serial.print(playerNum);
          Serial.println(" failed to press their button in time ");
        }
      }
      currentState = FINISHING;
    }

    if(settingsChanged) {
      currentState = FINISHING;
    }
  }
}

void shockingTime() {
  for(int playerNum = 0; playerNum < numberOfPlayers; playerNum++) {
    if (pressed[playerNum] == 0) {
      shockPlayer(playerNum);
    }
  }
}

void shockCheater(int playerNum) {
  int shockPin = shockPins[playerNum];

  Serial.print("Player ");
  Serial.print(playerNum + 1);
  Serial.println(" cheated.");

  noTone(buzzer);
  setAllPlayerLEDsToOneColor(CRGB::Red);

  digitalWrite (shockPin, HIGH);
  delay(1000); // cheaters get a full second
  digitalWrite (shockPin, LOW);

  flashPlayerLEDsWithOneColor(playerNum, CRGB::Red);

  currentState = FINISHING;
}

void shockPlayer(int playerNum) {
  int shockPin = shockPins[playerNum];

  Serial.print("Shocking player ");
  Serial.println(playerNum + 1);

  noTone(buzzer);  
  digitalWrite (shockPin, HIGH);
  delay(shockTime);
  digitalWrite (shockPin, LOW);

  flashPlayerLEDsWithOneColor(playerNum, CRGB::Red);
}

void setPlayerNumberLEDs() {
  FastLED.clear();
  for(int player = 0; player < numberOfPlayers; player++) {
    leds[player*numberOfLEDSPerPlayer] = CRGB::Green;
  }
  FastLED.show();
}

void setPlayerLEDPosition(int player, int position) {
  for(int led = 0; led < numberOfLEDSPerPlayer; led++) {
    if(led <= position) {
      leds[player*numberOfLEDSPerPlayer+led] = CRGB::White;
    }
    else {
      leds[player*numberOfLEDSPerPlayer+led] = CRGB::Black;
    }
  }
  FastLED.show();
}

void setPlayerLEDsToOneColor(int player, CRGB color) {
  for(int led = 0; led < numberOfLEDSPerPlayer; led++) {
    Serial.println(player*numberOfLEDSPerPlayer+led);
    leds[player*numberOfLEDSPerPlayer+led] = color;
  }
  FastLED.show();
}

void flashPlayerLEDsWithOneColor(int player, CRGB color) {
  for (int x = 0; x < 10; x++) {
    setPlayerLEDsToOneColor(player, color);
    delay(150);
    
    setPlayerLEDsToOneColor(player, CRGB::Black);
    delay(150);
  }
  setPlayerLEDsToOneColor(player, color);
}

void setAllPlayerLEDsToOneColor(CRGB color) {
  for(int player = 0; player < numberOfPlayers; player++) {
    for(int led = 0; led < numberOfLEDSPerPlayer; led++) {
    Serial.println(player*numberOfLEDSPerPlayer+led);
      leds[player*numberOfLEDSPerPlayer+led] = color;
    }
  }
  FastLED.show();
}

void sequentialLEDsForAllPlayers(CRGB color) {
  int nextLED = 3;
  if(leds[3] != CRGB::Black) {
    nextLED = 0;
  }
  else if(leds[2] != CRGB::Black) {
    nextLED = 3;
  }
  else if(leds[1] != CRGB::Black) {
    nextLED = 2;
  }
  if(nextLED == 0) {
    FastLED.clear(true);
  }
  for(int player = 0; player < numberOfPlayers; player++) {
    for(int led = 0; led < nextLED+1; led++) {
      leds[player*numberOfLEDSPerPlayer+led] = color;
    }
  }
  FastLED.show();
}

//  This function loops over each pixel, calculates the adjusted 'clock' that this pixel should use, and calls 
//  "CalculateOneTwinkle" on each pixel.  It then displays either the twinkle color of the background color, whichever is brighter.
void drawTwinkles( CRGBSet& L) {
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



