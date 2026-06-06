#include <Arduino.h>

// 定义LED连接的GPIO引脚数组
// 请根据实际接线修改这里的引脚号
const int ledPins[] = {2, 4, 5, 18, 15};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]); // 计算LED数量

// 用于存储每个LED当前的状态 (true: 亮, false: 灭)
bool ledStates[5] = {false, false, false, false, false};

void setup() {
  // 初始化串口通信，波特率设置为9600
  Serial.begin(9600);
  
  // 等待串口连接就绪 (可选，主要用于某些需要等待串口监视器打开的场景)
  // while (!Serial) { ; }

  // 初始化所有LED引脚为输出模式，并初始化为熄灭状态
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // 初始状态熄灭
    ledStates[i] = false;          // 记录初始状态为灭
  }
  
  Serial.println("System Ready. Send '1'-'5' to control LEDs.");
}

void loop() {
  // 检查串口缓冲区是否有数据
  if (Serial.available() > 0) {
    // 读取一个字节的数据
    char command = Serial.read();
    
    // 解析指令 '1' 到 '5'
    // 注意：ASCII码中 '1' 的值是 49，所以 command - '1' 可以得到索引 0
    if (command >= '1' && command <= '5') {
      int index = command - '1'; // 将字符 '1'-'5' 转换为数组索引 0-4
      
      // 确保索引在有效范围内 (虽然上面判断了字符范围，但加上更安全)
      if (index >= 0 && index < numLeds) {
        // 切换LED状态
        ledStates[index] = !ledStates[index];
        
        // 根据新状态设置引脚电平
        if (ledStates[index]) {
          digitalWrite(ledPins[index], HIGH);
          // 发送反馈信息：LEDx亮
          Serial.print("LED");
          Serial.print(index + 1);
          Serial.println("亮");
        } else {
          digitalWrite(ledPins[index], LOW);
          // 发送反馈信息：LEDx灭
          Serial.print("LED");
          Serial.print(index + 1);
          Serial.println("灭");
        }
      }
    } else {
      // 如果接收到无效指令，可以选择忽略或发送错误提示
      // Serial.println("Invalid Command"); 
    }
  }
}