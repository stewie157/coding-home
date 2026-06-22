#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------------- 引脚定义 ----------------------
// 步进电机引脚
#define MOTOR_IN1 27
#define MOTOR_IN2 14
#define MOTOR_IN3 12
#define MOTOR_IN4 13

// OLED 引脚 (I2C)
#define OLED_SDA 23
#define OLED_SCL 22

// 开关引脚
#define SW1_PIN 35
#define SW2_PIN 34
#define SW3_PIN 36 
#define SW4_PIN 39 

// ---------------------- 参数定义 ----------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C 

// 【修改点1】定义60度所需的步数
// 基于 28BYJ-48 (1.8度, 64减速, 8拍): 360度=12800步 -> 60度 ≈ 2133步
#define STEPS_FOR_60_DEG 1066 

// 【修改点2】停顿时间: 0.5s, 1s, 1.5s, 2s
const unsigned long delayTimes[] = {50,500,1000,2000};

// ---------------------- 全局对象 ----------------------
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 8拍励磁序列
const int stepSequence[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

int currentStep = 0;
int currentMode = 0; 
bool lastSwState[4] = {HIGH, HIGH, HIGH, HIGH}; 

// ---------------------- 函数声明 ----------------------
void setupMotor();
void setupOLED();
void updateDisplay(int mode);
void rotate60Degrees(); // 建议重命名函数以反映新功能
int readSwitches();
void debounceAndCheckSwitches();
void rotateSteps(int steps);

// ---------------------- 初始化 ----------------------
void setup() {
  Serial.begin(115200);
  
  setupMotor();
  setupOLED();
  
  pinMode(SW1_PIN, INPUT_PULLUP);
  pinMode(SW2_PIN, INPUT_PULLUP);
  pinMode(SW3_PIN, INPUT_PULLUP);
  pinMode(SW4_PIN, INPUT_PULLUP);

  updateDisplay(1); 
  Serial.println("System Started");
}

// ---------------------- 主循环 ----------------------
void loop() {
  debounceAndCheckSwitches();
  
  // 【修改点3】执行转动60度
  rotate60Degrees();
  
  // 根据当前模式停顿
  delay(delayTimes[currentMode]);
}

// ---------------------- 函数实现 ----------------------

void setupMotor() {
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
}

void setupOLED() {
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
}

void updateDisplay(int mode) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("mode:");
  display.println(mode);
  display.display();
}

int readSwitches() {
  if (digitalRead(SW1_PIN) == LOW) return 1;
  if (digitalRead(SW2_PIN) == LOW) return 2;
  if (digitalRead(SW3_PIN) == LOW) return 3;
  if (digitalRead(SW4_PIN) == LOW) return 4;
  return 0;
}

void debounceAndCheckSwitches() {
  int pressedKey = readSwitches();
  
  if (pressedKey != 0) {
    delay(50); 
    if (readSwitches() == pressedKey) {
      int newModeIndex = pressedKey - 1; 
      if (currentMode != newModeIndex) {
        currentMode = newModeIndex;
        updateDisplay(pressedKey);
        Serial.printf("Mode changed to: %d\n", pressedKey);
      }
      while(readSwitches() != 0) {
        delay(10);
      }
      delay(50);
    }
  }
}

void rotateSteps(int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(MOTOR_IN1, stepSequence[currentStep][0]);
    digitalWrite(MOTOR_IN2, stepSequence[currentStep][1]);
    digitalWrite(MOTOR_IN3, stepSequence[currentStep][2]);
    digitalWrite(MOTOR_IN4, stepSequence[currentStep][3]);
    
    delayMicroseconds(2000); 
    
    currentStep++;
    if (currentStep >= 8) {
      currentStep = 0;
    }
  }
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
}

// 【修改点4】重命名并更新函数内容
void rotate60Degrees() {
  rotateSteps(STEPS_FOR_60_DEG);
}