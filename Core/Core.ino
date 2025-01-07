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
          mem[mem[addr + 1]] += mem[mem[addr + 2]];
          Serial.println(mem[mem[addr + 1]]);
          addr += 3;
          break;
     case 0x02: //sub
          mem[mem[addr + 1]] -= mem[mem[addr + 2]];
          Serial.println(mem[mem[addr + 1]]);
          addr += 3;
          break;
     case 0x03: //inc
          mem[mem[addr + 1]]++;
          Serial.println(mem[mem[addr + 1]]);
          addr += 2;
          break;
      case 0x04: //dec
          mem[mem[addr + 1]]--;
          Serial.println(mem[mem[addr + 1]]);
          addr += 2;
          break;
    }
   if(std::size(mem) == addr){
      addr = 0;
   }
  }
}