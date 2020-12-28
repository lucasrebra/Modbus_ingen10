#include <Arduino.h>

#ifdef ESP8266
 #include <ESP8266WiFi.h>
#else //ESP32
 #include <WiFi.h>
#endif
#include <ModbusIP_ESP8266.h>
#include <ModbusRTU.h>

#define TO_REG 10
#define SLAVE_ID 1
#define PULL_ID 1
#define FROM_REG 20

ModbusRTU mb1;
ModbusIP mb2;

int segs=0;
int ts=0;

int registros[10]={10,20,30,40,50,60,70,80,90,100}; 

void CambiaReg(){
  //esta funcion recoge los valores de registros y setea los Hreg
  for(int n=0;n<10;n++){
    if(mb1.Hreg(n)!=registros[n]){
      registros[n]=mb1.Hreg(n);
    }
    if(mb2.Hreg(n)!=registros[n]){
      registros[n]=mb2.Hreg(n);
    }
  }

  for(int n=0;n<10;n++){
    mb1.addHreg(n,registros[n],1);
    mb2.addHreg(n,registros[n],1);
  }

}

void setup() {

  int registros[10]={10,20,30,40,50,60,70,80,90,100}; 

  Serial.begin(115200);
   WiFi.begin("twave-24", "KD6rUYrv");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Serial1.begin(9600, SERIAL_8N1); // Init Serial on default pins
  //Serial2.begin(19200, SERIAL_8N1, 19, 18); // Override default pins for ESP32
  mb1.begin(&Serial1);
  //mb1.begin(&Serial2, 17);  // Specify RE_DE control pin
  mb1.slave(1);
  //mb1.addHreg(0,registros[1],10);
  mb1.addHreg(0,registros[0],1);
  mb1.addHreg(1,registros[1],1);
  mb1.addHreg(2,registros[2],1);
  mb1.addHreg(3,registros[3],1);
  mb1.addHreg(4,registros[4],1);
  mb1.addHreg(5,registros[5],1);
  mb1.addHreg(6,registros[6],1);
  mb1.addHreg(7,registros[7],1);
  mb1.addHreg(8,registros[8],1);
  mb1.addHreg(9,registros[9],1);

  mb1.addHreg(30,0,1);

  mb2.server();
  //mb2.addHreg(0,registros[1],10);
  


  
/*
  mb2.addHreg(0,registros[0],1);
  mb2.addHreg(1,registros[1],1);
  mb2.addHreg(2,registros[2],1);
  mb2.addHreg(3,registros[3],1);
  mb2.addHreg(4,registros[4],1);
  mb2.addHreg(5,registros[5],1);
  mb2.addHreg(6,registros[6],1);
  mb2.addHreg(7,registros[7],1);
  mb2.addHreg(8,registros[8],1);
  mb2.addHreg(9,registros[9],1);*/


  

}

void loop() {
    
  if ( millis() > ts + 2000 ) 
  {
    segs++;
    ts = millis();
    mb1.Hreg(30,segs);//Escribimos en la direccion RTU los segundos
    
  }

  //CambiaReg();

  if(!mb1.slave())
    mb1.pullHreg(PULL_ID, FROM_REG, TO_REG);
  mb1.task();
  mb2.task();
  

  delay(10);
}