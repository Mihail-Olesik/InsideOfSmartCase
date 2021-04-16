#include <ESP8266WiFi.h>
#include "TinyGPS++.h"
#include <SoftwareSerial.h>

#define RightMotorSpeed 5
#define RightMotorDir 0
#define LeftMotorSpeed 4
#define LeftMotorDir 2
#define PIN_TRIG 12
#define PIN_ECHO 11

char incomingByte; // входящие данные
int minSpeed = 450;
int maxSpeed = 700;
int noSpeed = 0;
int forward = 0, back = 0;

TinyGPSPlus gps_parser;
SoftwareSerial bluetooth_serial(D3, D2);
SoftwareSerial gps_serial(D8, D7);

const char *ssid = "TP-LINK_841";  //type your ssid
const char *password = "a8235877"; //type your password

bool cantMove = false;

const char index_html[] PROGMEM = {"<!DOCTYPE html>\n"
                                   "<html lang=\"en\">\n"
                                   "<head>\n"
                                   "</head>\n"
                                   "<body style=\"width: 600px; height: 600px\">\n"
                                   "<form action=\"/PLUS/\">"
                                   "<button type=\"submit\">+</button>"
                                   "</form>"
                                   "<p>Light:"};

const char index_html2[] PROGMEM = {
    "</p>"
    "<form action=\"/MINUS/\">"
    "<button type=\"submit\">-</button>"
    "</form>"
    "  <br>\n"
    "  <a href=\"/FORWARD\"><img src=\"https://pp.userapi.com/c845120/v845120450/1afee0/Wk5gBFXqjg0.jpg\" width=\"100\" height=\"100\" hspace=\"110\"></a>\n"
    "  <br>\n"
    "  <a href=\"/RIGHT\"><img src=\"https://pp.userapi.com/c845120/v845120450/1afec8/ri2ipm2X1kg.jpg\" width=\"100\" height=\"100\"></a>\n"
    "  <a href=\"/STOP\"><img src=\"https://pp.userapi.com/c845120/v845120450/1afed8/nLvJ6Hw3K3E.jpg\" width=\"110\" height=\"110\"></a>\n"
    "  <a href=\"/LEFT\"><img src=\"https://pp.userapi.com/c845120/v845120450/1afed0/mzTeh6MljWQ.jpg\" width=\"100\" height=\"100\"></a>\n"
    "  <br>\n"
    "  <a href=\"/BACK\"><img src=\"https://pp.userapi.com/c845120/v845120450/1afec0/XoUr8PofyxM.jpg\" width=\"100\" height=\"100\" hspace=\"110\"></a>\n"
    "  <br>\n"
    "  <br>\n"
    "  <br>\n"
    "  <a href=\"/MOVE_RIGHT\"><img src=\"https://pp.userapi.com/c845121/v845121048/1b3477/TmTGRI6nkmQ.jpg\" width=\"100\" height=\"100\" hspace=\"40\"></a>\n"
    "  <a href=\"/MOVE_LEFT\"><img src=\"https://pp.userapi.com/c845121/v845121048/1b346f/sSPOwO6Dlbs.jpg\" width=\"100\" height=\"100\"></a>\n"
    "\n"
    "</body>\n"
    "</html>"};

WiFiServer server(80); //Service Port

