#include <SoftwareSerial.h>
#include <Wire.h>
#include "dht.h"
#define dht_apin A0 // Analog Pin DHT11 sensor is connected to
#define CO_apin A1 // Analog Pin CO(MQ-7) sensor is connected to
#define GAS_apin A2 // Analog Pin GAS(MQ-5) sensor is connected to
#define buzzer_dpin 12 //Digital Pin Buzzer is connected to
#define greenLed_dpin 11 //Digital Pin red led is connected to
#define redLed_dpin 10 //Digital Pin green led is connected to
#define fan1 9 //Digital Pin(INB) fan is connected to
#define fan2 8 //Digital Pin(INA) fan is connected to

//Tresholds
#define TEMP_TRE 25
#define HUM_TRE 85
#define GAS_TRE 500
#define CO_TRE 200

int COsensor_value;
int GASsensor_value;
int alert=0;
char message_T[30];
char message_H[30];
char message_G[30];
char message_C[30];
dht DHT;

SoftwareSerial BTserial(0, 1); //RX | TX
 
void setup(){
 
  //Serial.begin(9600);
  BTserial.begin(9600);
  pinMode(buzzer_dpin, OUTPUT);
  pinMode(greenLed_dpin, OUTPUT);
  pinMode(redLed_dpin, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);
  delay(500);//Delay to let system boot
  //Serial.println("DHT11 Humidity & temperature Sensor | CO sensor | GAS sensor\n\n");
  delay(30000);//Wait before accessing Sensor
 
}
 
void loop(){
  //Start of Program 
 
    DHT.read11(dht_apin);
    COsensor_value = analogRead(CO_apin);
    GASsensor_value = analogRead(GAS_apin);

    BTserial.print(DHT.temperature);  
    BTserial.print(",");  
    BTserial.print(DHT.humidity);  
    BTserial.print(",");  
    BTserial.print(COsensor_value);  
    BTserial.print(",");  
    BTserial.print(GASsensor_value);  
    BTserial.print(",");  

     if(DHT.temperature > TEMP_TRE || DHT.humidity >HUM_TRE || GASsensor_value>GAS_TRE || COsensor_value >CO_TRE)
    {
        if(GASsensor_value>GAS_TRE)
        {
          strcpy(message_G, "GAS_leak");
          alert=1;
        }
        else
          strcpy(message_G, "GAS_good");
        if(COsensor_value > CO_TRE)
        {
          strcpy(message_C, "CO_leak");
          alert=2;
        }
        else
          strcpy(message_C, "CO_good");
        if(DHT.humidity > HUM_TRE)
        {
          strcpy(message_H, "Reduce_hum");
          alert=3;
        }
        else
          strcpy(message_H, "Hum_good");
        if(DHT.temperature >TEMP_TRE)
        {
          strcpy(message_T, "Reduce_temp");
          alert=4;
        }
        else
          strcpy(message_T, "Temp_good");
      
      analogWrite(fan2, 255);
      digitalWrite(fan1,LOW);  
      digitalWrite(buzzer_dpin, HIGH);
      digitalWrite(redLed_dpin, HIGH);
      digitalWrite(greenLed_dpin, LOW);
    }
    else
    {
      strcpy(message_G, "GAS_good");
      strcpy(message_C, "CO_good");
      strcpy(message_H, "Hum_good");
      strcpy(message_T, "Temp_good");
      alert=0;

      digitalWrite(fan1, LOW);
      digitalWrite(fan2, LOW);
      digitalWrite(buzzer_dpin, LOW);
      digitalWrite(redLed_dpin, LOW);
      digitalWrite(greenLed_dpin, HIGH);
    }
    BTserial.print(message_T);  
    BTserial.print(",");
    BTserial.print(message_H);  
    BTserial.print(",");
    BTserial.print(message_G);  
    BTserial.print(",");
    BTserial.print(message_C);  
    BTserial.print(",");
    BTserial.print(alert);  
    BTserial.print(";");
   
    delay(2000);//Wait 2 seconds before reading sensors again.
 
}
