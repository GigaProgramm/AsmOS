#include <0.h>

int mem[4096];
int addr;
bool s; //start read op_mem
byte command;


void setup() {
  Serial.begin(115200);
}
void loop() {
  if (Serial.available()) {
        String com = Serial.readStringUntil('\n');
        if(!s) {
          if (com == "r") {
            addr++;
            Serial.print("r ");
            Serial.println(addr);
          } else if (com == "s"){
            Serial.println("Processor is start");
            addr = 0;
            s = true;
          } else {
            if(com == "add"){
              mem[addr] = 1;
              Serial.println("add");
            }
            else if(com == "sub"){
              mem[addr] = 2;
              Serial.println("sub");
            }
            else if(com == "inc"){
              mem[addr] = 3;
              Serial.println("inc");
            }
            else if(com == "dec"){
              mem[addr] = 4;
              Serial.println("dec");
            }
            else if(com == "mov"){
              mem[addr] = 5;
              Serial.println("mov");
            }
            else if(com == "mova"){
              mem[addr] = 6;
              Serial.println("mova");
            }
            else if(com == "jmp"){
              mem[addr] = 7;
              Serial.println("jmp");
            }
            else if(com == "jmpa"){
              mem[addr] = 8;
              Serial.println("jmpa");
            }
            else if(com == "cout"){
              mem[addr] = 9;
              Serial.println("cout");
            } 
            else {
              mem[addr] = com.toInt();
              Serial.println(mem[addr]);
            }
          }
        }
  }
  command = mem[addr];
  if(s){
    switch(command){
      case 0x00:
          break;
      case 0x01: //add
          add();
          break;
      case 0x02: //sub
          sub();
          break;
      case 0x03: //inc
          inc();
          break;
      case 0x04: //dec
          dec();
          break;
      case 0x05: //mov
          mov();
          break;
      case 0x06: //mova
          mova();
          break;
      case 0x07:
          jmp();
          break;
      case 0x08:
          jmpa();
          break;
      case 0x09:
          cout();
          break;
      default: break;
    }
   if(std::size(mem) == addr){
      addr = 0;
   }
  }
}

void add(){
    mem[mem[addr + 1]] += mem[mem[addr + 2]];
    addr += 3;
}
void sub(){
    mem[mem[addr + 1]] -= mem[mem[addr + 2]];
    addr += 3;
}
void inc(){
    mem[mem[addr + 1]]++;
    addr += 2;
}
void dec(){
    mem[mem[addr + 1]]--;
    addr += 2;
}
void mov(){
    mem[mem[addr + 1]] = mem[addr + 2];
    addr += 3;
}
void mova(){
    mem[mem[addr + 1]] = mem[mem[addr + 2]];
    addr += 3;
}
void jmp(){
    addr = mem[addr + 1];
}
void jmpa(){
    addr = mem[mem[addr + 1]];
}
void cout(){
    Serial.println(mem[mem[addr + 1]]);
    addr += 2;
}