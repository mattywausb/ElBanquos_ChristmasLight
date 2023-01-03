#define SECONDS_PER_DAY 86400

// packed palette Color 1 (Color of used element) | color 0 (Color of unused element)
//                                             0    1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23
const byte clock_hour_color[24]   PROGMEM ={0x80,0x60,0x60,0x60,0x60,0x26,0x26,0x26,0x26,0x26,0x22,0xA0,0x10,0x59,0x59,0x59,0x59,0x59,0x05,0x05,0x05,0x05,0x05,0x00};
#define GET_HOUR_COLOR_BYTE(hour) pgm_read_byte_near(clock_hour_color+hour*sizeof(byte))

#define MINUTE_FUTURE_COLOR 1
#define MINUTE_PAST_COLOR 6

const byte clock_minute_color[14]   PROGMEM ={10,2,7,9,5,MINUTE_PAST_COLOR};   // defines color sequence of minute indication light
#define GET_MINUTE_COLOR(segment) pgm_read_byte_near(clock_minute_color+segment*sizeof(byte))

const byte clock_minute_border[]   PROGMEM ={0,1,7,13,18,23,28,33,38,43,48,54,60};   // defines smalles minute to use the segment
#define GET_MINUTE_BORDER(segment) pgm_read_byte_near(clock_minute_border+segment*sizeof(byte))

const byte clock_second_6_color[7]   PROGMEM ={11,2,10,7,9,5,1};   // defines color sequence of 6 phase second  indication (10 sec per phase)
#define GET_SECOND_6_COLOR(segment) pgm_read_byte_near(clock_second_6_color+segment*sizeof(byte))

const byte clock_second_12_color[13]   PROGMEM ={0,11,6,11,8,6,2,10,7,1,9,5,0};   // defines color sequence of 12 phase second  indication  (5 sec per phase)
#define GET_SECOND_12_COLOR(segment) pgm_read_byte_near(clock_second_12_color+segment*sizeof(byte))

long g_clock_base_time=(18l*60l+57l)*60l;  // Time in seconds to add to systemclock to get realtime (18:58 as default)
unsigned long g_clock_sync_time=0;  // Time to calibrate "zeropoint" of systemclock 


/* ========= CLOCK_MODE ======== */

void enter_CLOCK_MODE() 
{
    #ifdef TRACE_MODES
      Serial.print(F("#CLOCK_MODE: "));
      Serial.print(freeMemory());
      Serial.println(F(" bytes free memory"));
    #endif
    g_process_mode=CLOCK_MODE;
    digitalWrite(LED_BUILTIN, false);
    input_IgnoreUntilRelease();
    long secondOfTheDay=((millis()-g_clock_sync_time)/1000+g_clock_base_time)%SECONDS_PER_DAY;
    for(int i=0;i<LAMP_COUNT;i++)  /* Initialize all lamps */
    {
         g_picture_lamp[i].setCurrentColor(0,0,0);
         g_picture_lamp[i].updateOutput(i);
       }
    order_next_clock_picture(secondOfTheDay,1);
    output_show();
    g_transition_start_time=secondOfTheDay;  // We use this variable to keep track, when second has changed , when entering the mode we pretend to be 1 second behind and trigger immediate change
}

