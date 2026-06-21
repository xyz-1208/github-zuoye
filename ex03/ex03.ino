unsigned long preT = 0;
const int led=2;
const unsigned short shortOn=200, longOn=600, gap=150, endGap=1500;
int sosSeq[] = {0,0,0,1,1,1,0,0,0};
int seqIndex = 0;
bool ledState = false;
void setup(){pinMode(led,OUTPUT);}
void loop(){
  unsigned long now = millis();
  if(now - preT >= (ledState?(sosSeq[seqIndex]?longOn:shortOn):gap)){
    preT = now;
    ledState = !ledState;
    digitalWrite(led,ledState);
    if(!ledState){
      seqIndex++;
      if(seqIndex >= sizeof(sosSeq)/sizeof(int)){
        delay(endGap);
        seqIndex = 0;
      }
    }
  }
}
