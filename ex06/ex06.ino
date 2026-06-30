// ex06_police_light.ino ESP32双通道反相PWM警灯
#define LED_A   2
#define LED_B   15

int brightness = 0;
int step = 2;

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  brightness += step;

  if (brightness >= 255 || brightness <= 0) {
    step = -step;
  }

  analogWrite(LED_A, brightness);
  analogWrite(LED_B, 255 - brightness);  // 反相输出
  delay(12);
}

