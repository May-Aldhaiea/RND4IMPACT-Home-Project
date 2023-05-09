const int DOOR_SENSOR_PIN = 13; 
int doorState;

void setup() {
  Serial.begin(9600);                     // initialize serial
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP); // set arduino pin to input pull-up mode
}

void loop() {
  doorState = digitalRead(DOOR_SENSOR_PIN); // read state

  if (doorState == HIGH) {
    Serial.println("The door is open");
  } else {
    Serial.println("The door is closed");
  }
  delay(1000);
}
