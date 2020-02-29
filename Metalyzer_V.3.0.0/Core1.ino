int timer,contador;
int t,rango,numero_de_serie;
int vmin=-1500,vmax=1500,vps=50 ,offset=2048,cicle=1,metodo =1;


void TaskVoltametry(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  TIMERG0.wdt_wprotect=TIMG_WDT_WKEY_VALUE;
  TIMERG0.wdt_feed=1;
  TIMERG0.wdt_wprotect=0;
  for (;;)
  {
    
     if(comando[0] == 'A' || comando[1] == 'A'){
        voltametria();
        comando[0]='S';comando[1]='S';
      }
     else if(comando[0] == 'B' || comando[1] == 'B'){ 
        leerBateria();
        comando[0]='S';comando[1]='S';
      }
     else if(comando[0] == 'C' || comando[1] == 'C'){
        enviarParametros();
        comando[0]='S';comando[1]='S';
     }
     else if(comando[0] == 'D' || comando[1] == 'D'){
        leerParams();
        comando[0]='S';comando[1]='S';
     }
     }
     vTaskDelay(10);  // one tick delay (15ms) in between reads for stability
  }

byte buffer[3];
void DAC(unsigned int val){
  buffer[0] = 0b01000000; // control byte
  buffer[1] = val >> 4; // MSB 11-4 shift right 4 places
  buffer[2] = val << 4; // LSB 3-0 shift left 4 places
  Wire.beginTransmission(MCP4725); // address device
  Wire.write(buffer[0]);  // pointer
  Wire.write(buffer[1]);  // 8 MSB
  Wire.write(buffer[2]);  // 4 LSB
  Wire.endTransmission();
}
void voltametria(){
      
      enviarIniciaAnalisis();
      
      unlockCell();
      int contador=0;
      int t_k=0;
      int voltage;
      float corriente;
      int n_ciclos;
      int ciclo_actual=0;
      t = millis();
      delay(0);
     
      int num_data = 0 ; 
      int j=1;
      for(n_ciclos=0;n_ciclos<cicle;n_ciclos++) // voltametria de todos los ciclos
      {
        for (int i =1800 ; i < 2948  ; i=i+5) { //semiciclo de subida
          DAC(i);
          voltage = i-offset;
          corriente = analogRead(WE)*3.300/4096;
          envio_ble(n_ciclos,voltage,corriente); 
          num_data++;
          delay(100);
          Serial.print(voltage);
          Serial.print("\t;\t");
          Serial.println(corriente,4);
          if(comando[0]== 'S' || comando[1]== 'S') return;
        }
        for (int i =2948 ; i >1800 ; i=i-5) { //semiciclo de subida
          DAC(i);
          voltage = i-offset;
          corriente = analogRead(WE)*3.300/4096;
          envio_ble(n_ciclos,voltage,corriente); 
          num_data++;
          delay(100);
          Serial.print(voltage);
          Serial.print("\t;\t");
          Serial.println(corriente,4);
          if(comando[0]== 'S' || comando[1]== 'S') return;
        }
        Serial.print("\t########################\t");
     }
     lockCell();
     digitalWrite(12,LOW);
     digitalWrite(14,HIGH);
     digitalWrite(27,LOW);

     enviarFinalizaAnalisis();
}
void envio_ble(int n_ciclos, float voltage, float corriente){
      SerialBT.print("AM;");
      SerialBT.print(n_ciclos);
      SerialBT.print(";");
      SerialBT.print(voltage);
      SerialBT.print(";");
      SerialBT.println(corriente,4);  
  }

void leerBateria(){
    SerialBT.print("B;");
    SerialBT.print(nivel_bateria);
    SerialBT.print("% ");
    SerialBT.print(voltaje_bateria);
    SerialBT.println("v");  
    Serial.print("BATERIA: ");
     Serial.print("B;");
    Serial.print(nivel_bateria);
    Serial.print("% ");
    Serial.print(voltaje_bateria);
    Serial.println("v");
  }




void lockCell(){
  digitalWrite(WE,LOW);
  digitalWrite(RE,LOW);
  digitalWrite(CE,LOW);
}
void unlockCell(){
  digitalWrite(WE,HIGH);
  digitalWrite(RE,HIGH);
  digitalWrite(CE,HIGH);
  }
void analisis() {
  //SerialBT.println(//analogRead(A0));
}


  