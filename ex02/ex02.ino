unsigned long preTime = 0;
const unsigned long interval = 500;
const int ledPin = 2;
void setup() {pinMode(ledPin,OUTPUT);}
void loop() {
  unsigned long curTime = millis();
  if(curTime - preTime >= interval){
    digitalWrite(ledPin,!digitalRead(ledPin));
    preTime = curTime;
  }
}

