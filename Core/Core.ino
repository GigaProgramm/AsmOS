// Определения пинов и глобальных переменных
const int PIN_CHIP_S_SD = 7; // Пин для работы с SD картой
String fString, fStringFileName, rwBuff; // Строковые переменные
int mem[4096]; // Память программы (массив из 4096 элементов)
int addr; // Адрес текущей ячейки памяти
bool s; // Флаг, указывающий на начало чтения
byte command; // Переменная для команды
String str; // Переменная для хранения прочитанной строки из файла
boolean FileOpen, FileClose, FileReady; // Флаги открытия и закрытия файла
String file = "/main.txt"; // Имя файла для чтения с SD карты
#include <string.h>
#include <SPI.h>
#include <SD.h> // Подключение библиотек для работы с SD картой

void setup() {
  Serial.begin(115200); // Инициализация порта для отладки
  delay(500); // Задержка для стабилизации соединения
  Serial.println("upload"); // Сообщение о старте
  writeProg(); // Запуск функции загрузки программы
}

void loop() {
  while (s) {
    command = mem[addr]; // Чтение команды из памяти по текущему адресу
    
    switch (command) {
      case 0x00:
        addr++; // Переход к следующей ячейке памяти
        break;
      case 0x01: // add
        add();
        break;
      case 0x02: // sub
        sub();
        break;
      case 0x03: // inc
        inc();
        break;
      case 0x04: // dec
        dec();
        break;
      case 0x05: // mov
        mov();
        break;
      case 0x06: // mova
        mova();
        break;
      case 0x07: // jmp
        jmp();
        break;
      case 0x08: // jmpa
        jmpa();
        break;
      case 0x09: // cout
        cout();
        break;
      default:
        break; // Если команда не распознана, пропускаем её
    }
    
    // Проверка выхода за пределы памяти
    if (addr >= sizeof(mem) / sizeof(mem[0])) {
      addr = 0; // Возврат к началу памяти, если адрес превышает размер массива
    }
  }
}
String trim_space(String str) {
  String result = "";
  
  for (int i = 0; i < str.length(); i++) {
    if (str[i] != ' ') {
      result += str[i];
    }
  }
  
  return result;
}

// Функция выполнения команды "add" (сложение)
void add() {
  mem[mem[addr + 1]] += mem[mem[addr + 2]]; // Сложение значений в памяти
  addr += 3; // Переход к следующей команде
}

// Функция выполнения команды "sub" (вычитание)
void sub() {
  mem[mem[addr + 1]] -= mem[mem[addr + 2]]; // Вычитание значений в памяти
  addr += 3; // Переход к следующей команде
}

// Функция выполнения команды "inc" (увеличение на 1)
void inc() {
  mem[mem[addr + 1]]++; // Увеличиваем значение на 1
  addr += 2; // Переход к следующей команде
}

// Функция выполнения команды "dec" (уменьшение на 1)
void dec() {
  mem[mem[addr + 1]]--; // Уменьшаем значение на 1
  addr += 2; // Переход к следующей команде
}

// Функция выполнения команды "mov" (перемещение значения)
void mov() {
  mem[mem[addr + 1]] = mem[addr + 2]; // Перемещаем значение в ячейку памяти
  addr += 3; // Переход к следующей команде
}

// Функция выполнения команды "mova" (перемещение значения из другой ячейки)
void mova() {
  mem[mem[addr + 1]] = mem[mem[addr + 2]]; // Перемещаем значение из одной ячейки в другую
  addr += 3; // Переход к следующей команде
}

// Функция выполнения команды "jmp" (прыжок по адресу)
void jmp() {
  addr = mem[addr + 1]; // Переход по адресу в памяти
}

// Функция выполнения команды "jmpa" (прыжок по адресу, извлеченному из памяти)
void jmpa() {
  addr = mem[mem[addr + 1]]; // Переход по адресу, хранящемуся в памяти
}

// Функция выполнения команды "cout" (вывод в Serial)
void cout() {
  Serial.println(mem[mem[addr + 1]]); // Вывод значения из памяти
  addr += 2; // Переход к следующей команде
}

// Функция для загрузки программы с SD карты
void writeProg() {
  int operand_1, operand_2;
  File dataFile; // Переменная для работы с файлом на SD карте

  // Если файл не открыт, то пытаемся открыть его
  if (FileOpen != 1) {
    if (!SD.begin(PIN_CHIP_S_SD)) { // Инициализация SD карты
      while (1); // Если ошибка, зацикливаем выполнение
    }
    FileOpen = 1;
    dataFile = SD.open(file); // Открытие файла для чтения

    // Проверка успешного открытия файла
    if (dataFile) {
      FileReady = 1;
    }
  }
  
  // Чтение строк из файла
    while (dataFile.available()) {
      if (!s) { // Если процессор еще не запущен
      str = dataFile.readStringUntil('\n'); // Чтение строки до символа новой строки
      String com = str.substring(0, str.indexOf(' '));
      if(com == "end"){
        Serial.println("Processor is start"); // Если команда "s", начинаем выполнение
        addr = 0;
        s = true;
        dataFile.close();
      }
      String operands = str.substring(str.indexOf(' '));
      operands = trim_space(operands);
      if(operands.indexOf(',') != -1){
        operand_1 = (operands.substring(0, operands.indexOf(','))).toInt();
        operand_2 = (operands.substring(operands.indexOf(',') + 1)).toInt();
      } else {
        operand_1 = operands.toInt();
      }
        // Распознаем команды и заносим их в память
        if (com == "add") {
          mem[addr] = 1;
          mem[addr+1] = operand_1;
          mem[addr+2] = operand_2;
          addr += 3;
          Serial.println("add");
        } else if (com == "sub") {
          mem[addr] = 2;
          mem[addr+1] = operand_1;
          mem[addr+2] = operand_2;
          addr += 3;
          Serial.println("sub");
        } else if (com == "inc") {
          mem[addr] = 3;
          mem[addr+1] = operand_1;
          addr += 2;
          Serial.println("inc");
        } else if (com == "dec") {
          mem[addr] = 4;
          mem[addr+1] = operand_1;
          addr += 2;
          Serial.println("dec");
        } else if (com == "mov") {
          mem[addr] = 5;
          mem[addr+1] = operand_1;
          mem[addr+2] = operand_2;
          addr += 3;
          Serial.println("mov");
        } else if (com == "mova") {
          mem[addr] = 6;
          mem[addr+1] = operand_1;
          mem[addr+2] = operand_2;
          addr += 3;
          Serial.println("mova");
        } else if (com == "jmp") {
          mem[addr] = 7;
          mem[addr+1] = operand_1;
          addr += 2;
          Serial.println("jmp");
        } else if (com == "jmpa") {
          mem[addr] = 8;
          mem[addr+1] = operand_1;
          addr += 2;
          Serial.println("jmpa");
        } else if (com == "cout") {
          mem[addr] = 9;
          mem[addr+1] = operand_1;
          addr += 2;
          Serial.println("cout");
        }
      } 
    }

  FileClose = 1; // Файл закрыт

  // Закрытие файла
  if (FileClose == 1) {
    dataFile.close(); // Закрытие файла
    FileClose = 0;
    FileOpen = 0;
  }
}
