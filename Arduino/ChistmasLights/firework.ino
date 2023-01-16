#include "particle.h"

#ifdef TRACE_ON
//#define TRACE_FIREWORK
//#define TRACE_FIREWORK_HIGH

#endif 

#define FW_MAX_RUNTIME 3600000     // 1 Hours 
//#define FW_MAX_RUNTIME 60000     // 1 Minute

#define FW_SHOW_TYPE_COUNT 7

/*  ===== Description of the Firework Implementation pattern =======
For Implementing the firework shows I use multiple layers of abstraction. 
Lamp Object = Has the ability to fade from one color / brightness to another in a defined time 
Path = A pre defined chain of Lamp objects, defining a stay duration and a fade duratoin
Particle = A virtual lamp trigger, "ignightin lamps" while running along a path
show = A set of instructions, starting particles at the right time for the effect

Show initiation
Every show has its own initiation phase, to load its needed paths into the global lamp_path_buffer.
By setting the g_fw_show_shot_limit and counting the the number of particles fired in g_fw_show_shot, the show defines 
it's ending criteria. Like in "Nature" this limit an be randomized in a approriate range

The main loop 
- first checks, if a show needs to be started, and will chose a new show
- Checks it the show is over by comparing number of shots and the limit
- Checks if the next particle has to be emitted according to g_picture_start_time and  g_picture_duration_time 
  and calls the particle emmitter function for the running show
- Let all active particles calculate their progress (wich might trigger new lamps)
- Let all active lamps calculate their progress
- Update the Lamps

Partice emitter functions
Every show has a particle emitter function, that will be called by the main loop, when the time for a new partice has been reached.
The emitter function needs to
- Determine the particles to start, depending on the g_fw_show_shot value
- increment g_fw_show_shot
- set g_picture_duration_time to the duration until the next particle has to be triggered

*/


                                                                      // lamp number and timing(4 bit fade, 4 bit stay duration)  for particle
                                                                      // fade=0-F = 10-535ms 35ms steps
                                                                      // framelength=0-F = 10-535ms 35ms steps  
                                                                      // Lamp 255 = Endmarker
//                                1       2       3       4       6       6       7       8        9      10
const byte fw_path_long_arc[] PROGMEM ={ 9,0x51,18,0x51, 2,0x62, 3,0x73,12,0x73,13,0x62, 14,0x62,255};   
const byte fw_path_central_arc[] PROGMEM ={ 21,0x51,1,0x62, 24,0x62, 3,0x73,18,0x73,9,0x62,255}; 

const byte fw_path_beam_3h[] PROGMEM ={ 4,0xc2,13,0xa2, 7,0x62,255};   
const byte fw_path_beam_5h[] PROGMEM ={ 5,0xc2,15,0xa2, 8,0x62,255};   
const byte fw_path_beam_7h[] PROGMEM ={ 1,0xc2,17,0xa2, 9,0x62,255};   
const byte fw_path_beam_9h[] PROGMEM ={ 2,0xc2,19,0xa2,10,0x62,255};  
const byte fw_path_beam_0h[] PROGMEM ={ 3,0xc2,11,0xa2, 6,0x62,255};
   
const byte fw_path_circle[] PROGMEM ={ 22,0xc0,4,0xf1,2,0xc0,11,0xb0, 20,0xe1,24,0xd0,19,0xc1,12,0xb0,255};   // stay duration 0 = light all lamps instanty
//const byte fw_path_circle[] PROGMEM ={ 22,0xc0,4,0xf0,2,0xc0,11,0xb0, 20,0xe0,24,0xd0,19,0xc0,12,0xb0,255};   // stay duration 0 = light all lamps instanty
const byte fw_path_straigth_accellerating[] PROGMEM ={ 9,0x53,18,0x42, 2,0x21, 3,0x11,11,0x11,6,0x11, 255};   
const byte fw_path_straigth_decellerating[] PROGMEM ={ 9,0x32,18,0x32, 2,0x43, 3,0x43,11,0x54,6,0x54, 255};  
const byte fw_path_peak_explode_1[] PROGMEM ={ 22,0xd0,12,0xcd, 20,0xa0, 3,0xb0, 255};  
const byte fw_path_peak_explode_2[] PROGMEM ={ 22,0xa0,23,0xdd, 20,0xb0, 13,0xc0, 255};  
const byte fw_path_middle_up[] PROGMEM ={ 21,0x32,1,0x22, 24,0x43, 6,0x43, 255};  


