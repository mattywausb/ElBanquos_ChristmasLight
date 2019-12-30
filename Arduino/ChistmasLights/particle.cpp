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

//                            0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23
const byte lamp_mirror_map[]={0, 4, 3, 2, 1, 5, 9, 8, 7, 6,11,10,19,18,17,16,15,14,13,12,20,22,21,23};

Particle::Particle(void)
{

}

void Particle::init(PictureLamp *pictureLamp)
{
  m_pathIndex=255;
  m_pictureLamp=pictureLamp;
}


void Particle::start(byte *pathArray,bool mirror,int time_scale,t_color_hsv color, byte globalFadeStart, float globalFadeRate) 
{
  #ifdef TRACE_PARTICLE
      Serial.println(F("TRACE_PARTICLE> start "));
  #endif
  m_pathArray=pathArray;
  m_color_hsv=color;
  m_pathFadeStartIndex=globalFadeStart;
  m_pathFadeRate = globalFadeRate;
  m_pathIndex=0;
  m_previousStepMillis=0;
  m_stepDuration=0;
  m_time_scale=time_scale;
  m_mirror=mirror,
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
  
  if(m_mirror) nextLamp=lamp_mirror_map[nextLamp];
 
  byte speedByte=m_pathArray[m_pathIndex+1];
  int pixelFadeDuration=(speedByte>>4);
  pixelFadeDuration=pixelFadeDuration*FIREWORK_FADE_TIME_SCALE+FIREWORK_TIME_SCALE_START;
  m_stepDuration=speedByte&0x000f;
  m_stepDuration=m_time_scale*m_stepDuration*FIREWORK_STEP_TIME_SCALE+FIREWORK_TIME_SCALE_START;

  // TODO: Variance in step speed and fading
  t_color_rgb color=get_color_rgb();
  m_pictureLamp[nextLamp].setCurrentColor(color.r,color.g,color.b);
  m_pictureLamp[nextLamp].setTargetColor(0,0,0);
  m_pictureLamp[nextLamp].startTransition(pixelFadeDuration);
  m_color_hsv.v*=m_pathFadeRate;
  
  m_pathIndex+=2;
  #ifdef TRACE_PARTICLE_HIGH
      Serial.print(F("TRACE_PARTICLE_HIGH> next duration="));Serial.println(m_stepDuration);
  #endif
}



t_color_rgb Particle::get_color_rgb()
{
    float      hue_segment, p, q, t, hue_segment_remainder;
    long        hue_segment_int;
    t_color_rgb         out;

    if(m_color_hsv.s <= 0.0) {       // zero or negative saturation will result in pure value 
        out.r = m_color_hsv.v;
        out.g = m_color_hsv.v;
        out.b = m_color_hsv.v;
        return out;
    }
    hue_segment = m_color_hsv.h;
    while(hue_segment >= 360.0) hue_segment -=360.0;
    hue_segment /= 60.0;
    hue_segment_int = (long)hue_segment;
    hue_segment_remainder = hue_segment - hue_segment_int;
    p = m_color_hsv.v * (1.0 - m_color_hsv.s); // whitening color element
    #define hsv_get_q   m_color_hsv.v * (1.0 - (m_color_hsv.s * hue_segment_remainder))
    #define hsv_get_t   m_color_hsv.v * (1.0 - (m_color_hsv.s * (1.0 - hue_segment_remainder)))

    switch(hue_segment_int) {
    case 0:   // pure red to pure yellow
        out.r = m_color_hsv.v;
        out.g = hsv_get_t;
        out.b = p;
        break;
    case 1:   // yellow to pure green
        out.r = hsv_get_q;
        out.g = m_color_hsv.v;
        out.b = p;
        break;
    case 2:   // green to cyan
        out.r = p;
        out.g = m_color_hsv.v;
        out.b = hsv_get_t;
        break;

    case 3:  // cyan to blue
        out.r = p;
        out.g = hsv_get_q;
        out.b = m_color_hsv.v;
        break;
    case 4: // blue to magenta
        out.r = hsv_get_t;
        out.g = p;
        out.b = m_color_hsv.v;
        break;
    case 5: // magenta to red
    default:
        out.r = m_color_hsv.v;
        out.g = p;
        out.b = hsv_get_q;
        break;
    }
    return out;     
}


        
