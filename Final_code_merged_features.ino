#include <Servo.h>
Servo servo;
int trigPin1 = 8;
int echoPin1 = 9;
int servoPin = 7;
int led1 = 13;

int led2 = 4;
int trigPin2 = 2;
int echoPin2 = 3;

int smokeA0 = A0;
int buzzer = 11;

long duration, dist, average;
long aver[3]; // array for average
float sensorValue;

void setup(){
  Serial.begin(9600); //for serial communication
  servo.attach(servoPin);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  servo.write(0); // close cap on power on
  delay(100);
  servo.detach();

  pinMode(led1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(led2, OUTPUT);

  pinMode(buzzer, OUTPUT);
  pinMode(smokeA0, INPUT);
  Serial.println("Gas sensor warming up !");
  delay(2000);
  noTone(buzzer);
}

void measure(){
  digitalWrite(10, HIGH);
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(15);
  digitalWrite(trigPin1, LOW);
  pinMode(echoPin1, INPUT);
  duration = pulseIn(echoPin1, HIGH);
  dist = (duration/2)/29.1; // obtain distance
}

void loop(){
  for (int i=0; i<=2; i++){ // average distance
    measure();
    aver[i] = dist;
    delay(10); // delay between measurements
  }
  dist = (aver[0]+aver[1]+aver[2]) / 3;

  if (dist<30){
    servo.attach(servoPin);
    delay(1);
    servo.write(0);
    delay(4000); // 4 seconds delay
    servo.write(150);
    delay(1000);
    servo.detach();
  }
  Serial.print("Distance: ");
  Serial.println(dist);

  // Ultrasonic sensor for LED
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration = pulseIn(echoPin2, HIGH);
  int distance = duration*0.034/2;  // converting the speed from (m/s) to (cm/μs)

  Serial.print("Distance: ");
  Serial.println(distance);
  if (distance<=5){
    digitalWrite(led2, HIGH);
  } 
  else{
    digitalWrite(led2, LOW);
  }

  // Smoke sensor
  sensorValue = analogRead(smokeA0);
  Serial.print("Sensor Value: ");
  Serial.print(sensorValue);

  if (sensorValue>100){
    Serial.print("| Smoke detected!");
    tone(buzzer, 1000, 200);
  } else {
    noTone(buzzer);
  }
  Serial.println("");
  delay(200);
}