byte g_next_free_particle=0;
byte g_fw_show_type=0;
int g_fw_show_shot=0; 
int g_fw_show_shot_limit=0; 
float  g_fw_main_color;
boolean g_mirror=false;
byte g_prev_fw_show_type=0;

// interval map that is used to create an uneven probabilty pattern. longer interval = higher probability
const byte show_probabilty_hitrange[] PROGMEM = {15,  // Sparkle left right  (15%)
                                                 35,  // Single random rockets (20%)
                                                 43,  // sunflower (8%)
                                                 55,  // Joker (12%)
                                                 75,  // roman lights (20%)
                                                 100};  // Glitter bomb (25%)





/* ========= FIREWORK_RUN ======== */

void enter_FIREWORK_RUN() 
{
    #ifdef TRACE_MODES
      Serial.print(F("#FIREWORK_RUN: "));
      Serial.print(freeMemory());
      Serial.println(F(" bytes free memory"));
    #endif
    delay(500);
    g_process_mode=FIREWORK_RUN;
    digitalWrite(LED_BUILTIN, false);
    input_IgnoreUntilRelease();
    for(int i=0;i<LAMP_COUNT;i++)  /* Initialize all lamps */
    {
         g_picture_lamp[i].setTargetColor(0,0,0);
         g_picture_lamp[i].endTransition();
         g_picture_lamp[i].updateOutput(i);
       }

    g_picture_start_time=millis();
    g_transition_follow_up_duration=millis();  // we use this global to track the overall runtime
    g_picture_duration_time=0;
    g_next_free_particle=0;
    for(int p=0;p<PARTICLE_COUNT;p++)
        g_firework_particle[p].end();
    g_fw_show_type=3;  // start with sun
    fw_init_show();
}

void process_FIREWORK_RUN()
{

    if(input_modeGotReleased()) {  
      if(input_getLastPressDuration()>LONG_PRESS_DURATION)   // Long press
         enter_SENSOR_CALIBRATION();
      else 
         enter_SHOW_MODE();
      return;
    }
    
    if(input_stepGotPressed()) {       //toggle to  next show
      if(++g_fw_show_type>=FW_SHOW_TYPE_COUNT)g_fw_show_type=1;
      #ifdef TRACE_FIREWORK
        Serial.println(F(">TRACE_FIREWORK: step to show "));
      #endif
      fw_init_show();
    } 
    
    if(g_fw_show_type==0 && ( millis()-g_picture_start_time )> g_picture_duration_time) {       //start next random show after a pause show
      byte probabiliy_hitrange[FW_SHOW_TYPE_COUNT];
      byte dice100;

      if(millis()-g_transition_follow_up_duration>FW_MAX_RUNTIME) {  // Leave mode after max runtime
          enter_SHOW_MODE();
          return;
      }
      
      memcpy_P(probabiliy_hitrange,show_probabilty_hitrange,FW_SHOW_TYPE_COUNT);
      g_fw_show_type=0;
      while(g_fw_show_type==g_prev_fw_show_type || g_fw_show_type==0) {
        dice100=random(100);
        for(g_fw_show_type=1; dice100>probabiliy_hitrange[g_fw_show_type-1];g_fw_show_type++);     
      }
      g_prev_fw_show_type=g_fw_show_type;
          
      #ifdef TRACE_FIREWORK
        Serial.println(F(">TRACE_FIREWORK: start show "));
      #endif
      digitalWrite(LED_BUILTIN, false);
      fw_init_show();
    }
    
    if(g_fw_show_shot>=g_fw_show_shot_limit) {   // end current show with  short pause
      digitalWrite(LED_BUILTIN, true);
      g_fw_show_type=0;
      g_fw_show_shot=0;
      g_fw_show_shot_limit=1;
      g_picture_start_time=millis();
      g_picture_duration_time=1000*random(5)+4000;  // choose a random pause time 4 to 9 sec
     
      #ifdef TRACE_FIREWORK
        Serial.print(F(">TRACE_FIREWORK: pausing for "));
        Serial.println(g_picture_duration_time);
      #endif
      return;
    }

    // Initiate new particles
    if(millis()-g_picture_start_time > g_picture_duration_time) {  
      g_picture_start_time=millis();
      switch (g_fw_show_type){
        case 1: show_1_next_particle(); break;
        case 2: show_2_next_particle(); break;
        case 3: show_3_next_particle(); break;
        case 4: show_4_next_particle(); break;
        case 5: show_5_next_particle(); break;
        case 6: show_6_next_particle(); break;
        default:
            #ifdef TRACE_FIREWORK
              Serial.print(F(">TRACE_FIREWORK: unhandled show type "));
              Serial.println(g_fw_show_type);
            #endif 
            break;
      }
    }
    
    // Update all running particles
    for(int i=0;i<PARTICLE_COUNT;i++) {
      if(g_firework_particle[i].inProgress()) g_firework_particle[i].process();
    }
 
    // update the transitioning lights
    for(int i=0;i<LAMP_COUNT;i++) 
    {
      if(g_picture_lamp[i].is_in_transition()) 
      {
        g_picture_lamp[i].updateOutput(i);
      }
    }
    output_show();
}

