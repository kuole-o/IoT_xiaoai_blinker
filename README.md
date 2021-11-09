# ESP32实现低成本全屋智能 o(￣▽￣)ｄ 
通过ESP32开发板，将传统家电接入小爱同学，语音控制、智能联动。#物联网

## 说明  
- xiaoai_on_off：小爱控制单个插座开关
- xiaoai_4_on_off：小爱控制多孔插座开关
- xiaoai_4_on_off_BlinkerButton5：小爱控制取暖器（兼容风扇）_支持Blinker App按键/语音控制（需要对照键名App设置），支持摇头_低温/弱风_高温/强风
- xiaoai_MicroSwitch：小爱模拟控制微动开关（按1次开，再按1次关；延迟200ms）
- xiaoai_MicroSwitch_1on_2off：小爱模拟控制微动开关（按1次开，再按1次防误触，再按1次关）
  
## 配网  
char auth[] = "xxxxxxx"; // 在点灯App里添加一个设备，生成的设备标识  
char ssid[] = "xxxxxxx"; // 要配网的WiFi名称(2.4GHZ优先)  
char pswd[] = "xxxxxxx"; // 要配网的WiFi密码(2.4GHZ优先)  
  
## 引脚接线  
查看源码，按注释接线即可  
  
## 1.Ardunio IDE[ 点此下载。](https://www.arduino.cc/en/Main/Software) [ 中国大陆安装方法。](https://www.arduino.cn/thread-81194-1-1.html)

## 2.Ardunio ESP32 Blinke[支持库](https://github.com/blinker-iot/blinker-library/archive/master.zip)

- Windows：将下载好的blinker库解压到 我的电脑>文档>Arduino>libraries 文件夹中

- Mac OS：将下载好的blinker库解压到 文稿>Arduino>libraries 文件夹中


## 3.Ardunio安装ESP32开发板。选择ESP32 Dev Module。

ESP32开发板安装方法：在Ardunio IDE——文件——首选项——附加开发板管理网址中加入 https://dl.espressif.com/dl/package_esp32_index.json ；

接着Ardunio IDE——工具——开发板——开发板管理器，搜索 ESP32 安装即可。

- 点灯科技：https://github.com/blinker-iot
- 乐鑫支持：https://github.com/espressif/arduino-esp32
