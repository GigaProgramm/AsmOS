String code = "";
String filename = "untitled.txt";
boolean isFileOpened = false;

// Ключевые слова C++
String[] cppKeywords = {
  "auto", "break", "case", "const", "continue", "default", "do", "else", 
  "enum", "extern", "for", "goto", "if", "return", "sizeof", "static", 
  "struct", "switch", "typedef", "union", "volatile", "while", "int", 
  "float", "double", "char", "void", "bool", "true", "false", "nullptr",
  "new", "delete", "class", "public", "private", "protected", "template",
  "this", "virtual", "using", "namespace", "try", "catch", "throw", "#include"
};

// Ключевые слова Assembler (x86)
String[] asmKeywords = {
  "mov", "push", "pop", "add", "sub", "inc", "dec", "imul", "idiv", 
  "and", "or", "xor", "not", "shl", "shr", "cmp", "test", "jmp", 
  "je", "jne", "jg", "jge", "jl", "jle", "call", "ret", "lea", 
  "int", "db", "dw", "dd", "dq", "equ", "times", "section", "global"
};

// Цвета VS Code Dark+
color backgroundColor = color(30, 30, 35);
color textColor = color(212, 212, 212);
color keywordColor = color(86, 156, 214);
color typeColor = color(78, 201, 176);
color stringColor = color(206, 145, 120);
color commentColor = color(106, 153, 85);
color numberColor = color(181, 206, 168);
color preprocessorColor = color(197, 134, 192);
color lineNumberColor = color(100, 100, 100);
color labelColor = color(156, 220, 254);

// Параметры тулбара
int toolbarWidth = 60;
color toolbarColor = color(45, 45, 48);
color buttonColor = color(60, 60, 64);
color buttonHoverColor = color(80, 80, 85);

void setup() {
  size(1100, 700);
  background(backgroundColor);
  textSize(15);
  fill(textColor);
  smooth();
  PFont font = createFont("Consolas", 14);
  textFont(font);
}

void draw() {
  background(backgroundColor);
  drawToolbar();
  displayCode();
  
  // Статус файла
  fill(lineNumberColor);
  text("Файл: " + filename + (isFileOpened ? "" : " (не сохранён)"), toolbarWidth + 10, height - 10);
}

void drawToolbar() {
  fill(toolbarColor);
  noStroke();
  rect(0, 0, toolbarWidth, height);
  
  // Кнопка "Открыть"
  drawButton(10, 30, 40, 40, "📂", mouseX < 50 && mouseY > 30 && mouseY < 70);
  
  // Кнопка "Сохранить"
  drawButton(10, 80, 40, 40, "💾", mouseX < 50 && mouseY > 80 && mouseY < 120);
  
  // Кнопка "Новый"
  drawButton(10, 130, 40, 40, "🆕", mouseX < 50 && mouseY > 130 && mouseY < 170);
}

void drawButton(int x, int y, int w, int h, String label, boolean isHovered) {
  fill(isHovered ? buttonHoverColor : buttonColor);
  rect(x, y, w, h, 5);
  fill(textColor);
  textAlign(CENTER, CENTER);
  text(label, x + w/2, y + h/2);
  textAlign(LEFT, BASELINE);
}

