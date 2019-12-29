#include "particle.h"
#include "Arduino.h"
#include "mainSettings.h"

#ifdef TRACE_ON
//  #define TRACE_PARTICLE
//  #define TRACE_PARTICLE_HIGH
#endif

#define FIREWORK_STEP_TIME_SCALE 15
#define FIREWORK_FADE_TIME_SCALE 30
#define FIREWORK_TIME_SCALE_START 10

Particle::Particle(void)
{

}

void Particle::init(PictureLamp *pictureLamp)
{
  m_pathIndex=255;
  m_pictureLamp=pictureLamp;
}


void Particle::start(byte *pathArray,int time_scale,float red, float green, float blue, byte globalFadeStart, float globalFadeRate) 
{
  #ifdef TRACE_PARTICLE
      Serial.println(F("TRACE_PARTICLE> start "));
  #endif
  m_pathArray=pathArray;
  m_color_red=red;
  m_color_green=green;
  m_color_blue=blue;
  m_pathFadeStartIndex=globalFadeStart;
  m_pathFadeRate = globalFadeRate;
  m_pathIndex=0;
  m_previousStepMillis=0;
  m_stepDuration=0;
  m_time_scale=time_scale;
  process();
}

void Particle::process()
{
  if(!inProgress() || ((millis()-m_previousStepMillis)<m_stepDuration)) return;
  #ifdef TRACE_PARTICLE_HIGH
      Serial.print(F("TRACE_PARTICLE_HIGH> time gone="));Serial.print((millis()-m_previousStepMillis));
      Serial.print(F(" m_pathIndex:"));Serial.println(m_pathIndex);
  #endif
  byte nextLamp=m_pathArray[m_pathIndex]-1;
  if(nextLamp>=254) { // End of path indicator found
    m_pathIndex=255;
    #ifdef TRACE_PARTICLE
       Serial.println(F("TRACE_PARTICLE> done"));
   #endif
     return;
  }
  m_previousStepMillis=millis();
 
  byte speedByte=m_pathArray[m_pathIndex+1];
  int pixelFadeDuration=(speedByte>>4);
  pixelFadeDuration=pixelFadeDuration*FIREWORK_FADE_TIME_SCALE+FIREWORK_TIME_SCALE_START;
  m_stepDuration=speedByte&0x000f;
  m_stepDuration=m_time_scale*m_stepDuration*FIREWORK_STEP_TIME_SCALE+FIREWORK_TIME_SCALE_START;

  // TODO: Variance in step speed and fading
  m_pictureLamp[nextLamp].setCurrentColor(m_color_red,m_color_green,m_color_blue);
  m_pictureLamp[nextLamp].setTargetColor(0,0,0);
  m_pictureLamp[nextLamp].startTransition(pixelFadeDuration);
  m_color_red*=m_pathFadeRate;
  m_color_blue*=m_pathFadeRate;
  m_color_green*=m_pathFadeRate;
  
  m_pathIndex+=2;
  #ifdef TRACE_PARTICLE_HIGH
      Serial.print(F("TRACE_PARTICLE_HIGH> next duration="));Serial.println(m_stepDuration);
  #endif
}


        
