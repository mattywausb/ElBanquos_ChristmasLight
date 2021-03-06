
#include "picturelamp.h"
#include "mainSettings.h"
#include "particle.h"

//#define DEBUG_ON

#ifdef TRACE_ON
//#define TRACE_LOGIC
//#define TRACE_PICTURES
#define TRACE_RND_COL_GEN
#define TRACE_MODES
//#define TRACE_TIMING
#define TRACE_CLOCK
#define TRACE_CLOCK_TIME
#endif 

#define LAMP_COUNT 24

#define PARTICLE_COUNT 8


Particle g_firework_particle[PARTICLE_COUNT];


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

#define LONG_PRESS_DURATION 1500

#define CLOCK_SECOND_LIVE_INTERVAL 60000

#define PICTURE_COUNT sizeof(g_pic_table)/sizeof(g_pic_table[0])

#define RANDOM_COLOR_INDEX_START 30


// Picture description
// number beginning at 30 define varaible palette. Defineing the group of same color in first digit and the color set to use in second

//                                         01 02 03 04 05  06 07 08 09 10  11 12 13 14 15 16 17 18 19 20  21 22 23 24

const byte pic_cassiopeia[24]  PROGMEM ={   0, 0, 0, 0, 6,  0, 6, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 6, 6, 0,  0, 0, 0, 6}; // cassiopeia
const byte pic_star_uni[24]    PROGMEM ={   1, 1, 1, 1, 1,  1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  0, 0, 0, 0}; // star yellow
const byte pic_star_color[24]  PROGMEM ={  34,34,34,34,34, 44,44,44,44,44, 54,64,54,64,54,64,54,64,54,64,  0, 0, 0, 0}; // star_color
const byte pic_pentagons[24]   PROGMEM ={  31,31,31,31,31, 43,43,43,43,43,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0}; // Pentagons
const byte pic_center_star[24] PROGMEM ={   0, 0, 0, 0, 0, 30,30,30,30,30,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 7}; // center star
const byte pic_gingerbread[24] PROGMEM ={  32,13,13,13,13, 13,13,13,13,13,  7, 7,13, 0,13, 0, 0,13, 0,13,  0,13,13,42}; // pic_gingerbread
const byte pic_angel[24]       PROGMEM ={   0, 2, 2, 2, 2,  2, 0, 0, 0, 0,  0, 0, 2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 0}; // Angle 
const byte pic_snow_man[24]    PROGMEM ={   0, 0, 7, 7, 0,  7, 0, 0, 0, 0,  0, 0, 0, 7, 7, 0, 0, 7, 7, 0,  7, 7, 7, 0}; // snow man
const byte pic_moon[24]        PROGMEM ={   0, 0, 7, 7, 0,  0, 0, 0, 0, 0, 12,12,12,12,12,12,12,12,12,12,  0, 0, 0, 5}; // Moon
const byte pic_flake_1[24]     PROGMEM ={   0, 0, 0, 0, 0, 30,30,30,30,30,  0,30, 0,30, 0,30, 0,30, 0,30,  0, 0, 0,40}; // flake_1
const byte pic_flake_2[24]     PROGMEM ={   0, 0, 0, 0, 0, 30,30,30,30,30, 30, 0,30, 0,30, 0,30, 0,30, 0,  0, 0, 0,40}; // flake_2
const byte pic_bell[24]        PROGMEM ={   0,12,12,12,12,  0, 0, 0, 0, 0,  0, 0, 0, 0,12,12,12,12, 0, 0,  7, 0, 0, 0}; // Bell
const byte pic_tree[24]        PROGMEM ={   3, 3, 3, 3, 3,  3, 0, 0, 0, 0,  3, 3, 0, 0, 3, 0, 0, 3, 0, 0,  4, 0, 0, 3}; // Tree
const byte pic_ichtys[24]      PROGMEM ={   6, 6, 0, 6, 6,  6, 0, 6, 0, 6,  0, 0, 0, 0, 6, 6, 0, 7, 6, 6,  0, 0, 6, 0}; // ychtis
const byte pic_3_wise[24]      PROGMEM ={  11, 0, 0, 0, 0,  7, 0, 0, 0, 0,  0, 0, 9, 9, 0, 0, 0, 0, 3, 3,  0, 0, 0,11}; // pic_3_wise
const byte pic_krippe[24]      PROGMEM ={   0, 9, 0, 0, 9,  0, 0, 9, 9, 0,  0, 0, 0, 0, 9, 0, 0, 9, 0, 0,  1, 0, 7, 9}; // krippe
const byte pic_half_moon[24]   PROGMEM ={   1, 0, 0, 1, 1,  0, 0, 0, 0, 0,  0, 1, 1, 1, 1, 1, 1, 1, 0, 0,  1, 0, 0, 0}; // half_moon

