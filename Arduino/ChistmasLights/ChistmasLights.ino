
#include "picturelamp.h"
#include "mainSettings.h"

#define DEBUG_ON

#ifdef TRACE_ON
//#define TRACE_LOGIC
#define TRACE_PICTURES
#define TRACE_MODES
#endif 

#define NUMLIGHTS 24


#ifndef DEBUG_ON
// normal timing setting
#define TRANSITION_DURATION_MINIMAL 8000
#define TRANSITION_DURATION_VARIANCE 3500
#define SHOW_DURATION_MINIMAL 150000  
#define SHOW_DURATION_VARIANCE 60000

#else
// debug timing setting
#define TRANSITION_DURATION_MINIMAL 500
#define TRANSITION_DURATION_VARIANCE 1000
#define SHOW_DURATION_MINIMAL 3000  
#define SHOW_DURATION_VARIANCE 1500
#endif


#define PICTURE_COUNT 10

byte g_picture_point[][24]= {
// 01 02 03 04 05  06 07 08 09 10  11 12 13 14 15 16 17 18 19 20  21 22 23 24
{   1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  0, 0, 0, 0}, //0 Star
{   0, 2, 2, 2, 2,  2, 0, 0, 0, 0,  0, 0, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 0}, //1 Angel
{   3, 3, 3, 3, 3,  3, 0, 0, 0, 0,  3, 3, 0, 0, 3, 3, 3, 3, 0, 0,  4, 0, 0, 3}, //2 Tree
{   7, 0, 7, 7, 0,  0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  0, 0, 0, 0}, //3 Moon
{   2, 2, 2, 2, 2,  3, 3, 3, 3, 3,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0}, //4 Pentagons
{   1, 1, 1, 1, 1,  0, 0, 0, 0, 0,  1, 1, 0, 0, 1, 0, 0, 1, 0, 0,  1, 0, 0, 0}, //5 Bell
{   0, 0, 5, 5, 0,  0, 0, 0, 0, 0,  0, 0, 5, 5, 5, 5, 5, 5, 5, 5,  5, 0, 0, 5}, //6 Heart
{   0, 0, 0, 0, 0,  2, 2, 2, 2, 2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 7}, //7 center star
{   0, 6, 0, 0, 0,  0, 0, 0, 0, 6,  0, 0, 0, 6, 6, 0, 0, 0, 0, 0,  0, 0, 0, 6}, //8 cassiopeia
{   0, 0, 7, 7, 0,  7, 0, 0, 0, 0,  0, 0, 0, 7, 7, 0, 0, 7, 7, 0,  7, 7, 7, 0}  //9 snow man
};

#define iRED 0
#define iGREEN 1
#define iBLUE 2

float g_color_palette[][3]={
          {0,0,0},  //0 = black
          {1,0.8,0},  //1 = yellow
          {0,1,1},  //2 = cyan
          {0,0.6,0.1}, //3 = mid green
          {0.5,0.15,0},  //4 = dark brown
          {0.8,0.0,0},  //5 = red
          {0,0,0.8},  //6 = blue
          {1,1,1}  //7 = white
};

PictureLamp g_picture_lamp[NUMLIGHTS];

unsigned long g_picture_start_time=0;
unsigned int g_picture_duration_time=5000;

byte g_pic_index=0; //

#define PICTURE_HISTORY_COUNT 3
byte g_picture_history [PICTURE_HISTORY_COUNT];
byte g_picture_history_next_entry_index=0;

/* Control */
enum PROCESS_MODES {
  SHOW_MODE, 
  TRANSITION_MODE,
  TEST_MODE_PLACEMENT,
  TEST_MODE_PICTURES,
  TEST_MODE_FADE_SOLO,
  TEST_MODE_FADE_IN_ENSEMBLE,
  TEST_MODE_SCALING,
};

PROCESS_MODES g_process_mode = SHOW_MODE; 

/* **************** setup ******************************* */

void setup() {
  #ifdef TRACE_ON 
    char compile_signature[] = "--- START (Build: " __DATE__ " " __TIME__ ") ---";   
    Serial.begin(9600);
    Serial.println(compile_signature); 
  #endif
  
  pinMode(LED_BUILTIN, OUTPUT);
  output_setup();
  for (int i=0;i<NUMLIGHTS;i++) {
    g_picture_lamp[i].setCurrentColor(0,0,0);
    g_picture_lamp[i].updateOutput(i);
  }
  output_show();
  input_setup();
  
  for (int i=0;i<PICTURE_HISTORY_COUNT;i++) {
      g_picture_history[i]=0;
  };
  set_target_picture( g_pic_index);
  randomSeed(analogRead(0));
  enter_SHOW_MODE();
}

