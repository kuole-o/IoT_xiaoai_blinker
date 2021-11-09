/* *****************************************************************
   Download latest Blinker library here:
   https://github.com/blinker-iot/blinker-library/archive/master.zip

   Blinker is a cross-hardware, cross-platform solution for the IoT.
   It provides APP, device and server support,
   and uses public cloud services for data transmission and storage.
   It can be used in smart home, data monitoring and other fields
   to help users build Internet of Things projects better and faster.

   Make sure installed 2.7.4 or later ESP8266/Arduino package,
   if use ESP8266 with Blinker.
   https://github.com/esp8266/Arduino/releases

   Make sure installed 1.0.4 or later ESP32/Arduino package,
   if use ESP32 with Blinker.
   https://github.com/espressif/arduino-esp32/releases

   Docs: https://diandeng.tech/doc
 * *****************************************************************
   Blinker 库下载地址:
   https://github.com/blinker-iot/blinker-library/archive/master.zip

   Blinker 是一套跨硬件、跨平台的物联网解决方案，提供APP端、设备端、
   服务器端支持，使用公有云服务进行数据传输存储。可用于智能家居、
   数据监测等领域，可以帮助用户更好更快地搭建物联网项目。

   如果使用 ESP8266 接入 Blinker,
   请确保安装了 2.7.4 或更新的 ESP8266/Arduino 支持包。
   https://github.com/esp8266/Arduino/releases

   如果使用 ESP32 接入 Blinker,
   请确保安装了 1.0.4 或更新的 ESP32/Arduino 支持包。
   https://github.com/espressif/arduino-esp32/releases

   文档: https://diandeng.tech/doc
 * *****************************************************************/

#define BLINKER_WIFI
#define BLINKER_MIOT_MULTI_OUTLET    //设置为小爱多个插座的模式

#include <Blinker.h>

char i;
char auth[] = "******";
char ssid[] = "******";
char pswd[] = "******";

bool oState[5] = { false };

// 新建组件对象，要和APP组件’数据键名’一致
BlinkerButton Button1("btn_a");
BlinkerButton Button2("btn_b");
BlinkerButton Button3("btn_c");
BlinkerButton Button4("btn_d");
BlinkerButton Button5("btn_all");

// 按下Blinker APP按键1即会执行该函数
void button1_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  digitalWrite(13, !digitalRead(13));
  if (state == "on") {
    digitalWrite(13, LOW);
    Button1.print("on");
    Button1.text("摇头：开");
  }
  else if (state == "off") {
    digitalWrite(13, HIGH);
    Button1.print("off");
    Button1.text("摇头：关");
  }
}

// 按下Blinker APP按键2即会执行该函数
void button2_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  digitalWrite(12, !digitalRead(12));
  if (state == "on") {
    digitalWrite(27, LOW);
    digitalWrite(12, HIGH);
    Button3.print("off");
    Button3.text("高温/强风");
    Button2.print("on");
    Button2.text("低温/弱风");
  }
  else if (state == "off") {
    digitalWrite(12, LOW);
    Button2.print("off");
    Button2.text("低温/弱风");
  }
}

// 按下Blinker APP按键3即会执行该函数
void button3_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  digitalWrite(27, !digitalRead(27));
  if (state == "on") {
    digitalWrite(12, LOW);
    digitalWrite(27, HIGH);
    Button2.print("off");
    Button2.text("低温/弱风");
    Button3.print("on");
    Button3.text("高温/强风");
  }
  else if (state == "off") {
    digitalWrite(27, LOW);
    Button3.print("off");
    Button3.text("高温/强风");
  }
}

// 按下Blinker APP按键4即会执行该函数
void button4_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  digitalWrite(25, !digitalRead(25));
  if (state == "on") {
    digitalWrite(25, HIGH);
    Button4.print("on");
    Button4.text("未定义：开");
  }
  else if (state == "off") {
    digitalWrite(25, LOW);
    Button4.print("off");
    Button4.text("未定义：关");
  }
}

