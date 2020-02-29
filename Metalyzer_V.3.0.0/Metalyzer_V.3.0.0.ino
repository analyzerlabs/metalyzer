#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#ifndef LED_BUILTIN
#define LED_BUILTIN 12
#endif

#define MCP4725 0x61
#define WE 32
#define CE 25
#define RE 35

#define current 26

#define BlueLed 12
#define RedLed 14
#define GreenLed 27

#define bateria 34
#define errorPin 23

#include <Wire.h>
#include "BluetoothSerial.h"
BluetoothSerial SerialBT;
// define two tasks for Blink & AnalogRead
void TaskBlink( void *pvParameters );
void TaskAnalogReadA3( void *pvParameters );
void TaskVoltametry( void *pvParameters );
void TaskBluetooth( void *pvParameters );
#include "soc/timer_group_struct.h"
#include "soc/timer_group_reg.h"
int x;
// the setup function runs once when you press reset or power the board
void setup() {
  
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);
  SerialBT.begin("METALYZER WHITE"); //Bluetooth device name
  Wire.begin();
  pinMode(BlueLed,OUTPUT);
  pinMode(GreenLed,OUTPUT);
  pinMode(RedLed,OUTPUT);
  // Now set up two tasks to run independently.
  xTaskCreatePinnedToCore(
    TaskBluetooth
    ,  "TaskBlink"   // A name just for humans
    ,  1024  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL 
    ,  0);

  xTaskCreatePinnedToCore(
    TaskVoltametry
    ,  "voltametria"
    ,  4096+
    // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  1);

  xTaskCreatePinnedToCore(
    LedBlink
    ,  "led indicadores"
    ,  1024  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  0);//core
  
  xTaskCreatePinnedToCore(
    ReadBattery
    ,  "Lectura de Bateria"
    ,  1024  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL 
    ,  0);//core

  // Now the task scheduler, which takes over control of scheduling individual tasks, is automatically started.
}

void loop()
{

}