/* **************** LOOP ******************************* */
void loop() 
{
   input_switches_scan_tick();

   switch(g_process_mode) {
    case SHOW_MODE: process_SHOW_MODE();break;
    case TRANSITION_MODE:process_TRANSITION_MODE();break;
    case TEST_MODE_PLACEMENT:process_TEST_MODE_PLACEMENT();break;
    case TEST_MODE_PICTURES:process_TEST_MODE_PICTURES();break;
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
    g_process_mode=SHOW_MODE;
    input_IgnoreUntilRelease();
    digitalWrite(LED_BUILTIN, false);
    
    for(int i=0;i<NUMLIGHTS;i++) 
      {
         g_picture_lamp[i].endTransition();
         g_picture_lamp[i].updateOutput(i);
       }
    output_show();

    g_picture_start_time=millis();
    g_picture_duration_time=SHOW_DURATION_MINIMAL+random(SHOW_DURATION_VARIANCE);
}

void process_SHOW_MODE()
{
    if(input_selectGotPressed()) 
    {
      if(++g_pic_index>=PICTURE_COUNT) g_pic_index=0;  // Switch to next picture in list
      enter_TRANSITION_MODE();
      return;
    }
    
    if(input_stepGotPressed()) {    // Switch to next picture in list
        if(++g_pic_index>=PICTURE_COUNT) g_pic_index=0;  
        set_target_picture(g_pic_index);
        enter_SHOW_MODE();
        }

     if(millis()-g_picture_start_time > g_picture_duration_time) {  // start transition to a random picture 
      g_pic_index=255;
      while (g_pic_index==255)
      {
        g_pic_index=random(PICTURE_COUNT);
        for(byte i=0;i<PICTURE_HISTORY_COUNT;i++) { // check history
          if(g_pic_index==g_picture_history[i]) 
          { 
            g_pic_index=255;  //bad choice
            break;
          }
        }
      }
      
      // finally a valid choice
      g_picture_history[g_picture_history_next_entry_index]=g_pic_index; // put choice into the history
      if(++g_picture_history_next_entry_index>=PICTURE_HISTORY_COUNT) g_picture_history_next_entry_index=0;
      
      enter_TRANSITION_MODE();
      return;  
     }
}

/* ========= TRANSITION_MODE ======== */

void enter_TRANSITION_MODE()
{
    #ifdef TRACE_MODES
      Serial.println(F("#TRANSITION_MODE"));
    #endif
    g_process_mode=TRANSITION_MODE;
    input_IgnoreUntilRelease();
    
    set_target_picture(g_pic_index);
    triggerNextTransition();
    digitalWrite(LED_BUILTIN, true);
}

void process_TRANSITION_MODE()
{
    int transitionsRunningCount=0;

    if(input_selectGotPressed()) 
    {
      enter_TEST_MODE_PLACEMENT();
      return;
    }

    if(input_stepGotPressed()){   // Fast foreward the transistion 
       enter_SHOW_MODE();
       return;
    }
    
    // update all transitioning lights
    for(int i=0;i<NUMLIGHTS;i++) 
    {
      if(g_picture_lamp[i].is_in_transition()) 
      {
        g_picture_lamp[i].updateOutput(i);
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


/* ========= TEST_MODE_PLACEMENT ======== */

void enter_TEST_MODE_PLACEMENT() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#TEST_MODE_PLACEMENT"));
    #endif
    g_process_mode=TEST_MODE_PLACEMENT;
    input_IgnoreUntilRelease();
    g_pic_index=0;
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
      enter_TEST_MODE_PICTURES();
      return;
    }
    
    if(input_stepGotPressed()) {  // foreward one patter
      if(++g_pic_index>3) g_pic_index=0;
      for(int i=0;i<NUMLIGHTS;i++)  output_setLightColorUnmapped(i,0,0,0);  // shut down all lights
      switch(g_pic_index) {
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
/* ========= TEST_MODE_PICTURES ======== */

void enter_TEST_MODE_PICTURES() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#TEST_MODE_PICTURES"));
    #endif
    g_process_mode=TEST_MODE_PICTURES;
    input_IgnoreUntilRelease();
    g_pic_index=0;
    set_picture( g_pic_index);
    output_show();
}

void process_TEST_MODE_PICTURES()
{
    
    if(input_selectGotPressed()) {
      enter_TEST_MODE_FADE_SOLO();
      return;
    }
    
    if(input_stepGotPressed()) {  // foreward one patter
      if(++g_pic_index>=PICTURE_COUNT) g_pic_index=0;
          set_picture( g_pic_index);
          output_show(); 
    }
      
    
}

/* ========= TEST_MODE_FADE_SOLO ======== */

void enter_TEST_MODE_FADE_SOLO() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#TEST_MODE_FADE_SOLO"));
    #endif
    g_process_mode=TEST_MODE_FADE_SOLO;
    input_IgnoreUntilRelease();
    g_pic_index=0;
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
      output_setLightColorUnmapped(g_pic_index,0,0,0); // Shut down current light
      if(++g_pic_index>=NUMLIGHTS) g_pic_index=0;
    }

    output_setLightColorUnmapped(g_pic_index,red,green,blue);
     output_show();
}

/* ========= TEST_MODE_FADE_IN_ENSEMBLE ======== */

void enter_TEST_MODE_FADE_IN_ENSEMBLE() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#TEST_MODE_FADE_IN_ENSEMBLE"));
    #endif
    g_process_mode=TEST_MODE_FADE_IN_ENSEMBLE;
    input_IgnoreUntilRelease();
    g_pic_index=0;
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
      output_setLightColorUnmapped(g_pic_index,0,128,128); // Shut down current light
      if(++g_pic_index>=NUMLIGHTS) g_pic_index=0;
    }

    output_setLightColorUnmapped(g_pic_index,red,green,blue);
     output_show();
}