// 按下Blinker APP按键5即会执行该函数
void button5_callback(const String & state)
{
  BLINKER_LOG("get button state: ", state);
  digitalWrite(13, !digitalRead(13));
  digitalWrite(12, !digitalRead(12));
  if (state == "on") {
    digitalWrite(13, HIGH);
    digitalWrite(12, HIGH);
    Button1.print("on");
    Button2.print("on");
    Button1.text("摇头：开");
    Button2.text("低温/弱风");
  }
  else if (state == "off") {
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    digitalWrite(27, LOW);
    Button1.print("off");
    Button2.print("off");
    Button3.print("off");
    Button1.text("摇头：关");
    Button2.text("低温/弱风");
    Button3.text("高温/强风");
  }
}

// 13-摇头/不摇头；12-低温/弱风；27-高温/强风
void miotPowerState(const String & state, uint8_t num)
{
  BLINKER_LOG("need set outlet: ", num, ", power state: ", state);
  i = 0; //初始化

  if (num == 1) {
    i = 13;
  }
  else if (num == 2) {
    i = 12;
  }
  else if (num == 3) {
    i = 27;
  }
  else if (num == 4) {
    i = 25;
  }

  if (state == BLINKER_CMD_ON) {
    switch (i) {
      case 12:
        digitalWrite(27, LOW);
        digitalWrite(i, !digitalRead(27));
        BlinkerMIOT.powerState("on", num);
        BlinkerMIOT.print();
        oState[num] = true;
        break;
      case 27:
        digitalWrite(12, LOW);
        digitalWrite(i, !digitalRead(12));
        BlinkerMIOT.powerState("on", num);
        BlinkerMIOT.print();
        oState[num] = true;
        break;
      default:
        digitalWrite(i, HIGH);
        BlinkerMIOT.powerState("on", num);
        BlinkerMIOT.print();
        oState[num] = true;
    }
  }
  else if (state == BLINKER_CMD_OFF) {
    switch (i) {
      case 12:
        digitalWrite(27, LOW);
        digitalWrite(i, LOW);
        BlinkerMIOT.powerState("off", num);
        BlinkerMIOT.print();
        oState[num] = false;
        break;
      case 27:
        digitalWrite(12, LOW);
        digitalWrite(i, LOW);
        BlinkerMIOT.powerState("off", num);
        BlinkerMIOT.print();
        oState[num] = false;
        break;
      default:
        digitalWrite(i, LOW);
        BlinkerMIOT.powerState("off", num);
        BlinkerMIOT.print();
        oState[num] = false;
    }

    if (num == 0) {
      for (uint8_t o_num = 0; o_num < 5; o_num++) {
        oState[o_num] = false;
      }
    }
  }
}

void miotQuery(int32_t queryCode, uint8_t num)
{
  BLINKER_LOG("AliGenie Query outlet: ", num, ", codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_ALL_NUMBER :
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER :
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
      BlinkerMIOT.print();
      break;
    default :
      BlinkerMIOT.powerState(oState[num] ? "on" : "off", num);
      BlinkerMIOT.print();
      break;
  }
}

void dataRead(const String & data)
{
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}

void setup()
{
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  pinMode(27, OUTPUT);
  digitalWrite(27, LOW);
  pinMode(25, OUTPUT);
  digitalWrite(25, LOW);

  Blinker.begin(auth, ssid, pswd);
  Blinker.attachData(dataRead);
  Button1.attach(button1_callback);
  Button2.attach(button2_callback);
  Button3.attach(button3_callback);
  Button4.attach(button4_callback);
  Button5.attach(button5_callback);

  BlinkerMIOT.attachPowerState(miotPowerState);
  BlinkerMIOT.attachQuery(miotQuery);
}

void loop()
{
  Blinker.run();
}