//const byte pic_heart[24]     PROGMEM ={   0, 0, 5, 5, 0,  0, 0, 0, 0, 0,  0, 0, 5, 5, 5, 5, 5, 5, 5, 5,  5, 0, 0, 5}; // Heart



//const byte pic_######[24]      PROGMEM ={   0, 0, 0, 0, 0,  0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0}; //## #####

                                                     // on: off rate (to test transition algorythm)
const byte* const g_pic_table [] ={pic_cassiopeia,  
                                   pic_star_uni,     // 16:1
                                   pic_star_color,   // 0:0
                                   pic_pentagons,    // 0:10
                                   pic_center_star,  // 1:5
                                   pic_gingerbread,  // 13:0
                                   pic_angel,        // 4:8
                                   pic_snow_man,     // 0:6
                                   pic_moon,         // 7:4
                                   pic_flake_1,      // 6:7
                                   pic_flake_2,      // 5:5
                                   pic_bell,         // 7:9
                                   pic_tree,         // 5:2
                                   pic_ichtys,       // 5:4
                                   pic_3_wise,       // 3:9
                                   pic_krippe,       // 8:6
                                   pic_half_moon     // 7:4                            
                                   }; 

#define PICTURE_POINT(pic,lamp) pgm_read_byte_near(g_pic_table[pic]+lamp*sizeof(byte))

#define iRED 0
#define iGREEN 1
#define iBLUE 2



float g_color_palette[][3]={
          {0  ,0  ,0  },    // 0 = black
          {1  ,0.65,0  },   // 1 = yellow
          {0  ,0.8  ,0.8  },// 2 = cyan
          {0  ,0.5,0.08},   // 3 = mid green
          {0.2,0.1,0 },     // 4 = dark brown
          {1,0.0,0  },      // 5 = red
          {0  ,0  ,0.8},    // 6 = blue
          {1  ,1  ,1  },    // 7 = white
          {0.8  ,0  ,0.8  },// 8 = pink
          {1  ,0.3,0  },    // 9 = orange
          {0  ,1  ,0  },    // 10 (A)= bright green
          {0.1  ,0  ,0.75 },// 11 (B)= dark purple
          {1, 0.55,0},      // 12 (C)= gold
          {0.5,0.0,0.07},   // 13 (D)= low pastell red
};
//                       GFEDCBA9, 87654321 
byte g_color_set[] =  { B00000011,B10110011,  // 0 = all saturated full colors
                        B00001000,B01000010,  // 1 = Gold, White, Cyan
                        B00000011,B10100011,  // 2 = all saturated full colors except red
                        B00000010,B10110100,  // 3 = Green, pink, blue,red,mid green
                        B00000010,B00110001,  // 4 = Green,, blue,red,yellow
                                       };
#define COLOR_SET_COUNT sizeof(g_color_set)/sizeof(g_color_set[0])/2;


byte g_random_color_map[4]={0,0,0,0};    // this is used to map from group to randomly chosen color
#define RANDOM_COLOR_MAP_COUNT sizeof(g_random_color_map)/sizeof(g_random_color_map[0])

PictureLamp g_picture_lamp[LAMP_COUNT];

