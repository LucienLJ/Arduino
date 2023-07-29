#include <Wire.h>  // Wire library - used for I2C communication

#include <SPI.h>
#include <SD.h>

//Carte SD
boolean d0;
boolean d1;
int val;
float donnees;
float vit_0;
float vitesse;
float tension;
File fichierSD;

//Accelero
int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out; 

void setup() {
  
  //Accelero
  Serial.begin(9600);
  Wire.begin(); 
  Wire.beginTransmission(ADXL345); 
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  // Enable measurement
  Wire.write(8); // (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  Wire.endTransmission();
  delay(10);

//Carte SD
    pinMode(53, OUTPUT);
 Serial.begin(9600);
 if(!SD.begin(10)) {//10 pour notre carte, peut etre different
 Serial.println(F("Initialisation impossible !"));
 return;
 }
 //Calcul
vit_0=0;
}

void loop() {
  //tension
  
  d0=analogRead(1);
  d1=analogRead(2);
  if (d0==0 and d1==0){tension=1.8;}
  if (d0==0 and d1==1){tension=2.5;}
  if (d0==1 and d1==0){tension=3.3;}
  if (d0==1 and d1==1){tension=3.6;}
   //Capteur de vibration
   val=analogRead(0);
  //Accelero
 
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read()| Wire.read() << 8); 
  X_out = X_out/256; //For a range of +-2g, we need to divide the raw values by 256, according to the datasheet
  Y_out = ( Wire.read()| Wire.read() << 8); 
  Y_out = Y_out/256;
  Z_out = ( Wire.read()| Wire.read() << 8);
  Z_out = Z_out/256;

  //Calcul vitesse 
  
  Serial.println(Y_out);
 // vitesse=vit_0 + Y_out * 0.75;
  //vit_0 = vitesse;
  vitesse = Y_out*9.81;
 Serial.println(vitesse); 
 
  //Carte SD
  
 fichierSD = SD.open("woobled5.csv", FILE_WRITE);

 if(fichierSD) {
 Serial.println(F("Ecriture en cours"));
 //Ecriture*
 fichierSD.print(tension);
 fichierSD.print(",");
 fichierSD.print(val);
 fichierSD.print(",");
 fichierSD.println(vitesse);
 fichierSD.close();
 }
 delay(750);//durée en ms entre les mesures 
}
