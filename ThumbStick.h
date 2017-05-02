#ifndef ThumbStick_h
#define ThumbStick_h

/**
 *  Class for analog JoyStick with a push button
 * 
 */
 
#include "PinButton.h"

class ThumbStick : public PinButton
{
private:
  /** Analog pin to read X */
  uint8_t m_pinX;
  /** Analog pin to read Y */
  uint8_t m_pinY;
  /** X is between 0 and 1023 */
  uint16_t m_x;
  /** Y is between 0 and 1023 */
  uint16_t m_y;

public:
  /** Just a public constructor */
  ThumbStick(uint8_t pinX, uint8_t pinY, uint8_t pinButton);
  
  /** 
   *  Updates the object state according to hardware, does de-bounce
   *    Returns true if the state changed
   *    Returns false if the state did not change
   */
  //bool update();

  /**
   *  call this (alternative to update) from the main loop passing to it the result of millis();
   *  It will call
   *          onThumbStickX(uint16_t x); -100 < x < 100
   *          onThumbStickY(uint16_t y); -100 < y < 100
   */
  bool getAndDispatchThumb(unsigned long now);

  /** -100 < x < 100 */
  virtual void onThumbStickX(int16_t x) = 0;
  /** -100 < y < 100 */
  virtual void onThumbStickY(int16_t y) = 0;

  /** returns joystick X value - 0...1023 */
  uint16_t getX() {
    return m_x;
  }
  /** returns joystick Y value - 0...1023 */
  uint16_t getY() {
    return m_y;
  }
  /** returns joystick X value -100..+100 */
  int16_t getXmapped() {
    return map(m_x, 0, 1023, -100, 100);
  }
  /** returns joystick Y value -100..+100 */
  int16_t getYmapped() {
    return map(m_y, 0, 1023, 100, -100); // also invert it!
  }

};

#endif


