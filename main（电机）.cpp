#include <Arduino.h>

// 定义连接 ULN2003 的 GPIO 引脚
// 顺序对应 IN1, IN2, IN3, IN4
const int motorPins[] = {2,4,5,18}; 
const int numMotorPins = 4;

// 28BYJ-48 步进电机参数
// 减速比 1:64，步距角 5.625度/64 = 0.08789度
// 一圈步数 = 360 / 0.08789 ≈ 4096 半步步? 
// 注意：28BYJ-48 通常标称一圈 2048 步是指全步模式下的脉冲数，
// 但在使用 8 拍半步驱动序列时，一圈通常需要 4096 个脉冲步骤。
// 这里我们使用标准的 8 拍半步驱动序列，因此一圈需要 4096 次循环。
// 如果你希望严格对应“2048步”的概念，可以使用 4 拍全步驱动，但扭矩较小。
// 为了平稳性，推荐使用 8 拍半步驱动，此时一圈为 4096 步。
const int stepsPerRevolution = 4096; // 8拍半步驱动模式下一圈的步数

// 步进延时，单位微秒。数值越小转速越快，但太小会导致丢步或停转。
// 建议范围：1000 (快) - 5000 (慢)
int stepDelay = 1000; 

// 8拍半步驱动序列 (Half-step sequence)
// 每个数组元素代表4个引脚的状态 (IN1, IN2, IN3, IN4)
// 1 表示 HIGH, 0 表示 LOW
const int stepSequence[8][4] = {
  {1, 0, 0, 0}, // Step 1
  {1, 1, 0, 0}, // Step 2
  {0, 1, 0, 0}, // Step 3
  {0, 1, 1, 0}, // Step 4
  {0, 0, 1, 0}, // Step 5
  {0, 0, 1, 1}, // Step 6
  {0, 0, 0, 1}, // Step 7
  {1, 0, 0, 1}  // Step 8
};

void setup() {
  // 初始化串口用于调试
  Serial.begin(115200);
  
  // 初始化电机引脚为输出
  for (int i = 0; i < numMotorPins; i++) {
    pinMode(motorPins[i], OUTPUT);
    digitalWrite(motorPins[i], LOW);
  }
  
  Serial.println("Motor Control Ready");
}

// 执行一步
void stepMotor(int stepIndex) {
  // stepIndex 应该是 0 到 7 之间的值
  int idx = stepIndex % 8;
  for (int i = 0; i < numMotorPins; i++) {
    digitalWrite(motorPins[i], stepSequence[idx][i]);
  }
  delayMicroseconds(stepDelay);
}

// 正转指定步数
void rotateForward(int steps) {
  for (int i = 0; i < steps; i++) {
    stepMotor(i % 8); // 依次取序列 0->1->...->7->0...
  }
}

// 反转指定步数
void rotateBackward(int steps) {
  for (int i = 0; i < steps; i++) {
    // 反向遍历序列: 7->6->...->0->7...
    // 计算反向索引: (8 - (i % 8)) % 8 
    // 或者更简单的: stepMotor(7 - (i % 8));
    int reverseIndex = (8 - (i % 8)) % 8;
    stepMotor(reverseIndex);
  }
}

void loop() {
  Serial.println("Rotating Forward 1 Revolution...");
  rotateForward(stepsPerRevolution);
  
  Serial.println("Paused for 2 seconds...");
  delay(2000); // 停顿2秒
  
  Serial.println("Rotating Backward 1 Revolution...");
  rotateBackward(stepsPerRevolution);
  
  Serial.println("Cycle Complete. Pausing for 2 seconds...");
  delay(2000); // 停顿2秒后再次循环
}