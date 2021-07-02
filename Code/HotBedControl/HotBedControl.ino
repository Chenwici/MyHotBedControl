#include <Wire.h>
#include <Adafruit_GFX.h>//需載
#include <Adafruit_SH1106.h>//需載

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SH1106 display(OLED_RESET);

const int sensorPin1 = 14;//控制溫度(可變電阻)連接腳位
const int sensorPin2 = 16;//熱床溫度(感溫電阻)連接腳位
const int HostBedMOSFET = 7;//熱床MOSFET開關連接腳位

int sensorValue1 = 0;//控制溫度(可變電阻)值
int sensorValue2 = 0;//熱床溫度(感溫電阻)值

int HotBedSwitch = 0;
void setup() {
  
  pinMode(HostBedMOSFET, OUTPUT);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin2, INPUT);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.display();
  digitalWrite(HostBedMOSFET, LOW);
  //Serial.begin(9600);
}

void sensorPin1Event() {//控制溫度(可變電阻)事件
  
  sensorValue1 = map(analogRead(sensorPin1),0,1023,0,99);
  
  //Serial.println("Control="+String(sensorValue1));
  
  delay(100);
}

void sensorPin2Event() {//熱床溫度(感溫電阻)事件
  
  sensorValue2 = analogRead(sensorPin2)*0.048828125;
  //sensorValue2=RT;
  //sensorValue2=map(RT,3.186,11093.8,-55,125);
  //Serial.println("Bed="+String(sensorValue2));
  
  delay(100);
}

void oledEvent() {//熱床溫度(感溫電阻)事件
  
  display.clearDisplay();
  //display.fillScreen(0);
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(5, 5);
  display.print("HotBedCtrl");
  display.setCursor(5, 25);
  display.print("TC:"+String(sensorValue2)+"/"+String(sensorValue1)+"C");
  display.setCursor(5, 45);
  display.print("HotBedSW="+String(HotBedSwitch));
  display.display();
  
  delay(100);
}

void bedRelayEvent() {//熱床溫度(感溫電阻)事件

  if(sensorValue1>40)
  {
      if(sensorValue2<sensorValue1)
      {
          HotBedSwitch=1;
          digitalWrite(HostBedMOSFET, HIGH);
          //Serial.println("HostBedMOSFET--HIGH");
      }
      else
      {
          HotBedSwitch=0;
          digitalWrite(HostBedMOSFET, LOW);
          //Serial.println("HostBedMOSFET--LOW2");
      }
  }
  else
  {
      HotBedSwitch=0;
      digitalWrite(HostBedMOSFET, LOW);
      //Serial.println("HostBedMOSFET--LOW1");
  }
  
  delay(100);
}

void loop() {
  
  sensorPin1Event();
  sensorPin2Event();

  bedRelayEvent();

  oledEvent();
  
}
