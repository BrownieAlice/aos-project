#include <M5Stack.h>
#include <array>

constexpr std::array<int, 3> plus_pins = {3, 21, 2};
// モタドラ正方向の入力(回転方向を決めるやつ).
constexpr std::array<int, 3> gnd_pins = {17, 22, 5};
// モタドラGND方向の入力(回転方向を決めるやつ).
constexpr std::array<int, 3> pwm_pins = {16, 23, 18};
// モタドラのPWM入力.
constexpr int lamp_pin = 26;
// パトランプのピン.
constexpr int first_pwm_channel = 3;
// PWMピンの最初のチャンネル番号.
constexpr int pwm_clock = 500;
// PWMのクロック.

void init_robot_pins() {
  // ロボット操作周りのピンの初期化.
  for (const auto i : plus_pins) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  // plusピン初期化.

  for (const auto i : gnd_pins) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  // GNDピン初期化.

  auto channel = first_pwm_channel;
  for (const auto i : pwm_pins) {
    pinMode(i, OUTPUT);
    ledcSetup(channel, pwm_clock, 10);
    ledcAttachPin(i, channel);
    ledcWrite(channel, 0);
    channel++;
  }
  // PWMピン初期化.

  pinMode(lamp_pin, OUTPUT);
  digitalWrite(lamp_pin, LOW);
}

void start_move() {
  // ロボットを動かす.

  for (const auto i : plus_pins) {
    digitalWrite(i, HIGH);
  }
  // plusピン設定.

  auto channel = first_pwm_channel;
  for (const auto i : pwm_pins) {
    ledcWrite(channel, 300);
    channel++;
  }
  // PWMピン設定.

  digitalWrite(lamp_pin, HIGH);
  // ランプピン設定.
}

void stop_move() {
  // ロボットを止める.

  for (const auto i : plus_pins) {
    digitalWrite(i, LOW);
  }
  // plusピン設定.

  auto channel = first_pwm_channel;
  for (const auto i : pwm_pins) {
    ledcWrite(channel, 0);
    channel++;
  }
  // PWMピン設定.

  digitalWrite(lamp_pin, LOW);
  // ランプピン設定.
}

void setup(){
  M5.begin(true, false, true);

  M5.Lcd.println("Hello World");
  Serial.println("Hello World");

  init_robot_pins();
}

// the loop routine runs over and over again forever
void loop() {
  M5.update();

  if (M5.BtnA.wasReleased()) {
      M5.Lcd.println("pressed!");
      vTaskDelay(5000 / portTICK_RATE_MS);

      start_move();
      vTaskDelay(3000 / portTICK_RATE_MS);

      stop_move();
  }
}