void displayCode() {
  pushMatrix();
  translate(toolbarWidth, 0);
  
  String[] lines = split(code, '\n');
  for (int i = 0; i < lines.length; i++) {
    String line = lines[i];
    int x = 50;
    int y = 30 + i * 20;

    // Номера строк
    fill(lineNumberColor);
    text((i + 1) + ":", 10, y);

    // Подсветка синтаксиса
    String[] tokens = splitTokens(line, " \t{}()[],;+-*/%=&|<>!\"'");
    for (String token : tokens) {
      if (token.startsWith("//")) {
        fill(commentColor);
        text(token + " ", x, y);
        x += textWidth(token + " ");
        continue;
      }
      
      if (token.startsWith("\"") || (token.length() > 0 && token.charAt(0) == '\'')) {
        fill(stringColor);
        text(token + " ", x, y);
        x += textWidth(token + " ");
        continue;
      }
      
      if (token.matches("-?\\d+(\\.\\d+)?(f|L)?") || token.matches("0x[0-9a-fA-F]+")) {
        fill(numberColor);
        text(token + " ", x, y);
        x += textWidth(token + " ");
        continue;
      }
      
      if (token.startsWith("#")) {
        fill(preprocessorColor);
        text(token + " ", x, y);
        x += textWidth(token + " ");
        continue;
      }
      
      boolean isKeyword = false;
      for (String kw : cppKeywords) {
        if (token.equals(kw)) {
          fill(keywordColor);
          text(token + " ", x, y);
          x += textWidth(token + " ");
          isKeyword = true;
          break;
        }
      }
      if (isKeyword) continue;
      
      for (String asmKw : asmKeywords) {
        if (token.equalsIgnoreCase(asmKw)) {
          fill(typeColor);
          text(token + " ", x, y);
          x += textWidth(token + " ");
          isKeyword = true;
          break;
        }
      }
      if (isKeyword) continue;
      
      if (token.endsWith(":")) {
        fill(labelColor);
        text(token + " ", x, y);
        x += textWidth(token + " ");
        continue;
      }
      
      fill(textColor);
      text(token + " ", x, y);
      x += textWidth(token + " ");
    }
  }
  
  popMatrix();
}

void mousePressed() {
  if (mouseX < toolbarWidth) {
    // Кнопка "Открыть"
    if (mouseY > 30 && mouseY < 70) {
      selectInput("Выберите файл", "fileSelected");
    }
    // Кнопка "Сохранить"
    else if (mouseY > 80 && mouseY < 120) {
      saveCodeToFile();
    }
    // Кнопка "Новый"
    else if (mouseY > 130 && mouseY < 170) {
      newFile();
    }
  }
}

void newFile() {
  code = "";
  filename = "untitled.cpp";
  isFileOpened = false;
}

void fileSelected(File selection) {
  if (selection == null) return;
  
  filename = selection.getName();
  if (filename.endsWith(".c") || filename.endsWith(".cpp") || 
      filename.endsWith(".h") || filename.endsWith(".hpp") || 
      filename.endsWith(".asm") || filename.endsWith(".txt")) {
    String[] lines = loadStrings(selection.getAbsolutePath());
    code = join(lines, "\n");
    isFileOpened = true;
  } else {
    println("Не поддерживаемый формат файла");
  }
}

void saveCodeToFile() {
  if (!isFileOpened) {
    selectOutput("Сохранить файл", "fileSaved");
    return;
  }
  
  saveToFile(sketchPath(filename));
}

void fileSaved(File selection) {
  if (selection == null) return;
  
  filename = selection.getName();
  saveToFile(selection.getAbsolutePath());
}

void saveToFile(String path) {
  PrintWriter output = createWriter(path);
  output.print(code);
  output.close();
  isFileOpened = true;
  println("Файл сохранён: " + filename);
}

void keyPressed() {
  // Ctrl+S / Cmd+S
  if ((key == 's' || key == 'S') && (ctrlPressed() || cmdPressed())) {
    saveCodeToFile();
    return;
  }
  
  // Ctrl+O / Cmd+O
  if ((key == 'o' || key == 'O') && (ctrlPressed() || cmdPressed())) {
    selectInput("Выберите файл", "fileSelected");
    return;
  }
  
  // Ctrl+N / Cmd+N
  if ((key == 'n' || key == 'N') && (ctrlPressed() || cmdPressed())) {
    newFile();
    return;
  }
  
  if (key == CODED) return;
  
  if (key == BACKSPACE) {
    if (code.length() > 0) {
      code = code.substring(0, code.length() - 1);
    }
  } else if (key == ENTER) {
    code += "\n";
  } else if (key == TAB) {
    code += "    ";
  } else if (key >= 32 && key <= 126) {
    code += key;
  }
}

boolean ctrlPressed() {
  return keyEvent.isControlDown();
}

boolean cmdPressed() {
  return keyEvent.isMetaDown();
}
