#include <Arduino.h>

// 定义LED连接的GPIO引脚数组
// 请根据实际接线修改这里的引脚号
const int ledPins[] = {2, 4, 5, 18, 15};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]); // 计算LED数量
const int delayTime = 200; // 流水灯切换间隔时间（毫秒），可修改此值观察速度变化

void setup() {
  // 初始化所有LED引脚为输出模式
  for (int i = 0; i < numLeds; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW); // 初始状态熄灭
  }
}

void loop() {
  // 正向流水：从左到右依次点亮
  for (int i = 0; i < numLeds; i++) {
    digitalWrite(ledPins[i], HIGH); // 点亮当前LED
    delay(delayTime);               // 延时
    digitalWrite(ledPins[i], LOW);  // 熄灭当前LED
  }

  // 反向流水：从右到左依次点亮（可选，增加效果丰富度）
  for (int i = numLeds - 1; i >= 0; i--) {
    digitalWrite(ledPins[i], HIGH); // 点亮当前LED
    delay(delayTime);               // 延时
    digitalWrite(ledPins[i], LOW);  // 熄灭当前LED
  }
}