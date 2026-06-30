// ex04_touch_lock.ino  ESP32触摸自锁开关
#define LED_PIN     2
#define TOUCH_PIN   T0   // T0对应GPIO4

bool ledState = false;
bool touchLast = false;
unsigned long debounceDelay = 60;
unsigned long timeLast = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  unsigned long timeNow = millis();
  int touchValue = touchRead(TOUCH_PIN);
  bool touchNow = (touchValue < 35);  // ESP32触摸阈值

  // 边沿检测：只有从无触摸变为触摸瞬间才翻转
  if (timeNow - timeLast > debounceDelay) {
    if (touchNow && !touchLast) {
      ledState = !ledState;
      timeLast = timeNow;
    }
  }
  touchLast = touchNow;

  digitalWrite(LED_PIN, ledState);
}

