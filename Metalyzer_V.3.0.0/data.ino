#include "SimpleBLE.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

SimpleBLE BT;

int incoming;
int LED_BUILTIN = 2;
float voltmin,voltmax;
int voltpsec,nrociclos,metodo;


void setup()  
{
  Serial.begin(9600);
  BT.begin("ESP32_LED_Control");
  Serial.println("Bluetooth Device is Ready to Pair");
  // Inicializo parametros
  voltmin=-5.00;
  voltmax=5.00;
  voltpsec = 10;
  nrociclos = 10;
  metodo = 1;
}



void loop() 
{
  if (BT.available()>0)
  {
      char dato=BT.read();
      Serial.println(dato);
      switch(dato)
      {
          case 'A':
            analisis();
            break;
          case 'B':
            enviarBateria();
            break;
          case 'C':
            enviarParams();
            break;
          case 'D':
            leerParams();
            mostrarParams();
            break;
          default:
            break;
      }
  }
}
/**
 * Se envia informacion de bateria
 */
void enviarBateria()
{
  int porc = 80;
  int mah = 1500;
  BT.print("B;");
  BT.print(porc);
  BT.print("% ");
  BT.print(mah);
  BT.println("mAH");
  
  Serial.print("B;");
  Serial.print(porc);
  Serial.print("% ");
  Serial.print(mah);
  Serial.println("mAH");
}

/**
 * Se lee los parametros enviados por bluetooth
 */
void leerParams()
{
  String in = BT.readString();
  Serial.println(in);
 /* char *str;
  str = in;
  char *delimitador = ";";
  String resultado[] = {"","","","",""};
  int limites[]={0,0,0,0,0,0};
  int j=1;
    Serial.println(strlen(str));
    
    
    for( i =0 ;i <= strlen(str);i++){
        if(*(str+i) == *delimitador){
            limites[j]=i+1;
            Serial.println(str[i]);
            j++;
        }      
    }
    for (i=0 ; i<=5;i++){
        for(int j=limites[i];j<limites[i+1]-1;j++){
            resultado[i].concat(str[j]); 
        }
        Serial.println(resultado[i]);
    }

  voltmin = resultado[1];
  voltmax = resultado[2];
  voltpsec = resultado[3];
  nrociclos = resultado[4];
  metodo = 1;*/
}

/**
 * Se muestran los parametros en la consola de arduino
 */
void mostrarParams()
{
  Serial.println("Parametros guardados:");
  Serial.print("Voltaje minimo:\t");
  Serial.println(voltmin);
  Serial.print("Voltaje maximo:\t");
  Serial.println(voltmax);
  Serial.print("Voltaje por segundo:\t");
  Serial.println(voltpsec);
  Serial.print("Numero de ciclos:\t");
  Serial.println(nrociclos);
  Serial.print("Metodo:\t");
  Serial.println(metodo);
}

/**
 * Envia por bluetooth los parametros guardados
 */
void enviarParams()
{
  int porc = 80;
  int mah = 1500;
  BT.print("C;");
  //BT.print(porc);
  //BT.print("% ");
  //BT.print(mah);
  //BT.print("mAH");
  //BT.print(";");
  BT.print(voltmin);
  BT.print(";");  
  BT.print(voltmax);
  BT.print(";");
  BT.print(voltpsec);
  BT.print(";");
  BT.print(nrociclos);
  BT.print(";");
  BT.println(metodo);

  Serial.print("C;");
  Serial.print(voltmin);
  Serial.print(";");  
  Serial.print(voltmax);
  Serial.print(";");
  Serial.print(voltpsec);
  Serial.print(";");
  Serial.print(nrociclos);
  Serial.print(";");
  Serial.println(metodo);
}

void analisis()
{
  // Inicia enviando un mensaje a la app
  BT.println("AS"); // analisis empieza
  Serial.println("Analisis empieza");
  switch(metodo){
    case 1:
    case 2:
    case 3:
      metodoAnalisis();      
      break;
    default:
      break;
  }
  BT.println("AF"); // analisis termina
  Serial.println("Analisis termina");
}

void metodoAnalisis()
{
  int i;
  long v,a,d=voltpsec;
  for(v=voltmin*1000;v<=voltmax*1000;v+=d){
    a = analogRead(A0);
    BT.print("AM;");
    BT.print(v/1000.0);
    BT.print(";");
    BT.println(a/1000.0);
    Serial.print("AM;");
    Serial.print(v/1000.0);
    Serial.print(";");
    Serial.println(a/1000.0);
    delay(1000/d);
  }
}