/* ========= TEST_MODE_SCALING ======== */

void enter_TEST_MODE_SCALING() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#TEST_MODE_SCALING"));
    #endif
    g_process_mode=TEST_MODE_SCALING;
    input_IgnoreUntilRelease();
    g_pic_index=0;
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
      
      if(++g_pic_index>=NUMLIGHTS) g_pic_index=0;
    }
    
    for(int i=0;i<NUMLIGHTS;i++) {
      if (i<=g_pic_index) output_setLightColorUnmapped(i,red,green,blue); // Shut down current light
      else output_setLightColorUnmapped(i,0,0,0); 
    }
    output_show();
}




/* ----- internal picture presentation helpers --------- */

/* set_picture
 *  Orders all lamp objects to go to the defined picuture
 */
void set_picture(int picture_index)
{
#ifdef TRACE_PICTURES
  Serial.print(F("TRACE_PICTURES::set_picture"));
  Serial.println(picture_index);
#endif
  for (int i=0;i<NUMLIGHTS;i++) {
     byte p_index=g_picture_point[picture_index][i];
     g_picture_lamp[i].setCurrentColor(g_color_palette[p_index][iRED],g_color_palette[p_index][iGREEN],g_color_palette[p_index][iBLUE]);
     g_picture_lamp[i].updateOutput(i);
  }
}

/* Set Target Picture
 *  Sets the target values of all lamp to the picture defined by the index 
 */
void set_target_picture(int picture_index)
{
#ifdef TRACE_PICTURES
  Serial.print(F("TRACE_PICTURES::set_target_picture "));
  Serial.println(picture_index);
#endif
  for (int i=0;i<NUMLIGHTS;i++) {
     byte p_index=g_picture_point[picture_index][i];
     g_picture_lamp[i].setTargetColor(g_color_palette[p_index][iRED],g_color_palette[p_index][iGREEN],g_color_palette[p_index][iBLUE]);
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
    if(!g_picture_lamp[i].is_in_transition())
    {
      total_count++;
       switch (g_picture_lamp[i].getTransitionType())
       {
        case TT_ON: on_count++;break;
        case TT_OFF: off_count++;break;
       } //switch
    }
  } // for NUMLIGHTS
  
  if(total_count==0) return false;  /* nothing more to start */

 int trigger_count=0;
  
  for(int i=0;i<NUMLIGHTS;i++) {
    if(g_picture_lamp[i].is_transition_pending() && !g_picture_lamp[i].is_in_transition())
    {
      g_picture_lamp[i].startTransition(TRANSITION_DURATION_MINIMAL+random(TRANSITION_DURATION_VARIANCE));
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
    if(!g_picture_lamp[i].is_in_transition()) theCount++;
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
    if(!g_picture_lamp[i].is_transition_pending()) theCount++;
  }
  return theCount;
}