void process_CLOCK_MODE()
{
    long secondOfTheDay=0;
    if(input_modeGotPressed()) {
       enter_FIREWORK_RUN() ;
      return;
    } // Select got pressed
    
    if(input_stepGotPressed()) {  
      enter_CLOCK_SET_HOUR_MODE();
      return;
    } // Step got pressed
    
    secondOfTheDay=((millis()-g_clock_sync_time)/1000+g_clock_base_time)%SECONDS_PER_DAY; //86400=Seconds in a day

    if(secondOfTheDay>0 and secondOfTheDay<10) { // Use the last second for dimming the last light to be sure e dont miss it, we start firework on any of the first 10 seconds of the day (normally at 0)
      enter_FIREWORK_RUN();
      return; 
    }
    
    if(secondOfTheDay!=g_transition_start_time) { // Trigger new Lamp Targets
      g_transition_start_time=secondOfTheDay;
      order_next_clock_picture(secondOfTheDay,550);
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
     // Blink Builtin LED
     if(g_process_mode==CLOCK_SET_HOUR_MODE)    digitalWrite(LED_BUILTIN, millis()%500<250);
     else digitalWrite(LED_BUILTIN, millis()%250<125);
    
     if(input_modeGotPressed()) {
       if(g_process_mode==CLOCK_SET_HOUR_MODE) g_clock_base_time-=3600;
       else g_clock_base_time-=60;
       if (g_clock_base_time<0)g_clock_base_time+=SECONDS_PER_DAY;
       changed=true;
    } // Select got pressed
    
    if(input_stepGotReleased()) {  
      if(input_getLastPressDuration()>LONG_PRESS_DURATION)   // Long press
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
    
    if (changed) { 
      order_next_clock_picture(g_clock_base_time,1);
      #ifdef TRACE_CLOCK_TIME
          Serial.println(F(">TRACE_CLOCK_TIME Set Time:"));
          clock_print_to_Serial_time(g_clock_base_time);
          Serial.println();
        #endif
    };  
    
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

void clock_print_to_Serial_time(long secondOfTheDay)
{
  int currentHour=secondOfTheDay/3600;  
  int currentMinute=(secondOfTheDay/60)%60;  
  int currentSecond=secondOfTheDay%60;  

  Serial.print(currentHour);Serial.print(F(":"));
  Serial.print(currentMinute);Serial.print(F(":"));
  Serial.print(currentSecond);Serial.print(F(" "));
}

void order_next_clock_picture(long secondOfTheDay,int transitionTime)
{
  int currentHour=secondOfTheDay/3600;  
  int currentMinute=(secondOfTheDay/60)%60;  
  int currentSecond=secondOfTheDay%60;  

  byte color2=0;
  byte lamp=0;
  byte colorIndex=0;
  short color1_border=0;
  short color2_border=0;
  int segment=0;
 

  /* HOUR */  /* Lamps 6-10 */
  byte colorByte=GET_HOUR_COLOR_BYTE(currentHour);

  byte color1=colorByte>>4;
  byte color0=colorByte&0x0f;

  unsigned short pattern=0x0fe0;  // defines the color to take from the set: color 1 or color 0. Bit 0=Lamp 6   Bit 5=Lamp 10
  if(currentHour==0) pattern=0x0000; // no hour lamp at midnight
  else
    if(currentHour<11 )  pattern>>=currentHour%5;
    else  if(currentHour<13 )  pattern=0x0fff;
          else if( currentHour<18) pattern>>=currentHour-13;
               else if(currentHour<23) pattern>>=currentHour-18;
                    else pattern=0x0fff;
   #ifdef TRACE_CLOCK
          Serial.print(F(">order_next_clock_picture secondOfTheDay: "));Serial.println(secondOfTheDay);
          Serial.print(F(">order_next_clock_picture HOUR: "));
          Serial.print(currentHour);
   #endif
   for(int i=5;i<10;i++) {
    #ifdef TRACE_CLOCK
          Serial.print(F("/"));
       #endif
        if(pattern&0x0001) {
             #ifdef TRACE_CLOCK
              Serial.print(color1);
             #endif
          g_picture_lamp[i].setTargetColor_int(g_color_palette[color1][iRED],g_color_palette[color1][iGREEN],g_color_palette[color1][iBLUE]);
        } else { 
             #ifdef TRACE_CLOCK
              Serial.print(color0);
             #endif
          g_picture_lamp[i].setTargetColor_int(g_color_palette[color0][iRED],g_color_palette[color0][iGREEN],g_color_palette[color0][iBLUE]);
        }
    
       g_picture_lamp[i].startTransition(transitionTime);
       pattern>>=1;
   }
   #ifdef TRACE_CLOCK
      Serial.println(F(" "));
   #endif

   /* MINUTE RING */ /* Lamps 12-20,11 */ 
   if(currentMinute<59)  { /* Normal Minute */
       for(segment=0; currentMinute>=GET_MINUTE_BORDER(segment+1);segment++);
       #ifdef TRACE_CLOCK
          Serial.print(F(">order_next_clock_picture MINUTE_BORDER: "));    Serial.print( GET_MINUTE_BORDER(segment)); Serial.print(F(" segment="));Serial.println(segment);
       #endif
       color1_border=segment-1;
       color2_border=segment;
       color0=MINUTE_PAST_COLOR;
       color2=MINUTE_FUTURE_COLOR;
       color1=GET_MINUTE_COLOR(currentMinute-GET_MINUTE_BORDER(segment));
       if(currentHour==0) { /* Recover from black to blue in first hour of the day */
        color0=color0==1?0:color0;
        color1=color1==1?0:color1;
        color2=color2==1?0:color2;
       }
   } else {                               /* Countdown last Minute */
      segment=currentSecond/10;
      color1_border=currentSecond%10;
      color2_border=100;
      color0=GET_SECOND_6_COLOR(segment+1); /* newset color */
      color1=GET_SECOND_6_COLOR(segment);  /* previous color */
      if(currentHour==23&&currentMinute==59&&currentSecond>30) { /* Final countdown to Black instead of yellow */
        color0=color0==1?0:color0;
      }
   }
   

   #ifdef TRACE_CLOCK
          Serial.print(F(">order_next_clock_picture MINUTE: "));
          Serial.print(currentMinute);
          Serial.print(F(" c0="));Serial.print(color0); Serial.print(F(" b1="));Serial.print(color1_border);
          Serial.print(F(" c1="));Serial.print(color1); Serial.print(F(" b2="));Serial.print(color2_border);
          Serial.print(F(" c2="));Serial.println(color2);Serial.print(F(">order_next_clock_picture MINUTE lmp: "));
   #endif
   
   for (int i=0;i<11;i++) {  // Iterate over sequence of minute ring lamps */
    lamp= i!=9 ? i+11 : 10; // maps to lamp index 11...19,10 
    if(currentMinute<59) {  // in minute mode insert lamp 21 as 6th, equals to sequence 11..15,20,17...19,10
      if(i>5) { lamp= i!=10 ? i+10 : 10;}
       else { if(i==5) lamp=20; }     
    } else if(i>9) {       // in second mode after 10 lamps switch of lamp 20 and bail out 
        g_picture_lamp[20].setTargetColor_int(0,0,0);
        g_picture_lamp[20].startTransition(transitionTime);
        break; 
    }
        
    if(i<color1_border) colorIndex=color0;
      else if(i<color2_border) colorIndex=color1;
        else colorIndex=color2;
    g_picture_lamp[lamp].setTargetColor_int(g_color_palette[colorIndex][iRED],g_color_palette[colorIndex][iGREEN],g_color_palette[colorIndex][iBLUE]);
    g_picture_lamp[lamp].startTransition(transitionTime);
    #ifdef TRACE_CLOCK
          Serial.print(F("/"));
          Serial.print(colorIndex);
     #endif   
   } // Iteration over lamps
   
   #ifdef TRACE_CLOCK
      Serial.println(F(" "));
   #endif


   /* SECONDS RING */  /* Lamps 4-5,1-3 */

   // Black as default
   color0=0;
   color1=0; 
   color1_border=0;
   
   if(currentMinute<59) {
       if(currentHour>=23||millis()-g_clock_sync_time<CLOCK_SECOND_LIVE_INTERVAL) { // Fast Animation
           segment=currentSecond/5;
           color0=GET_SECOND_12_COLOR(segment+1); /* newset color */
           color1=GET_SECOND_12_COLOR(segment); 
           color1_border=currentSecond%5;
       } else {
        if(currentHour>=18) {   // Slow Animation
           colorIndex=GET_SECOND_6_COLOR(currentSecond/10); 
           g_picture_lamp[23].setTargetColor_int(g_color_palette[colorIndex][iRED],g_color_palette[colorIndex][iGREEN],g_color_palette[colorIndex][iBLUE]);
           g_picture_lamp[23].startTransition(transitionTime);
           #ifdef TRACE_CLOCK
                 Serial.print(F(">order_next_clock_picture SECOND: >"));Serial.print(colorIndex);Serial.println(F("<"));
           #endif  
        }
       }
   } else  {   // seconds are on the minute ring
         g_picture_lamp[23].setTargetColor_int(0,0,0);   // Switch off the center
         g_picture_lamp[23].startTransition(transitionTime);
   }

   #ifdef TRACE_CLOCK
          Serial.print(F(">order_next_clock_picture SECOND: "));
          Serial.print(currentSecond);
          Serial.print(F(" c0="));Serial.print(color0); Serial.print(F(" b1="));Serial.print(color1_border);
          Serial.print(F(" c1="));Serial.println(color1); 
          Serial.print(F(">order_next_clock_picture SECOND lmp: "));
   #endif
   for (int i=0;i<5;i++) {  /* Iterate over seconds ring */
    lamp=i<2?i+3:i-2;
    if(i<color1_border) colorIndex=color0;
      else  colorIndex=color1;
    g_picture_lamp[lamp].setTargetColor_int(g_color_palette[colorIndex][iRED],g_color_palette[colorIndex][iGREEN],g_color_palette[colorIndex][iBLUE]);
    g_picture_lamp[lamp].startTransition(transitionTime);
    #ifdef TRACE_CLOCK
          Serial.print(F("/"));
          Serial.print(colorIndex);
     #endif   
   } // Iteration over lamps
   #ifdef TRACE_CLOCK
      Serial.println(F(" "));
   #endif   
   
   
} //order_next_clock_picture


