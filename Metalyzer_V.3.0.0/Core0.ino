char comando[2] = {'X','X'};
int conexion =-1;
boolean BLINK=false;
int color;
int t_blink=0;
int t_run;
void TaskBluetooth(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed=1;
  TIMERG0.wdt_wprotect=0;
  // initialize digital LED_BUILTIN on pin 13 as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  t_run=millis();
  for (;;) // A Task shall never return or exit.
  {
    
    if(conexion==-1){
       if(millis()-t_run>2000){
          t_run=millis();
          Serial.println("Esperando comando ....");
       }
        
      }
    if (SerialBT.available()> 0 || Serial.available()>0)
    {
      BLINK=true;
      color = BlueLed;
      t_blink = 100;
      conexion=1;
      comando[0]= Serial.read();
      comando[1]= SerialBT.read();
      Serial.println(comando[1]);
      
    }
    
    vTaskDelay(1);
  }
}

void LedBlink(void *pvParameters)  // This is a task.
{
  (void) pvParameters;

  for (;;) // A Task shall never return or exit.
  {
    if(BLINK == true){
      digitalWrite(color,HIGH);
      vTaskDelay(t_blink);
      digitalWrite(color,LOW);
      BLINK = false;
    }
      vTaskDelay(1);
  }
}
float voltaje_bateria;
short nivel_bateria;
float porcentaje;
void ReadBattery(void *pvParameters){
  (void) pvParameters;
  for(;;){
    float valor = analogRead(bateria);
    voltaje_bateria = valor * 1.65 / 4096;
    if(voltaje_bateria<= 1.35){
      porcentaje=0;
    }
    else{
      porcentaje=(voltaje_bateria-1.35)/4.2*100;  
    }
    
    nivel_bateria = round(porcentaje);
    vTaskDelay(1000);
    }
}