//Function, which read WiFi request and make needed action
void checkWiFiRequest(WiFiClient client)
{ 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // If obstacle
  if(cantMove)
  {
    return;
  }

  Serial.println(maxSpeed);
  // Match the request

  int value = LOW;
  if (request.indexOf("/PLUS") != -1)
  {
    if (maxSpeed == 1000)
    {
      maxSpeed = 1000;
    }
    else
    {
      maxSpeed = maxSpeed + 50;
    }
    Serial.println("New speed: ");
    Serial.println(maxSpeed);
  }

  if (request.indexOf("/MINUS") != -1)
  {
    if (maxSpeed == 450)
    {
      maxSpeed = 450;
    }
    else
    {
      maxSpeed = maxSpeed - 50;
    }

    Serial.println("New speed: ");
    Serial.println(maxSpeed);
  }

  //back
  if (request.indexOf("/BACK") != -1 || back == 1)
  {
    digitalWrite(RightMotorDir, LOW);
    digitalWrite(LeftMotorDir, LOW);
    analogWrite(RightMotorSpeed, maxSpeed);
    analogWrite(LeftMotorSpeed, maxSpeed);
    forward = 0;
    back = 1;
  }
  //forward
  if (request.indexOf("/FORWARD") != -1 || forward == 1)
  {
    digitalWrite(RightMotorDir, HIGH);
    digitalWrite(LeftMotorDir, HIGH);
    analogWrite(RightMotorSpeed, maxSpeed);
    analogWrite(LeftMotorSpeed, maxSpeed);
    forward = 1;
    back = 0;
  }

  //stop
  if (request.indexOf("/STOP") != -1)
  {
    digitalWrite(RightMotorDir, LOW);
    digitalWrite(LeftMotorDir, LOW);
    analogWrite(RightMotorSpeed, noSpeed);
    analogWrite(LeftMotorSpeed, noSpeed);
    forward = 0;
    back = 0;
  }

  //right
  if (request.indexOf("/RIGHT") != -1)
  {
    digitalWrite(RightMotorDir, HIGH);
    digitalWrite(LeftMotorDir, HIGH);
    analogWrite(RightMotorSpeed, maxSpeed);
    analogWrite(LeftMotorSpeed, maxSpeed - 200);
    forward = 0;
    back = 0;
  }

  //left
  if (request.indexOf("/LEFT") != -1)
  {
    digitalWrite(RightMotorDir, HIGH);
    digitalWrite(LeftMotorDir, HIGH);
    analogWrite(RightMotorSpeed, maxSpeed - 200);
    analogWrite(LeftMotorSpeed, maxSpeed);
    forward = 0;
    back = 0;
  }

  if (request.indexOf("/MOVE_RIGHT") != -1)
  {
    digitalWrite(RightMotorDir, HIGH);
    digitalWrite(LeftMotorDir, LOW);
    analogWrite(RightMotorSpeed, maxSpeed);
    analogWrite(LeftMotorSpeed, maxSpeed);
    forward = 0;
    back = 0;
  }
  if (request.indexOf("/MOVE_LEFT") != -1)
  {
    digitalWrite(RightMotorDir, LOW);
    digitalWrite(LeftMotorDir, HIGH);
    analogWrite(RightMotorSpeed, maxSpeed);
    analogWrite(LeftMotorSpeed, maxSpeed);
    forward = 0;
    back = 0;
  }

  client.println(index_html);
  client.print(maxSpeed);
  client.println(index_html2);

  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
}

//Send coordinats in JSON format
void sendCoordinatsByBT(String lat, String lng)
{
  bluetooth_serial.println("{ " + lat + ", " + lng + " }");
}

void setup()
{
  Serial.begin(9600);
  gps_serial.begin(9600);
  //Serial.begin(115200);
  delay(10);

  pinMode(RightMotorSpeed, OUTPUT);
  pinMode(LeftMotorSpeed, OUTPUT);
  pinMode(RightMotorDir, OUTPUT);
  pinMode(LeftMotorDir, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop()
{
//  if (Serial.available() > 0)
//  {                               //если пришли данные || key==1
//    incomingByte = Serial.read(); // считываем байт
//    
//    if (incomingByte == '0')
//    {
//      digitalWrite(RightMotorDir, HIGH);
//      digitalWrite(LeftMotorDir, HIGH);
//      analogWrite(RightMotorSpeed, maxSpeed);
//      analogWrite(LeftMotorSpeed, maxSpeed);
//      String stringVar = String(maxSpeed, DEC);
//      Serial.println(stringVar);
//    }
//
//    if (incomingByte == '1')
//    {
//      digitalWrite(RightMotorDir, LOW);
//      digitalWrite(LeftMotorDir, LOW);
//      analogWrite(RightMotorSpeed, maxSpeed);
//      analogWrite(LeftMotorSpeed, maxSpeed);
//      String stringVar = String(maxSpeed, DEC);
//      Serial.println(stringVar);
//    }
//
//    if (incomingByte == '2')
//    {
//      digitalWrite(RightMotorDir, LOW);
//      digitalWrite(LeftMotorDir, LOW);
//      analogWrite(RightMotorSpeed, noSpeed);
//      analogWrite(LeftMotorSpeed, noSpeed);
//      String stringVar = String(noSpeed, DEC);
//      Serial.println(stringVar);
//    }
//  }

  // Check if a client has connected
  WiFiClient wifiClient = server.available();
  if (!wifiClient)
  {
    return;
  }

  //Get data from GPS module
  while(gps_serial.available() > 0) {
    char temp = gps_serial.read();
    Serial.write(temp);
    gps_parser.encode(temp);
  }

  String lat  = "lat";
  String lng  = "lng";
  if(gps_parser.location.isValid()) {
      lat = "\"Lat\": " + String(gps_parser.location.lat(), 6);
      lng = "\"Lng\": " + String(gps_parser.location.lng(), 6);
  }

  Serial.println("Coordinats:");
  Serial.println(lat);
  Serial.println(lng);

  sendCoordinatsByBT(lat, lng);

  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);

  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  long duration = pulseIn(PIN_ECHO, HIGH);
  long cm = (duration / 2) / 29.1;

  //Is obstacle
  cantMove = (cm < 5) ? true : false;
  if(cantMove)
  {
    digitalWrite(RightMotorDir, LOW);
    digitalWrite(LeftMotorDir, LOW);
    analogWrite(RightMotorSpeed, noSpeed);
    analogWrite(LeftMotorSpeed, noSpeed);
    forward = 0;
    back = 0;
  }

  // Check if client sends some data
  if (wifiClient.available())
  {
    Serial.println("new client");
    checkWiFiRequest(wifiClient);
  }  
}
