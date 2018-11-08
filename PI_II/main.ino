#include <SoftwareSerial.h>
//Carrega a biblioteca Wire
#include<Wire.h>

//Usaremos os pinos 8 e 9, como RX e TX, respectivamente.
SoftwareSerial mySerial(8, 9);  

//Endereco I2C do MPU6050
const int MPU=0x68; 

//Variaveis para armazenar valores dos sensores
int AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void setup() { //Inicializa a porta serial
  Serial.begin(9600);
  mySerial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  //Inicializa o MPU-6050
  Wire.write(0); 
  Wire.endTransmission(true);
} //Fim da setup
 
void loop() {

  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  //Solicita os dados do sensor
  Wire.requestFrom(MPU,14,true);  
  GyX=Wire.read()<<8|Wire.read();  //0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=Wire.read()<<8|Wire.read();  //0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=Wire.read()<<8|Wire.read();  //0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
   
  //Envia valor X do giroscopio para a serial e Bluetooth
  Serial.print(" | GyX = "); Serial.print(GyX); mySerial.print(GyX); mySerial.print(" | ");
   
  //Envia valor Y do giroscopio para a serial e o LCD  
  Serial.print(" | GyY = "); Serial.print(GyY); mySerial.print(GyY); mySerial.print(" | ");
   
  //Envia valor Z do giroscopio para a serial e o LCD
  Serial.print(" | GyZ = "); Serial.println(GyZ); mySerial.println(GyZ);
   
  //Aguarda 1000 ms e reinicia o processo
  delay(1000);
}
