
#include "picturelamp.h"
#include "mainSettings.h"

//#define DEBUG_ON

#ifdef TRACE_ON
//#define TRACE_LOGIC
#define TRACE_PICTURES
#define TRACE_MODES
#define TRACE_TIMING
#define TRACE_CLOCK
#endif 

#define LAMP_COUNT 24


#ifndef DEBUG_ON
// normal timing setting
#define TRANSITION_RYTHM_MINIMAL 3000
#define TRANSITION_RYTHM_VARIANCE 5000
#define TRANSITION_DURATION_MINIMAL 6000
#define TRANSITION_DURATION_VARIANCE 4000
#define SHOW_DURATION_MINIMAL 400000  
#define SHOW_DURATION_VARIANCE 200000

#else
// debug timing setting
#define TRANSITION_RYTHM_MINIMAL 700
#define TRANSITION_RYTHM_VARIANCE 5000
#define TRANSITION_DURATION_MINIMAL 1500
#define TRANSITION_DURATION_VARIANCE 4000
#define SHOW_DURATION_MINIMAL 3000  
#define SHOW_DURATION_VARIANCE 1500
#endif


#define PICTURE_COUNT sizeof(g_pic_table)/sizeof(g_pic_table[0])

//                                         01 02 03 04 05  06 07 08 09 10  11 12 13 14 15 16 17 18 19 20  21 22 23 24
const byte pic_star_yellow[24] PROGMEM ={   1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  0, 0, 0, 0}; //0 star yellow
const byte pic_angel[24]       PROGMEM ={   0, 2, 2, 2, 2,  2, 0, 0, 0, 0,  0, 0, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 0}; //1 Angle 
const byte pic_tree[24]        PROGMEM ={   3, 3, 3, 3, 3,  3, 0, 0, 0, 0,  3, 3, 0, 0, 3, 3, 3, 3, 0, 0,  4, 0, 0, 3}; //2 Tree
const byte pic_moon[24]        PROGMEM ={   0, 0, 7, 7, 0,  0, 0, 0, 0, 0,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  0, 0, 0, 5}; //3 Moon
const byte pic_pentagons[24]   PROGMEM ={   2, 2, 2, 2, 2,  3, 3, 3, 3, 3,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0}; //4 Pentagons
const byte pic_bell[24]        PROGMEM ={   1, 1, 1, 1, 1,  0, 0, 0, 0, 0,  1, 1, 0, 0, 1, 0, 0, 1, 0, 0,  1, 0, 0, 0}; //5 Bell
//const byte pic_heart[24]       PROGMEM ={   0, 0, 5, 5, 0,  0, 0, 0, 0, 0,  0, 0, 5, 5, 5, 5, 5, 5, 5, 5,  5, 0, 0, 5}; //6 Heart
const byte pic_center_star[24] PROGMEM ={   0, 0, 0, 0, 0,  2, 2, 2, 2, 2,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 7}; //7 center star
const byte pic_cassiopeia[24]  PROGMEM ={   0, 0, 0, 0, 6,  0, 6, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 6, 6, 0,  0, 0, 0, 6}; //8 cassiopeia
const byte pic_snow_man[24]    PROGMEM ={   0, 0, 7, 7, 0,  7, 0, 0, 0, 0,  0, 0, 0, 7, 7, 0, 0, 7, 7, 0,  7, 7, 7, 0}; //9 snow man
const byte pic_krippe[24]      PROGMEM ={   0, 9, 0, 0, 9,  0, 0, 9, 9, 0,  0, 0, 0, 0, 9, 0, 0, 9, 0, 0,  1, 0, 7, 9}; //10 krippe
const byte pic_ichtys[24]      PROGMEM ={   6, 6, 0, 6, 6,  6, 0, 6, 0, 6,  0, 0, 0, 0, 6, 6, 0, 7, 6, 6,  0, 0, 6, 0}; //11 ychtis
const byte pic_flake_pple[24]  PROGMEM ={   0, 0, 0, 0, 0,  8, 8, 8, 8, 8,  0, 8, 0, 8, 0, 8, 0, 8, 0, 8,  0, 0, 0, 6}; //12 flake_pple
const byte pic_flake_oran[24]  PROGMEM ={   0, 0, 0, 0, 0,  9, 9, 9, 9, 9,  9, 0, 9, 0, 9, 0, 9, 0, 9, 0,  0, 0, 0, 1}; //13 flake_oran
const byte pic_star_color[24]  PROGMEM ={   6, 6, 6, 6, 6,  5, 5, 5, 5, 5, 10, 1,10, 1,10, 1,10, 1,10, 1,  0, 0, 0, 0}; //14 star_color
const byte pic_half_moon[24]   PROGMEM ={   1, 0, 0, 1, 1,  0, 0, 0, 0, 0,  0, 1, 1, 1, 1, 1, 1, 1, 0, 0,  1, 0, 0, 0}; //15 half_moon
const byte pic_gingerbread[24] PROGMEM ={   8, 5, 5, 5, 5,  5, 5, 5, 5, 5,  7, 7, 5, 0, 5, 5, 5, 5, 0, 5,  5, 5, 5,10}; //16 pic_gingerbread

