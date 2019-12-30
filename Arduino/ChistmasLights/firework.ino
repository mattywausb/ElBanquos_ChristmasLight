#include "particle.h"

#ifdef TRACE_ON
#define TRACE_FIREWORK
#define DISARM_1_HOUR_RESET
#endif 



#define FW_PATH_MAX_STEPS 10
#define FW_PATH_BUFFER_LENGTH FW_PATH_MAX_STEPS*2+1
/* Fireworks */

                                                                      // fade=0-F = 10-535ms 35ms steps
                                                                      // framelength=0-F = 10-535ms 35ms steps  
                                                                      // Lamp 255 = Endmarker
                                                                      // lamp and duration(4 bit fade, 4 bit framelenth)  for particle
//                                1       2       3       4       6       6       7       8        9      10
const byte fw_path_long_arc[] PROGMEM ={ 9,0x51,18,0x51, 2,0x62, 3,0x73,12,0x73,13,0x62, 14,0x62,255};   
const byte fw_path_central_arc[] PROGMEM ={ 21,0x51,1,0x62, 24,0x62, 3,0x73,18,0x73,9,0x62,255}; 
const byte fw_path_beam_3h[] PROGMEM ={ 4,0x62,13,0x62, 7,0x62,255};   
const byte fw_path_beam_5h[] PROGMEM ={ 5,0x62,15,0x62, 8,0x62,255};   
const byte fw_path_beam_7h[] PROGMEM ={ 1,0x62,17,0x62, 9,0x62,255};   
const byte fw_path_beam_9h[] PROGMEM ={ 2,0x62,19,0x62, 10,0x62,255};  
const byte fw_path_beam_0h[] PROGMEM ={ 3,0x62,11,0x62, 6,0x62,255};   
const byte fw_path_circle[] PROGMEM ={ 22,0xd0,4,0xf0,2,0xc0,11,0xb0, 20,0xe0,24,0xd0,19,0xc0,12,0xb0,255};   
const byte fw_path_straigth_accellerating[] PROGMEM ={ 9,0x53,18,0x42, 2,0x21, 3,0x11,11,0x11,6,0x11, 255};   



byte g_fw_path_buffer[PARTICLE_COUNT][FW_PATH_BUFFER_LENGTH];
byte g_next_free_particle=0;
#define FW_SHOW_TYPE_COUNT 3
byte g_fw_show_type=0;
int g_fw_show_shot=0; 
int g_fw_show_shot_limit=0; 
boolean g_mirror=false;


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
    input_IgnoreUntilRelease();
    for(int i=0;i<LAMP_COUNT;i++)  /* Initialize all lamps */
    {
         g_picture_lamp[i].setTargetColor(0,0,0);
         g_picture_lamp[i].endTransition();
         g_picture_lamp[i].updateOutput(i);
       }

    g_picture_start_time=millis();
    g_picture_duration_time=0;
    g_next_free_particle=0;
    for(int p=0;p<PARTICLE_COUNT;p++)
        g_firework_particle[p].end();
    g_fw_show_type=2;
    fw_init_show();
}

void process_FIREWORK_RUN()
{

    if(input_selectGotPressed()) {
      enter_TEST_MODE_PLACEMENT();
      return;
    }
    
    long secondOfTheDay=((millis()-g_clock_sync_time)/1000+g_clock_base_time)%SECONDS_PER_DAY; // Todo switch to 1 hour after ENtering mode
    if(secondOfTheDay>3600) {
      #ifndef DISARM_1_HOUR_RESET
          enter_SHOW_MODE();
      #endif
    }

    if(input_stepGotPressed()) {       //toggle to  next show
      if(++g_fw_show_type>=FW_SHOW_TYPE_COUNT)g_fw_show_type=1;
      #ifdef TRACE_FIREWORK
        Serial.println(F(">TRACE_FIREWORK: step to show "));
      #endif
      fw_init_show();
    } 
    
    if(g_fw_show_type==0 && ( millis()-g_picture_start_time )> g_picture_duration_time) {       //start next show
      g_fw_show_type=1+random(FW_SHOW_TYPE_COUNT-1);
      #ifdef TRACE_FIREWORK
        Serial.println(F(">TRACE_FIREWORK: start show "));
      #endif
      fw_init_show();
    }
    
    if(g_fw_show_shot>=g_fw_show_shot_limit) {   // end current show with  short pause
      g_fw_show_type=0;
      g_fw_show_shot=0;
      g_fw_show_shot_limit=1;
      g_picture_start_time=millis();
      g_picture_duration_time=1000*random(10)+5000;
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
  g_picture_start_time=millis();
  g_picture_duration_time=0;
  g_next_free_particle=0;
  g_fw_show_shot=0; 
  switch(g_fw_show_type) {
    case 1:           // Sparkle from left and right bottom
           g_fw_show_shot_limit=50; 
           memcpy_P(g_fw_path_buffer[0],fw_path_long_arc,FW_PATH_BUFFER_LENGTH);
           g_fw_path_buffer[0][FW_PATH_BUFFER_LENGTH-1]=255; //guarantee final stop of pattern
           break;
    case 2:          // single Rockets
           g_fw_show_shot_limit=10;   // 5 Rockets
           memcpy_P(g_fw_path_buffer[0],fw_path_straigth_accellerating,FW_PATH_BUFFER_LENGTH);  // We use long arc for liftoff
           g_fw_path_buffer[0][8]=255; // Limit the path at explosion point
           memcpy_P(g_fw_path_buffer[1],fw_path_circle,FW_PATH_BUFFER_LENGTH);  // This is the colorfull circle
           break;

  }  
}       

void show_1_next_particle()  // Sparkle from left and right bottom
{
      t_color_hsv color;
      color.h=40; // mostly yello with little Orange
      color.s=random(60)/100.0; // Nearly white
      color.v=1;  // Brightest
      g_firework_particle[g_next_free_particle].start(g_fw_path_buffer[0],4+random(3),random(2),3,color,255,0.7);
      if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
      g_picture_duration_time=150+random(400);
      g_fw_show_shot++;
}

void show_2_next_particle()  // Launch Rockets
{
      t_color_hsv color;
      if(g_fw_show_shot%2==0) { //Launch
          color.h=25; // Orange
          color.s=0.8; 
          color.v=0.2; 
          g_mirror=random(2);
          g_firework_particle[g_next_free_particle].start(g_fw_path_buffer[0],g_mirror,3,color);
          g_picture_duration_time=1000+random(200);
      } else {          // Explode
          color.h=random(360); 
          color.s=1; 
          color.v=1; 
          g_firework_particle[g_next_free_particle].start(g_fw_path_buffer[1],g_mirror,5,color);
          g_picture_duration_time=3000+random(3000);    
      }
      g_fw_show_shot++;
}
