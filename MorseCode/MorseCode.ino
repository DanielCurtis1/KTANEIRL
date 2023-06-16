#include <LiquidCrystal.h>
#include <elapsedMillis.h>

//The Various Buttons and LEDs aswell as their pin numbers (subject to change)
#define ModuleGreen 2
#define ModuleRed 3
#define TXButton 4
#define MorseLED 5
#define RightButton 6
#define LeftButton 7
//The 4 data pins for the LCD display
#define TXLCD4 8
#define TXLCD5 9
#define TXLCD6 10
#define TXLCD7 11
//Define morse code delays
#define DotDelay 500;

String chosenWord;
String chosenTX;

//if globalTX = randomSelect when TX is pressed then the module is defused successfully, otherwise a red light will flash
int globalTX = 0;
int randomSelect;

//set the buttons to off
int leftButtonState = 0;
int rightButtonState = 0;
int TXButtonState = 0;

//used to detect what state the button is in
int prevTXState = 0;
int prevRightState = 0;
int prevLeftState = 0;

//initialize hz screen (13 = rs, 12 = enable)
LiquidCrystal lcd(13, 12, TXLCD4, TXLCD5, TXLCD6, TXLCD7);

//declare currentTime
elapsedMillis currentTime;

//morse code to display on LED
const String morse[]{
  "... .... . .-.. .-..",
  ".... .- .-.. .-.. ...",
  "... .-.. .. -.-. -.-",
  "- .-. .. -.-. -.-",
  "-... --- -..- . ...",
  ".-.. . .- -.- ...",
  "... - .-. --- -... .",
  "-... .. ... - .-. ---",
  "..-. .-.. .. -.-. -.-",
  "-... --- -- -... ...",
  "-... .-. . .- -.-",
  "-... .-. .. -.-. -.-",
  "... - . .- -.-",
  "... - .. -. --.",
  "...- . -.-. - --- .-.",
  "-... . .- - ...",
};

//hz for correct answer
const String TX[]{
  "3.505 MHz",
  "3.515 MHz",
  "3.522 MHz",
  "3.532 MHz",
  "3.535 MHz",
  "3.542 MHz",
  "3.545 MHz",
  "3.552 MHz",
  "3.555 MHz",
  "3.565 MHz",
  "3.572 MHz",
  "3.575 MHz",
  "3.582 MHz",
  "3.592 MHz",
  "3.595 MHz",
  "3.600 MHz"
};

void setup() {
  Serial.begin(9600);
  //LEDs
  pinMode(ModuleGreen, OUTPUT);
  pinMode(ModuleRed, OUTPUT);
  pinMode(MorseLED, OUTPUT);
  //Buttons
  pinMode(RightButton, INPUT_PULLUP);
  pinMode(LeftButton, INPUT_PULLUP);

  //initialize the lcd screen and print the first hz
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print(TX[globalTX]);

  //choose a random word and its hz to initizalize module
  randomSeed(analogRead(A1));
  randomSelect = random(0, 16);
  chosenWord = morse[randomSelect];
  chosenTX = TX[randomSelect];

  digitalWrite(MorseLED, HIGH);

  elapsedMillis currentTime = 0;
}

void loop() {
  rightButton();
  leftButton();
  tXButton();

  printMorse();
}

void setColor(int redValue, int greenValue) {
  analogWrite(ModuleRed, redValue);
  analogWrite(ModuleGreen, greenValue);
}

void rightButton() {

  rightButtonState = digitalRead(RightButton);
  if (rightButtonState != prevRightState ) {
    if (rightButtonState == LOW && TX[globalTX] != "3.600 MHz") {
      globalTX = globalTX + 1;
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print(TX[globalTX]);
    }
    delay(20);  // debounce
  }
  prevRightState = rightButtonState;
}

void leftButton() {

  leftButtonState = digitalRead(LeftButton);
  if (leftButtonState != prevLeftState && TX[globalTX] != "3.505 MHz") {
    if (leftButtonState == LOW) {
      globalTX = globalTX - 1;
      lcd.clear();
      lcd.setCursor(4,0);
      lcd.print(TX[globalTX]);
    }
    delay(20);  // debounce
  }
  prevLeftState = leftButtonState;
}

void tXButton() {

  TXButtonState = digitalRead(TXButton);
  if (TXButtonState != prevTXState ) {
    if (TXButtonState == LOW && globalTX == randomSelect) {
      digitalWrite(MorseLED, LOW);
      setColor(0, 255);
      while(true){}
    }
    else if (TXButtonState == LOW && globalTX != randomSelect){
      setColor(255, 0);
      delay(250);
      setColor(0, 0);
    }
    delay(20);  // debounce
  }
  prevTXState = TXButtonState;
}

void printMorse()
{
  static int pos;
  static int alt;
  static int delayTime;

  if (delayTime <= currentTime) {
    if (alt == 1) {
      digitalWrite(MorseLED, LOW);
      delayTime = DotDelay;
      alt = 0;

    } else {
      switch (morse[randomSelect][pos]) {
        case '.':
          digitalWrite(MorseLED, HIGH);
          delayTime = DotDelay;
          break;
        case '-':
          digitalWrite(MorseLED, HIGH);
          delayTime = 2 * DotDelay;
          break;
        case ' ':
          digitalWrite(MorseLED, LOW);
          delayTime = DotDelay;
          break;
        case '\0':
          digitalWrite(MorseLED, LOW);
          delayTime = 2 * DotDelay;
          pos = -1;
          break;
      }

      alt = 1;
      pos += 1;
      
    }
    currentTime = 0;
  }
}
