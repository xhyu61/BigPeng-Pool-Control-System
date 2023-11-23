#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>
#include <Arduino_FreeRTOS.h>

// DHT11模块
#define DHTPIN 2      //设定2号引脚为数据输入
#define DHTTYPE DHT11 // DHT 11
DHT dht(DHTPIN, DHTTYPE);
float pH = dht.readHumidity();    //读湿度传感器数据,pH
float t = dht.readTemperature();  //读温度传感器数据,t

// LDR光敏电阻模块
const int LDR_PIN = A0; //设定电压输出位A0口
float ntu;              //定义浑浊度NTU

// BMP180气压传感模块
Adafruit_BMP085 bmp;    // 初始化BMP180
float orp;              // 定义氧化还原电位ORP

int led_pin[] = {8, 9, 10, 11, 12, 13}; // LED灯的连接的数据引脚，五个灯反应五种模块设备工作状态
int led_cnt = 6;                        // led灯的数量
int motor1 = 6;                         // 水温调节
int motor2 = 7;                         // pH调节

bool is_auto = true; //手动自动模式，默认自动

TaskHandle_t BeginTask_Handler; //开启任务
TaskHandle_t Send_Task_Handler; //串口发送数据与控制任务

void begin_task(void *pvParameters);
void send_task(void *pvParameters);

void setup()
{

    Serial.begin(9600);//串口打开
    dht.begin(); // dht初始化

    //判断BMP是否开启
    if (!bmp.begin())
    {
        Serial.println("Could not find a valid BMP085 sensor, check wiring!");
        while (1)
        {
        }
    }
    //led灯初始化
    for (int i = 0; i < led_cnt; i++)
    {
        pinMode(led_pin[i], OUTPUT);
        digitalWrite(led_pin[i], LOW);
    }
    //初始化电机与蜂鸣器
    pinMode(motor1, OUTPUT);
    pinMode(motor2, OUTPUT);
    // pinMode(sound, OUTPUT);

    xTaskCreate(begin_task, "begin_task", 128, NULL, 2, &BeginTask_Handler);
    vTaskStartScheduler(); // 开启任务调度
}

void loop()
{
}

void begin_task(void *pvParameters)
{
    taskENTER_CRITICAL(); // 进入临界区
    xTaskCreate(send_task, "send_task", 128, NULL, 2, &Send_Task_Handler);
    vTaskDelete(BeginTask_Handler); //删除start_task
    taskEXIT_CRITICAL();            // 退出临界区
}

void send_task(void *pvParameters)
{
    String comdata = ""; //串口字符串
    for (;;)
    {
        // delay(500);
        //【手动控制】
        while (Serial.available() > 0)
        {
            comdata += char(Serial.read());

            if (comdata == "1" && !is_auto)
            {
                //水温调节开启
                //digitalWrite(led_pin[4], HIGH);
                digitalWrite(motor1, HIGH);
            }
            else if (comdata == "2" && !is_auto)
            {
                //水温调节关闭
                //digitalWrite(led_pin[4], LOW);
                digitalWrite(motor1, LOW);
            }
            else if (comdata == "3" && !is_auto)
            {
                //pH调节开启
                // digitalWrite(led_pin[5], HIGH);
                digitalWrite(motor2, HIGH);
            }
            else if (comdata == "4" && !is_auto)
            {
                //pH调节关闭
                //digitalWrite(led_pin[5], LOW);
                digitalWrite(motor2, LOW);
            }
            else if (comdata == "5" && !is_auto)
            {
                //净化装置开启
                digitalWrite(led_pin[4], HIGH);
                // digitalWrite(motor2, HIGH);
            }
            else if (comdata == "6" && !is_auto)
            {
                //净化装置关闭
                digitalWrite(led_pin[4], LOW);
            // digitalWrite(motor2, LOW);
            }
            
            else if (comdata == "9" && !is_auto)
            {
                //电位控制装置开启
                digitalWrite(led_pin[5], HIGH);
            }
            else if (comdata == "0" && !is_auto)
            {
                //电位控制装置关闭
                digitalWrite(led_pin[5], LOW);
            }
            else if (comdata == "f")
            {
                //变成手动控制
                is_auto = false;
                Serial.print(is_auto);
            }
            else if (comdata == "t")
            {
                //恢复自动控制
                is_auto = true;
                Serial.print(is_auto);
            }
            comdata = "";
        }

        //【监测】，读取传感器数据：
        pH = dht.readHumidity();                                    //从湿度传感器读取pH数据
        t = dht.readTemperature();                                  //从温度传感器读取水温数据
        ntu = analogRead(LDR_PIN);                                  //从光强传感器读取浑浊度数据
        orp = bmp.readSealevelPressure();                           //从压强传感器读取氧化还原电位数据


        //数据适配性处理
        ntu = (ntu - 19.0) / 100.0;
        pH = (pH + 2.0) / 10.0;
        orp = orp / 150.0;
        t = t - 8;
        

        // 失败处理
        if (isnan(pH) || isnan(t))
        {
            Serial.println(F("Failed to read from DHT sensor!"));
            return;
        }

        //【自动控制】
        if (is_auto)
        { 
            //水温
            if (t > 27 || t < 25)
            {
                //水温调节开启
                digitalWrite(led_pin[0], HIGH);
                digitalWrite(motor1, HIGH);
            }
            else
            {
                //水温调节关闭
                digitalWrite(led_pin[0], LOW);
                digitalWrite(motor1, LOW);
            }
            //pH
            if (pH < 6.7 or pH > 7.3)
            {
                //pH调节开启
                digitalWrite(led_pin[1], HIGH);
                digitalWrite(motor2, HIGH);
            }
            else
            {
                //pH调节关闭
                digitalWrite(led_pin[1], LOW);
                digitalWrite(motor2, LOW);
            }
            //浑浊度
            if (ntu >= 1)
            {
                //净化开启
                digitalWrite(led_pin[2], HIGH);
                digitalWrite(led_pin[4], HIGH);
            }
            else
            {
                //净化关闭
                digitalWrite(led_pin[2], LOW);
                digitalWrite(led_pin[4], LOW);
            }
            //氧化还原电位
            if (orp < 650)
            {
                //氧化还原电位调节开启
                digitalWrite(led_pin[3], HIGH);
                digitalWrite(led_pin[5], HIGH);
            }
            else
            {   //氧化还原电位调节关闭
                digitalWrite(led_pin[2], LOW);
                digitalWrite(led_pin[5], LOW);
            }
            Serial.print("s");
            Serial.print(",");
            Serial.print(1);
            Serial.print(",");
        }
        else
        {
            Serial.print("s");
            Serial.print(",");
            Serial.print(0);
            Serial.print(",");
        }
        //【监测】，打印传感器数据：
        Serial.print(t);          //打印温度
        Serial.print(",");
        Serial.print(pH);         //打印pH
        Serial.print(",");
        Serial.print(ntu);        //打印浑浊度
        Serial.print(",");
        Serial.print(orp);        //打印氧化还原电位
        Serial.print(",");
        Serial.print(digitalRead(motor1));//打印水温调节状态
        Serial.print(",");
        Serial.print(digitalRead(motor2));//打印pH调节设备装填
        Serial.print(",");
        Serial.print(digitalRead(led_pin[4]));//打印净化装置状态
        Serial.print(",");
        Serial.println(digitalRead(led_pin[5]));//打印氧化还原电位调节设备状态
        // delay(1000);
    }
}