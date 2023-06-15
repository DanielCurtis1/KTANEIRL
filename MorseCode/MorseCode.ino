
//The Various Buttons and LEDs aswell as their pin numbers (subject to change)
#define ModuleStateLED 3
#define MorseLED 4
#define RightButton 5
#define LeftButton 6
#define MorseLCD 7

String chosenWord;
String chosenTX;

int leftButtonState = 0;
int rightButtonState = 0;

void setup() {
  //LEDs
  pinMode(ModuleStateLED, OUTPUT);
  pinMode(MorseLED, OUTPUT);
  //Buttons
  pinMode(RightButton, INPUT);
  pinMode(LeftButton, INPUT);

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
  const double TX[]{
    3.505,
    3.515,
    3.522,
    3.532,
    3.535,
    3.542,
    3.545,
    3.552,
    3.555,
    3.565,
    3.572,
    3.575,
    3.582,
    3.592,
    3.595,
    3.600
  };

  //choose a random word and its hz to initizalize module
  int randomSelect = random(0, 15);
  chosenWord = morse[randomSelect];
  chosenTX = TX[randomSelect];
}

void loop() {
  
  //Flash LED
  for(int i = 0; i <= chosenWord.length(); i++){
    if (chosenWord.charAt(i) == "."){

      digitalWrite(MorseLED, HIGH);
      delay(500);
      digitalWrite(MorseLED, LOW);
      delay(500);

    }
    else if (chosenWord.charAt(i) == "-"){
      
      digitalWrite(MorseLED, HIGH);
      delay(1000);
      digitalWrite(MorseLED, LOW);
      delay(500);

    }
    else{
      delay(1000);
    }
  }
}
