#include <Arduino.h>
#include "Trace.h"
#include "DRV8833.h"
#include "ThumbStick.h"

/**
 * Divides a given PWM pin frequency by a divisor.
 *
 * The resulting frequency is equal to the base frequency divided by
 * the given divisor:
 *   - Base frequencies:
 *      o The base frequency for pins 3, 9, 10, and 11 is 31250 Hz.
 *      o The base frequency for pins 5 and 6 is 62500 Hz.
 *   - Divisors:
 *      o The divisors available on pins 5, 6, 9 and 10 are: 1, 8, 64,
 *        256, and 1024.
 *      o The divisors available on pins 3 and 11 are: 1, 8, 32, 64,
 *        128, 256, and 1024.
 *
 * PWM frequencies are tied together in pairs of pins. If one in a
 * pair is changed, the other is also changed to match:
 *   - Pins 5 and 6 are paired on timer0
 *   - Pins 9 and 10 are paired on timer1
 *   - Pins 3 and 11 are paired on timer2
 *
 * Note that this function will have side effects on anything else
 * that uses timers:
 *   - Changes on pins 3, 5, 6, or 11 may cause the delay() and
 *     millis() functions to stop working. Other timing-related
 *     functions may also be affected.
 *   - Changes on pins 9 or 10 will cause the Servo library to function
 *     incorrectly.
 *
 * Thanks to macegr of the Arduino forums for his documentation of the
 * PWM frequency divisors. His post can be viewed at:
 *   http://forum.arduino.cc/index.php?topic=16612#msg121031
 */
static void setPwmFrequency(int pin, int divisor) 
{
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x07; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}


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
    DEBUG_PRINT("onThumbStickX, x="); DEBUG_PRINTDEC(x); DEBUG_PRINTLN("");    
    if(x == 0)
      g_motor1.stop();
    else
      g_motor1.setSpeed((x>0), abs(x));
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

  setPwmFrequency(5, 1);
  setPwmFrequency(6, 1);
}

void loop()
{
  unsigned long ulNow = millis();
  //g_stick.getAndDispatchKey(ulNow);
  g_stick.getAndDispatchThumb(ulNow);
  delay(1);
}

