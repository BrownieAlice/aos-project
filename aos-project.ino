#include <WiFi.h>
#include <array>
#include <M5Stack.h>
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;
// 音周りの設定.
std::array<char*, 7> music_list = {"/uzuho.mp3", "/fran.mp3", "/hina.mp3", "/koishi.mp3", "/okina.mp3", "/remilia.mp3", "/sakuya.mp3"};
// 音リスト.

TaskHandle_t th;
// マルチスレッド用の変数.
xSemaphoreHandle xSemaphore = NULL;
// 音楽再生するか否か.

constexpr std::array<int, 3> plus_pins = {3, 21, 2};
// モタドラ正方向の入力(回転方向を決めるやつ).
constexpr std::array<int, 2> pwm_pins = {17, 5};
// モタドラのPWM入力.
constexpr int lamp_pin = 3;
// パトランプのピン.
constexpr int pwm_channel = 3;
// PWMピンの最初のチャンネル番号.
constexpr int pwm_clock = 500;
// PWMのクロック.


void music_func(void *pvParameters) {
  // 音楽用の関数.
  // Core0で回す.

  while(1) {
    if (xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
      if (mp3->isRunning()) {
        if (!mp3->loop()) mp3->stop();
      }
      xSemaphoreGive(xSemaphore);
    }

    vTaskDelay(1 / portTICK_RATE_MS);
    // WatchDogTime向けにダミーのDelayをいれる.
  }
}

void init_robot_pins() {
  // ロボット操作周りのピンの初期化.

  for (const auto i : plus_pins) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  // plusピン初期化.

  for (const auto i : pwm_pins) {
    pinMode(i, OUTPUT);
    ledcSetup(pwm_channel, pwm_clock, 10);
    ledcAttachPin(i, pwm_channel);
    ledcWrite(pwm_channel, 0);
  }
  // PWMピン初期化.

  pinMode(lamp_pin, OUTPUT);
  digitalWrite(lamp_pin, LOW);
  // パトランプピン初期化.

  out = new AudioOutputI2S(0, 1);
  out->SetOutputModeMono(true);
  mp3 = new AudioGeneratorMP3();
  // 音周りの設定.
}

void start_move() {
  // ロボットを動かす.

  for (const auto i : plus_pins) {
    digitalWrite(i, HIGH);
  }
  // plusピン設定.

  for (const auto i : pwm_pins) {
    ledcWrite(pwm_channel, 100);
  }
  // PWMピン設定.

  digitalWrite(lamp_pin, HIGH);
  // パトランプピン設定.

  static size_t music_num = 0;
  // プレイリスト番号.
  WiFi.mode(WIFI_OFF);
  // WiFiはOFFにしないと鳴らない.
  M5.Lcd.println(music_list[music_num]);
  delete id3;
  delete file;
  // メモリ解放.
  file = new AudioFileSourceSD(music_list[music_num]);
  id3 = new AudioFileSourceID3(file);
  // いろいろと確保.
  mp3->begin(id3, out);
  xSemaphoreGive(xSemaphore);
  // 再生開始.

  music_num++;
  if (music_list.size() <= music_num) {
    music_num = 0;
  }
  // 次の曲の設定.
}

void stop_move() {
  // ロボットを止める.

  for (const auto i : plus_pins) {
    digitalWrite(i, LOW);
  }
  // plusピン設定.

  for (const auto i : pwm_pins) {
    ledcWrite(pwm_channel, 0);
  }
  // PWMピン設定.

  digitalWrite(lamp_pin, LOW);
  // パトランプピン設定.

  xSemaphoreTake(xSemaphore, portMAX_DELAY);
  mp3->stop();
  WiFi.mode(WIFI_STA);
  // 再生停止.
}

void setup(){
  M5.begin();

  M5.Lcd.println("Hello World");
  Serial.println("Hello World");

  init_robot_pins();

  xSemaphore = xSemaphoreCreateMutex();
  xTaskCreatePinnedToCore(music_func, "music_func", 4096, NULL, 1, &th, 0);
  // 音楽関連の処理は別のコアで実行させる.
}

// the loop routine runs over and over again forever
void loop() {
  M5.update();

  if (M5.BtnA.wasReleased()) {
      M5.Lcd.println("pressed!");
      vTaskDelay(5000 / portTICK_RATE_MS);

      start_move();

      vTaskDelay(10000 / portTICK_RATE_MS);

      stop_move();
  }
}
