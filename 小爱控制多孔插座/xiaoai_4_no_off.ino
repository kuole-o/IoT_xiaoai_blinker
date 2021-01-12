#define BLINKER_WIFI
#define BLINKER_MIOT_MULTI_OUTLET    //设置为小爱多个插座的模式

#include <Blinker.h>

char i;
char auth[] = "******";
char ssid[] = "******";
char pswd[] = "******";

bool oState[5] = { false };

void miotPowerState(const String & state, uint8_t num)  
{
    BLINKER_LOG("need set outlet: ", num, ", power state: ", state); 
    i=0;   //初始化  重要！！
    if(num==1)
    {
      i=D1;         
    }
    else if(num==2)
    {
      i=D2;
    }
    else if(num==3)
    {
      i=D3;
    }

    if (state == BLINKER_CMD_ON) 
    {      
        digitalWrite(i, LOW);      
        BlinkerMIOT.powerState("on", num);
        BlinkerMIOT.print();

        oState[num] = true;
        
    }
    else if (state == BLINKER_CMD_OFF) 
    {
        digitalWrite(i, HIGH);
        BlinkerMIOT.powerState("off", num);
        BlinkerMIOT.print();

        oState[num] = false;

        if (num == 0)
        {
            for (uint8_t o_num = 0; o_num < 5; o_num++)
            {
                oState[o_num] = false;
            }
        }
    }
}

void miotQuery(int32_t queryCode, uint8_t num)
{
    BLINKER_LOG("AliGenie Query outlet: ", num,", codes: ", queryCode);

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

    pinMode(D1, OUTPUT);
    digitalWrite(D1, HIGH);
    pinMode(D2, OUTPUT);
    digitalWrite(D2, HIGH);
    pinMode(D3, OUTPUT);
    digitalWrite(D3, HIGH);
 
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachQuery(miotQuery);

}

void loop()
{
    Blinker.run();
}