void fw_init_show() {
  #ifdef TRACE_FIREWORK
        Serial.print(F(">TRACE_FIREWORK: fw_init_show "));
        Serial.println(g_fw_show_type);
  #endif
  int step;
  g_picture_start_time=millis();
  g_picture_duration_time=0;
  g_next_free_particle=0;
  g_fw_show_shot=0; 

  switch(g_fw_show_type) {
    case 1:           // Sparkle from left and right bottom
           g_fw_show_shot_limit=70+random(20); 
           memcpy_P(g_lamp_path_buffer[0],fw_path_long_arc,LAMP_PATH_BUFFER_LENGTH);
           g_lamp_path_buffer[0][LAMP_PATH_BUFFER_LENGTH-1]=255; //guarantee final stop of pattern
           break;
    case 2:          // single Rockets
           g_fw_show_shot_limit=10+random(4)*2;   // 5(+2) Rockets
           memcpy_P(g_lamp_path_buffer[0],fw_path_straigth_accellerating,LAMP_PATH_BUFFER_LENGTH);  // We use long arc for liftoff
           g_lamp_path_buffer[0][8]=255; // Cut off the path at explosion point
           memcpy_P(g_lamp_path_buffer[1],fw_path_circle,LAMP_PATH_BUFFER_LENGTH);  // This is the colorfull circle
           break;
    case 3:          // Sunflower
           g_fw_show_shot_limit=1500;   // about 22 seconds with 15 ms per shot
           memcpy_P(g_lamp_path_buffer[0],fw_path_beam_3h ,LAMP_PATH_BUFFER_LENGTH);
           memcpy_P(g_lamp_path_buffer[1],fw_path_beam_5h ,LAMP_PATH_BUFFER_LENGTH);   
           memcpy_P(g_lamp_path_buffer[2],fw_path_beam_7h ,LAMP_PATH_BUFFER_LENGTH);  
           memcpy_P(g_lamp_path_buffer[3],fw_path_beam_9h ,LAMP_PATH_BUFFER_LENGTH); 
           memcpy_P(g_lamp_path_buffer[4],fw_path_beam_0h ,LAMP_PATH_BUFFER_LENGTH);
           g_fw_main_color=random(360); 
           break;
    case 4:          // Joker
           g_fw_show_shot_limit=50+random(12);   
           memcpy_P(g_lamp_path_buffer[0],fw_path_beam_3h ,LAMP_PATH_BUFFER_LENGTH);
           memcpy_P(g_lamp_path_buffer[1],fw_path_beam_5h ,LAMP_PATH_BUFFER_LENGTH);   
           memcpy_P(g_lamp_path_buffer[2],fw_path_beam_7h ,LAMP_PATH_BUFFER_LENGTH);  
           memcpy_P(g_lamp_path_buffer[3],fw_path_beam_9h ,LAMP_PATH_BUFFER_LENGTH); 
           memcpy_P(g_lamp_path_buffer[4],fw_path_beam_0h ,LAMP_PATH_BUFFER_LENGTH);
           break;
  case 5:          // Roman lights
           g_fw_show_shot_limit=2*(6+random(4)); // one cycle needs 2 steps
           memcpy_P(g_lamp_path_buffer[0],fw_path_straigth_decellerating ,LAMP_PATH_BUFFER_LENGTH);  
           memcpy_P(g_lamp_path_buffer[1],fw_path_peak_explode_1 ,LAMP_PATH_BUFFER_LENGTH);  
           memcpy_P(g_lamp_path_buffer[2],fw_path_peak_explode_2 ,LAMP_PATH_BUFFER_LENGTH);
           break;
  case 6:          // Glitterbomb
           g_fw_show_shot_limit=2*7; // one cycle needs 7 shots
            
           memcpy_P(g_lamp_path_buffer[0],fw_path_middle_up,LAMP_PATH_BUFFER_LENGTH);
           g_lamp_path_buffer[0][6]=255; // Stop after 3rd element (middle led)
             
           for(step=0;step<10;step++) { //Put all innter and outer star lamps into the pattern
            g_lamp_path_buffer[1][step*2]=step+1;
            g_lamp_path_buffer[1][step*2+1]=0xf0;
           }
           g_lamp_path_buffer[1][4*2+1]=0xff; // set long step time from inner to outer ring so we can insert the middle
           g_lamp_path_buffer[1][20]=255;

           for(step=0;step<10;step++) { //Put all 10 "minute ring" leds into the pattern
            g_lamp_path_buffer[2][step*2]=step+11;
            g_lamp_path_buffer[2][step*2+1]=0xf0;
           }
           g_lamp_path_buffer[2][20]=255;

           break;
  }  
}       