unsigned long g_picture_start_time=0;
unsigned long g_picture_duration_time=5000;
unsigned long g_transition_start_time=0;
unsigned long g_transition_follow_up_duration=5000;


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
  FIREWORK_RUN,
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

 for(int p=0;p<PARTICLE_COUNT;p++)
        g_firework_particle[p].init(g_picture_lamp);

  delay(700); // wait for chains to power up completetly

  #ifdef TRACE_RND_COL_GEN
    enter_TEST_MODE_PICTURES();
    return;
  #endif
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
    case FIREWORK_RUN:process_FIREWORK_RUN();break;
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
      Serial.print(F("#SHOW_MODE: "));
      Serial.print(freeMemory());
      Serial.print(F(" bytes free memory. "));
      Serial.print(millis()/1000);
      Serial.println(F(" seconds uptime"));
    #endif

    
    if(g_process_mode!=TRANSITION_MODE) {  // When not coming from Transition, init a random picture
            g_pic_index=random(PICTURE_COUNT);
            set_picture( g_pic_index);
            output_show();
    } else { 
            // immediate end of transition (Picture will fully displayed)
            for(int i=0;i<LAMP_COUNT;i++) 
                  {
                     g_picture_lamp[i].endTransition();
                     g_picture_lamp[i].updateOutput(i);
                   }
            output_show();
    }


    g_process_mode=SHOW_MODE;
    input_IgnoreUntilRelease();
    digitalWrite(LED_BUILTIN, false);
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
    //delay(500);
    #endif
}



/* ----- internal picture presentation helpers --------- */

/* check for random colors, make a unique wish and provide it in random_color_map
 */

void populate_random_color_map(int picture_index){
  byte color_list[16];
  byte color_list_length=0;
  
  for(int i=0;i<RANDOM_COLOR_MAP_COUNT;i++) g_random_color_map[i]=0;  // Reset color map
  
  for (int lmp=0;lmp<LAMP_COUNT;lmp++) {
     byte p_index=PICTURE_POINT(picture_index,lmp);
     if(p_index>=RANDOM_COLOR_INDEX_START) {  // Lamp has random color declaration
        int map_slot=(p_index-RANDOM_COLOR_INDEX_START)/10;
        
        if(g_random_color_map[map_slot]==0) {  // only choose a color for a mapslot, if not already done
          byte color_set_index=p_index%10;
          #ifdef TRACE_RND_COL_GEN
            Serial.print(F("#TRACE_RND_COL_GEN: Choosing for "));
            Serial.print(map_slot);
            Serial.print(F(" from color set "));
            Serial.println(color_set_index);
          #endif  
          // Convert bitmap of color set into list of palette indexes
          color_list_length=0;
          for(int byteindex=0;byteindex<2;byteindex++) {
            byte color_set_bitmap=g_color_set[color_set_index*2+byteindex];
            for (int bitindex=0;bitindex<8;bitindex++) {
              if(bitRead(color_set_bitmap,bitindex)) {  
                color_list[color_list_length]=bitindex+((1-byteindex)*8)+1;
                  #ifdef TRACE_RND_COL_GEN
                    Serial.println(color_list[color_list_length]);
                  #endif  
                color_list_length++;
                }
             } // Loop over bits
          } // Loop over bytes

          // Chose random element of the list and check if color already used, if not traverse until you find one
          byte choice=random(color_list_length);
          for(int check=0;check<color_list_length ;check++) { // only try until traversing full list
              int m=0;
              for(;m<RANDOM_COLOR_MAP_COUNT;m++) {
                if(g_random_color_map[m]==color_list[choice]) break; 
              }
              if(m>=RANDOM_COLOR_MAP_COUNT) {  // This color is not used yet
                g_random_color_map[m]=color_list[choice];
                break; // leave the check loop
              }
              if(++choice >= color_list_length) choice=0;
          } // end check loop
          if(g_random_color_map[map_slot]==0) g_random_color_map[map_slot]=color_list[choice]; // if decision could not be made use first idea
          #ifdef TRACE_RND_COL_GEN
            Serial.print(F("#TRACE_RND_COL_GEN: Final choice is "));
            Serial.println(g_random_color_map[map_slot]);
          #endif 
        } // end map slot is empty
        
     } // end this is a random color entry
  } // loop over all lamps in the picture
}