//const byte pic_######[24]      PROGMEM ={   0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0}; //## #####


const byte* const g_pic_table [] ={pic_star_yellow,pic_angel,pic_tree,pic_moon,pic_pentagons,   // 0-4
                                   pic_bell,/*pic_heart,*/pic_center_star,pic_cassiopeia,pic_snow_man, //5-9
                                   pic_krippe,pic_ichtys,pic_flake_pple,pic_flake_oran,pic_star_color, // 10-14
                                   pic_half_moon,pic_gingerbread}; // 15-19

#define PICTURE_POINT(pic,lamp) pgm_read_byte_near(g_pic_table[pic]+lamp*sizeof(byte))

#define iRED 0
#define iGREEN 1
#define iBLUE 2

#define SECONDS_PER_DAY 86400

float g_color_palette[][3]={
          {0  ,0  ,0  },  //0 = black
          {1  ,0.7,0  },  //1 = yellow
          {0  ,0.8  ,0.8  },  //2 = cyan
          {0  ,0.5,0.08}, //3 = mid green
          {0.2,0.1,0 },  //4 = dark brown
          {1,0.0,0  },  //5 = red
          {0  ,0  ,0.8},  //6 = blue
          {1  ,1  ,1  },  //7 = white
          {0.8  ,0  ,0.8  },  //8 = pink
          {1  ,0.3,0  },  //9 = orange
          {0  ,1  ,0  },  // 10 =bright green
          {0.5  ,0  ,1  }  //11 = purple
};

//                                             1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23   24 
const byte clock_hour_color[24]   PROGMEM ={0x90,0x90,0x90,0x90,0x90,0x19,0x19,0x19,0x19,0x19,0xA0,0x60,0x50,0x50,0x50,0x50,0x50,0x07,0x07,0x07,0x07,0x07,0x00,0x00};
#define GET_HOUR_COLOR_BYTE(hour) pgm_read_byte_near(clock_hour_color+(hour-1)*sizeof(byte))

PictureLamp g_picture_lamp[LAMP_COUNT];

unsigned long g_picture_start_time=0;
unsigned long g_picture_duration_time=5000;
unsigned long g_transition_start_time=0;
unsigned long g_transition_follow_up_duration=5000;

long g_clock_base_time=54000;  // Time in seconds to add to systemclock to get realtime (15:00 as default)
unsigned long g_clock_sync_time=0;  // Time to calibrate "zeropoint" of systemclock 

byte g_pic_index=0; //

#define PICTURE_HISTORY_COUNT 4
byte g_picture_history [PICTURE_HISTORY_COUNT];
byte g_picture_history_next_entry_index=0;

