#include <OneWire.h> 
#include <DallasTemperature.h>
#include "U8glib.h"
#include <dht11.h>                     //引用dht11库文件，使得下面可以调用相关参数
#define ONE_WIRE_BUS 2  // 定义DS18B20数据口连接arduino的2号IO上
#define DHT11PIN 13          //定义温湿度针脚号为2号引脚
#define hit_io 3
#define hiton 25
#define hitoff 32
//#define  480000 // 8分钟 
//#define

dht11 DHT11;                     //实例化一个对象
// 初始连接在单总线上的单总线设备
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//定义一些全局变量
int Water = 10;
int Room_T = 11;
int Room_S = 12;
int Hit = 0;
unsigned long on_time = 480000 ; //8分钟 持续工作on_time
unsigned long down_time = 120000 ; //2分钟 休息关机 down_time
unsigned long work_down_time =0; //承载强制休息关机 = nowtime+on_time
unsigned long work_on_time =0; //承载允许设备开机


// 中文字体定义
const uint8_t shi []   U8G_PROGMEM  =
{0x02,0x00,0x01,0x00,0x7F,0xFE,0x40,0x02,0x80,0x04,0x3F,0xF8,0x04,0x00,0x08,0x20,
0x1F,0xF0,0x01,0x10,0x01,0x00,0x3F,0xF8,0x01,0x00,0x01,0x00,0xFF,0xFE,0x00,0x00
};
const uint8_t wen []   U8G_PROGMEM  =
{0x00,0x00,0x23,0xF8,0x12,0x08,0x12,0x08,0x83,0xF8,0x42,0x08,0x42,0x08,0x13,0xF8,
0x10,0x00,0x27,0xFC,0xE4,0xA4,0x24,0xA4,0x24,0xA4,0x24,0xA4,0x2F,0xFE,0x00,0x00
};
const uint8_t du []   U8G_PROGMEM  =
{0x01,0x00,0x00,0x80,0x3F,0xFE,0x22,0x20,0x22,0x20,0x3F,0xFC,0x22,0x20,0x22,0x20,
0x23,0xE0,0x20,0x00,0x2F,0xF0,0x24,0x10,0x42,0x20,0x41,0xC0,0x86,0x30,0x38,0x0E
};
const uint8_t jia []   U8G_PROGMEM  =
{0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x7C,0xFE,0x44,0x12,0x44,0x12,0x44,0x12,0x44,
0x12,0x44,0x12,0x44,0x12,0x44,0x12,0x44,0x22,0x44,0x22,0x7C,0x4A,0x44,0x84,0x00
};
const uint8_t re []   U8G_PROGMEM  =
{0x10,0x40,0x10,0x40,0x10,0x40,0xFD,0xF8,0x10,0x48,0x10,0x48,0x1C,0xC8,0x30,0x48,
0xD0,0xAA,0x10,0xAA,0x51,0x06,0x22,0x02,0x00,0x00,0x48,0x88,0x44,0x44,0x84,0x44
};
const uint8_t si []   U8G_PROGMEM  =
{0x00,0x00,0x27,0xF8,0x14,0x08,0x14,0x08,0x87,0xF8,0x44,0x08,0x44,0x08,0x17,0xF8,
0x11,0x20,0x21,0x20,0xE9,0x24,0x25,0x28,0x23,0x30,0x21,0x20,0x2F,0xFE,0x00,0x00
};
const uint8_t kai []   U8G_PROGMEM  =
{0x00,0x00,0x7F,0xFC,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0xFF,0xFE,
0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x20,0x10,0x20,0x20,0x20,0x40,0x20
};
const uint8_t guan []   U8G_PROGMEM  =
{0x10,0x10,0x08,0x10,0x08,0x20,0x00,0x00,0x3F,0xF8,0x01,0x00,0x01,0x00,0x01,0x00,
0xFF,0xFE,0x01,0x00,0x02,0x80,0x02,0x80,0x04,0x40,0x08,0x20,0x30,0x18,0xC0,0x06
};
const uint8_t sui []   U8G_PROGMEM  =
{0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x08,0x01,0x08,0x7D,0x90,0x05,0xA0,0x09,0x40,
0x09,0x40,0x11,0x20,0x11,0x10,0x21,0x08,0x41,0x06,0x81,0x00,0x05,0x00,0x02,0x00
};

//屏幕点亮
U8GLIB_SSD1306_128X64 u8g(12, 11, 10, 9); // SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9

