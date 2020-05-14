
#include <Wire.h> 
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int YPR[3];
unsigned char Re_buf[8],counter=0;
unsigned char sign=0;
int led = 13;


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Serial.begin(115200);  
  delay(2000);    
  Serial.write(0XA5); 
  Serial.write(0X52);    //初始化GY25,连续输出模式
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
}
//-------------------------------------------------------------
void loop() {
  if(sign)
  {  
     sign=0;
     if(Re_buf[0]==0xAA && Re_buf[7]==0x55)        //检查帧头，帧尾
     {  	       
            YPR[0]=(Re_buf[1]<<8|Re_buf[2])/100;   //合成数据，去掉小数点后2位
            YPR[1]=(Re_buf[3]<<8|Re_buf[4])/100;
            YPR[2]=(Re_buf[5]<<8|Re_buf[6])/100;

            display.clearDisplay();
          
            display.setTextSize(2); // Draw 2X-scale text
            display.setTextColor(SSD1306_WHITE);
            display.setCursor(0, 0);
            display.print("Dir: ");
            display.println(YPR[0]);
            display.setCursor(0, 20);
            display.print("Pit: ");
            display.println(YPR[1]);
            display.setCursor(0, 40);
            display.print("Rol: ");
            display.println(YPR[2]);
            display.display();      // Show initial text

            delay(100);           
   }
  } 
} 
//---------------------------------------------------------------
void serialEvent() {
  while (Serial.available()) {   
    Re_buf[counter]=(unsigned char)Serial.read();
    if(counter==0&&Re_buf[0]!=0xAA) return;      // 检查帧头         
    counter++;       
    if(counter==8)                //接收到数据
    {    
       counter=0;                 //重新赋值，准备下一帧数据的接收 
       sign=1;
    }      
  }
}

