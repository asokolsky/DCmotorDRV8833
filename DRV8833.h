/**
 *  A library to controll the TI DRV8833 2xH-bridge
 *  
 *  nFault - ignored
 *  
 *  Alex Sokolsky asokolsky@yahoo.com 5/2017
 */
#pragma once

 
class DCMotor 
{

public:
  /** IN1, IN2.  Both should be PWM pins */
  DCMotor(byte pinIn1, byte pinIn2);

  /** start rotation at the rate of m_speed in m_bDirectionCW */
  void go();
  /** stops rotation */
  void stop();
  /** executes coast */
  void coast();
   
  /** set the speed is in 0%...100% and start rotation */
  void setSpeed(boolean bCW, byte speed)
  {
    setSpeedPWM(bCW, map(speed, 0, 100, 0, 255));
  }
  /** set the speed is in 0...255 and start rotation */
  void setSpeedPWM(boolean bCW, byte speed);
  
  /** are we spinning? */ 
  boolean isGoing() 
  {
    return m_bGoing;
  }

#ifdef DEBUG
  void DUMP(const char *szText = 0) const;
#else
  void DUMP(const char *szText = 0) {}
#endif
  
   
private:
  byte m_pinCW;  // a.k.a. IN1, PWM capable
  byte m_pinCCW; // a.k.a. IN2, PWM capable
   
  boolean m_bGoing = false;
  boolean m_bDirectionCW = true;
  /** fast or slow decay mode */
  boolean m_bFastDecay = true;
  /** current PWM speed - direction independent */
  byte m_speed = 255;
};


