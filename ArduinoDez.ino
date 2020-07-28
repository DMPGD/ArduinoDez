#include <Wire.h>
#include <VL6180X.h>

VL6180X sensor;

int KEY1 = 6;
int KEY2 = 9;
int LED1 = 2;
int LED2 = 7;
int WATERLEVEL = 8;
int BUZZER = 4;
boolean PFlag = false;
boolean LFlag = false;
boolean HandHere = false;

int TimerMain = 5000;
int TimerPump = 100;
int TimerLight = 499;

unsigned long currentTime1 = 0; 
unsigned long currentTime2 = 0;
unsigned long currentTime3 = 0;


void setup()
{
  Serial.begin(9600);
  Wire.begin();

  sensor.init();
  sensor.configureDefault();
  sensor.setTimeout(500);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(KEY1, OUTPUT);
  pinMode(KEY2, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(WATERLEVEL, INPUT);
  
  LEDStatus();
  Light(true);
}
void Timer()
{
  if (millis() == 0) 
  {
     currentTime1 = 0; 
     currentTime2 = 0; 
     currentTime3 = 0; 
    }
  }

boolean WATERLINE () 
   {
    return digitalRead(WATERLEVEL);
    }
void VOICE()
{
   tone(BUZZER, 1000, 200);
   }
   
void LEDStatus()
{
  if (WATERLINE()) {digitalWrite(LED2,HIGH); digitalWrite(LED1,LOW);} else { digitalWrite(LED2,LOW); digitalWrite(LED1,HIGH); }
  }

boolean Light(boolean flag)
{
  if (flag) digitalWrite(KEY2,HIGH); else digitalWrite(KEY2,LOW);
  return flag;
  }
  
void LightFlashing()
{
  if (millis() - currentTime3 > TimerLight) 
     { 
        currentTime3 = millis();        
        Light(LFlag);
        LFlag=!LFlag;
      }  
  }

void ChangeMode()
{ 
  TimerPump = 50;
  TimerPump = 150;
  TimerPump = 200;
  }   



void PUMPTIMER ()
  {
     if ((millis() - currentTime2 > TimerPump) && (PFlag)) // Если время контроллера millis, больше переменной на 1000, то запускаем условие if
     { 
        currentTime2 = millis();        // Приравниваем переменную текущего времени к времени контроллера, чтобы через 1000мс опять сработал наш цикл.
        digitalWrite(KEY1, LOW);
        PFlag = false;
      }
    }

void PUMP()
{
  if (millis() - currentTime1 > TimerMain)
  {if ((sensor.readRangeSingleMillimeters() < 255) && (WATERLINE()) && (!HandHere) ) 
  {   
    VOICE();
    digitalWrite(KEY1, HIGH);
    PFlag = true;
    currentTime1 = millis();
    currentTime2 = millis();
    HandHere=true;
    Light(true);
  }
  else {
  if (sensor.readRangeSingleMillimeters() < 255) {} else {HandHere=false; Light(false);}
  }
  } else LightFlashing();
}
  
void loop()
{
  Timer();
  LEDStatus();
  PUMPTIMER();
  PUMP();       
}
