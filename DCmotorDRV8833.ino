#include <Arduino.h>
#include "Trace.h"
#include "DRV8833.h"
#include "ThumbStick.h"

DCMotor g_motor1(5/*pinCW*/, 6/*pinCCW*/);

/**
 * Derive a class in order to provide callbacks
 */
class MyThumbStick : public ThumbStick
{
 
public:  
  MyThumbStick(uint8_t pinX, uint8_t pinY, uint8_t pinButton) : 
    ThumbStick(pinX, pinY, pinButton)
  {
      
  }
  
  bool onUserInActivity(unsigned long ulNow)
  {
    return false;
  }
  bool onKeyAutoRepeat()
  {
    return false;
  }
  bool onKeyDown()
  {
    return false;
  }
  bool onLongKeyDown()
  {
    return false;
  }
  bool onKeyUp(bool bLong)
  {
    return false;
  }
  bool onClick()
  {
    DEBUG_PRINTLN("onClick()");
    return false;
  }
  bool onDoubleClick()
  {
    return false;
  }
  
  /** -100 < x < 100 */
  void onThumbStickX(int16_t x)
  {
    const int16_t zeroX = -9;
    const int16_t zeroDelta = 5;
    
    DEBUG_PRINT("onThumbStickX, x="); DEBUG_PRINTDEC(x); DEBUG_PRINTLN("");
    x = x - zeroX;
    if(abs(x) < zeroDelta)
    {
      g_motor1.stop();
    }
    else
    {
      g_motor1.setSpeed(x > 0, (x > 0) ? (byte)x : (byte)(-x));
    }
    
  }
  /** -100 < y < 100 */
  virtual void onThumbStickY(int16_t y)
  {
    
  }

  
};

MyThumbStick g_stick(A0 /*pinX*/, A1 /*pinY */, 2 /* pinButton */);

void setup()
{  
  Serial.begin(115200);
  
  delay(1000);   
  //while(!Serial)  ; // wait for serial port to connect. Needed for Leonardo only
  DEBUG_PRINTLN("DCmotorTB6612FNG test!");
    
}

void loop()
{
  unsigned long ulNow = millis();
  //g_stick.getAndDispatchKey(ulNow);
  g_stick.getAndDispatchThumb(ulNow);
  delay(1);
}

