#include <LiquidCrystal.h>
#include <elapsedMillis.h>
#include "CountDown.h"

//The Various Buttons and LEDs aswell as their pin numbers (subject to change)
#define YeButton 2
#define NoButton 3
//The 4 data pins for the LCD display
#define TXLCD4 8
#define TXLCD5 9
#define TXLCD6 10
#define TXLCD7 11
//digit display
#define DLCD4 A0
#define DLCD5 A1
#define DLCD6 A2
#define DLCD7 A3

//used to determine prompt
int randomSelect;

CountDown CD(CountDown::SECONDS);

//set the buttons to off
int nButtonState = 0;
int yButtonState = 0;

//used to detect what state the button is in
int prevyState = 0;
int prevnState = 0;

//initialize hz screen (13 = rs, 12 = enable)
LiquidCrystal lcd(13, 12, TXLCD4, TXLCD5, TXLCD6, TXLCD7);
LiquidCrystal lcd2(A5, A4, DLCD4, DLCD5, DLCD6, DLCD7);

//declare currentTime and randomizerTime
elapsedMillis currentTime;
elapsedMillis TimerTime;
//used to determine whether to press y or n
bool IsPrompt;
bool IsVent;

//Time between prompts
//the huge prompt delay makes it so you can't start another prompt while one is active
int PromptDelay = 50000;
int TimerDelay = 0;

void setup() {

  Serial.begin(9600);
  //Buttons
  pinMode(YeButton, INPUT_PULLUP);
  pinMode(NoButton, INPUT_PULLUP);

  //initialize the lcd screens
  lcd.begin(16,2);
  lcd.clear();

  lcd2.begin(16,2);
  lcd.clear();

  IsPrompt = false;
  IsVent = false;

  currentTime = 0;
  TimerTime = 0;

  prompt();
}

void loop() {
  //Button Input
  YButton();
  NButton();
  
  if (TimerTime >= TimerDelay) {
    TimerTime = 0;
    TimerDelay = 500;
    if (CD.isRunning() && CD.remaining() <= 1){
      IsPrompt = false;
      IsVent = false;
      lcd.clear();
      lcd2.clear();

      CD.stop();

      currentTime = 0;
      PromptDelay = 30000;
      Serial.println("strike."); //debug line
      //strike CAN Bus code goes here
    } else if (CD.isRunning() && CD.remaining() >= 1) {
      lcd2.clear();
      if (int(CD.remaining()) - 1 >= 10) {
        lcd2.setCursor(7,0);
        lcd2.print(int(CD.remaining()) - 1);
      } else {
        lcd2.setCursor(8,0);
        lcd2.print(int(CD.remaining()) - 1);
      }
    }
  }
  //Run prompt
  if (currentTime >= PromptDelay) {
    //the huge prompt delay makes it so you can't start another prompt while one is active
    currentTime = 0;
    PromptDelay = 50000;
    prompt();
  }

}

void YButton() {

  yButtonState = digitalRead(YeButton);
  if (yButtonState != prevyState ) {
    if (yButtonState == LOW) {
      if (IsPrompt == true) {
        if (IsVent == true) {
          ///////
          lcd.setCursor(5,1);
          lcd.print("YES");
          ///////
          IsPrompt = false;
          delay(2000);
          ///////
          lcd.clear();
          lcd.setCursor(4,0);
          lcd.print("VENTING");
          lcd.setCursor(3,1);
          lcd.print("COMPLETE");
          ///////
          currentTime = 0;
          PromptDelay = 30000;
          CD.stop();
          lcd2.clear();
        } else {
          ///////
          lcd.setCursor(5,1);
          lcd.print("YES");
          delay(2000);
          lcd.clear();
          lcd.setCursor(2,0);
          lcd.print("GAME OVER...");                 //ADD CAN BUS SYSTEM HERE
          ///////
          CD.stop();
          lcd2.clear();
          while (true) {}
        }
      }
    }
    delay(20);  // debounce
  }
  prevyState = yButtonState;
}

void NButton() {

  nButtonState = digitalRead(NoButton);
  if (nButtonState != prevnState ) {
    if (nButtonState == LOW) {
      if (IsPrompt == true) {
        if (IsVent == true) {
          ///////
          lcd.setCursor(5,1);
          lcd.print("NO");
          delay(500);
          ///////
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("VENTING PREVENTS");
          lcd.setCursor(3,1);
          lcd.print("EXPLOSIONS");
          delay(500);
          lcd.clear();
          delay(500);
          lcd.setCursor(0,0);
          lcd.print("VENTING PREVENTS");
          lcd.setCursor(3,1);
          lcd.print("EXPLOSIONS");
          delay(500);
          lcd.clear();
          delay(500);
          lcd.setCursor(0,0);
          lcd.print("VENTING PREVENTS");
          lcd.setCursor(3,1);
          lcd.print("EXPLOSIONS");
          delay(2000);
          lcd.clear();
          ///////
          lcd.setCursor(4,0);
          lcd.print("Vent Gas?");
          ///////
        } 
        else {
          ///////
          lcd.setCursor(5,1);
          lcd.print("NO");
          delay(2000);
          lcd.clear();
          ///////
          currentTime = 0;
          PromptDelay = 30000;
          CD.stop();
          lcd2.clear();
        }
      }
    }
    delay(20);  // debounce
  }
  prevnState = nButtonState;
}

void prompt() {
  IsPrompt = true;
  randomSeed(analogRead(A1));
  randomSelect = random(0, 2);
  if (randomSelect == 0) {
    IsVent = true;
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("Vent Gas?");
    CD.start(46);
  } else {
    IsVent = false;
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Detonate?");
    CD.start(46);
  }
}