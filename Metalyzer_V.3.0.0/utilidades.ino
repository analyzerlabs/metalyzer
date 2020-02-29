////////////////////utilidades/////////////////
int V_parametros[5]={0,0,0,0,0};

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void leerParams()
{
  String params = SerialBT.readString();
  int dir=1;
  char aux[50];
  for(int j=0;j<5;j++){
    for(int i=dir;i<50;i++){
      aux[i-dir]= params[i];
      if(params[i]==';')
        {
          dir=i;
          break;
        }
    }
    V_parametros[j] =atoi(aux);
    
  }
  vmin = V_parametros[0];
  vmax = V_parametros[1];
  vps  = V_parametros[2];
  cicle = V_parametros[3];
  metodo = V_parametros[4];
  Serial.print("vmin  = ");
  Serial.println(vmin,5);
  Serial.print("vmax  = ");
  Serial.println(vmax,5);
  Serial.print("vps   = ");
  Serial.println(vps);
  Serial.print("cicle = ");
  Serial.println(cicle);
  Serial.print("metodo= ");
  Serial.println(metodo);
  enviarParametros();
}

void leerParams_Serial()
{
  String params = Serial.readString();
  int dir=1;
  char aux[10] = "";
  for(int j=0;j<5;j++){
    for(int i=dir;i<50;i++){
      aux[i-dir]= params[i];
      if(params[i]==';')
        {
          dir=i;
          break;
        }
    }
    V_parametros[j] =atoi(aux);
  }
  vmin = V_parametros[0];
  vmax = V_parametros[1];
  vps  = V_parametros[2];
  cicle = V_parametros[3];
  metodo = V_parametros[4];
 
  enviarParametros();
}

void enviarParametros(){
      SerialBT.print("C;");
      SerialBT.print(vmin);
      SerialBT.print(";");
      SerialBT.print(vmax);
      SerialBT.print(";");
      SerialBT.print(vps);
      SerialBT.print(";");
      SerialBT.print(cicle);
      SerialBT.print(";");
      SerialBT.println(metodo);
      leerBateria();
  }

void enviarIniciaAnalisis(){
    SerialBT.println("AS");
}

void enviarFinalizaAnalisis(){
    SerialBT.println("AF");
}