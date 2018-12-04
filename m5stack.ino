#include <M5Stack.h>
#include <WiFi.h>
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"

AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;
const unsigned int size = 7;
char* list[size] = {"/uzuho.mp3", "/fran.mp3", "/hina.mp3", "/koishi.mp3", "/okina.mp3", "/remilia.mp3", "/sakuya.mp3"};
unsigned int pos = 3;

// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  M5.begin();
  WiFi.mode(WIFI_OFF);
  delay(500);

  // LCD display
  M5.Lcd.println("Hello World");
  Serial.println("Hello World");

  out = new AudioOutputI2S(0, 1); // Output to builtInDAC
  out->SetOutputModeMono(true);
  mp3 = new AudioGeneratorMP3();
  file = new AudioFileSourceSD(list[pos]);
  id3 = new AudioFileSourceID3(file);
}

// the loop routine runs over and over again forever
void loop() {
  M5.update();
  if (mp3->isRunning()) {
    if (!mp3->loop()) mp3->stop();
  } else {
    M5.Lcd.printf("MP3 done\n");
    delay(1000);
  }
  if (M5.BtnA.wasReleased() && !mp3->isRunning()) {
    if (size <= ++pos) {
      pos = 0;
    }
    file = new AudioFileSourceSD(list[pos]);
    id3 = new AudioFileSourceID3(file);
    M5.Lcd.printf("Start!\n");
    mp3->begin(id3, out);
  }
}
