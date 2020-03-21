/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include <TFT_eSPI.h>
#include <HTTPClient.h>
#include "pics.h"
#include "pic1.h"
#include "pic2.h"
#include "pic3.h"
#include "pic4.h"
#include "pic5.h"
#include "piczan.h"

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

void initTFT(){
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(1);
  

  if (TFT_BL > 0)
  {                          // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
    pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
    digitalWrite(TFT_BL, 1);
                             // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
  }
  while(0){
    tft.pushImage(0,0,240,150,piczan);
    delay(5000);
  }
}


void setup()
{
    initTFT();
    
    Serial.begin(115200);
    delay(10);
    
    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");

    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Connecting", tft.width() / 2, tft.height() / 2);

    WiFi.begin("peter", "13706737886");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");

    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("WiFi connected", tft.width() / 2, tft.height() / 2,4);

    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
String fansCount="";
String likesCount="";
bool needUpdateBG=false;
void getData(void *pvPar){
  while(1){
    while (WiFi.status() != WL_CONNECTED) {
        vTaskDelay(500 / portTICK_PERIOD_MS);
        Serial.print(".");
    }
    HTTPClient http;

        Serial.print("[HTTP] begin...\n");
          // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin("http://api.bilibili.com/x/relation/stat?vmid=268164490"); //HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP headerFFF
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                int pos=(payload.indexOf("follower"));
                String fansCount1=payload.substring(pos+10,payload.length()-2);
                Serial.println(payload);
                if(!fansCount.equals(fansCount1)){
                  fansCount=fansCount1;
                  needUpdateBG=true;
                }
                
                //tft.fillScreen(TFT_BLACK);
                
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
        //获取点赞数
        Serial.print("[HTTP] begin...\n");
          // configure traged server and url
        //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
        http.begin("http://api.bilibili.com/x/space/upstat?mid=268164490"); //HTTP

        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP headerFFF
        httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            //Serial.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                int pos=(payload.indexOf("likes"));
                String likesCount1=payload.substring(pos+7,payload.length()-2);
                Serial.println(payload);
                if(!likesCount.equals(likesCount1)){
                  likesCount=likesCount1;
                  needUpdateBG=true;
                }
                
                //tft.fillScreen(TFT_BLACK);
                
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
        
        vTaskDelay(20000 / portTICK_PERIOD_MS);
  }
}
int value = 0;

void loop()
{
    xTaskCreate(

      getData, /* Task function. */

      "TaskOne", /* String with name of task. */

      10000, /* Stack size in bytes. */

      NULL, /* Parameter passed as input of the task */

      1, /* Priority of the task. */

      NULL); /* Task handle. */
  int k=60;
  tft.fillScreen(TFT_BLACK);

    while(1){
        if(WiFi.status() != WL_CONNECTED){
          WiFi.begin("peter", "13706737886");
          tft.fillScreen(TFT_BLACK);
          while(WiFi.status() != WL_CONNECTED){
            tft.setTextDatum(MC_DATUM);
            tft.fillScreen(TFT_BLACK);
            tft.drawString("reconnecting..", tft.width() / 2, tft.height()/2,4);
            delay(250);
            tft.fillScreen(TFT_BLACK);
            tft.drawString("reconnecting...", tft.width() / 2, tft.height()/2,4);
            delay(250);
          }
          tft.fillScreen(TFT_BLACK);
        }
        if(needUpdateBG){
           tft.fillScreen(TFT_BLACK);
           needUpdateBG=false;
        }
     
        tft.setTextDatum(MC_DATUM);
        tft.drawString("fan " + fansCount, tft.width() / 2-30, tft.height()/2+15,4);
        tft.drawString("like " + likesCount, tft.width() / 2-30, tft.height()/2-15,4);
        tft.pushImage(144,40,96,96,pic1);
        tft.pushImage(144,40,96,96,pic0);//1
        delay(k*3);
        tft.pushImage(144,40,96,96,pic1);//3
        delay(k);
        tft.pushImage(144,40,96,96,pic2);//4
        delay(k);
        tft.pushImage(144,40,96,96,pic3);//5
        delay(k*2);
        tft.pushImage(144,40,96,96,pic4);//7
        delay(k);
        tft.pushImage(144,40,96,96,pic3);//8
        delay(k);
        tft.pushImage(144,40,96,96,pic5);
        delay(k);
    }
    
}


// 
// #include <SPI.h>
// #include "WiFi.h"
// #include <Wire.h>
// #include "esp_adc_cal.h"
// #include "esp_system.h"
// #include "esp_wifi.h"
// #include "esp_event_loop.h"
// #include "esp_log.h"
// #include "nvs_flash.h"
// #include "freertos/event_groups.h"
// #include <WiFi.h>

// #ifndef TFT_DISPOFF
// #define TFT_DISPOFF 0x28
// #endif

// #ifndef TFT_SLPIN
// #define TFT_SLPIN 0x10
// #endif

// #define TFT_MOSI 19
// #define TFT_SCLK 18
// #define TFT_CS 5
// #define TFT_DC 16
// #define TFT_RST 23

// #define TFT_BL 4 // Display backlight control pin
// #define ADC_EN 14
// #define ADC_PIN 34
// #define BUTTON_1 35
// #define BUTTON_2 0

// TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library

// char buff[512];
// int vref = 1100;
// int btnCick = false;

// //! Long time delay, it is recommended to use shallow sleep, which can effectively reduce the current consumption
// void delay(int ms)
// {
//   esp_sleep_enable_timer_wakeup(ms * 1000);
//   esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
//   esp_light_sleep_start();
// }


// void showVoltage()
// {
//   static uint64_t timeStamp = 0;
//   if (millis() - timeStamp > 1000)
//   {
//     timeStamp = millis();
//     uint16_t v = analogRead(ADC_PIN);
//     float battery_voltage = ((float)v / 4095.0) * 2.0 * 3.3 * (vref / 1000.0);
//     String voltage = "Voltage :" + String(battery_voltage) + "V";
//     Serial.println(voltage);
//     //tft.fillScreen(TFT_BLACK);
//     tft.setTextDatum(MC_DATUM);
//     tft.drawString(voltage, tft.width() / 2, tft.height() / 2);
//   }
// }

// void taskOne(void *parameter)

// {

//   while (true)
//   {
    
//     Serial.println("sssssss" + portTICK_PERIOD_MS);
//     vTaskDelay(1000/portTICK_PERIOD_MS);
//   }

//   Serial.println("Ending task 1");

//   vTaskDelete(NULL);
// }

// void setup()
// {
//   delay(10000);
//   Serial.begin(115200);
//   Serial.println("Start");
//   Serial.print("Connecting to ");
//   Serial.println("peter");

//   Serial.print(WiFi.begin("peter", "13706737886"));
  

//   while (WiFi.status() != WL_CONNECTED) {
//       delay(500);
//       Serial.print(".");
//   }

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());

  
//   tft.init();
//   tft.setRotation(1);
//   tft.fillScreen(TFT_BLACK);
//   tft.setTextSize(2);
//   tft.setTextColor(TFT_WHITE);
//   tft.setCursor(0, 0);
//   tft.setTextDatum(MC_DATUM);
//   tft.setTextSize(1);
//   // xTaskCreate(

