

#include "config.h"

#define TrigPin D3
#define EchoPin D2
#define LED1 D4
#define LED2 D5
#define LED3 D6
float Value_cm;

#define BUTTON_PIN D4

// LED Pin
#define LED_PIN LED_BUILTIN

// button state
bool btn_state = false;
bool prv_btn_state = false;

// set up the 'led' feed
AdafruitIO_Feed *led = io.feed("led");

// set up the 'button' feed
AdafruitIO_Feed *button = io.feed("button");
AdafruitIO_Feed *distance = io.feed("distance");
AdafruitIO_Feed *waterlevel = io.feed("waterlevel");
void setup() {
  pinMode(LED1, OUTPUT);   //定义led引脚为输出模式
  pinMode(LED2, OUTPUT);   //定义led引脚为输出模式
  pinMode(LED3, OUTPUT);   //定义led引脚为输出模式
    digitalWrite(LED1, 0); //led引脚输出低电平，点亮led
    digitalWrite(LED2, 0); //led引脚输出低电平，点亮led
    digitalWrite(LED3, 0); //led引脚输出低电平，点亮led  
    delay(1000);
    digitalWrite(LED1, 1); //led引脚输出低电平，点亮led
    digitalWrite(LED2, 1); //led引脚输出低电平，点亮led
    digitalWrite(LED3, 1); //led引脚输出低电平，点亮led      
  // set button pin as an input
  pinMode(BUTTON_PIN, INPUT);

  // set LED pin as an output
  pinMode(LED_PIN, OUTPUT);

  // start the serial connection
  Serial.begin(115200);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  // wait for serial monitor to open
  while (! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // set up a message handler for the count feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  led->onMessage(handleMessage);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  led->get();

}
long timer1;
int yewei=0;
int yeweimax=100;//空液位
int yeweimin=20;//满液位
void loop() {


  io.run();

  if (millis() - timer1 > 10000)
  {
    timer1 = millis();
    digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);
    Value_cm = float( pulseIn(EchoPin, HIGH) * 17 ) / 1000; //将回波时间换算成cm
    Serial.print(Value_cm);
    Serial.println("cm");
    
    distance->save(Value_cm);//换算到 0-100
    if(Value_cm<yeweimin)
      Value_cm=yeweimin;
    if(Value_cm>yeweimax)
      Value_cm=yeweimax;
    yewei=map(Value_cm,yeweimin,yeweimax,100,0);  
    if(yewei>80)
    {
    digitalWrite(LED1, 0); //led引脚输出低电平，点亮led
    digitalWrite(LED2, 0); //led引脚输出低电平，点亮led
    digitalWrite(LED3, 0); //led引脚输出低电平，点亮led        
    }
    else if(yewei>50)
    {
    digitalWrite(LED1, 1); //led引脚输出低电平，点亮led
    digitalWrite(LED2, 0); //led引脚输出低电平，点亮led
    digitalWrite(LED3, 0); //led引脚输出低电平，点亮led        
    }  
    else if(yewei>20)
    {
    digitalWrite(LED1, 1); //led引脚输出低电平，点亮led
    digitalWrite(LED2, 1); //led引脚输出低电平，点亮led
    digitalWrite(LED3, 0); //led引脚输出低电平，点亮led        
    }    
    else 
    {
    digitalWrite(LED1, 1); //led引脚输出低电平，点亮led
    digitalWrite(LED2, 1); //led引脚输出低电平，点亮led
    digitalWrite(LED3, 1); //led引脚输出低电平，点亮led        
    }           
    waterlevel->save(yewei);
  }

}

// this function is called whenever a 'led' message
// is received from Adafruit IO. it was attached to
// the counter feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  Serial.print("received <- ");

  if (data->toPinLevel() == HIGH)
    Serial.println("HIGH");
  else
    Serial.println("LOW");

  digitalWrite(LED_PIN, data->toPinLevel());
}
