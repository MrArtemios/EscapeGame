int y = 0;
String reponse = "";

const int GREEN = 3;
const int BLUE = 4;
const int YELLOW = 5;
const int RED = 6;
const int GREEN_BTN = 7;
const int BLUE_BTN = 8;
const int YELLOW_BTN = 9;
const int RED_BTN = 10;
const int BUZZER = 2;
const int RESET = 13;

const char* codes[] = {"4,5,3", "4,5,3,6,5,4,5", "4,5,3,6,5,4,5,6,3,4,6", "4,5,3,6,5,4,5,6,3,4,6,4,6,3,6,4", "4,5,3,6,5,4,5,6,3,4,6,4,6,3,6,4,5,3,6,5"};
struct Note {
  int frequency;
  char code;
};
Note notes[] = {
  {300, '3'},
  {350, '4'},
  {400, '5'},
  {450, '6'}
};

unsigned long startTime;

void setup() {
  Serial.begin(9600);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN_BTN, INPUT_PULLUP);
  pinMode(BLUE_BTN, INPUT_PULLUP);
  pinMode(YELLOW_BTN, INPUT_PULLUP);
  pinMode(RED_BTN, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(RESET, INPUT_PULLUP);
}


void play_note(char code, int del) {
  for (int i = 0; i<4; i++) {
    if (notes[i].code == code) {
      tone(BUZZER, notes[i].frequency, del);
      delay(del*2);
      noTone(BUZZER);
      break;
    }
  }
}


void boutons() {
  if (digitalRead(GREEN_BTN) == LOW) {
    reponse += 3;
    digitalWrite(GREEN, HIGH);
    play_note('3', 50);
    digitalWrite(GREEN, LOW);
    Serial.println(reponse);
    startTime = millis();
    delay(500);
  }

  else if (digitalRead(BLUE_BTN) == LOW) {
    reponse += 4;
    digitalWrite(BLUE, HIGH);
    play_note('4', 50);
    digitalWrite(BLUE, LOW);
    Serial.println(reponse);
    startTime = millis();
    delay(500);
  }
    
  else if (digitalRead(YELLOW_BTN) == LOW) {
    reponse += 5;
    digitalWrite(YELLOW, HIGH);
    play_note('5', 50);
    digitalWrite(YELLOW, LOW);
    Serial.println(reponse);
    startTime = millis();
    delay(500);
  }
    
  else if (digitalRead(RED_BTN) == LOW) {
    reponse += 6;
    digitalWrite(RED, HIGH);
    play_note('6', 50);
    digitalWrite(RED, LOW);
    Serial.println(reponse);
    startTime = millis();
    delay(500);
  }
}


void animationSuccess() {
  digitalWrite(GREEN, HIGH); 
  play_note('3', 40); 
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, HIGH); 
  play_note('4', 40);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, HIGH);
  play_note('5', 40);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, HIGH); 
  play_note('6', 40);
  digitalWrite(RED, LOW);
  delay(100);

  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(YELLOW, HIGH);
  digitalWrite(RED, HIGH);
  tone(BUZZER, 500, 100);
  delay(200);
  noTone(BUZZER);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, LOW);
}


void doCode(char* code_source) {

  char* code = strdup(code_source);

  String codeTest(code);
  codeTest.replace(",","");

  char* codeArr = strtok(code, ",");

  while (codeArr != NULL) {
    for (int i = 0; i<4; i++) {
      if (notes[i].code == *codeArr) {
        tone(BUZZER, notes[i].frequency, 50);
        digitalWrite(atoi(codeArr), HIGH);
        delay(500);
        digitalWrite(atoi(codeArr), LOW);
        break;
      }
    }
    codeArr = strtok(NULL, ",");
  }

  free(code);
  reponse = "";
  startTime = millis();

  while (reponse.length() != codeTest.length()) {
    boutons();
    if(millis() - startTime >= 10000) {
      break;
    }
  }

  if (reponse == codeTest) {
    animationSuccess();
  }

  else if (reponse != codeTest){
    digitalWrite(RED, HIGH);
    tone(BUZZER, 200, 250);
    delay(500);
    noTone(BUZZER);
    digitalWrite(RED, LOW);
    reponse = "";
    y = 0;
  }
}


void win() {
  delay(1000);
  for (int i = 0; i<4; i++) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, HIGH);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, HIGH);
    tone(BUZZER, 500, 250);
    delay(500);
    noTone(BUZZER);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
    delay(500);
  }
}


void loop() {
  if ((digitalRead(RESET) == LOW) && (y == 0)) {
    Serial.println("OKAY !");
    jeu();
  }
}



void jeu() {
  // Initialisation du jeu
  if (y == 0) {
    for (int i=0; i<6; i++) {
      digitalWrite(GREEN, HIGH); 
      delay(75);
      digitalWrite(GREEN, LOW);
      play_note('3', 40); 
      digitalWrite(BLUE, HIGH); 
      delay(75);
      digitalWrite(BLUE, LOW);
      play_note('4', 40);
      digitalWrite(YELLOW, HIGH);
      delay(75);
      digitalWrite(YELLOW, LOW);
      play_note('5', 40);
      digitalWrite(RED, HIGH); 
      delay(75);
      digitalWrite(RED, LOW);
      play_note('6', 40);
    }
    y = 1;

    for (int cds = 0; cds < 5; cds++) {
      delay(1000);
      Serial.println(codes[cds]);
      doCode(codes[cds]);
      if (y == 0) {
        break;
      }
    }
    if (y != 0) {
      y = 0;
      win();
    }
  }
}