/* Control */
enum PROCESS_MODES {
  SHOW_MODE, 
  TRANSITION_MODE,
  CLOCK_MODE,
  CLOCK_SET_HOUR_MODE,
  CLOCK_SET_MINUTE_MODE,
  FIREWORKS_MODE,
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

  input_setup();
  
  for (int i=0;i<PICTURE_HISTORY_COUNT;i++) {
      g_picture_history[i]=0;
  };
  set_target_picture( g_pic_index);
  randomSeed(analogRead(0));

  delay(700); // wait for chains to power up completetly

  enter_SHOW_MODE();
}

/* **************** LOOP ******************************* */
void loop() 
{
   input_switches_scan_tick();

   switch(g_process_mode) {
    case SHOW_MODE: process_SHOW_MODE();break;
    case TRANSITION_MODE:process_TRANSITION_MODE();break;
    case CLOCK_MODE:process_CLOCK_MODE();break;
    case CLOCK_SET_HOUR_MODE:process_CLOCK_SET_MODE();break;
    case CLOCK_SET_MINUTE_MODE:process_CLOCK_SET_MODE();break;
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
      Serial.print(F("#SHOW_MODE: start "));
      Serial.println(millis()/1000);
    #endif
    g_process_mode=SHOW_MODE;
    input_IgnoreUntilRelease();
    digitalWrite(LED_BUILTIN, false);
    
    for(int i=0;i<LAMP_COUNT;i++) 
      {
         g_picture_lamp[i].endTransition();
         g_picture_lamp[i].updateOutput(i);
       }
    output_show();

    g_picture_start_time=millis();
    g_picture_duration_time=SHOW_DURATION_MINIMAL+random(SHOW_DURATION_VARIANCE);
    #ifdef TRACE_TIMING
      Serial.print(F("TRACE_TIMING:g_picture_duration_time="));
      Serial.println(g_picture_duration_time/1000);
    #endif
}

void process_SHOW_MODE()
{
    if(input_selectGotPressed()) 
    {
      enter_CLOCK_MODE();
      return;
    }
    
    if(input_stepGotPressed()) {    // Switch to next picture in list
      if(++g_pic_index>=PICTURE_COUNT) g_pic_index=0;  // Switch to next picture in list
      enter_TRANSITION_MODE();
      return;
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
      Serial.print(F("#TRANSITION_MODE:start "));
      Serial.println(millis()/1000);
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
      enter_CLOCK_MODE();
      return;
    }

    if(input_stepGotPressed()){   // Fast foreward the transistion 
       enter_SHOW_MODE();
       return;
    }
    
    // update all transitioning lights
    for(int i=0;i<LAMP_COUNT;i++) 
    {
      if(g_picture_lamp[i].is_in_transition()) 
      {
        g_picture_lamp[i].updateOutput(i);
        transitionsRunningCount++;
      }
    }
    output_show();

    // Order next transitions
    if(transitionsRunningCount<1 || millis()-g_transition_start_time>g_transition_follow_up_duration) 
    {
      triggerNextTransition();
      if ( getTransitionsRunningCount()==0 ) enter_SHOW_MODE();
      #ifdef TRACE_LOGIC
        Serial.print(F("TRACE_LOGIC::transitions running: "));
        Serial.println(getTransitionsRunningCount());
      #endif
      g_transition_start_time=millis();
      g_transition_follow_up_duration=TRANSITION_RYTHM_MINIMAL+random(TRANSITION_RYTHM_VARIANCE);

    }
    #ifdef TRACE_LOGIC 
    delay(500);
    #endif
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
  for (int i=0;i<LAMP_COUNT;i++) {
     byte p_index=PICTURE_POINT(picture_index,i);
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
  for (int i=0;i<LAMP_COUNT;i++) {
     byte p_index=PICTURE_POINT(picture_index,i);
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
  int duration=TRANSITION_DURATION_MINIMAL+random(TRANSITION_DURATION_VARIANCE);

  for(int i=0;i<LAMP_COUNT;i++) {
    if(g_picture_lamp[i].is_transition_pending() && !g_picture_lamp[i].is_in_transition())
    {
       total_count++;
       switch (g_picture_lamp[i].getTransitionType())
       {
        case TT_ON: on_count++;break;
        case TT_OFF: off_count++;break;
       } //switch
    }
  } // for LAMP_COUNT

  #ifdef TRACE_LOGIC
    Serial.print(F("TRACE_LOGIC::total_count="));Serial.print(total_count);
    Serial.print(F(" on_count="));Serial.print(on_count);
    Serial.print(F(" off_count="));Serial.println(off_count);
  #endif
  
  if(total_count==0) return false;  // nothing more to start

  int trigger_lamp=getRandomPendingLamp();  // start transition of a random lamp
  g_picture_lamp[trigger_lamp].startTransition(duration);  
  #ifdef TRACE_LOGIC
    Serial.print(F("TRACE_LOGIC::trigger_lamp ")); Serial.println(trigger_lamp);
  #endif
  if(g_picture_lamp[trigger_lamp].getTransitionType()==TT_BLEND) return true;  // for a blend, one lamp is enough

  int additional_on_lamps=min(3,off_count>0?on_count/off_count:on_count);  // determine ratio between on and off, use all on when there is no off, but never more then 3
  int additional_off_lamps=min(3,on_count>0?off_count/on_count:off_count);

  if(g_picture_lamp[trigger_lamp].getTransitionType()==TT_ON) additional_on_lamps--;  // Remove already triggered lamp from count
  else additional_off_lamps--;

  #ifdef TRACE_LOGIC
    Serial.print(F("TRACE_LOGIC::additional_on_lamps="));Serial.print(additional_on_lamps);
    Serial.print(F(" additional_off_lamps="));Serial.println(additional_off_lamps);
  #endif
  
  for(int i=0;i<additional_on_lamps;i++)
  {
    g_picture_lamp[getRandomLampOfTransitionType(TT_ON)].startTransition(duration);  
  }

  for(int i=0;i<additional_off_lamps;i++)
  {
    g_picture_lamp[getRandomLampOfTransitionType(TT_OFF)].startTransition(duration);   
  }
  return true;
}

/* getRandomPendingLamp
 *  return: Return a random lamp index of a pending lamp
 */

byte getRandomPendingLamp()
{
  byte random_lamp=random(LAMP_COUNT);
  int i;
  for(i=0;i<LAMP_COUNT;i++) // search only one round
  {
    if(g_picture_lamp[random_lamp].is_transition_pending()&& !g_picture_lamp[random_lamp].is_in_transition()) break;  // leave loop when lamp is found
    if(++random_lamp>=LAMP_COUNT) random_lamp=0;
  }
  
  #ifdef TRACE_LOGIC
  if(i>=LAMP_COUNT) 
    Serial.println(F("TRACE_LOGIC::getRandomPendingLamp-ERROR - no pending lamp found "));
  #endif

  return random_lamp;
}

/* getRandomLampOfTransitionType
 *  return: Return a random lamp index of a lamp with a specific transition type
 */
byte getRandomLampOfTransitionType(transition_type_t wanted_type)
{
  byte random_lamp=random(LAMP_COUNT);
  int i;
  for(i=0;i<LAMP_COUNT;i++) // search only one round
  {
    if(g_picture_lamp[random_lamp].getTransitionType()==wanted_type && !g_picture_lamp[random_lamp].is_in_transition()) break;  // leave loop when lamp is found
    if(++random_lamp>=LAMP_COUNT) random_lamp=0;
  }
  #ifdef TRACE_LOGIC
  if(i>=LAMP_COUNT) 
    Serial.println(F("TRACE_LOGIC::getRandomLampOfTransitionType-ERROR - no lamp of desired transition type found "));
  #endif

  return random_lamp;
}

/* getTransisionRunningCount
 *  return: Number of transitions currently processed by the chain
 */

int getTransitionsRunningCount()
{
  int theCount=0;
  for(int i=0;i<LAMP_COUNT;i++) {
    if(g_picture_lamp[i].is_in_transition()) theCount++;
  }
  return theCount;
}

/* getTransitionsPendingCount
 *  return: Number of transitions still needed to be triggered
 */
int getTransitionsPendingCount()
{
  int theCount=0;
  for(int i=0;i<LAMP_COUNT;i++) {
    if(!g_picture_lamp[i].is_transition_pending()) theCount++;
  }
  return theCount;
}


/* ========= CLOCK_MODE ======== */

void enter_CLOCK_MODE() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#CLOCK_MODE"));
    #endif
    g_process_mode=CLOCK_MODE;
    input_IgnoreUntilRelease();
    long secondOfTheDay=((millis()-g_clock_sync_time)/1000+g_clock_base_time)%SECONDS_PER_DAY;
    order_next_clock_picture(secondOfTheDay,0);
    g_transition_start_time=secondOfTheDay;  // We use this variable to keep track, when second has changed 
}

void process_CLOCK_MODE()
{
    long secondOfTheDay=0;
    if(input_selectGotPressed()) {
       enter_TEST_MODE_PLACEMENT();
      return;
    } // Select got pressed
    
    if(input_stepGotPressed()) {  
      enter_CLOCK_SET_HOUR_MODE();
      return;
    } // Step got pressed
    
    secondOfTheDay=((millis()-g_clock_sync_time)/1000+g_clock_base_time)%SECONDS_PER_DAY; //86400=Seconds in a day
    
    if(secondOfTheDay!=g_transition_start_time) { // Trigger new Lamp Targets
      g_transition_start_time=secondOfTheDay;
      order_next_clock_picture(secondOfTheDay,950);
    }
    
   // update all transitioning lights
    for(int i=0;i<LAMP_COUNT;i++) 
    {
      if(g_picture_lamp[i].is_in_transition()) 
      {
        g_picture_lamp[i].updateOutput(i);
      }
    }
    output_show();  
}

/* ========= CLOCK_SET_HOUR_MODE ======== */

void enter_CLOCK_SET_HOUR_MODE() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#CLOCK_SET_HOUR_MODE"));
    #endif
    g_process_mode=CLOCK_SET_HOUR_MODE;
    g_clock_base_time=((millis()-g_clock_sync_time)/1000+g_clock_base_time)%SECONDS_PER_DAY; //freeze current time 
    g_clock_base_time=(g_clock_base_time/60)*60; //Remove seconds from time
    input_IgnoreUntilRelease();

     // TODO: Switch on central lamp

}

void enter_CLOCK_SET_MINUTE_MODE() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#CLOCK_SET_MINUTE_MODE"));
    #endif
    g_process_mode=CLOCK_SET_MINUTE_MODE;
    input_IgnoreUntilRelease();
     // TODO: Switch on central lamp
}

