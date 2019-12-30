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
//                                1       2       3       4       6       6       7       8        9      10
const byte fw_path_1[] PROGMEM ={ 9,0x51,18,0x51, 2,0x62, 3,0x73,11,0x73,12,0x62, 4,0x62, 5,0x62,15,0x62, 8,0xf2,255};   // lamp and duration(4 bit fade, 4 bit framelenth)  for particle
const byte fw_path_2[] PROGMEM ={ 8,0x51,15,0x51, 5,0x62, 4,0x73,12,0x73,11,0x62, 3,0x62, 2,0x62,18,0x62, 9,0xf2,255};   // lamp and duration(4 bit fade, 4 bit framelenth)  for particle
const byte* const g_fw_path_table [] = {fw_path_1,fw_path_2};


byte g_fw_path_buffer[PARTICLE_COUNT][FW_PATH_BUFFER_LENGTH];
byte g_next_free_particle=0;
#define FW_SHOW_TYPE_COUNT 1
byte g_fw_show_type=0;
int g_fw_show_shots=0; 
int g_fw_show_shots_limit=0; 


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
    g_fw_show_type=0;
    g_fw_show_shots=0; 
    g_fw_show_shots_limit=0; 
}

void process_FIREWORK_RUN()
{

    if(input_selectGotPressed()) {
      enter_TEST_MODE_PLACEMENT();
      return;
    }
    
    long secondOfTheDay=((millis()-g_clock_sync_time)/1000+g_clock_base_time)%SECONDS_PER_DAY;
    if(secondOfTheDay>3600) {
      #ifndef DISARM_1_HOUR_RESET
          enter_SHOW_MODE();
      #endif
    }

    if(g_fw_show_type==0 and millis()-g_picture_start_time < g_picture_duration_time) {       //start next show
      g_fw_show_type=1+random(FW_SHOW_TYPE_COUNT);
      fw_init_show();
    }
    
    if(g_fw_show_shots>=g_fw_show_shots_limit) {   // Create short pause
      g_fw_show_type=0;
      g_picture_start_time=millis();
      g_picture_duration_time=1000*random(10)+5;
      return;
    }

    // Initiate new particles
    if(millis()-g_picture_start_time > g_picture_duration_time) {  
      g_picture_start_time=millis();
      switch (g_fw_show_type){
        case 1: show_1_next_particle();
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
  g_picture_start_time=millis();
  g_picture_duration_time=0;
  g_next_free_particle=0;
  switch(g_fw_show_type) {
    case 1:           // Sparkle from left and right bottom
           g_fw_show_shots=0; 
           g_fw_show_shots_limit=50; 
           memcpy_P(g_fw_path_buffer[0],g_fw_path_table[0],FW_PATH_BUFFER_LENGTH);
           g_fw_path_buffer[0][FW_PATH_BUFFER_LENGTH-1]=255; //guarantee final stop of pattern
           break;
  }  
}       

void show_1_next_particle()  // Sparkle from left and right bottom
{
      t_color_hsv color;
      color.h=40; // mostly yello with little Orange
      color.s=random(60)/100.0; // Nearly white
      color.v=1;  // Brightest
      g_firework_particle[g_next_free_particle].start(g_fw_path_buffer[0],random(2),3,color,255,0.7);
      if(++g_next_free_particle>=PARTICLE_COUNT)g_next_free_particle=0;
      g_picture_duration_time=200+random(600);
}