void fw_trace_paths_to_serial()
{
  for(int bank=0;bank<LAMP_PATH_BANK_COUNT;bank++) {
              Serial.print(F("Path Bank "));
              Serial.print(bank);
              Serial.print(F(" : "));
      for(byte step=0;step<LAMP_PATH_MAX_STEPS;step++) {
              Serial.print(g_lamp_path_buffer[bank][step*2]);
              Serial.print(F(","));
              Serial.print(g_lamp_path_buffer[bank][step*2+1],HEX);
              Serial.print(F(" / "));
      }
      Serial.println();
  }
}

void show_1_next_particle()  // Sparkle from left and right bottom
{
      t_color_hsv color;
      color.h=40; // mostly yello with little Orange
      color.s=random(60)/100.0; // Nearly white
      color.v=1;  // Brightest
      g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[0],4+random(3),random(2),3,color,255,0.7);
      if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
      g_picture_duration_time=150+random(400);
      g_fw_show_shot++;
}

void show_2_next_particle()  // Launch Rockets
{
// path 0 is th launch path
// path 1 is a circle, but the points are not in order, so cutting of path 1 will still have particles in all directions, only less then possible
      t_color_hsv color;
      if(g_fw_show_shot%2==0) { //Launch
          color.h=25; // Orange
          color.s=0.8; 
          color.v=0.4; 
          g_mirror=random(2);
          g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[0],g_mirror,5,color);
          if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
          g_picture_duration_time=1000+random(200);
      } else {          // Explode
          color.h=random(360);   // in any color
          color.s=1; 
          color.v=1; 
          g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[1],9-random(6),  // limit path randomly (path lamps are scattered, so )
                                                                                g_mirror,
                                                                                5,    // timescale 5 (path has 0 step duration for immediate illumination)
                                                                                color,
                                                                                255,1); // dont fade the particle (high start index, fade factor is 1)
          if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
          g_picture_duration_time=3000+random(1500);    
      }
      g_fw_show_shot++;
}

