String incomingData;      // a variable to read incoming serial data into

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  // initialize the LED pin as an output:
}

void loop() {
  // see if there's incoming serial data:

  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingData = Serial.readStringUntil("\n");
  }

  // Para mandar los datos a la HMI hay que separarlos con comas
  //Primero mandar el angulo medido por el sensor
  //Despues mandar el error
  //Y por ultimo la manipulacion

  Serial.print(map(analogRead(A0),0,1024,0,254));
  Serial.print(",");
  Serial.print(map(analogRead(A0),0,1024,254,500));
  Serial.print(",");
  Serial.print(map(analogRead(A0),0,1024,500,750));
  Serial.print(",");
  Serial.println(incomingData);

}
