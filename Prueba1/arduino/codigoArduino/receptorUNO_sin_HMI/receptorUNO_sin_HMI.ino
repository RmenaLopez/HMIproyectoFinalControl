//Uno reciever
// read two ints from a Uno
// see Uno_transmitter.ino

#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

int botonPin = 8;
int pulso = 0;
int pot = 2;
int SetPoint = 0;
int angulo;
int control;

float T = 0.09;
float matrizE[13];
float matrizM[13];
float td = 0.9*0.8;

float a = exp(-T/td);

int i;

float ki = 0.2344, kc = 0.325, kd = 0.0901;
float realki = 0.2344, realkc = 0.325, realkd = 0.0901;

String info;
String ctrl;
String sp;

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup()  
{
 Serial.begin(9600);
 Serial.flush();
 mySerial.begin(9600);
 mySerial.flush();

 pinMode(botonPin, INPUT);
}

void loop() 
{
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    info = Serial.readStringUntil("\n");
    ctrl = getValue(info, ',', 0);
    control = ctrl.toInt();
    sp = getValue(info, ',', 1);
    SetPoint = sp.toInt() + 60;
    
  }
  
 if (mySerial.available()) {   //read the two speeds

   angulo = mySerial.parseInt() + 60; 

   /*for(i=12;i==1;i--){
    matrizE[i] = matrizE[i-1];
    }*/

    matrizE[12] = matrizE[11];
    matrizE[11] = matrizE[10];
    matrizE[10] = matrizE[9];
    matrizE[9] = matrizE[8];
    matrizE[8] = matrizE[7];
    matrizE[7] = matrizE[6];
    matrizE[6] = matrizE[5];
    matrizE[5] = matrizE[4];
    matrizE[4] = matrizE[3];
    matrizE[3] = matrizE[2];
    matrizE[2] = matrizE[1];
    matrizE[1] = matrizE[0];
    
   matrizE[0] = SetPoint - angulo;

   /*for(i=12;i==1;i--){
    matrizM[i] = matrizM[i-1];
    }*/

    matrizM[12] = matrizM[11];
    matrizM[11] = matrizM[10];
    matrizM[10] = matrizM[9];
    matrizM[9] = matrizM[8];
    matrizM[8] = matrizM[7];
    matrizM[7] = matrizM[6];
    matrizM[6] = matrizM[5];
    matrizM[5] = matrizM[4];
    matrizM[4] = matrizM[3];
    matrizM[3] = matrizM[2];
    matrizM[2] = matrizM[1];
    matrizM[1] = matrizM[0];
    
   if (control == 1){   //PID
      kc = realkc;
      ki = realki;
      kd = realkd;
      matrizM[0] = ((kc+ki*T/2+kd*2/T)*matrizE[0]) +2*(((ki*T/2)-2*kd/T)*matrizE[1]) + ((-kc+ki*T/2+2*kd/T)*matrizE[2]) + matrizM[2] ;
   }
   if (control == 2){   //PI
      kc = realkc;
      ki = realki;
      kd = 0;
      matrizM[0] = ((kc+ki*T/2+kd*2/T)*matrizE[0]) +2*(((ki*T/2)-2*kd/T)*matrizE[1]) + ((-kc+ki*T/2+2*kd/T)*matrizE[2]) + matrizM[2] ;
   }
   if (control == 3){   //P
      kc = realkc;
      ki = 0;
      kd = 0;
      matrizM[0] = ((kc+ki*T/2+kd*2/T)*matrizE[0]) +2*(((ki*T/2)-2*kd/T)*matrizE[1]) + ((-kc+ki*T/2+2*kd/T)*matrizE[2]) + matrizM[2] ;
   }
    if (control == 4){   //PD
      kc = realkc;
      ki = 0;
      kd = realkd;
      matrizM[0] = ((kc+ki*T/2+kd*2/T)*matrizE[0]) +2*(((ki*T/2)-2*kd/T)*matrizE[1]) + ((-kc+ki*T/2+2*kd/T)*matrizE[2]) + matrizM[2] ;
    }
    if (control == 5){  //ID
      kc = 0;
      ki = realki;
      kd = realkd;
      matrizM[0] = ((kc+ki*T/2+kd*2/T)*matrizE[0]) +2*(((ki*T/2)-2*kd/T)*matrizE[1]) + ((-kc+ki*T/2+2*kd/T)*matrizE[2]) + matrizM[2] ;
    }
    if (control == 6){  //I
      kc = 0;
      ki = realki;
      kd = 0;
      matrizM[0] = ((kc+ki*T/2+kd*2/T)*matrizE[0]) +2*(((ki*T/2)-2*kd/T)*matrizE[1]) + ((-kc+ki*T/2+2*kd/T)*matrizE[2]) + matrizM[2] ;
    }
    if (control == 7){  //D
      kc = 0;
      ki = 0;
      kd = realkd;
      matrizM[0] = ((kc+ki*T/2+kd*2/T)*matrizE[0]) +2*(((ki*T/2)-2*kd/T)*matrizE[1]) + ((-kc+ki*T/2+2*kd/T)*matrizE[2]) + matrizM[2] ;
    }
    if (control == 8){  //DAHLIN
      matrizM[0] = -(1-a)*matrizM[1] + a*matrizM[2] + (1-a)*matrizM[11] + (1-a)*matrizM[12] + (1-a)*matrizE[1]/0.1419 - (1-a)*matrizE[2]*0.9043/0.1419;
    }
    
      if (matrizM[0] < 0){
      matrizM[0] = 0;
    }
   if (matrizM[0] > 90){
      matrizM[0] = 90;
    }

   analogWrite(9,255*matrizM[0]/100);
    
    }
   // One-step
   //matrizM[0] = (7.047*matrizE[1] - 6.373*matrizE[2] - matrizM[1] + matrizM[11] + matrizM[12])

  Serial.print(angulo);
  Serial.print(",");
  Serial.print(matrizE[0]);
  Serial.print(",");
  Serial.print(matrizM[0]);
  Serial.print(",");
  Serial.println(control);
 
delay (T*100);

}


