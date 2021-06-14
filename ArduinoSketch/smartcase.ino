#include <SoftwareSerial.h>

#define RightMotorSpeed A0
#define LeftMotorSpeed  A1
#define RightMotorDir   A2
#define LeftMotorDir    A3

char incomingByte;
int minSpeed = 450;
int maxSpeed = 700;
int noSpeed = 0;
int forward = 0, back = 0;

void setup() {
  Serial.begin(9600);
  delay(10);
  // put your setup code here, to run once:
  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0 ) {  //если пришли данные || key==1
    incomingByte = Serial.read(); // считываем байт
    if(incomingByte == 'b') {
      digitalWrite(RightMotorDir,HIGH); 
      digitalWrite(LeftMotorDir,HIGH);
      analogWrite(RightMotorSpeed,maxSpeed);
      analogWrite(LeftMotorSpeed,maxSpeed);
      String stringVar =  String(maxSpeed, DEC);
      Serial.println(stringVar); 
    }
    
    if(incomingByte == 'g') {
        digitalWrite(RightMotorDir,LOW); 
        digitalWrite(LeftMotorDir,LOW);
        analogWrite(RightMotorSpeed,maxSpeed);
        analogWrite(LeftMotorSpeed,maxSpeed);
        String stringVar =  String(maxSpeed, DEC);
        Serial.println(stringVar);
    }

    if(incomingByte == 's') {
        digitalWrite(RightMotorDir,LOW); 
        digitalWrite(LeftMotorDir,LOW);
        analogWrite(RightMotorSpeed,noSpeed);
        analogWrite(LeftMotorSpeed,noSpeed);
        String stringVar =  String(noSpeed, DEC);
        Serial.println(stringVar);
    }
    if(incomingByte == 'l') {
        digitalWrite(RightMotorDir,LOW); 
        digitalWrite(LeftMotorDir,HIGH);
        analogWrite(RightMotorSpeed,noSpeed);
        analogWrite(LeftMotorSpeed,maxSpeed);
        String stringVar =  String(noSpeed, DEC);
        Serial.println(stringVar);
    }  
    if(incomingByte == 'r') {
        digitalWrite(RightMotorDir,HIGH); 
        digitalWrite(LeftMotorDir,LOW);
        analogWrite(RightMotorSpeed,maxSpeed);
        analogWrite(LeftMotorSpeed,noSpeed);
        String stringVar =  String(noSpeed, DEC);
        Serial.println(stringVar);
    }  
  }
}
