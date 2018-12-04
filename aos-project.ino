#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  M5.begin(true, false, true);

  // LCD display
  M5.Lcd.println("Hello World");
  Serial.println("Hello World");
  pinMode(3, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  digitalWrite(3, LOW);
  digitalWrite(17, LOW);
  digitalWrite(16, LOW);
  digitalWrite(21, LOW);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);
  digitalWrite(2, LOW);
  digitalWrite(5, LOW);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
}

// the loop routine runs over and over again forever
void loop() {
  M5.update();

  if (M5.BtnA.wasReleased()) {
      M5.Lcd.println("pressed!");
      vTaskDelay(5000 / portTICK_RATE_MS);

      digitalWrite(3, HIGH);
      digitalWrite(17, LOW);
      digitalWrite(21, HIGH);
      digitalWrite(22, LOW);
      digitalWrite(2, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(26, HIGH);

      ledcSetup(3, 490, 8);
      ledcAttachPin(16, 3);
      ledcAttachPin(23, 3);
      ledcAttachPin(25, 3);
      ledcWrite(3, 100);
      vTaskDelay(5000 / portTICK_RATE_MS);

      digitalWrite(3, LOW);
      digitalWrite(17, LOW);
      digitalWrite(21, LOW);
      digitalWrite(22, LOW);
      digitalWrite(2, LOW);
      digitalWrite(5, LOW);
      digitalWrite(26, LOW);
  }
}
