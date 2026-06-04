#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include <HTTPClient.h>

WiFiMulti wifiMulti;

int led1 = 2;
int led2 = 3;
int led3 = 4;
int buzzer = 5;
int trig = 8;
int echo = 9;
const char* serverIP = "192.168.1.199";


void setup() {

  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %u...\n", t);
    Serial.flush();
    delay(1000);
  }

  wifiMulti.addAP("IT LAB", "ITLab@2029@");

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  delay(1000);
  Serial.println("AT");
  delay(1000);
  Serial.println("AT+CMGF=1");
  delay(1000);
}

void loop() {

  if ((wifiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    float distance = pulseIn(echo, HIGH) * 0.034 / 2;

    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(buzzer, LOW);

    String statustext = "SAFE";

    if (distance <= 10) {

      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(buzzer, HIGH);
      delay(1000);
      statustext = "CRITICAL";

      Serial.println("AT+CMGS=\"\\+250794238513\"");
      delay(1000);
      Serial.print("EMERGENCY RESPONSE : BREACH ! DISTANCE ! : ");
      Serial.print(distance);
      Serial.println("cm. ");
      delay(1000);
      Serial.write(26);
      delay(3000);

    }

    else if (distance <= 20) {

      digitalWrite(led1, HIGH);
      delay(1000);
      digitalWrite(led1, LOW);
      delay(500);
      statustext = "CLOSE";

    }

    else if (distance <= 30) {

      digitalWrite(led2, HIGH);
      delay(1000);
      digitalWrite(led2, LOW);
      delay(500);
      statustext = "MIDDLE_RANGE";

    }

    else if (distance <= 50) {

      digitalWrite(led3, HIGH);
      delay(1000);
      digitalWrite(led3, LOW);
      delay(500);
      statustext = "FAR";
    }

    delay(1000);

    String url = "http://";
    url += serverIP;
    url += "/monitor.php?temperature_level=";
    url += distance;
    url += "&fan_status=";
    url += statustext;

    http.begin(url);
    int httpCode = http.GET();


    if (httpCode > 0) {

      Serial.printf("[HTTP] GET... code: %d\n", httpCode);


      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(5000);
}