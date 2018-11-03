
#include "picturelamp.h"
#include "mainSettings.h"

#define NUMLIGHTS 8

int delayval = 100; // delay between animation steps
int duration=8000; // Transition duration
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
  switch(index) {
    case 0:
      for (int i=0;i<NUMLIGHTS;i++) {
       picture_lamp[i].setTargetColor(i*20/255,i*15/255,(160-i*20)/255);
      }
      break;
    case 1:
      for (int i=0;i<NUMLIGHTS;i++) {
        picture_lamp[i].setTargetColor(((i%3)*12)/255,(100-(i%4)*15)/255,(i*20)/255);
      }
      break;
  } // switch
}

bool triggerNextTransition(){
  int trigger_count=0;
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
      if(!picture_lamp[i].is_in_transition()) 
      {
        picture_lamp[i].updateOutput(i);
        transitionsRunningCount++;
      }
   }
   //output_show();

   if(transitionsRunningCount<1) 
   {
       if (!triggerNextTransition()) {
        digitalWrite(LED_BUILTIN, true);
        delay(2000);
        digitalWrite(LED_BUILTIN, false);
        if(++pic_index>=2) pic_index=2;
        setTarget_picture(pic_index);
        triggerNextTransition();
       }
   }

}
