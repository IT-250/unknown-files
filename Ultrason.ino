int led1 = 2;
int led2 = 3;
int led3 = 4;
int buzzer = 5;
int trig = 8;
int echo = 9;

void setup() {

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
}

void loop() {

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  float distance = pulseIn(echo, HIGH) * 0.034 / 2;

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  noTone(buzzer);

  if (distance < 10) {

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    tone(buzzer, 1000);  
  }
  else if (distance < 20) {

    digitalWrite(led2, LOW);  
    digitalWrite(led3, LOW);  
    digitalWrite(led1, HIGH);
    delay(1000);
    digitalWrite(led1, LOW);
    delay(500);
  }
  else if (distance < 40) {

    digitalWrite(led1, LOW);  
    digitalWrite(led3, LOW);  
    digitalWrite(led2, HIGH);
    delay(1000);
    digitalWrite(led2, LOW);
    delay(500);
  }
  else if (distance < 50) {

    digitalWrite(led2, LOW);  
    digitalWrite(led1, LOW);  
    digitalWrite(led3, HIGH);
    delay(1000);
    digitalWrite(led3, LOW);
    delay(500);
  }

  delay(500);
}