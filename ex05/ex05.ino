// ex05_touch_breath.ino ESP32多档位触摸呼吸灯
#define LED_PIN     2
#define TOUCH_PIN   T0

int pwmVal = 0;
int direction = 1;
int speedLevel = 1;  // 1慢速 2中速 3快速
int stepSize;

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
  bool touchNow = (touchValue < 35);

  // 触摸切换档位
  if (timeNow - timeLast > debounceDelay) {
    if (touchNow && !touchLast) {
      speedLevel++;
      if (speedLevel > 3) speedLevel = 1;
      timeLast = timeNow;
    }
  }
  touchLast = touchNow;

  // 不同档位设置不同变化步长
  switch (speedLevel) {
    case 1: stepSize = 1; break;
    case 2: stepSize = 4; break;
    case 3: stepSize = 8; break;
  }

  pwmVal += direction * stepSize;
  if (pwmVal >= 255 || pwmVal <= 0) {
    direction = -direction;
  }

  analogWrite(LED_PIN, pwmVal);
  delay(10);
}
