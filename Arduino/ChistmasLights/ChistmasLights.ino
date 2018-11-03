
#include "picturelamp.h"
#include "mainSettings.h"

#ifdef TRACE_ON
#define TRACE_LOGIC
#endif 

#define NUMLIGHTS 8

int delayval = 100; // delay between animation steps
int duration=3000; // Transition duration
int pic_index=0; //

PictureLamp picture_lamp[NUMLIGHTS];

void setup() {
  #ifdef TRACE_ON 
    char compile_signature[] = "--- START (Build: " __DATE__ " " __TIME__ ") ---";   
    Serial.begin(9600);
    Serial.println(compile_signature); 
  #endif
  
  pinMode(LED_BUILTIN, OUTPUT);
 output_setup();
 for (int i=0;i<NUMLIGHTS;i++) {
  picture_lamp[i].setCurrentColor(0,0,0);
  picture_lamp[i].updateOutput(i);
 }
 output_show();
 setTarget_picture(pic_index);
}

void setTarget_picture(int index){
#ifdef TRACE_LOGIC
  Serial.print(F("TRACE_LOGIC::setTarget_picture "));
  Serial.println(index);
#endif

  switch(index) {
    case 0:
      for (int i=0;i<NUMLIGHTS;i++) {
       picture_lamp[i].setTargetColor(i*0.10,i*0.015,0.7-i*0.06);
      }
      break;
    case 1:
      for (int i=0;i<NUMLIGHTS;i++) {
        picture_lamp[i].setTargetColor((i%3)*0.2,(i%4)*0.2,i*0.03);
      }
      break;
  } // switch
}

bool triggerNextTransition(){
  int trigger_count=0;
  #ifdef TRACE_LOGIC
    Serial.println(F("TRACE_LOGIC::triggerNextTransition "));
  #endif

  for(int i=0;i<NUMLIGHTS;i++) {
    if(picture_lamp[i].is_transition_pending() && !picture_lamp[i].is_in_transition())
    {
      picture_lamp[i].startTransition(duration);
      trigger_count++;
      if (trigger_count>=1) break;
    }
  }
  return trigger_count!=0;
}

int getTransitionsRunningCount()
{
  int theCount=0;
  for(int i=0;i<NUMLIGHTS;i++) {
    if(!picture_lamp[i].is_in_transition()) theCount++;
  }
  return theCount;
}

int getTransitionsPendingCount()
{
  int theCount=0;
  for(int i=0;i<NUMLIGHTS;i++) {
    if(!picture_lamp[i].is_transition_pending()) theCount++;
  }
  return theCount;
}

void loop() {
   int transitionsRunningCount=0;

   // update all transitioning lights
   for(int i=0;i<NUMLIGHTS;i++) 
   {
      if(picture_lamp[i].is_in_transition()) 
      {
        picture_lamp[i].updateOutput(i);
        transitionsRunningCount++;
      }
   }
   output_show();

   if(transitionsRunningCount<1) 
   {
       if (!triggerNextTransition()) {
        digitalWrite(LED_BUILTIN, true);
        delay(2000);
        digitalWrite(LED_BUILTIN, false);
        if(++pic_index>=2) pic_index=0;
        setTarget_picture(pic_index);
        triggerNextTransition();
       }
   }
  #ifdef TRACE_LOGIC 
  delay(500);
  #endif
}