void show_3_next_particle()  // Sunflower Particle
{   
      #define SHOW3_END_PHASE_LIMIT 150
      t_color_hsv color;
      int shotsLeft=g_fw_show_shot_limit-g_fw_show_shot;
      float value_ramp=shotsLeft>SHOW3_END_PHASE_LIMIT?0.5:shotsLeft/250.0; // set intensitiy to 1 unless for last 250 particles fade to 0
      
      color.h=shotsLeft>SHOW3_END_PHASE_LIMIT?g_fw_main_color:30; // Set color unless for SHOW3_END_PHASE_LIMIT particles set to orange
      color.s=shotsLeft>SHOW3_END_PHASE_LIMIT?(random(12)?1:0):0.8; //1/12 chance  to get white; stick to color for last SHOW3_END_PHASE_LIMIT particles
      color.v=value_ramp; 
      byte lfo=shotsLeft/800; // Counting slowly to 0 over runtime of show
      int pathIndex=(g_fw_show_shot/(2+(lfo/3)))%5;
      if(g_mirror)pathIndex=4-pathIndex;  // flip rotation in mirror mode
      g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[pathIndex],g_mirror,1,color);
      if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
      g_picture_duration_time=12;
      g_fw_show_shot++;
}

void show_4_next_particle()  // Joker
{
      t_color_hsv color;
      color.h=30+random(15); // mostly Orange to yello
      color.s=0.9-random(20)/100.0; // more colorful
      color.v=1;  // Brightness
      g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[random(5)],
                                                        random(2),  // mirror
                                                        1,  // Timescale 2
                                                        color);
      if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
      g_picture_duration_time=350+random(400); // time to next new particle
      g_fw_show_shot++;
}

void show_5_next_particle()  // Roman Lights
{
      t_color_hsv color;
      if(g_fw_show_shot%2==0) { //Launch
          color.h=g_fw_show_shot%4==0?0:120; // red or green
          color.s=1;
          color.v=1;  // Brightness
          g_mirror=random(2);
          g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[0],g_mirror,2,color);
          if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
          g_picture_duration_time=450;  
      } else {  // explode 
          color.h=35; // orange
          color.s=1;
          color.v=0.3;  // Brightness
          g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[1+random(2)],g_mirror,3,color);
          if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
          g_picture_duration_time=1000+random(300); 
      }
      g_fw_show_shot++;
}

void show_6_next_particle()  // Glitterbomb
{
    t_color_hsv color;
    byte step;
    byte phase=g_fw_show_shot%7;
    #ifdef TRACE_FIREWORK_HIGH
        Serial.print(F(">TRACE_FIREWORK_HIGH: show_6_next_particle: phase "));
        Serial.println(phase);
    #endif
    switch (phase) {
      case 0:  // Launch
            for(byte bank=3;bank<LAMP_PATH_BANK_COUNT;bank++){  // Create glitter Pattern in bank 3-4
              for(step=0;step<LAMP_PATH_MAX_STEPS;step++) {
                g_lamp_path_buffer[bank][step*2]=random(LAMP_COUNT)+1;
                g_lamp_path_buffer[bank][step*2+1]=0x01|((10+random(5))<<4);
              }
              g_lamp_path_buffer[bank][step*2]=255;
            }
            #ifdef TRACE_FIREWORK_HIGH
                 fw_trace_paths_to_serial();
            #endif
            color.h=15; // nearly red
            color.s=1;
            color.v=0.3;  // Brightness
            g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[0],false,2,color);
            if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
            g_picture_duration_time=1300;
            break;
      case 1:  // Explode inner/outer ring
            g_fw_main_color=random(360); 
            color.h=g_fw_main_color;
            color.s=1;
            color.v=1;
            g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[1],false,8,color);
            if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
            g_picture_duration_time=1000;
            break;          
      case 2:  // Explode middle ring
            color.h=g_fw_main_color;
            color.s=1;
            color.v=1;
            g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[2],false,8,color);
            if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
            g_picture_duration_time=1000;
            break;
      case 3:
      case 4:
      case 5:
      case 6:
            color.h=30; // gold
            color.s=1-random(50)/100.0;  // Slight saturation differences
            color.v=0.8;  // Brightness
            g_firework_particle[g_next_free_particle].start(g_lamp_path_buffer[3+(phase%2)],phase%4,4,color);
            if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
            if(phase<6) g_picture_duration_time=150;
            else g_picture_duration_time=2000+random(1500);
    }
    g_fw_show_shot++;
}

