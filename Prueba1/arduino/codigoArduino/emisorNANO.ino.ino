//Written by Ahmet Burkay KIRNIK
//TR_CapaFenLisesi
//Measure Angle with a MPU-6050(GY-521)

#include<Wire.h>
#include <SoftwareSerial.h>

const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

int minVal=265;
int maxVal=402;

double z;
double angle;

SoftwareSerial mySerial(4, 5); // RX, TX

///////////////////////////////////////////////////////////
const int numReadings = 40;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin = A0;
///////////////////////////////////////////////////////////
 
void setup(){  
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  mySerial.begin(9600);
  mySerial.flush();

  Serial.begin(9600);
  Serial.flush();

  ////////////////////////////////////////////////////////
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  ////////////////////////////////////////////////////////
}
void loop(){
  
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true);
  AcX=Wire.read()<<8|Wire.read();
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  
    int xAng = map(AcX,minVal,maxVal,-90,90);
    int yAng = map(AcY,minVal,maxVal,-90,90);
    int zAng = map(AcZ,minVal,maxVal,-90,90);
    
       z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);

       //z = analogRead(0); prueba con un pot

       if (z < 10){
        z+=360;
       }

     angle = (0.0002*pow(z,2))+(-0.9099*z)+(226.77);

      if(angle < 0){
        angle += 5;
      }
     angle = int(angle);

     ////////////////////////////////////////////////
       total = total - readings[readIndex];
       readings[readIndex] = angle;
       total = total + readings[readIndex];
       readIndex = readIndex + 1;

       if (readIndex >= numReadings) {
       // ...wrap around to the beginning:
       readIndex = 0;
       }
       average = total / numReadings;
       /////////////////////////////////////////////
      
     mySerial.print(average);
     mySerial.print('\n');
     Serial.println(average);
     Serial.println("-----------------------------------");

     //delay(10);
}
