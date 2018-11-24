
#include "picturelamp.h"
#include "mainSettings.h"

#ifdef TRACE_ON
//#define TRACE_LOGIC
#define TRACE_MODES
#endif 

#define NUMLIGHTS 24

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
          {1,0.9,0},  //1 = yellow
          {0,1,1},  //2 = cyan
          {0,0.6,0.1}, //3 = mid green
          {0.5,0.15,0},  //4 = dark brown
          {0.8,0.0,0},  //5 = red
          {0,0,0.8},  //6 = blue
          {1,1,1}  //7 = white
};

PictureLamp picture_lamp[NUMLIGHTS];

unsigned long picture_start_time=0;
unsigned int picture_duration_time=5000;



/* Control */
enum PROCESS_MODES {
  SHOW_MODE, 
  TRANSITION_MODE,
  TEST_MODE_FADE_SOLO,
  TEST_MODE_FADE_IN_ENSEMBLE,
  TEST_MODE_SCALING,
  TEST_MODE_PLACEMENT
};

PROCESS_MODES process_mode = SHOW_MODE; 

/* **************** setup ******************************* */

void setup() {
  #ifdef TRACE_ON 
    char compile_signature[] = "--- START (Build: " __DATE__ " " __TIME__ ") ---";   
    Serial.begin(9600);
    Serial.println(compile_signature); 
  #endif
  
  pinMode(LED_BUILTIN, OUTPUT);
  output_setup();
  input_setup();
  for (int i=0;i<NUMLIGHTS;i++) {
    picture_lamp[i].setCurrentColor(0,0,0);
    picture_lamp[i].updateOutput(i);
  }
  output_show();
  enter_TRANSITION_MODE();
}

/* **************** LOOP ******************************* */
void loop() 
{
   input_switches_scan_tick();

   switch(process_mode) {
    case SHOW_MODE: process_SHOW_MODE();break;
    case TRANSITION_MODE:process_TRANSITION_MODE();break;
    case TEST_MODE_PLACEMENT:process_TEST_MODE_PLACEMENT();break;
    case TEST_MODE_FADE_SOLO:process_TEST_MODE_FADE_SOLO();break;
    case TEST_MODE_FADE_IN_ENSEMBLE:process_TEST_MODE_FADE_IN_ENSEMBLE();break;
    case TEST_MODE_SCALING: process_TEST_MODE_SCALING();break;
   } // switch
}

/* ========= SHOW_MODE ======== */
void enter_SHOW_MODE()
{
    #ifdef TRACE_MODES
      Serial.println(F("#SHOW_MODE"));
    #endif
    process_mode=SHOW_MODE;
    input_IgnoreUntilRelease();
    digitalWrite(LED_BUILTIN, false);
    
    for(int i=0;i<NUMLIGHTS;i++) 
      {
         picture_lamp[i].endTransition();
         picture_lamp[i].updateOutput(i);
       }
    output_show();

    picture_start_time=millis();
}

void process_SHOW_MODE()
{
    if(input_selectGotPressed()) 
    {
      enter_TEST_MODE_PLACEMENT();
      return;
    }
    if(input_stepGotPressed() ||  
        millis()-picture_start_time > picture_duration_time) {
          
        
        if(++pic_index>=9) pic_index=0;
        enter_TRANSITION_MODE();
        }
}

/* ========= TRANSITION_MODE ======== */

void enter_TRANSITION_MODE()
{
    #ifdef TRACE_MODES
      Serial.println(F("#TRANSITION_MODE"));
    #endif
    process_mode=TRANSITION_MODE;
    input_IgnoreUntilRelease();
    
    setTarget_picture(pic_index);
    triggerNextTransition();
    digitalWrite(LED_BUILTIN, true);
}

void process_TRANSITION_MODE()
{
    int transitionsRunningCount=0;

    if(input_stepGotPressed()){   // Fast foreward the transistion 
       enter_SHOW_MODE();
       return;
    }
    
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

    // Order next transitions
    if(transitionsRunningCount<1) 
    {
       if (!triggerNextTransition()) enter_SHOW_MODE();
    }
    #ifdef TRACE_LOGIC 
    delay(500);
    #endif
}

/* ========= TEST_MODE_FADE_SOLO ======== */

void enter_TEST_MODE_FADE_SOLO() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#TEST_MODE_FADE_SOLO"));
    #endif
    process_mode=TEST_MODE_FADE_SOLO;
    input_IgnoreUntilRelease();
    pic_index=0;
    for(int i=0;i<NUMLIGHTS;i++)  output_setLightColorUnmapped(i,0,0,0);  // shut down all lights
    output_show();
}

void process_TEST_MODE_FADE_SOLO()
{
    unsigned long current_time=millis();
    byte red=0;
    byte green=(current_time/10)%255;
    byte blue=0;

    if(input_selectGotPressed()) {
      enter_TEST_MODE_FADE_IN_ENSEMBLE();
      return;
    }
    if(input_stepGotPressed()) {
      output_setLightColorUnmapped(pic_index,0,0,0); // Shut down current light
      if(++pic_index>=NUMLIGHTS) pic_index=0;
    }

    output_setLightColorUnmapped(pic_index,red,green,blue);
     output_show();
}

/* ========= TEST_MODE_FADE_IN_ENSEMBLE ======== */

void enter_TEST_MODE_FADE_IN_ENSEMBLE() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#TEST_MODE_FADE_IN_ENSEMBLE"));
    #endif
    process_mode=TEST_MODE_FADE_IN_ENSEMBLE;
    input_IgnoreUntilRelease();
    pic_index=0;
    for(int i=0;i<NUMLIGHTS;i++)  output_setLightColorUnmapped(i,0,128,128);  // shut down all lights
    output_show();
}

