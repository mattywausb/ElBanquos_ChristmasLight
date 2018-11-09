
#include "picturelamp.h"
#include "mainSettings.h"

#ifdef TRACE_ON
#define TRACE_LOGIC
#endif 

#define NUMLIGHTS 8

int delayval = 100; // delay between animation steps
int duration=3000; // Transition duration
int pic_index=0; //



byte picture_point[][24]= {
// 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24
{   1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, //0 Star
{   0, 2, 2, 2, 2, 2, 0, 2, 2, 0, 2, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 2, 0}, //1 Angel
{   3, 3, 3, 3, 3, 3, 0, 0, 0, 0, 3, 3, 0, 0, 3, 3, 3, 3, 0, 0, 4, 0, 0, 3}, //2 Tree
{   7, 0, 7, 7, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0}, //3 Moon
{   2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //4 Pentagons
{   1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0}, //5 Bell
{   0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 5}, //6 Heart
{   0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7}, //7 center star
{   0, 6, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6}, //8 cassiopeia
{   0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0}  //9 snow man
};

#define iRED 0
#define iGREEN 1
#define iBLUE 2

float color_palette[][3]={
          {0,0,0},  //0 = black
          {1,1,0},  //1 = yellow
          {0,1,1},  //2 = cyan
          {0,0.6,0.1}, //3 = mid green
          {0.5,0.15,0},  //4 = dark brown
          {0.8,0.0,0},  //5 = red
          {0,0,0.8},  //6 = blue
          {1,1,1}  //7 = white
};

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

void setTarget_picture(int picture_index){
#ifdef TRACE_LOGIC
  Serial.print(F("TRACE_LOGIC::setTarget_picture "));
  Serial.println(picture_index);
#endif
  for (int i=0;i<NUMLIGHTS;i++) {
     byte p_index=picture_point[picture_index][i];
     picture_lamp[i].setTargetColor(color_palette[p_index][iRED],color_palette[p_index][iGREEN],color_palette[p_index][iBLUE]);
  }

}

bool triggerNextTransition(){
  #ifdef TRACE_LOGIC
    Serial.println(F("TRACE_LOGIC::triggerNextTransition "));
  #endif
  int on_count=0;
  int off_count=0;
  int total_count=0;

  for(int i=0;i<NUMLIGHTS;i++) {
    if(!picture_lamp[i].is_in_transition())
    {
      total_count++;
       switch (picture_lamp[i].getTransitionType())
       {
        case TT_ON: on_count++;break;
        case TT_OFF: off_count++;break;
       } //switch
    }
  } // for NUMLIGHTS
  
  if(total_count==0) return false;  /* nothing more to start */

 int trigger_count=0;
  
  for(int i=0;i<NUMLIGHTS;i++) {
    if(picture_lamp[i].is_transition_pending() && !picture_lamp[i].is_in_transition())
    {
      picture_lamp[i].startTransition(duration);
      trigger_count++;
      if (trigger_count>=2) break;
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
        delay(5000);
        digitalWrite(LED_BUILTIN, false);
        if(++pic_index>=9) pic_index=0;
        setTarget_picture(pic_index);
        triggerNextTransition();
       }
   }
  #ifdef TRACE_LOGIC 
  delay(500);
  #endif
}
