//-------НАСТРОЙКИ---------
const int PIN_CHIP_S_SD = 7;
//-------НАСТРОЙКИ---------
String fString, fStringFileName, rwBuff;
//-------БИБЛИОТЕКИ---------
String buffer;//тут хранится прочитанный из карты текст
boolean FileOpen, FileClose, FileReady;
String file = "/test.txt";
#include <SPI.h>
#include <SD.h>
//-------БИБЛИОТЕКИ---------
void FileRead() {
  File dataFile;
  if (FileOpen != 1) {
    if (!SD.begin(PIN_CHIP_S_SD)) {
      while (1);
    }
    FileOpen = 1;
    dataFile = SD.open(file);
    if (dataFile) {
      FileReady = 1;
    }
  }

  while (dataFile.available()) {
    buffer = dataFile.readStringUntil('\n');
    Serial.println(buffer);
    delay(50);
  }
  FileClose = 1;
  
  if (FileClose == 1) {
    dataFile.close();
    //FileClose = 0;
    //FileOpen = 0;
  }
}
void setup() {
  Serial.begin(115200); 
  FileRead();                  // открыть порт для связи с ПК для отладки
}

void loop() {
    Serial.println("//////////////////////");
    
    Serial.println("//////////////////////");
    delay(500);
}