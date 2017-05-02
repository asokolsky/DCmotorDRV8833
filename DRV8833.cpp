#include <Arduino.h>
#include "Trace.h"
#include "DRV8833.h"

DCMotor::DCMotor(byte pinCW, byte pinCCW) :
  m_pinCW(pinCW), m_pinCCW(pinCCW)
{ 
  pinMode(m_pinCW, OUTPUT);
  pinMode(m_pinCCW, OUTPUT);
}

#ifdef DEBUG
void DCMotor::DUMP(const char *szText /*= 0*/) const
{
  if(szText != 0) {
    DEBUG_PRNT(szText);
  }
  DEBUG_PRINT(" DCMotorDRV8833@"); DEBUG_PRINTDEC((int)this); 
  
  DEBUG_PRINT(" m_pinCW="); DEBUG_PRINTDEC((int)m_pinCW);
  DEBUG_PRINT(" m_pinCCW="); DEBUG_PRINTDEC((int)m_pinCCW);

  DEBUG_PRINT(" m_bGoing="); DEBUG_PRINTDEC((int)m_bGoing);
  DEBUG_PRINT(" m_bDirectionCW="); DEBUG_PRINTDEC((int)m_bDirectionCW);
  DEBUG_PRINT(" m_speed="); DEBUG_PRINTDEC((int)m_speed);
   
  DEBUG_PRINTLN("");
}
#endif


void DCMotor::go() 
{
  DUMP("DCMotor::go(");
  if(m_bFastDecay)
  {
    if(m_bDirectionCW)
    {
      // fast decay forward
      analogWrite(m_pinCW,  m_speed);
      digitalWrite(m_pinCCW, LOW);
    }
    else
    {
      // fast decay reverse
      digitalWrite(m_pinCW,  LOW);
      analogWrite(m_pinCCW, m_speed);
    }
  }
  else
  {
    if(m_bDirectionCW)
    {
      // slow decay forward
      digitalWrite(m_pinCW,  HIGH);
      analogWrite(m_pinCCW, m_speed);
    }
    else
    {
      // slow decay reverse
      analogWrite(m_pinCW,  m_speed);
      digitalWrite(m_pinCCW, HIGH);
    }
  }
  m_bGoing = true;
}

void DCMotor::stop() 
{
  DUMP("DCMotor::stop()");
  digitalWrite(m_pinCW, HIGH);
  digitalWrite(m_pinCCW, HIGH);
  m_bGoing = false;
}

void DCMotor::coast() 
{
  DUMP("DCMotor::coast()");
  digitalWrite(m_pinCW, LOW);
  digitalWrite(m_pinCCW, LOW);
  m_bGoing = true;
}

/*void DCMotor::setSpeedRegulation(byte speedRegulation) 
{
  m_speedRegulation = speedRegulation;
  if(m_bGoing)
    go();
}*/

void DCMotor::setSpeed(boolean bCW, byte speed) 
{
  DEBUG_PRINT("DCMotor::setSpeed bCW="); DEBUG_PRINTDEC(bCW);  DEBUG_PRINT(" speed=");  DEBUG_PRINTDEC(speed);  DEBUG_PRINTLN("");
  
  m_bDirectionCW = bCW;
  m_speed = map(speed, 0, 100, 0, 255);
  go();
}