void process_CLOCK_SET_MODE()
{
    boolean changed=false;
     // TODO: Animate central lamp
     
     if(input_selectGotPressed()) {
       if(g_process_mode==CLOCK_SET_HOUR_MODE) g_clock_base_time-=3600;
       else g_clock_base_time-=60;
       if (g_clock_base_time<0)g_clock_base_time+=SECONDS_PER_DAY;
       changed=true;
    } // Select got pressed
    
    if(input_stepGotReleased()) {  
      if(input_getLastPressDuration()>2000)   // Long press
         {if (g_process_mode==CLOCK_SET_HOUR_MODE) enter_CLOCK_SET_MINUTE_MODE();  
          else {
            g_clock_sync_time=millis();  //Set our systemtime reference to now = relative zero
            enter_CLOCK_MODE();
          }
          return;}
      else {  // Short press
        if(g_process_mode==CLOCK_SET_HOUR_MODE) g_clock_base_time+=3600;
         else g_clock_base_time+=60;
        if (g_clock_base_time>=SECONDS_PER_DAY)g_clock_base_time-=SECONDS_PER_DAY;
        changed=true;
      }
    } // Step got pressed
    
    if (changed) order_next_clock_picture(g_clock_base_time,1);  
    
    // update all transitioning lights
    for(int i=0;i<LAMP_COUNT;i++) 
    {
      if(g_picture_lamp[i].is_in_transition()) 
      {
        g_picture_lamp[i].updateOutput(i);
      }
    }
    output_show();
}

