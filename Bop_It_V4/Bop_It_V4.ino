//declare pins
const int microphonePin = PC0;
const int buttonPin = PC1;
const int joystickPin1 = PC2;
const int joystickPin2 = PC3;

const int sr_dataPin = PD0;
const int sr_clockPin = PD1;
const int sr_latchPin = PD2;
const int buzzer = PD3;
const int LED1 = PD5;
const int LED2 = PD6; 
const int LED3 = PD7;

//declare variables
int newGame;
int score;
int timeLimit;
int count;
bool fail;
int currInstruction;
int restingJoystick1;
int restingJoystick2;

void setup() {
  Serial.begin(9600);
  pinMode(microphonePin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(joystickPin1, INPUT);
  pinMode(joystickPin2, INPUT);
  
  pinMode(buzzer, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(sr_dataPin, OUTPUT);
  pinMode(sr_clockPin, OUTPUT);
  pinMode(sr_latchPin, OUTPUT);
  digitalWrite(sr_latchPin, HIGH);

  //instantiate game
  score = 0;
  newGame = 0;
  fail = false;
  
  //get resting joystick positions
  restingJoystick1 = analogRead(joystickPin1);
  restingJoystick2 = analogRead(joystickPin2);
}

void loop() {  
  
  //check for new game request
  if (readInputButton() == 1) {
    newGame = 1;
    score = 0; //set score to 0 on new game
    displayScore();
  }
  
  //start new game
  while (newGame == 1) {
    //get new command
    delay(50);  //avoid double input read;
    currInstruction = fetchInstruction();
    playInstruction(currInstruction);

    //set time limit
    count = 0;
    if (score < 10) timeLimit = 30;
    else if (score < 25) timeLimit = 20;
    else if (score < 50) timeLimit = 15;
    else if (score < 75) timeLimit = 10;
    else if (score < 90) timeLimit = 5;
    else timeLimit = 1;

    //check for correct input
    for (int i=0; i<timeLimit; i++) {
      if (currInstruction==1 && readInputMicrophone()==1) {
        tone(buzzer,3000);
        delay(50);
        noTone(buzzer);
        score++;
        count = 0;
        break;
      }
      else if (currInstruction==2 && readInputButton()==1) {
        tone(buzzer,3000);
        delay(50);
        noTone(buzzer);
        score++;
        count = 0;
        break;
      }
      else if (currInstruction==3 && readInputJoystick()==1) {
        tone(buzzer,3000);
        delay(50);
        noTone(buzzer);
        score++;
        count = 0;
        break;
      }
      else if (readInputButton()==0 && readInputJoystick()==0 && readInputMicrophone())  //no input
        count++;
      else {  //wrong input
        fail = true;
        break;
      }
      delay(100); //delay 1 second per timeLimit 
    }

    //check for loss
    if (fail || count==timeLimit) {
      tone(buzzer, 500);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      delay(50);
      
      noTone(buzzer);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      delay(50);
      
      tone(buzzer, 500);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      delay(50);
      
      noTone(buzzer);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      delay(50);
      
      tone(buzzer, 500);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      delay(150);
      
      noTone(buzzer);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      delay(50);
      newGame = 0;
    }

    //check for win
    if (score == 99) {
      tone(buzzer, 2000);
      digitalWrite(LED3, HIGH);
      delay(50);
      
      noTone(buzzer);
      digitalWrite(LED3, LOW);
      digitalWrite(LED2, HIGH);
      delay(50);
      
      tone(buzzer, 2000);
      digitalWrite(LED2, LOW);
      digitalWrite(LED1, HIGH);
      delay(50);
      
      noTone(buzzer);
      digitalWrite(LED1, LOW);
      delay(50);
      
      tone(buzzer, 2500);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      delay(150);
      
      noTone(buzzer);
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
      digitalWrite(LED3, LOW);
      delay(50);
      newGame = 0;
    }
    
    //display score to user
    displayScore();
  }
  
}

int fetchInstruction() {
  return random(2,4);
}

void playInstruction(int instruction) {
  //play tone and light LED based on instruction
  if (instruction == 1) {
    digitalWrite(LED1, HIGH);
    tone(buzzer, 2000);
    delay(100);
    noTone(buzzer);
    digitalWrite(LED1, LOW);
  }
  else if (instruction == 2) {
    digitalWrite(LED2, HIGH);
    tone(buzzer, 1000);
    delay(100);
    noTone(buzzer);
    digitalWrite(LED2, LOW);
  }
  else if (instruction == 3) {
    digitalWrite(LED3, HIGH);
    tone(buzzer, 500);
    delay(100);
    noTone(buzzer);
    digitalWrite(LED3, LOW);
  }
}

int readInputMicrophone() {
  return digitalRead(microphonePin);
}

int readInputButton() {
  if (analogRead(buttonPin) > 500)
    return 1;
  return 0;
}

int readInputJoystick() {
  if (abs(analogRead(joystickPin1)-restingJoystick1) > 300 || abs(analogRead(joystickPin2)-restingJoystick2) > 300)
    return 1;
  return 0;
}

void displayScore() {
  if((score > 99)||(score < 0))
  {
    Serial.println("Score is out of bounds.");
  }
  byte tensByte = num2seg(score/10);
  byte onesByte = num2seg(score%10);
  digitalWrite(sr_latchPin, LOW);
  shiftOut(sr_dataPin, sr_clockPin, LSBFIRST, onesByte);
  shiftOut(sr_dataPin, sr_clockPin, LSBFIRST, tensByte);
  digitalWrite(sr_latchPin, HIGH);
}

byte num2seg(int n) {
  byte out;
  switch(n) {
    case 0:
      Serial.println((String)"Displaying "+n+"...");
      out = B00000011;
      break;
    case 1:
      Serial.println((String)"Displaying "+n+"...");
      out = B10011111;
      break;
    case 2:
      Serial.println((String)"Displaying "+n+"...");
      out = B00100101;
      break;
    case 3:
      Serial.println((String)"Displaying "+n+"...");
      out = B00001101;
      break;
    case 4:
      Serial.println((String)"Displaying "+n+"...");
      out = B10011001;
      break;
    case 5:
      Serial.println((String)"Displaying "+n+"...");
      out = B01001001;
      break;
    case 6:
      Serial.println((String)"Displaying "+n+"...");
      out = B01000001;
      break;
    case 7:
      Serial.println((String)"Displaying "+n+"...");
      out = B00011111;
      break;
    case 8:
      Serial.println((String)"Displaying "+n+"...");
      out = B00000001;
      break;
    case 9:
      Serial.println((String)"Displaying "+n+"...");
      out = B00001001;
      break;
    default:
      Serial.println("Error: Out of bounds. Displaying Error...");
      out = B10010011;
      break;
  }
  return B11111111 ^ out;
}
