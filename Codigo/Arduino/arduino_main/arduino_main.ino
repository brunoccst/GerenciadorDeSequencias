int currentSequence = 0;
char s[3] = {0, 0, 0};
void sequence1() {
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW); 
  delay(500);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH); 
  delay(500);
  s[0]++;
}

void sequence2() {
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW); 
  delay(1000);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH); 
  delay(1000);
  s[1]++;
}

void sequence3() {
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
  digitalWrite(11, LOW); 
  delay(2000);
  digitalWrite(13, HIGH);
  digitalWrite(12, HIGH);
  digitalWrite(11, HIGH); 
  delay(2000);
  s[2]++;
}

void sendSequencesCont(){
  Serial.write(s);
  
}

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // make the LED's pin as output:
  pinMode(13, OUTPUT); 
  pinMode(12, OUTPUT); 
  pinMode(11, OUTPUT); 
}

// the loop routine runs over and over again forever:
void loop() {
 
  char inByte = 0;
  
  if (Serial.available()) { 
      inByte = Serial.read();
      
      switch(inByte){
       case '1': currentSequence = currentSequence + 1;break;
       case '2': currentSequence = currentSequence - 1;break;
       case '3': sendSequencesCont();break;
       default:break; 
      }
      
      if (currentSequence > 2)
        currentSequence = 0;
      if (currentSequence < 0)
        currentSequence = 2;
      switch(currentSequence)
      {
        case 0: sequence1();break;
        case 1: sequence2();break;
        case 2: sequence3();break;
      }
  }
}