/*  Clock Helper functions */

void order_next_clock_picture(long secondOfTheDay,int transitionTime)
{
  int currentHour=secondOfTheDay/3600;  
  int currentMinute=(secondOfTheDay%60)/60;  
  int currentSecond=secondOfTheDay%60;  

  byte colorByte=GET_HOUR_COLOR_BYTE(currentHour);

  byte color1=colorByte>>4;
  byte color0=colorByte&0x0f;
  unsigned short pattern=0x0fe0;
  if(currentHour<11 )  pattern>>=currentHour;
  else  if(currentHour<13 )  pattern=0x0fff;
        else if( currentHour<18) pattern=currentHour-12;
             else if(currentHour<23) pattern>>=currentHour-18;
                  else pattern=0x0fff;
   #ifdef TRACE_CLOCK
          Serial.print(F(">order_next_clock_picture secondOfTheDay: "));Serial.println(secondOfTheDay);
          Serial.print(F(">order_next_clock_picture HOUR: "));
          Serial.print(currentHour);
   #endif
   for(int i=6;i<11;i++) {
    #ifdef TRACE_CLOCK
      Serial.print(F("/"));
   #endif
    if(pattern&0x0001) {
         #ifdef TRACE_CLOCK
          Serial.print(color1);
         #endif
      g_picture_lamp[i].setTargetColor(g_color_palette[color1][iRED],g_color_palette[color1][iGREEN],g_color_palette[color1][iBLUE]);
    } else { 
         #ifdef TRACE_CLOCK
          Serial.print(color0);
         #endif
      g_picture_lamp[i].setTargetColor(g_color_palette[color0][iRED],g_color_palette[color0][iGREEN],g_color_palette[color0][iBLUE]);
    }

   g_picture_lamp[i].startTransition(transitionTime);
   pattern>>=1;
   }
   #ifdef TRACE_CLOCK
      Serial.println(F(" "));
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
    for(int i=0;i<LAMP_COUNT;i++)  output_setLightColorUnmapped(i,0,0,0);  // shut down all lights
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
      for(int i=0;i<LAMP_COUNT;i++)  output_setLightColorUnmapped(i,0,0,0);  // shut down all lights
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
    for(int i=0;i<LAMP_COUNT;i++)  output_setLightColorUnmapped(i,0,0,0);  // shut down all lights
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
      if(++g_pic_index>=LAMP_COUNT) g_pic_index=0;
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
    for(int i=0;i<LAMP_COUNT;i++)  output_setLightColorUnmapped(i,0,128,128);  // shut down all lights
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
      if(++g_pic_index>=LAMP_COUNT) g_pic_index=0;
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
    for(int i=0;i<LAMP_COUNT;i++)  output_setLightColorUnmapped(i,0,0,0);  // shut down all lights
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
      
      if(++g_pic_index>=LAMP_COUNT) g_pic_index=0;
    }
    
    for(int i=0;i<LAMP_COUNT;i++) {
      if (i<=g_pic_index) output_setLightColorUnmapped(i,red,green,blue); // Shut down current light
      else output_setLightColorUnmapped(i,0,0,0); 
    }
    output_show();
}




