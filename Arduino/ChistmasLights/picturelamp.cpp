#include "Arduino.h"
#include "picturelamp.h"
#include "mainSettings.h"

#define TRACE_PICTURELAMP

PictureLamp::PictureLamp()
{
  current_red=current_green=current_blue=
  target_red=target_green=target_blue=0.0;
  transition_duration=0.0;
  transition_type=TT_NONE;
}


void PictureLamp::updateOutput(byte light_index)
{
   float interpolated_red, interpolated_green, interpolated_blue;
   float normalized_transition_point;
   float transition_time;
   if(is_in_transition())
   {
    transition_time=millis()-start_transition_time;   
    if(transition_time<transition_duration) {
        normalized_transition_point=transition_time/transition_duration;
        interpolated_red=current_red-(current_red-target_red)*normalized_transition_point;
        interpolated_green=current_green-(current_green-target_green)*normalized_transition_point;
        interpolated_blue=current_blue-(current_blue-target_blue)*normalized_transition_point;
    } else {
        endTransition();
    }
   }
 
   if(!is_in_transition())
   {
        interpolated_red=current_red;
        interpolated_green=current_green;
        interpolated_blue=current_blue;
   }
   if(interpolated_red>1.0) interpolated_red=1.0;
   if(interpolated_green>1.0) interpolated_green=1.0;
   if(interpolated_blue>1.0) interpolated_blue=1.0;

#ifdef TRACE_PICTURELAMP
    Serial.print(F("TRACE_PICTURELAMP> "));
    Serial.print(light_index);Serial.print(F("="));
    Serial.print(start_transition_time);Serial.print(F("T"));
    Serial.print(transition_time);Serial.print(F("T"));
    Serial.print(normalized_transition_point);Serial.print(F("|"));  
    Serial.print(interpolated_red);Serial.print(F("/"));
    Serial.print(interpolated_green);Serial.print(F("/"));
    Serial.println(interpolated_blue);
 #endif

   output_setLightColor(light_index,
                  static_cast<int>(255.0*interpolated_red),
                  static_cast<int>(255.0*interpolated_green),
                  static_cast<int>(255.0*interpolated_blue));
}

void PictureLamp::setCurrentColor(float red, float green, float blue)
{
  current_red=red;
  current_green=green;
  current_blue=blue;
}

void PictureLamp::setTargetColor(float red, float green, float blue)
{
  target_red=red;
  target_green=green;
  target_blue=blue;
  transition_type=TT_BLEND;
  if(target_red==0 && target_green==0 && target_blue==0) transition_type=TT_OFF;
  if(current_red==0 && current_green==0 && current_blue==0) transition_type=TT_ON;
  if(target_red==current_red && target_green==current_green && target_blue==current_blue) transition_type=TT_NONE;
  
  #ifdef TRACE_PICTURELAMP
    Serial.print(F("TRACE_PICTURELAMP>::setTargetColor "));
    Serial.print(target_red);Serial.print(F("/"));
    Serial.print(target_green);Serial.print(F("/"));
    Serial.println(target_blue);
 #endif
}

void PictureLamp::startTransition(unsigned long duration)
{
  #ifdef TRACE_PICTURELAMP
    Serial.print(F("TRACE_PICTURELAMP::startTransition for "));
    Serial.println(duration);
  #endif
  transition_duration=duration;
  start_transition_time=millis();
}

void PictureLamp::endTransition() {
  #ifdef TRACE_PICTURELAMP
    Serial.println(F("TRACE_PICTURELAMP::endTransition"));
  #endif
    current_red=target_red;
    current_green=target_green;
    current_blue=target_blue;
    transition_duration=0.0;
    start_transition_time=0;
    transition_type=TT_NONE;
};

/* ---- State information ---- */


bool PictureLamp::is_in_transition() 
{
  return transition_duration>0.0;
}
bool PictureLamp::is_transition_pending() 
{
  return (transition_type!=TT_NONE);
}