// 字体设置函数
void setfont(){
u8g.setFont(u8g_font_unifont);
}
//字体显示函数
void draw(void) {
  // graphic commands to redraw the complete screen should be placed here  
  setfont();
  //u8g.setFont(u8g_font_osb21);
  u8g.drawStr( 0, 12, "Prism control v1");
  //u8g.drawStr( 0, 27, "Hello :World!");
  
  //u8g.setFont(u8g_font_7x14B);
  //u8g.setPrintPos(0, 20);
  //u8g.print(t1);
  
  u8g.drawBitmapP ( 0 , 17 , 2 , 16 , jia);
  u8g.drawBitmapP ( 17 , 17 , 2 , 16 , re);
  u8g.drawStr( 32, 33, ":");
  //if(XXX=on)
  if(Hit == 1){
  u8g.drawBitmapP ( 42 , 17 , 2 , 16 , kai);
  }// 开or关
  else if(Hit == 0){
  u8g.drawBitmapP ( 42 , 17 , 2 , 16 , guan);
  }// 开or关}
  u8g.drawStr( 60, 30, "|");
  u8g.drawStr( 69, 30, "T");
  u8g.drawBitmapP ( 79 , 17 , 2 , 16 , sui);
  //u8g.drawBitmapP ( 79 , 17 , 2 , 16 , wen);
  //u8g.drawBitmapP ( 96 , 17 , 2 , 16 , du);
  u8g.drawStr( 94, 33, ":");
  u8g.setFont(u8g_font_7x14B);
  u8g.setPrintPos(104, 32);
  u8g.print(Water);
    setfont();
  u8g.drawStr( 120, 32, "c");
  u8g.drawBitmapP ( 0 , 45 , 2 , 16 , shi);
  u8g.drawBitmapP ( 17 , 45 , 2 , 16 , wen);  
  u8g.drawStr( 32, 62, ":");
  u8g.setFont(u8g_font_7x14B);
  u8g.setPrintPos(39, 61);
  u8g.print(Room_T);
    setfont();
  u8g.drawStr( 53, 61, "c");
  u8g.drawBitmapP ( 63 , 45 , 2 , 16 , si);
  u8g.drawBitmapP ( 80 , 45 , 2 , 16 , du); 
    setfont();
  u8g.drawStr( 94, 62, ":");
    u8g.setFont(u8g_font_7x14B);
  u8g.setPrintPos(103, 61);
  u8g.print(Room_S);
    setfont();
  u8g.drawStr( 119, 62, "%");
  
  
  
  
  
}
//温湿度检测
void getdht11(){
    sensors.requestTemperatures(); // 发送命令获取温度
  Serial.println(sensors.getTempCByIndex(0));
  Water = sensors.getTempCByIndex(0);

  
  int chk = DHT11.read(DHT11PIN);                 //将读取到的值赋给chk

  int tem=(float)DHT11.temperature;               //将温度值赋值给tem
  int hum=(float)DHT11.humidity;                   //将湿度值赋给hum
  Serial.print("Tempeature:");                        //打印出Tempeature:
  Serial.println(tem);                                     //打印温度结果
  

  Serial.print("Humidity:");                            //打印出Humidity:
  Serial.print(hum);                                     //打印出湿度结果
  Serial.println("%");                                  //打印出%
  
    Room_S = int(hum);
    Room_T = int(tem);
    Water = int(sensors.getTempCByIndex(0));
    Serial.print(Room_S);
    Serial.println("*");       
    Serial.print(Room_T);
    Serial.println("*"); 
    Serial.print(Water);
    Serial.println("*"); 
  Serial.print(Hit);
    Serial.println("*"); 
    
  // rebuild the picture after some delay
  delay(1000);
}
//加热函数
void hitnow(){
  unsigned long nowtime=millis();
  if(Water<hiton)
  {
    if(Hit == 0)
    {
      if(work_on_time<nowtime)
      {
        Hit=1 ;
        digitalWrite(hit_io,LOW);
        delay(100);
        work_on_time=0 ;
        work_down_time = nowtime + on_time;
      }
      if(work_on_time>nowtime)
      {
        Hit=0 ;
        digitalWrite(hit_io,HIGH);
        delay(100);
      }
    }
    if(Hit==1)
    {
      if(work_down_time>nowtime)
      {
        Hit=1 ;
        digitalWrite(hit_io,LOW);
        delay(100);
      }
      if(work_down_time<nowtime)
      {
        Hit=0 ;
        digitalWrite(hit_io,HIGH);
        delay(100);
        work_down_time =0 ;
        work_on_time = nowtime + down_time;
      }
    }
  }
  if(Water>hitoff)
  {
    Hit=0;
    digitalWrite(hit_io,HIGH);
    delay(1000);
    work_down_time = 0 ;
    work_on_time = 0 ;
  }

}
//第一次setup（）
void setup(void) {
  
  if ( u8g.getMode() == U8G_MODE_R3G3B2 ) {
    u8g.setColorIndex(255);     // white
  }
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT ) {
    u8g.setColorIndex(3);         // max intensity
  }
  else if ( u8g.getMode() == U8G_MODE_BW ) {
    u8g.setColorIndex(1);         // pixel on
  }
  else if ( u8g.getMode() == U8G_MODE_HICOLOR ) {
    u8g.setHiColorByRGB(255,255,255);
  }
  // 设置串口通信波特率
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
 
  // 初始库
  sensors.begin();
  
  //pinMode(8, OUTPUT);
  pinMode(DHT11PIN,OUTPUT);
  pinMode(hit_io,OUTPUT);
}
//循环
void loop(void) {
  // picture loop
  getdht11();
  hitnow();

    u8g.firstPage();  
  do {
    
    draw();
  } while( u8g.nextPage() );
}