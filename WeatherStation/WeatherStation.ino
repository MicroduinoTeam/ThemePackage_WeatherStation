#include <Microduino_Dust.h>
#include <Microduino_Tem_Hum.h>
#include <Miroduino_Light.h>
#include "oled.h"

Tem_Hum_S2 termo;
Light_D1 tsl;
SoftwareSerial mySerial(4, -1); // RX, TX
Dust pmSensor(&mySerial);     //PM2.5使用软串口（4，-1）

#define INTERVAL_LCD    500   //OLED及传感器刷新时间间隔   

#define LIGHTD1PIN A0

uint32_t long lcd_time;
float sensorTemp;       //温度值
float sensorHumi;       //湿度值
float sensorPM25;       //pm2.5浓度
float sensorLight;      //光照强度

boolean LightSta;

void setup(void) {

  Serial.begin(115200);     //初始化波特率
  termo.begin();            //初始化温湿度
  LightSta = tsl.begin(); //初始化光照
  if (LightSta) {
    tsl.enableAutoGain(true); //设置光照自动增益
  }
}

void loop(void) {
  if (lcd_time > millis()) lcd_time = millis();
  if (millis() - lcd_time > INTERVAL_LCD) {  //定时更新传感器、OLED

    sensorTemp = termo.getTemperature();  //获取温度
    sensorHumi = termo.getHumidity();    //获取湿度
    if (LightSta) {
      sensorLight = tsl.getLuminosity();   //获取光照
    }
    else {
      sensorLight = analogRead(LIGHTD1PIN);
    }
    if (pmSensor.available())
      sensorPM25 = pmSensor.getPM25();   //获取PM2.5

    volcd(sensorTemp, sensorHumi, sensorLight, sensorPM25);   //更新OLED显示

    lcd_time = millis();    //更新计时器
  }
}
