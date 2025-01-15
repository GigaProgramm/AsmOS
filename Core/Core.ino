#include <SPI.h>
#include <SD.h>

const int PIN_CHIP_S_SD = 7; // Пин для работы с SD картой
int mem[4096]; // Память программы (массив из 4096 элементов)
int addr = 0; // Адрес текущей ячейки памяти
bool s = false; // Флаг, указывающий на начало выполнения
String file = "/main.txt"; // Имя файла для чтения с SD карты
String binFile = "/main_bin.txt"; // Новый файл для записи в текстовом формате
boolean FileOpen, FileClose, FileReady;
String out_mem;
bool cf, zf, sf, of; 
int reg[4];
void setup() {
  Serial.begin(115200); // Инициализация порта для отладки
  delay(500); // Задержка для стабилизации соединения
  Serial.println("upload"); // Сообщение о старте
  openFile(); // Открытие файла и загрузка команд
}

void loop() {
  // Начинаем выполнение программы после загрузки
  while (s) {
    byte command = mem[addr]; // Чтение команды из памяти по текущему адресу
    
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

// Функции для команд (add, sub, inc и т. д.):

void add() {
  mem[mem[addr + 1]] += mem[mem[addr + 2]]; // Сложение значений в памяти
  addr += 3; // Переход к следующей команде
}

void sub() {
  mem[mem[addr + 1]] -= mem[mem[addr + 2]]; // Вычитание значений в памяти
  addr += 3; // Переход к следующей команде
}

void inc() {
  mem[mem[addr + 1]]++; // Увеличиваем значение на 1
  addr += 2; // Переход к следующей команде
}

void dec() {
  mem[mem[addr + 1]]--; // Уменьшаем значение на 1
  addr += 2; // Переход к следующей команде
}

void mov() {
  mem[mem[addr + 1]] = mem[addr + 2]; // Перемещаем значение в ячейку памяти
  addr += 3; // Переход к следующей команде
}

void mova() {
  mem[mem[addr + 1]] = mem[mem[addr + 2]]; // Перемещаем значение из одной ячейки в другую
  addr += 3; // Переход к следующей команде
}

void jmp() {
  addr = mem[addr + 1]; // Переход по адресу в памяти
}

void jmpa() {
  addr = mem[mem[addr + 1]]; // Переход по адресу, хранящемуся в памяти
}

void cout() {
  Serial.println(mem[mem[addr + 1]]); // Вывод значения из памяти
  addr += 2; // Переход к следующей команде
}


void WriteToMem(){
  File dataFile;
  int op_1, op_2, space1, space2;
  byte com;
  if (FileOpen != 1) {
    if (!SD.begin(PIN_CHIP_S_SD)) {
      while (1);
    }
    FileOpen = 1;
    dataFile = SD.open(binFile);
    if (dataFile) {
      FileReady = 1;
    }
  }

  while (dataFile.available()) {
    out_mem = dataFile.readStringUntil('\n');
    com = (out_mem.substring(0, out_mem.indexOf(' '))).toInt();
    space1 = out_mem.indexOf(' ');
    space2 = out_mem.indexOf(' ' ,space1 + 1);
    op_1 = (out_mem.substring(space1 , space2)).toInt();
    op_2 = (out_mem.substring(space2 + 1)).toInt();
    switch(com) {
      case 0x00: break;
      case 0x01: 
          mem[addr] = com;
          mem[addr + 1] = op_1;
          mem[addr + 2] = op_2;
          addr += 3;
          break;
      case 0x02: 
          mem[addr] = com;
          mem[addr + 1] = op_1;
          mem[addr + 2] = op_2;
          addr += 3;
          break;
      case 0x03: 
          mem[addr] = com;
          mem[addr + 1] = op_1;
          addr += 2;
          break;
      case 0x04: 
          mem[addr] = com;
          mem[addr + 1] = op_1;
          addr += 2;
          break;
      case 0x05: 
          mem[addr] = com;
          mem[addr + 1] = op_1;
          mem[addr + 2] = op_2;
          addr += 3;
          break;
      case 0x06: 
          mem[addr] = com;
          mem[addr + 1] = op_1;
          mem[addr + 2] = op_2;
          addr += 3;
          break;
      case 0x07: 
          mem[addr] = com;
          mem[addr + 1] = op_1;
          addr += 2;
          break;
      case 0x08: 
          mem[addr] = com;
          mem[addr + 1] = op_1;
          addr += 2;
          break;
      case 0x09: 
          mem[addr] = com;
          mem[addr + 1] = op_1;
          addr += 2;
          break;

    }
  }
  FileClose = 1;
  
  if (FileClose == 1) {
    dataFile.close();
    FileClose = 0;
    FileOpen = 0;
    s = true;
  }

}
// Функция для открытия и обработки текстового файла
void openFile() {
  File dataFile; // Переменная для работы с файлом на SD карте
  
  // Инициализация SD карты и открытие файла
  if (!SD.begin(PIN_CHIP_S_SD)) { // Инициализация SD карты
    Serial.println("Ошибка SD карты!");
    while (1); // Если ошибка, зацикливаем выполнение
  }
  if(SD.exists("main_bin.txt"))WriteToMem();
  dataFile = SD.open(file); // Открытие файла для чтения

  // Проверка успешного открытия файла
  if (!dataFile) {
    Serial.println("Не удалось открыть файл!");
    while (1); // Если файл не открылся, зацикливаем выполнение
  }

  // Чтение строк из файла и запись их в текстовый файл
  File bin = SD.open(binFile, FILE_WRITE); // Открытие текстового файла для записи
  if (!bin) {
    Serial.println("Ошибка при открытии текстового файла для записи.");
    return;
  }

  // Процесс чтения команд из текстового файла и записи в новый текстовый файл
  while (dataFile.available()) {
    String line = dataFile.readStringUntil('\n');
    processLine(line, bin);
  }

  dataFile.close(); // Закрытие исходного файла
  bin.close(); // Закрытие текстового файла
  WriteToMem();

  //ESP.restart();
}

// Обработка строки из файла и запись команды в текстовый файл
void processLine(String line, File &bin) {
  // Удаление комментариев
  if (line.indexOf(';') != -1) {
    line = line.substring(0, line.indexOf(';'));
  }

  // Разбор команды и операндов
  String com = line.substring(0, line.indexOf(' '));
  String operands = line.substring(line.indexOf(' ') + 1);
  operands = trim_space(operands);

  int operand_1 = 0, operand_2 = 0;

  // Извлечение операндов
  if (operands.indexOf(',') != -1) {
    operand_1 = operands.substring(0, operands.indexOf(',')).toInt();
    operand_2 = operands.substring(operands.indexOf(',') + 1).toInt();
  } else {
    operand_1 = operands.toInt();
  }

  // Запись команды и операндов в текстовый файл
  if (com == "add") {
    bin.println("1 " + String(operand_1) + " " + String(operand_2)); // Команда add
  } else if (com == "sub") {
    bin.println("2 " + String(operand_1) + " " + String(operand_2)); // Команда sub
  } else if (com == "inc") {
    bin.println("3 " + String(operand_1)); // Команда inc
  } else if (com == "dec") {
    bin.println("4 " + String(operand_1)); // Команда dec
  } else if (com == "mov") {
    bin.println("5 " + String(operand_1) + " " + String(operand_2)); // Команда mov
  } else if (com == "mova") {
    bin.println("6 " + String(operand_1) + " " + String(operand_2)); // Команда mova
  } else if (com == "jmp") {
    bin.println("7 " + String(operand_1)); // Команда jmp
  } else if (com == "jmpa") {
    bin.println("8 " + String(operand_1)); // Команда jmpa
  } else if (com == "cout") {ax, bx, cx, dx;
    bin.println("9 " + String(operand_1)); // Команда cout
  }
}

// Функция для обрезки пробелов
String trim_space(String str) {
  String result = "";
  for (int i = 0; i < str.length(); i++) {
    if (str[i] != ' ') {
      result += str[i];
    }
  }
  return result;
}
