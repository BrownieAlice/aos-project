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
  digitalWrite(3, LOW);
  digitalWrite(17, LOW);
  digitalWrite(16, LOW);
}

// the loop routine runs over and over again forever
void loop() {
  M5.update();

  if (M5.BtnA.wasReleased()) {
      M5.Lcd.println("pressed!");
      vTaskDelay(5000 / portTICK_RATE_MS);

      digitalWrite(3, HIGH);
      digitalWrite(17, LOW);
      ledcSetup(3, 490, 8);
      ledcAttachPin(16, 3);
      ledcWrite(3, 125);
      vTaskDelay(5000 / portTICK_RATE_MS);

      digitalWrite(3, LOW);
      digitalWrite(17, LOW);
      digitalWrite(16, LOW);
  }
}
