#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------- OLED 配置 ----------------
#define SCREEN_WIDTH 128 // OLED 宽度，像素
#define SCREEN_HEIGHT 64 // OLED 高度，像素

// 声明 SSD1306 显示对象，使用 I2C 通信
// 参数：宽度, 高度, &Wire, -1 (复位引脚，-1表示共享复位或无复位引脚)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);

  // ---------------- OLED 初始化 ----------------
  // SSD1306_SWITCHCAPVCC 表示内部产生高压驱动 OLED
  // 0x3C 是常见的 I2C 地址，如果不行请尝试 0x3D
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // 如果初始化失败，停止运行
  }
  
  Serial.println("OLED Initialized");

  // 清除缓冲区
  display.clearDisplay();
  
  // 设置文本属性
  display.setTextColor(SSD1306_WHITE); // 白色文字
  display.setTextSize(1);              // 标准字体大小
  display.setCursor(0, 0);             // 光标位于左上角
  
  // 打印内容
  display.println("Hello World!");
  
 

  // 将缓冲区内容刷新到屏幕显示
  display.display();
}

void loop() {
  // 静态显示无需在 loop 中执行任何操作
  // 如果需要动态效果（如闪烁、滚动），可在此处添加代码
  delay(1000);
}