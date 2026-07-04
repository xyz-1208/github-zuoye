#include <WiFi.h>
#include <WebServer.h>

const char apName[] = "ESP32-Light";
const char apPwd[] = "12345678";
const int ledPin = 2;
int ledBright = 0;
WebServer server(80);

String htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>LED无极调光</title>
</head>
<body style="text-align:center;margin-top:100px;">
<h2>LED无极调光</h2>
<input type="range" min="0" max="255" value="0" id="slider" style="width:80%;height:30px;">
<p>亮度数值：<span id="val">0</span></p>
<script>
let slider = document.getElementById("slider");
let showVal = document.getElementById("val");
slider.oninput = function(){
    let num = this.value;
    showVal.innerText = num;
    fetch("/set?b="+num);
}
</script>
</body>
</html>
)HTML";

void handleRoot(){
  server.send(200,"text/html; charset=utf-8",htmlPage);
}

void setBright(){
  if(server.hasArg("b")){
    ledBright = server.arg("b").toInt();
    ledBright = constrain(ledBright,0,255);
    analogWrite(ledPin,ledBright);
  }
  server.send(200,"text/plain; charset=utf-8","ok");
}

void setup(){
  pinMode(ledPin,OUTPUT);
  analogWrite(ledPin,0);
  Serial.begin(115200);
  WiFi.softAP(apName,apPwd);
  Serial.print("ESP32热点IP：");
  Serial.println(WiFi.softAPIP());
  server.on("/",handleRoot);
  server.on("/set",setBright);
  server.begin();
}

void loop(){
  server.handleClient();
}

