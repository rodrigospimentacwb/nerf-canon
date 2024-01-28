#include <Servo.h>

Servo s1;
Servo s2;
Servo s3;
String readString, optservo, serv1pos, serv2pos, serv3pos;

void setup() {
  Serial.begin(9600);
  s1.attach(D1);
  s2.attach(D2);
  s3.attach(D5);

  s1.write(90);
  s2.write(90);
  s3.write(160);

  pinMode(D7, OUTPUT);
  digitalWrite(D7, LOW);
}

void loop() {

  while (Serial.available()) {
    delay(3);
    if (Serial.available() >0) {
      char c = Serial.read();
      readString += c;
    } 
  }

  if (readString.length() > 0) {
      Serial.println(readString);

      optservo = readString.substring(0, 1);

      Serial.println(optservo);

      if(optservo == "x" || optservo == "X") {
        serv1pos = readString.substring(1, (readString.length() -1));
        Serial.println(serv1pos);

        s1.write(serv1pos.toInt());
      }

      if(optservo == "y" || optservo == "Y") {
        serv2pos = readString.substring(1, (readString.length() -1));
        Serial.println(serv2pos);

        s2.write(serv2pos.toInt());
      }

      if(optservo == "t" || optservo == "T") {
        serv3pos = readString.substring(1, (readString.length() -1));
        Serial.println(serv3pos);

        s3.write(serv3pos.toInt());
      }

      if(optservo == "f" || optservo == "F") {
        digitalWrite(D7, HIGH);
        s3.write(0);
        delay(300);
        s3.write(160);
        delay(300);
        digitalWrite(D7, LOW);
      }

      readString="";
  }
}
