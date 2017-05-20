void nextSequence(){
  digitalWrite(13, LOW); 
  digitalWrite(12, LOW); 
  digitalWrite(11, LOW); 
  delay(100);
  digitalWrite(13 ,HIGH); 
  digitalWrite(12, HIGH); 
  digitalWrite(11, HIGH); 
  delay(100);
  digitalWrite(13, LOW); 
  digitalWrite(12, LOW); 
  digitalWrite(11, LOW); 
  
}
void previousSequence(){
  digitalWrite(13, HIGH); 
  digitalWrite(12, HIGH); 
  digitalWrite(11, HIGH); 
  
}
void sendSequencesCont(){
  digitalWrite(13, LOW); 
  digitalWrite(12, LOW); 
  digitalWrite(11, LOW); 
  
}

void buildSequences(){
  
}


// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  // make the LED's pin as output:
  pinMode(13, OUTPUT); 
  pinMode(12, OUTPUT); 
  pinMode(11, OUTPUT); 
  
  digitalWrite(13, LOW); 
  digitalWrite(12, LOW); 
  digitalWrite(11, LOW); 
}

// the loop routine runs over and over again forever:
void loop() {
 
  char inByte = 0;
  
  if (Serial.available()) { 
      inByte = Serial.read();
  }
  
  switch(inByte){
   case '1': nextSequence();break;
   case '2': previousSequence();break;
   case '3': sendSequencesCont();break;
   default:break; 
  }
  
  delay(1000);
}