void process_TEST_MODE_FADE_IN_ENSEMBLE()
{
    unsigned long current_time=millis();
    byte red=(current_time/10)%255;
    byte green=0;
    byte blue=0;

    if(input_selectGotPressed()) {
      enter_TEST_MODE_SCALING();
      return;
    }
    if(input_stepGotPressed()) {
      output_setLightColorUnmapped(pic_index,0,128,128); // Shut down current light
      if(++pic_index>=NUMLIGHTS) pic_index=0;
    }

    output_setLightColorUnmapped(pic_index,red,green,blue);
     output_show();
}


/* ========= TEST_MODE_SCALING ======== */

void enter_TEST_MODE_SCALING() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#TEST_MODE_SCALING"));
    #endif
    process_mode=TEST_MODE_SCALING;
    input_IgnoreUntilRelease();
    pic_index=0;
    for(int i=0;i<NUMLIGHTS;i++)  output_setLightColorUnmapped(i,0,0,0);  // shut down all lights
    output_show();
}

void process_TEST_MODE_SCALING()
{
    unsigned long current_time=millis();
    byte red=(current_time/10)%255;
    byte green=(current_time/10)%255;
    byte blue=(current_time/10)%255;

    if(input_selectGotPressed()) {
      enter_SHOW_MODE();
      return;
    }
    if(input_stepGotPressed()) {
      
      if(++pic_index>=NUMLIGHTS) pic_index=0;
    }
    
    for(int i=0;i<NUMLIGHTS;i++) {
      if (i<=pic_index) output_setLightColorUnmapped(i,red,green,blue); // Shut down current light
      else output_setLightColorUnmapped(i,0,0,0); 
    }
    output_show();
}

/* ========= TEST_MODE_PLACEMENT ======== */

void enter_TEST_MODE_PLACEMENT() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#TEST_MODE_PLACEMENT"));
    #endif
    process_mode=TEST_MODE_PLACEMENT;
    input_IgnoreUntilRelease();
    pic_index=4;
    for(int i=0;i<NUMLIGHTS;i++)  output_setLightColorUnmapped(i,0,0,0);  // shut down all lights
    output_setLightColor(0,255,0,0);
    output_setLightColor(1,255,255,0);
    output_setLightColor(2,0,255,0);
    output_setLightColor(3,0,255,255);
    output_setLightColor(4,0,0,255);
    output_show();
}

void process_TEST_MODE_PLACEMENT()
{
    
    if(input_selectGotPressed()) {
      enter_TEST_MODE_FADE_SOLO();
      return;
    }
    
    if(input_stepGotPressed()) {  // foreward one patter
      if(++pic_index>3) pic_index=0;
      for(int i=0;i<NUMLIGHTS;i++)  output_setLightColorUnmapped(i,0,0,0);  // shut down all lights
      switch(pic_index) {
       case 0:       // inner pentagon
           output_setLightColor(0,255,0,0);
           output_setLightColor(1,255,255,0);
           output_setLightColor(2,0,255,0);
           output_setLightColor(3,0,255,255);
           output_setLightColor(4,0,0,255);
           break;
       case 1:       // outer pentagon
           output_setLightColor(5,255,0,0);
           output_setLightColor(6,255,255,0);
           output_setLightColor(7,0,255,0);
           output_setLightColor(8,0,255,255);
           output_setLightColor(9,0,0,255);
           break;
       case 2:       // the circle
           output_setLightColor(10,128,0,0);
           output_setLightColor(11,255,0,0);
           output_setLightColor(12,128,128,0);
           output_setLightColor(13,255,255,0);
           output_setLightColor(14,0,128,0);
           output_setLightColor(15,0,255,0);
           output_setLightColor(16,0,128,128);
           output_setLightColor(17,0,255,255);
           output_setLightColor(18,0,0,128);
           output_setLightColor(19,0,0,255);
           break;
       case 3:       // the additionals
           output_setLightColor(20,255,0,0);
           output_setLightColor(21,255,255,0);
           output_setLightColor(22,0,255,0);
           output_setLightColor(23,0,255,255);
           break;
      }// switch
      output_show();
    } // select_got_pressed
    
}


/* ----- internal picture presentation helpers --------- */

/* Set Target Picture
 *  Sets the target values of all lamp to the picture defined by the index 
 */
void setTarget_picture(int picture_index)
{
#ifdef TRACE_LOGIC
  Serial.print(F("TRACE_LOGIC::setTarget_picture "));
  Serial.println(picture_index);
#endif
  for (int i=0;i<NUMLIGHTS;i++) {
     byte p_index=picture_point[picture_index][i];
     picture_lamp[i].setTargetColor(color_palette[p_index][iRED],color_palette[p_index][iGREEN],color_palette[p_index][iBLUE]);
  }
}

/*  triggerNextTransotion
 *   Searched for the next lamp that needs a transition and instructs the lamp to start the transition
 *   Return: true if some lamp got triggered, false: There is no more lamp to start
 */
bool triggerNextTransition()
{
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

/* getTransisionRunningCount
 *  return: Number of transitions currently processed by the chain
 */
 
int getTransitionsRunningCount()
{
  int theCount=0;
  for(int i=0;i<NUMLIGHTS;i++) {
    if(!picture_lamp[i].is_in_transition()) theCount++;
  }
  return theCount;
}

/* getTransitionsPendingCount
 *  return: Number of transitions still needed to be triggered
 */
int getTransitionsPendingCount()
{
  int theCount=0;
  for(int i=0;i<NUMLIGHTS;i++) {
    if(!picture_lamp[i].is_transition_pending()) theCount++;
  }
  return theCount;
}


