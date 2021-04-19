#include <EEPROM.h>
#include <SoftwareSerial.h> //Create software serial object to communicate with SIM800L
SoftwareSerial GSM(8, 9);   //SIM800L Tx & Rx is connected to Arduino #8 & #9

#include <DFPlayer_Mini_Mp3.h>

char *phone_no = "+923378655465"; //change +92 with country code and 3378655465 with phone number to sms

unsigned long currentTime;
unsigned long loopTime1;
unsigned long loopTime2;

#define Relay1 2 // Load1 Pin Out
#define Relay2 3 // Load2 Pin Out
#define Relay3 4 // Load3 Pin Out
#define Relay4 5 // Load4 Pin Out

int load1, load2, load3, load4;

void setup() {// put your setup code here, to run once

pinMode(Relay1, OUTPUT); digitalWrite(Relay1, 1); 
pinMode(Relay2, OUTPUT); digitalWrite(Relay2, 1); 
pinMode(Relay3, OUTPUT); digitalWrite(Relay3, 1); 
pinMode(Relay4, OUTPUT); digitalWrite(Relay4, 1); 

Serial.begin(9600);//Begin serial communication with Arduino and Arduino IDE (Serial Monitor)  
GSM.begin(9600);   //Begin serial communication with Arduino and SIM800L

mp3_set_serial (Serial);
mp3_set_volume (29);

//Serial.println("Initializing....");
initModule("AT","OK",300);          //Scan for GSM Module
initModule("ATE0","OK",300);
initModule("AT+CPIN?","READY",300);
initModule("AT+CLIP=1","OK",300);  
initModule("AT+DDET=1","OK",300);
//Serial.println("Initialized Successfully");  

load1 = EEPROM.read(1);
load2 = EEPROM.read(2);
load3 = EEPROM.read(3);
load4 = EEPROM.read(4);

relays();
}

void loop(){
currentTime = millis(); 
if(currentTime >= (loopTime1 + 2000)){
initModule("AT","OK",300);   
loopTime1 = currentTime;
}
    
if(GSM.find("CLIP:")) {
 //Serial.println("RING!");
 delay(200);

while(1){     
if(GSM.find(phone_no)){ 
initModule("ATA","OK",300);      
delay(1500); 
mp3_play (28);
delay(4000); 
mp3_play (0);
delay(3000);
loopTime2 = currentTime;
while (1){   
   currentTime = millis();  
if(currentTime >= (loopTime2 + 30000)){
  //Serial.println("Call End");
  loopTime2 = currentTime; 
  break;
}       

if(GSM.find("+DTMF:")){
  int Data = GSM.parseInt();
  switch(Data){                                

case 1: {
   initModule("AT+DDET=0","OK",300);  
   load1=!load1;
   eeprom_write();
   relays();
    if(load1==0){mp3_play (1);}
            else{mp3_play (2);}       
   initModule("AT+DDET=1","OK",300);
loopTime2 = currentTime;    
   break;}

case 2: {
   initModule("AT+DDET=0","OK",300);  
   load2=!load2;
   eeprom_write();
   relays();
    if(load2==0){mp3_play (3);}
            else{mp3_play (4);}       
   initModule("AT+DDET=1","OK",300);
loopTime2 = currentTime;   
   break;}

case 3: {
   initModule("AT+DDET=0","OK",300);  
   load3=!load3;
   eeprom_write();
   relays();
    if(load3==0){mp3_play (5);}
            else{mp3_play (6);}      
   initModule("AT+DDET=1","OK",300);   
loopTime2 = currentTime;
   break;}

case 4: {
   initModule("AT+DDET=0","OK",300);  
   load4=!load4;
   eeprom_write();
   relays();
    if(load4==0){mp3_play (7);}
            else{mp3_play (8);}      
   initModule("AT+DDET=1","OK",300);
loopTime2 = currentTime;     
   break;}

case 8: {
   initModule("AT+DDET=0","OK",300);  
   load1=0,load2=0,load3=0,load4=0;
   eeprom_write();
   relays();
   mp3_play (9);   
   initModule("AT+DDET=1","OK",300); 
loopTime2 = currentTime;   
   break;}


case 0: {
   initModule("AT+DDET=0","OK",300);  
   load1=1,load2=1,load3=1,load4=1;
   eeprom_write();
   relays();
   mp3_play (10);    
   initModule("AT+DDET=1","OK",300);   
loopTime2 = currentTime;   
   break;}

case 9: {
   initModule("AT+DDET=0","OK",300);  
   if(load1==0 && load2==0 && load3==0 && load4==0){ 
      mp3_play (19); 
   }else{   
    if(load1==1 && load2==1 && load3==1 && load4==1){
      mp3_play (20);
    }else{
     if(load1==0){mp3_play (11);}
             else{mp3_play (12);}
     delay(2000);
     if(load2==0){mp3_play (13);}
             else{mp3_play (14);}
     delay(2000);   
     if(load3==0){mp3_play (15);}
             else{mp3_play (16);}
     delay(2000);       
     if(load4==0){mp3_play (17);}
             else{mp3_play (18);}    
    }
   } 
   initModule("AT+DDET=1","OK",300);   
loopTime2 = currentTime;   
   break;}

case 7: {
initModule("AT+DDET=0","OK",300);   
mp3_play (21);delay(2000); 
mp3_play (22);delay(2000);  
mp3_play (23);delay(2000);  
mp3_play (24);delay(2000);  
mp3_play (25);delay(2000);  
mp3_play (26);delay(2000);  
mp3_play (27);
initModule("AT+DDET=1","OK",300); 
loopTime2 = currentTime;    
   break;}
    } 
   }
  }   
 } 
else {
initModule("AT","OK",300); 
break;            
 } 
}
 
initModule("AT+CHUP;","OK",300); 
 }
}

void eeprom_write(){
EEPROM.write(1,load1);
EEPROM.write(2,load2);
EEPROM.write(3,load3);
EEPROM.write(4,load4);  
}

void relays(){  
digitalWrite(Relay1, load1); 
digitalWrite(Relay2, load2); 
digitalWrite(Relay3, load3); 
digitalWrite(Relay4, load4); 
}

 // Init GSM Module 
void initModule(String cmd, char *res, int t){
while(1){
    //Serial.println(cmd);
    GSM.println(cmd);
    delay(100);
    while(GSM.available()>0){
       if(GSM.find(res)){
        //Serial.println(res);
        delay(t);
        return;
       }else{//Serial.println("Error");
       }}
    delay(t);
  }
}   
  

