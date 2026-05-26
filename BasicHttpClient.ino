#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

WiFiMulti wifiMulti;

// --- STEP 1: PHYSICAL HARDWARE PIN ALLOCATION ---
int led1   = 2;   // Green LED (Pin labeled D0 on board)
int led2   = 4;   // Yellow LED (Pin labeled D1 on board)
int led3   = 12;  // Red LED (Pin labeled D2 on board)
int buzzer = 13;  // Alarm Buzzer (Pin labeled D3 on board)
int trig   = 14;  // Ultrasonic Transmit Trigger (Pin labeled CLK)
int echo   = 15;  // Ultrasonic Receive Echo (Pin labeled CMD)

// --- STEP 2: NETWORK & SYSTEM VARIABLES ---
const char* serverIP = "192.168.1.15"; // Run 'ipconfig' in cmd to get this!

void setup() {
  // Configured to 9600 to talk natively with the GSM hardware module
  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %u...\n", t);
    Serial.flush();
    delay(1000);
  }

  // Your exact network credentials provided
  wifiMulti.addAP("IT LAB", "ITLab@2029@");

  // Setting pin orientations
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Readying the GSM Module communication mode
  delay(1000);
  Serial.println("AT");           
  delay(1000);
  Serial.println("AT+CMGF=1");   
  delay(1000);
}

void loop() {
  
  if ((wifiMulti.run() == WL_CONNECTED)) {

    HTTPClient http;

    // --- STEP 3: ULTRASONIC DISTANCE LOGIC ---
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    float distance = pulseIn(echo, HIGH) * 0.034 / 2;

    // Clear alert system indicators before recalculation
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(buzzer, LOW);

    String statusText = "SAFE DISTANCE";

    if (distance <= 10) {
      digitalWrite(led1, HIGH); 
      digitalWrite(led2, HIGH); 
      digitalWrite(led3, HIGH); 
      digitalWrite(buzzer, HIGH);
      statusText = "CRITICAL CLOSE";

      // 🚨 EMERGENCY GSM BACKUP ALERT (Fires ONLY in severe zone)
      Serial.println("AT+CMGS=\"+250794238513\""); // Using your network target number
      delay(1000);
      Serial.print("EMERGENCY RESPONSE: Proximity breach! Distance detected: ");
      Serial.print(distance);
      Serial.println(" cm.");
      delay(1000);
      Serial.write(26); // Sends literal Ctrl+Z command to dispatch message
      delay(3000);
    }
    else if (distance <= 20) { 
      digitalWrite(led1, HIGH); 
      statusText = "WARNING CLOSE"; 
      delay(1000); 
      digitalWrite(led1, LOW); 
    }
    else if (distance <= 30) { 
      digitalWrite(led2, HIGH); 
      statusText = "ALERT PROXIMITY"; 
      delay(1000); 
      digitalWrite(led2, LOW); 
    }
    else if (distance <= 50) { 
      digitalWrite(led3, HIGH); 
      statusText = "NOTICE OBJECT"; 
      delay(1000); 
      digitalWrite(led3, LOW); 
    }

    // --- STEP 4: URL COMPOSITION WITH NEW SCHEMES ---
    String url = "http://" + String(serverIP) + "/monitor.php?measured_distance=" + String(distance) + "&alert_status=" + statusText;
    url.replace(" ", "%20"); 

    Serial.print("[HTTP] begin...\n");
   
    // Passing the built URL directly into the HTTP context
    http.begin(url);  

    Serial.print("[HTTP] GET...\n");
   
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

  // Set to 2 seconds for real-time telemetry updates to XAMPP
  delay(2000);
}
