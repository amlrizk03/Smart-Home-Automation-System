/*
  Smart Home + Smart Plant System 
  - Gas: moving average + hysteresis + buzzer pattern
  - Soil: moving average + hysteresis
  - Lights: based on Motion + LDR
  - Fan: based on Motion + Temperature
*/

float x, y, z, temp;
int gassensor = 0;
int soil_sensor = 0; 

void setup() {
  pinMode(8, INPUT);   // Motion (PIR)
  pinMode(5, OUTPUT);  // Light 1
  pinMode(A5, INPUT);  // LDR (Light Sensor)
  pinMode(A4, INPUT);  // Temp Sensor
  
  pinMode(A0, INPUT);  // Gas Sensor 
  pinMode(4, OUTPUT);  // Gas LED
  pinMode(7, OUTPUT);  // Speaker/Buzzer
  
  pinMode(3, OUTPUT);  // House Fan 

  // Plant System Pins 
  pinMode(A1, INPUT);  // SOIL SENSOR
  pinMode(2, OUTPUT);  // WATER PUMP
  pinMode(9, OUTPUT);  // Soil LED

  Serial.begin(9600);
}

void loop() {
  // 1. READ ALL SENSORS
  gassensor = analogRead(A0);
  soil_sensor = analogRead(A1); //soil moisture
  x = digitalRead(8);    // Motion
  y = analogRead(A5);    // LDR
  z = analogRead(A4);    // Temp Raw
  
  // 2. CALCULATE TEMPERATURE
  temp = (double)z / 1024;       
  temp = temp * 5;                 
  temp = temp - 0.5;               
  temp = temp * 100;

  // 3. PRINT DATA
  Serial.print("Gas: ");    Serial.print(gassensor);
  Serial.print(" | Soil: ");   Serial.print(soil_sensor); 
  Serial.print(" | Motion: "); Serial.print(x);
  Serial.print(" | Temp: ");   Serial.print(temp);
  Serial.println(" C");

  // 4. GAS LOGIC
  if (gassensor >= 500) {
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(4, LOW);
  }

  if (gassensor >= 600) {
    tone(7, 261, 100); 
  } else {
    noTone(7); 
  }

  // 5. FAN LOGIC (Motion + Heat)
  if (x > 0 && temp > 30) {
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(3, LOW);
  }

  // 6. MOTION AND LDR LOGIC (Lights)
  if (x > 0) {
    if (y < 550 && temp > 30) {
      digitalWrite(5, HIGH); digitalWrite(6, HIGH);
    } 
    else if (y < 550 && temp < 30) {
      digitalWrite(5, HIGH); digitalWrite(6, LOW);
    } 
    else if (y > 550 && temp > 30) {
      digitalWrite(5, LOW); digitalWrite(6, HIGH);
    } 
    else {
      digitalWrite(5, LOW); digitalWrite(6, LOW);
    }
  } else {
    digitalWrite(5, LOW); digitalWrite(6, LOW);
  }

 // 7. PLANT LOGIC
if (soil_sensor < 500) { 
  digitalWrite(2, HIGH); // Pump ON
  digitalWrite(9, HIGH); // LED ON (Soil Dry)
} else {
  digitalWrite(2, LOW);  // Pump OFF
  digitalWrite(9, LOW);  // LED OFF (Soil Wet)
}

  
  delay(250); 
}