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


//小爱模拟微动开关，控制家电

#define BLINKER_WIFI
#define BLINKER_MIOT_OUTLET

#include <Blinker.h>

char auth[] = "********";//你的点灯科技中分配的唯一key
char ssid[] = "********";//你的WiFi名称
char pswd[] = "********";//你的WiFi密码

bool oState = false;

#define jiashiqi 12 //定义使用的引脚
int nSignalState = 0;

unsigned long lSignalSovleStartTimer = 0;

long GetTickCount()
{
  return millis();
}

//小爱电源类操作的回调函数:当小爱同学向设备发起控制, 设备端需要有对应控制处理函数
void miotPowerState(const String & state)
{
  BLINKER_LOG("need set power state: ", state);
  if (state == BLINKER_CMD_OFF)
  {
    nSignalState = 1;
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
  }

  else if (state == BLINKER_CMD_ON)
  {
    nSignalState = 2;
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
  }
}


void setup() {
  // 初始化串口，并开启调试信息，调试用可以删除
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  // 初始化IO
  pinMode(jiashiqi, OUTPUT);
  digitalWrite(jiashiqi, LOW);

  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  // Button1.attach(button1_callback);

  //小爱同学务必在回调函数中反馈该控制状态
  BlinkerMIOT.attachPowerState(miotPowerState);//注册回调函数
}


void loop() {
  if (nSignalState == 1) {
  if (lSignalSovleStartTimer == 0) {
      //设置拉高
      digitalWrite(jiashiqi, HIGH);
      lSignalSovleStartTimer = GetTickCount();
    }
    else if ((GetTickCount() - lSignalSovleStartTimer) > 500) {
      digitalWrite(jiashiqi, LOW);
      nSignalState = 0;
      lSignalSovleStartTimer = 0;
    }
  }
  else if (nSignalState == 2) {
  if (lSignalSovleStartTimer == 0) {
      //设置拉高
      digitalWrite(jiashiqi, HIGH);
      lSignalSovleStartTimer = GetTickCount();
    }
    else if ((GetTickCount() - lSignalSovleStartTimer) > 500) {
      digitalWrite(jiashiqi, LOW);
      nSignalState = 0;
      lSignalSovleStartTimer = 0;
    }
  }
  Blinker.run();
}
