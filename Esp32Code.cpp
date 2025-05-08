#include <WiFi.h>
#include <WebServer.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

const char* ssid = "ESP32-Network";
const char* password = "123456789";

const int motorPin1 = 14;
const int motorPin2 = 12;
const int motorPin3 = 27;
const int motorPin4 = 26;
const int enableA = 25;  
const int enableB = 33;  
int motorSpeed = 100;    

const int RECV_PIN = 4;  
IRrecv irrecv(RECV_PIN);
decode_results results;


SoftwareSerial mySerial(16, 17); 
DFRobotDFPlayerMini myDFPlayer;
WebServer server(80);


void playSound(int track) {
  myDFPlayer.play(track);
}


void setMotorSpeed(int speed) {
  analogWrite(enableA, speed);
  analogWrite(enableB, speed);
}

void moveForward() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  setMotorSpeed(motorSpeed);
}

void moveBackward() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  setMotorSpeed(motorSpeed);
}

void rotateLeft() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
  setMotorSpeed(motorSpeed);
}

void rotateRight() {
  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
  setMotorSpeed(motorSpeed);
}

void stopRobot() {
  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
  setMotorSpeed(0);
}


void handleMoveForward() { moveForward(); server.send(200, "text/plain", "Moving forward"); }
void handleMoveBackward() { moveBackward(); server.send(200, "text/plain", "Moving backward"); }
void handleRotateLeft() { rotateLeft(); server.send(200, "text/plain", "Rotating left"); }
void handleRotateRight() { rotateRight(); server.send(200, "text/plain", "Rotating right"); }
void handleStop() { stopRobot(); server.send(200, "text/plain", "Stopping"); }
void handleSpeed() { motorSpeed = server.arg("value").toInt(); server.send(200, "text/plain", "Speed Updated"); }
void handleStartSound() { playSound(1); server.send(200, "text/plain", "Start Sound"); }
void handleTargetReachedSound() { playSound(2); server.send(200, "text/plain", "Target Reached Sound"); }

void setup() {
  Serial.begin(115200);

  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);
  pinMode(enableA, OUTPUT);
  pinMode(enableB, OUTPUT);


  WiFi.softAP(ssid, password);
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());


  mySerial.begin(9600);
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("DFPlayer Mini not detected!");
  } else {
    myDFPlayer.volume(10);
  }

  irrecv.enableIRIn();

  server.on("/move_forward", handleMoveForward);
  server.on("/move_backward", handleMoveBackward);
  server.on("/rotate_left", handleRotateLeft);
  server.on("/rotate_right", handleRotateRight);
  server.on("/stop", handleStop);
  server.on("/set_speed", handleSpeed);
  server.on("/start_sound", handleStartSound);
  server.on("/target_reached_sound", handleTargetReachedSound);

  server.begin();
}

void loop() {
  server.handleClient();


  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
}ip.dst == 40.126.35.80
