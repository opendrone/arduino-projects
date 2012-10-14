#include <LiquidCrystal.h>

// select the pins used on the LCD panel
// LiquidCrystal(rs, enable, d4, d5, d6, d7) 
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
 
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;

// Buttons
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Custom chars
#define PLAYER_CHAR  0
#define COIN_CHAR    1
#define MONSTER_RIGHT_CHAR 2
#define MONSTER_LEFT_CHAR 3
#define MONSTER_CLOSED_CHAR 4

// Possible directions
#define DIR_RIGHT 0
#define DIR_LEFT  1

// Other useful variables
String whiteLine = "                ";
int playerPos[2] = {0,0}; // {x,y}
int coinPos[2] = {0,0}; // {x,y}
int monsters[2][3] = {{0,0,DIR_RIGHT},{15,1,DIR_LEFT}}; // {x,y,dir}
int score = 0;
boolean walls = true; // used if you want an infinite field (loop from one side to the other)
long loopNumber=0;
boolean playerOnMonster = false; // this is used to know if the player has already lost a point because he got eaten or not (3 loops for one position for monsters)

// returns true only if the player is on the coin
boolean playerIsOnCoin() {
 return playerPos[0] == coinPos[0] && playerPos[1] == coinPos[1]; 
}

// returns true only if the player is on the same spot as a monster
boolean playerMeetMonster() {
  return (playerPos[0] == monsters[0][0] && playerPos[1] == monsters[0][1]) || (playerPos[0] == monsters[1][0] && playerPos[1] == monsters[1][1]);
}

// process the player's move (mainly scoring)
void processPos() {
  if (playerIsOnCoin()) {
    score++;
    generateNewCoin();
  }
  if (playerMeetMonster() && !playerOnMonster) {
    score--;
    playerOnMonster = true;
  }
  if(!playerMeetMonster()) {
    playerOnMonster = false;
  }
}

// updates the monsters position and look
void moveMonsters() {
  if(loopNumber % 3 == 0) {
    for(int i = 0; i<2; i++) {
      int* monsterPos = monsters[i];
      if(monsterPos[0] == 15) {
        monsterPos[2] = DIR_LEFT;
        monsterPos[0] = 14;
      } else if(monsterPos[0] == 0) {
        monsterPos[2] = DIR_RIGHT;
        monsterPos[0] = 1;
      } else if(monsterPos[2] == DIR_RIGHT) {
        monsterPos[0] += 1;
      } else {
        monsterPos[0] -= 1;
      }
    }
  }
}

// Change the position of the coin to a new random one
void generateNewCoin() {
  while(playerIsOnCoin()) {
    coinPos[0] = random(0,16);
    coinPos[1] = random(0,2);
  }
}


// draw the field
void drawField() {
  showLines(whiteLine,whiteLine);
  lcd.setCursor(coinPos[0],coinPos[1]);
  lcd.write((uint8_t)COIN_CHAR);
  lcd.setCursor(playerPos[0],playerPos[1]);
  lcd.write((uint8_t)PLAYER_CHAR);
  for(int i = 0; i<2; i++) {
    lcd.setCursor(monsters[i][0],monsters[i][1]);
    if(loopNumber % 3 == 1) {
      lcd.write((uint8_t)MONSTER_CLOSED_CHAR);
    } else if(monsters[i][2] == DIR_RIGHT) {
      lcd.write((uint8_t)MONSTER_RIGHT_CHAR);
    } else {
      lcd.write((uint8_t)MONSTER_LEFT_CHAR);
    }
  }
}

// read the buttons
// this method was copied from http://www.dfrobot.com/wiki/index.php?title=Arduino_LCD_KeyPad_Shield_(SKU:_DFR0009)#Example_use_of_LiquidCrystal_library
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
 return btnNONE;  // when all others fail, return this...
}

// a loop to display characters on the screen. can be used for animation
void niceDisplayLoop1(char c1,char c2,boolean indirect) {
    int val;
  for( int i=0; i<8; i++) {
    char o = i % 2 == 0 ? c1 : c2;
    char p = i % 2 == 0 ? c2 : c1;
    val = indirect ? i : 7-i;
    lcd.setCursor(val,1);
    lcd.print(o);
    val = indirect ? 15-i : 8+i;
    lcd.setCursor(val,1);
    lcd.print(p);
    val = indirect ? 7-i : i;
    lcd.setCursor(val,0);
    lcd.print(p);
    val = indirect ? 8+i :15-i ;
    lcd.setCursor(val,0);
    lcd.print(o);
    delay(100);
  }
}

