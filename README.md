



开发环境

platform io（vscode插件）

板子

# TTGO T-Display

![1584209506130](C:\Users\Administrator.VZMRW9ETL45BX39\AppData\Roaming\Typora\typora-user-images\1584209506130.png)

https://api.bilibili.com/x/relation/stat?vmid=268164490



唯一干了的事就是剪了下制作过程的视频。教程什么的后面再剪。

#### 剪完视频后就忘了写记录了。。

先来附上视频链接https://www.bilibili.com/video/av96798878/

### 1.项目第一步当然是环境啦。

这边我们采用vscode+platform io，环境安装传送门，还没写，后面随缘写，因为没什么难度

### 2.这个环境配完后呢我们就可以开始新建工程了。

#### 如果想白嫖就直接open我的项目就好了。这边把项目放在github上，[传送门](https://github.com/ActivePeter/bilibili_fansCount_esp32)

但是我这边还是讲讲从头开始怎么新建的。

我们使用platform io 点击new project

1.Name输入项目名，2.Broad选择板子类型。我的是ttgo。它里面没有，所以选择默认的那个 叫做。spressif ESP32 Dev Module3.选择框架。我才用的arduino。因为我喜欢c++.点击finish

然后我们的main程序是在src里。

### 3.开始撸代码。

第一步：屏幕

首先我使用的是ttgo的板子，它上面有一个ips屏。我们要用到一个库。https://github.com/Bodmer/TFT_eSPI，这个库的使用是这样

![O9V7RV1TLCJ__ML2NKZI9RN.png](https://i.loli.net/2020/03/22/KCgFTnbYN8uELHU.png)

下载下来放入项目的lib文件夹下，找到User_Setup_Select.h文件

将22行//#include <User_Setup.h> 注释掉

找到52行#include <User_Setups/Setup25_TTGO_T_Display.h> 也就是我们的板子。将注释去除

main里面#include <TFT_eSPI.h>

以下代码是初始化tft屏幕。

```c++
TFT_eSPI tft = TFT_eSPI(135, 240);
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


}
```

主要的屏幕操作语句就这几句

```c++
tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Connecting", tft.width() / 2, tft.height() / 2);
```

------

第二步：链接wifi。

先#include <WiFi.h>

然后 

```c++
WiFi.begin("WiFi名", "wifi密码"); 
while (WiFi.status() != WL_CONNECTED) {//这里是阻塞程序，直到链接成功
        delay(500);
        Serial.print(".");
    }
```

 arduino的库是真的方便。。

------

第三步：调用api

先#include <HTTPClient.h>

```c++
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
```

接口链接是http://api.bilibili.com/x/relation/stat?vmid=268164490。这个是我的。你自己的得去浏览器里进开发者面板，看数据。



gif图这一次就不讲了。我后面有个新的项目做完再讲