byte get_mapped_color_number(byte pic_color_entry) {
  if(pic_color_entry<RANDOM_COLOR_INDEX_START) return pic_color_entry;
  byte map_slot=(pic_color_entry-RANDOM_COLOR_INDEX_START)/10;
  return g_random_color_map[map_slot];
}

/* set_picture
 *  Orders all lamp objects to go to the defined picuture
 */
void set_picture(int picture_index)
{
#ifdef TRACE_PICTURES
  Serial.print(F("TRACE_PICTURES::set_picture"));
  Serial.println(picture_index);
#endif
  populate_random_color_map(picture_index);
  for (int i=0;i<LAMP_COUNT;i++) {
     byte c_index=get_mapped_color_number(PICTURE_POINT(picture_index,i));
     g_picture_lamp[i].setCurrentColor(g_color_palette[c_index][iRED],g_color_palette[c_index][iGREEN],g_color_palette[c_index][iBLUE]);
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
  populate_random_color_map(picture_index);
  for (int i=0;i<LAMP_COUNT;i++) {
     byte  c_index=get_mapped_color_number(PICTURE_POINT(picture_index,i));
     g_picture_lamp[i].setTargetColor(g_color_palette[c_index][iRED],g_color_palette[c_index][iGREEN],g_color_palette[c_index][iBLUE]);
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
    Serial.print(millis()/1000);
    Serial.println(F(" seconds uptime"));
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

  if(g_picture_lamp[trigger_lamp].getTransitionType()==TT_ON && off_count>0 ) {// There is a partner to switch off available
    if(random(on_count)<=off_count) {
      trigger_lamp=getRandomLampOfTransitionType(TT_OFF);
      g_picture_lamp[trigger_lamp].startTransition(duration);
      #ifdef TRACE_LOGIC
          Serial.print(F("TRACE_LOGIC::triggered additional off lamp ")); Serial.println(trigger_lamp);
      #endif  
    }
  } else {  // it is not an on transitioning lamp
    if(g_picture_lamp[trigger_lamp].getTransitionType()==TT_OFF && on_count>0 ) {// There is a partner to switch on available
      if(random(off_count)<=on_count) {
        trigger_lamp=getRandomLampOfTransitionType(TT_ON);
        g_picture_lamp[trigger_lamp].startTransition(duration);
        #ifdef TRACE_LOGIC
            Serial.print(F("TRACE_LOGIC::triggered additional on lamp ")); Serial.println(trigger_lamp);
        #endif  
      }
    }
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




/* ========= TEST_MODE_PLACEMENT ======== */

void enter_TEST_MODE_PLACEMENT() 
{
    #ifdef TRACE_MODES
      Serial.println(F("#TEST_MODE_PLACEMENT"));
    #endif
    g_process_mode=TEST_MODE_PLACEMENT;
    input_IgnoreUntilRelease();
    digitalWrite(LED_BUILTIN, false);
    g_pic_index=0;
    for(int i=0;i<LAMP_COUNT;i++)  output_setLightColorUnmapped(i,0,0,0);  // shut down all lights
    output_show();
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
    digitalWrite(LED_BUILTIN, false);
    g_pic_index=0;
    set_picture( g_pic_index);
    output_show();
}

void process_TEST_MODE_PICTURES()
{
     if(input_selectGotReleased()) {  
      if(input_getLastPressDuration()>LONG_PRESS_DURATION) {   // Long press
                    enter_TEST_MODE_FADE_SOLO();
                   return;
      }
      if(g_pic_index==0 || g_pic_index>=PICTURE_COUNT) g_pic_index=PICTURE_COUNT; // backward one pattern
      --g_pic_index;
      set_picture( g_pic_index);
      output_show(); 
    }
    
    if(input_stepGotPressed()) {  // foreward one pattern
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
    digitalWrite(LED_BUILTIN, false);
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
    digitalWrite(LED_BUILTIN, false);
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
    digitalWrite(LED_BUILTIN, false);
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

/* ******************** Memory Helper *************** */
 
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}


