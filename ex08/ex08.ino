#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 2
#define TOUCH_PIN T0 // D4对应触摸通道T0（GPIO4）

bool systemArm = false;
bool alarmLock = false;
int touchThreshold = 40;

WebServer server(80);
const char apName[] = "ESP32-Alarm";
const char apPwd[] = "12345678";

String htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>安防报警主机</title>
<style>
body{text-align:center;margin-top:80px;font-size:22px;}
button{font-size:20px;padding:15px 40px;margin:10px;cursor:pointer;}
#state{font-size:26px;color:#d00;}
</style>
</head>
<body>
<h2>ESP32 安防报警系统</h2>
<div id="state">当前状态：撤防未监控</div>
<br>
<button onclick="armSys()">布防(Arm)</button>
<button onclick="disarmSys()">撤防(Disarm)</button>
<script>
function armSys(){fetch("/arm");location.reload();}
function disarmSys(){fetch("/disarm");location.reload();}
</script>
</body>
</html>
)HTML";

void handleRoot(){
  String page = htmlPage;
  String statusText;
  if(systemArm && alarmLock){
    statusText = "当前状态：布防，入侵报警锁定！";
  }else if(systemArm){
    statusText = "当前状态：布防监控中";
  }else{
    statusText = "当前状态：撤防未监控";
  }
  page.replace("当前状态：撤防未监控", statusText);
  server.send(200, "text/html; charset=utf-8", page);
}

void handleArm(){
  systemArm = true;
  server.send(200, "text/plain; charset=utf-8", "已布防");
}

void handleDisarm(){
  systemArm = false;
  alarmLock = false;
  digitalWrite(LED_PIN, LOW);
  server.send(200, "text/plain; charset=utf-8", "已撤防，报警复位");
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  WiFi.softAP(apName, apPwd);
  Serial.print("安防主机IP地址：");
  Serial.println(WiFi.softAPIP());
  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();
  uint16_t touchVal = touchRead(TOUCH_PIN);
  if(systemArm){
    if(touchVal < touchThreshold && !alarmLock){
      alarmLock = true;
    }
  }
  if(alarmLock){
    digitalWrite(LED_PIN, HIGH);
    delay(80);
    digitalWrite(LED_PIN, LOW);
    delay(80);
  }else{
    digitalWrite(LED_PIN, LOW);
  }
}