//   //     taskOne, /* Task function. */

//   //     "TaskOne", /* String with name of task. */

//   //     10000, /* Stack size in bytes. */

//   //     NULL, /* Parameter passed as input of the task */

//   //     1, /* Priority of the task. */

//   //     NULL); /* Task handle. */

//   if (TFT_BL > 0)
//   {                          // TFT_BL has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
//     pinMode(TFT_BL, OUTPUT); // Set backlight pin to output mode
//     digitalWrite(TFT_BL, 1);
//                              // Turn backlight on. TFT_BACKLIGHT_ON has been set in the TFT_eSPI library in the User Setup file TTGO_T_Display.h
//   }
  


//   tft.setSwapBytes(true);
//   delay(5000);

//   tft.setRotation(0);
//   int i = 5;

//   while (true)
//   {
//     tft.fillScreen(TFT_RED);
//     showVoltage();
//     delay(1000);
//     tft.fillScreen(TFT_BLUE);
//     showVoltage();
//     delay(1000);
//     tft.fillScreen(TFT_GREEN);
//     showVoltage();
//     delay(1000);
//   }

//   esp_adc_cal_characteristics_t adc_chars;
//   esp_adc_cal_value_t val_type = esp_adc_cal_characterize((adc_unit_t)ADC_UNIT_1, (adc_atten_t)ADC1_CHANNEL_6, (adc_bits_width_t)ADC_WIDTH_BIT_12, 1100, &adc_chars);
//   //Check type of calibration value used to characterize ADC
//   if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF)
//   {
//     Serial.printf("eFuse Vref:%u mV", adc_chars.vref);
//     vref = adc_chars.vref;
//   }
//   else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP)
//   {
//     Serial.printf("Two Point --> coeff_a:%umV coeff_b:%umV\n", adc_chars.coeff_a, adc_chars.coeff_b);
//   }
//   else
//   {
//     Serial.println("Default Vref: 1100mV");
//   }
// }

// void loop()
// {
//   delay(1000);
// }