// helper to show 2 lines in a nice looking animation
void crossAppear(String line1, String line2,boolean stay) {
  int max = stay? 16 : 32;
  char charToPrint;
  for( int i=0; i<16; i++) {
    for( int k=0; k<=i; k++) {
      lcd.setCursor(k,0);
      charToPrint = 15+k-i < line1.length() ? line1[15-i+k] : ' ';
      lcd.print(charToPrint);
      lcd.setCursor(15-k,1);
      charToPrint = i-k < line2.length() ? line2[i-k] : ' ';
      lcd.print(charToPrint);
    }
    delay(80);
  }
  if(!stay) {
    delay(50);
    blinkDisplay(line1,line2,4,100);
    delay(200);
    for( int i=0; i<16; i++) {
      for( int k=i; k<16; k++) {
        lcd.setCursor(k,0);
        charToPrint = k-i-1 < line1.length() ? line1[k-i-1]:' ';
        lcd.print(charToPrint);
        lcd.setCursor(15-k,1);
        charToPrint = i-k+16 < line2.length() ? line2[i-k+16]:' ';
        lcd.print(charToPrint);
      }
      delay(80);
    }
  }
}

// helper to display 2 String
void showLines(String line1, String line2) {
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
}

// helper to have some text displayed and blinking
void blinkDisplay(String line1, String line2, int times, int wait) {
  for(int i=0;i<times;i++) {
    showLines(line1,line2);
    delay(wait);
    showLines(whiteLine,whiteLine);
    delay(wait);
  }
  showLines(line1,line2);
}

// tihs function is called in Setup. it will create custom chars
void initLcd() { 
  byte player[8] = {
    B01110,
    B00100,
    B01110,
    B10101,
    B00100,
    B01010,
    B01010,
  };
  lcd.createChar(PLAYER_CHAR,player);
  byte monster_left[8] = {
    B01100,
    B01110,
    B00111,
    B00011,
    B00111,
    B01110,
    B01100,
  };
  lcd.createChar(MONSTER_LEFT_CHAR,monster_left);
  byte monster_right[8] = {
    B00110,
    B01110,
    B11100,
    B11000,
    B11100,
    B01110,
    B00110,
  };
  lcd.createChar(MONSTER_RIGHT_CHAR,monster_right);
  byte monster_closed[8] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B11111,
    B00000,
    B00000,
  };
  lcd.createChar(MONSTER_CLOSED_CHAR,monster_closed);
  byte coin[8] = {
    B00100,
    B01010,
    B10001,
    B10101,
    B10001,
    B01010,
    B00100,
  };
  lcd.createChar(COIN_CHAR,coin);
  lcd.begin(16, 2);              // start the library
}

// sequence displayed at start (called in setup())
void displayStartSequence() {
  //niceDisplayLoop1(' ',' ',false);
  //niceDisplayLoop1('*','-',true);
  //niceDisplayLoop1('*','-',false);
  crossAppear("  Catch coins   ","  oO0Oo..oO0Oo  ",false);
  crossAppear("   but don't    ","   get caught   ",false);
}
 
// function to display scores. It will "pause" the game and show the player's score
void showScores() {
  crossAppear("  Your score is ",String(score),false);
}
 
// Keys processing. the hardware allows only one key press at a time
void selectKeyPressed() {
  showScores();
}
void leftKeyPressed() {
  if(playerPos[0] == 0) {
    playerPos[0] = walls ? 0 : 15;
  } else {
    playerPos[0] -= 1;
  }
}
void rightKeyPressed() {
  if(playerPos[0] == 15) {
    playerPos[0] = walls ? 15 : 0;
  } else {
    playerPos[0] += 1;
  }
}
void upKeyPressed() {
  if(playerPos[1] == 0) {
    playerPos[1] = walls ? 0 : 1;
  } else {
    playerPos[1] -= 1;
  }
}
void downKeyPressed() {
  if(playerPos[1] == 1) {
    playerPos[1] = walls ? 1 : 0;
  } else {
    playerPos[1] = 1;
  }
}
void noKeyPressed() {}

// setup 
void setup()
{
  // defining the baud rate for debug
  Serial.begin(9600);
  // seeding the random function (for coin position)
  randomSeed(analogRead(2));
  initLcd();
  displayStartSequence();
  generateNewCoin();
  drawField();
}
 
// main loop 
void loop()
{
  loopNumber++;
  long time = millis();
  moveMonsters();
 
  int lcd_key = read_LCD_buttons();
  switch (lcd_key)
  {
    case btnRIGHT:
    {
      rightKeyPressed();
      break;
    }
    case btnLEFT:
    {
      leftKeyPressed();
      break;
    }
    case btnUP:
    {
      upKeyPressed();
      break;
    }
    case btnDOWN:
    {
      downKeyPressed();
      break;
    }
    case btnSELECT:
    {
      selectKeyPressed();
      break;
    }
    case btnNONE:
    {
      noKeyPressed();
      break;
    }
  }
  processPos();
  drawField();
  if(millis()-time < 100) {
    delay(100-millis()+time);
  }
}
