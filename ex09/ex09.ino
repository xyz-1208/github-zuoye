#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 2
#define TOUCH_PIN T0
int touchRaw = 0;
WebServer server(80);
const char apName[] = "ESP32-Dashboard";
const char apPwd[] = "12345678";

// 网页面板，AJAX每100ms自动拉取传感器数据
String htmlUI = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>触摸传感器实时仪表盘</title>
<style>
*{margin:0;padding:0;font-family:Arial;}
body{background:#121212;color:#fff;text-align:center;margin-top:120px;}
h2{font-size:28px;margin-bottom:40px;color:#0cf;}
.data-box{font-size:70px;font-weight:bold;color:#ff6633;border:4px solid #ff6633;width:320px;margin:0 auto;padding:40px;border-radius:20px;}
.desc{margin-top:20px;font-size:18px;color:#aaa;}
</style>
</head>
<body>
<h2>Touch 实时采集数值</h2>
<div class="data-box" id="valBox">0</div>
<p class="desc">手靠近引脚 → 数值减小 | 松开手 → 数值回升</p>

<script>
// 定时循环拉取传感器数据
function updateData(){
  fetch("/getTouch")
  .then(res=>res.text())
  .then(data=>{
    document.getElementById("valBox").innerText = data;
  })
}
// 100毫秒刷新一次，实时流畅
setInterval(updateData, 100);
updateData();
</script>
</body>
</html>
)HTML";

// 主页仪表盘
void rootPage(){
  server.send(200, "text/html; charset=utf-8", htmlUI);
}

// 接口：返回当前触摸原始数值
void sendTouchData(){
  touchRaw = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchRaw));
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
  // 创建AP热点
  WiFi.softAP(apName, apPwd);
  Serial.print("仪表盘访问IP：");
  Serial.println(WiFi.softAPIP());
  // 绑定网页路由
  server.on("/", rootPage);
  server.on("/getTouch", sendTouchData);
  server.begin();
}

void loop() {
  server.handleClient();
  touchRaw = touchRead(TOUCH_PIN);
}
