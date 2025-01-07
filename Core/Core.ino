#include <0.h>

int mem[16];
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
              mem[addr] = com.toInt();
              Serial.println(mem[addr]);
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
      default: break;
    }
   if(std::size(mem) == addr){
      addr = 0;
   }
  }
}

void add(){
    mem[mem[addr + 1]] += mem[mem[addr + 2]];
    Serial.println(mem[mem[addr + 1]]);
    addr += 3;
}
void sub(){
    mem[mem[addr + 1]] -= mem[mem[addr + 2]];
    Serial.println(mem[mem[addr + 1]]);
    addr += 3;
}
void inc(){
    mem[mem[addr + 1]]++;
    Serial.println(mem[mem[addr + 1]]);
    addr += 2;
}
void dec(){
    mem[mem[addr + 1]]--;
    Serial.println(mem[mem[addr + 1]]);
    addr += 2;
}
void mov(){
    mem[mem[addr + 1]] = mem[addr + 2];
    addr += 3;
    Serial.println(mem[mem[addr + 1]]);
}
void mova(){
    mem[mem[addr + 1]] = mem[mem[addr + 2]];
    addr += 3;
    Serial.println(mem[mem[addr + 1]]);
}