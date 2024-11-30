/* *****************************************************************
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
#define BLINKER_MIOT_MULTI_OUTLET  //设置为小爱多个插座的模式

#include <Blinker.h>

// 使用宏定义或常量表示引脚编号，增强可读性和可维护性
const int PIN_YAO_TOU_ON_OFF = 13;         //摇头/不摇头
const int PIN_LOW_TEMP_WEAK_WIND = 12;     //低温/弱风
const int PIN_HIGH_TEMP_STRONG_WIND = 27;  //高温/强风
const int PIN_UNDEFINED = 25;              //未定义

char auth[] = "********";  //点灯 App 申请的 Secret Key
char ssid[] = "********";
char pswd[] = "********";

// 电源状态
bool outletPowerStates[5] = { false };

// 新建组件对象，要和APP组件’数据键名’一致
BlinkerButton Button1("btn_a");
BlinkerButton Button2("btn_b");
BlinkerButton Button3("btn_c");
BlinkerButton Button4("btn_d");
BlinkerButton Button5("btn_all");

// 用于记录Button1、Button2、Button3状态的全局变量
bool button1CurrentState = false;
bool button2CurrentState = false;
bool button3CurrentState = false;

// 封装设置引脚状态并更新按钮相关信息的函数
void setPinAndButtonState(int pin, BlinkerButton &button, const String &state, const String &text) {
  BLINKER_LOG("当前按键预期 state: ", state);

  digitalWrite(pin, state == "on" ? HIGH : LOW);
  button.print(state);
  button.text(text);
}

// 按下Blinker APP按键1即会执行该函数
void button1_callback(const String &state) {
  BLINKER_LOG("get button1 state: ", state);

  if (state == "off") {
    setPinAndButtonState(PIN_YAO_TOU_ON_OFF, Button1, "off", "摇头");
  } else if (state == "on") {
    setPinAndButtonState(PIN_YAO_TOU_ON_OFF, Button1, "on", "摇头");
  }
}

// 按下Blinker APP按键2即会执行该函数
void button2_callback(const String &state) {
  BLINKER_LOG("get button2 state: ", state);

  if (state == "off") {
    setPinAndButtonState(PIN_LOW_TEMP_WEAK_WIND, Button2, "off", "低温/弱风");
  } else if (state == "on") {
    setPinAndButtonState(PIN_LOW_TEMP_WEAK_WIND, Button2, "on", "低温/弱风");
    setPinAndButtonState(PIN_HIGH_TEMP_STRONG_WIND, Button3, "off", "高温/强风");
  }
}

// 按下Blinker APP按键3即会执行该函数
void button3_callback(const String &state) {
  BLINKER_LOG("get button3 state: ", state);

  if (state == "off") {
    setPinAndButtonState(PIN_HIGH_TEMP_STRONG_WIND, Button3, "off", "高温/强风");
  } else if (state == "on") {
    setPinAndButtonState(PIN_LOW_TEMP_WEAK_WIND, Button2, "off", "低温/弱风");
    setPinAndButtonState(PIN_HIGH_TEMP_STRONG_WIND, Button3, "on", "高温/强风");
  }
}

// 按下Blinker APP按键4即会执行该函数
void button4_callback(const String &state) {
  BLINKER_LOG("get button4 state: ", state);

  if (state == "off") {
    setPinAndButtonState(PIN_UNDEFINED, Button4, "off", "电蚊香");
  } else if (state == "on") {
    setPinAndButtonState(PIN_UNDEFINED, Button4, "on", "电蚊香");
  }
}

// 按下Blinker APP按键5即会执行该函数
void button5_callback(const String &state) {
  BLINKER_LOG("get button state: ", state);

  if (!digitalRead(PIN_YAO_TOU_ON_OFF) && !digitalRead(PIN_LOW_TEMP_WEAK_WIND) && !digitalRead(PIN_HIGH_TEMP_STRONG_WIND)) {
    setPinAndButtonState(PIN_YAO_TOU_ON_OFF, Button1, "on", "摇头");
    setPinAndButtonState(PIN_LOW_TEMP_WEAK_WIND, Button2, "on", "低温/弱风");
  } else {
    setPinAndButtonState(PIN_YAO_TOU_ON_OFF, Button1, "off", "摇头");
    setPinAndButtonState(PIN_LOW_TEMP_WEAK_WIND, Button2, "off", "低温/弱风");
    setPinAndButtonState(PIN_HIGH_TEMP_STRONG_WIND, Button3, "off", "低温/弱风");
  }
}

// 13-摇头/不摇头；12-低温/弱风；27-高温/强风
void miotPowerState(const String &state, uint8_t num) {
  BLINKER_LOG("need set outlet: ", num, ", power state: ", state);

  int pinIndex;
  switch (num) {
    case 1:
      pinIndex = PIN_YAO_TOU_ON_OFF;
      break;
    case 2:
      pinIndex = PIN_LOW_TEMP_WEAK_WIND;
      break;
    case 3:
      pinIndex = PIN_HIGH_TEMP_STRONG_WIND;
      break;
    case 4:
      pinIndex = PIN_UNDEFINED;
      break;
  }

  if (state == BLINKER_CMD_OFF) {
    if (num == 2) {
      digitalWrite(PIN_HIGH_TEMP_STRONG_WIND, LOW);
    } else if (num == 3) {
      digitalWrite(PIN_LOW_TEMP_WEAK_WIND, LOW);
    }
    digitalWrite(pinIndex, HIGH);
    BlinkerMIOT.powerState("on");
    BlinkerMIOT.print();
    outletPowerStates[num] = true;
  } else if (state == BLINKER_CMD_ON) {
    digitalWrite(pinIndex, LOW);
    BlinkerMIOT.powerState("off");
    BlinkerMIOT.print();
    outletPowerStates[num] = false;
  }

  // 处理全部插座关闭的情况
  if (num == 0 && state == "off") {
    for (uint8_t o_num = 0; o_num < 5; o_num++) {
      outletPowerStates[o_num] = false;
    }
  }
}

// 添加错误处理逻辑到查询函数中
void miotQuery(int32_t queryCode, uint8_t num) {
  BLINKER_LOG("小爱语控指令: ", num, ", codes: ", queryCode);

  // 先进行数据一致性检查，确保状态值有效
  if (num < 0 || num >= 5) {
    BLINKER_LOG("Invalid outlet number in query!");
    return;
  }

  switch (queryCode) {
    case BLINKER_CMD_QUERY_ALL_NUMBER:
      BLINKER_LOG("MIOT Query All");
      BlinkerMIOT.powerState(outletPowerStates[num] ? "on" : "off", num);
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER:
      BLINKER_LOG("MIOT Query Power State");
      BlinkerMIOT.powerState(outletPowerStates[num] ? "on" : "off", num);
      BlinkerMIOT.print();
      break;
    default:
      BlinkerMIOT.powerState(outletPowerStates[num] ? "on" : "off", num);
      BlinkerMIOT.print();
      break;
  }
}

void dataRead(const String &data) {
  BLINKER_LOG("Blinker readString: ", data);

  Blinker.vibrate();

  uint32_t BlinkerTime = millis();

  Blinker.print("millis", BlinkerTime);
}

void setup() {
  Serial.begin(115200);
  BLINKER_DEBUG.stream(Serial);
  //BLINKER_DEBUG.debugAll();

  // 使用宏定义的引脚编号设置引脚模式和初始电平
  pinMode(PIN_YAO_TOU_ON_OFF, OUTPUT);
  digitalWrite(PIN_YAO_TOU_ON_OFF, LOW);

  pinMode(PIN_LOW_TEMP_WEAK_WIND, OUTPUT);
  digitalWrite(PIN_LOW_TEMP_WEAK_WIND, LOW);

  pinMode(PIN_HIGH_TEMP_STRONG_WIND, OUTPUT);
  digitalWrite(PIN_HIGH_TEMP_STRONG_WIND, LOW);

  pinMode(PIN_UNDEFINED, OUTPUT);
  digitalWrite(PIN_UNDEFINED, LOW);

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

void loop() {
  Blinker.run();
}