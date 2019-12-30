#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#ifdef TRACE_ON
//#define TRACE_OUTPUT
//#define TRACE_OUTPUT_HIGH
#endif

#define MOCKUP_LIGHTS

#define CHAIN_PIN_1 7
#define CHAIN_PIN_2 8
#define CHAIN_PIN_3 9

 // MOCKUP PIXEL STRIP definition
 
#ifdef MOCKUP_LIGHTS   

#define NUMCHAINS 2
#define NUMPIXELS 12
#define PIXEL_BRIGHTNESS 120 // only 1/2 of amps necessary

Adafruit_NeoPixel light_chain[2]={ Adafruit_NeoPixel(NUMPIXELS, CHAIN_PIN_1, NEO_GRB + NEO_KHZ800)
                                  ,Adafruit_NeoPixel(NUMPIXELS, CHAIN_PIN_2, NEO_GRB + NEO_KHZ800)};

/* This map translates the picture light index in to the physical light index 
   an must be adapted to the current physical setup
 */
#define xx 32

/* Ring and stripe */

byte light_index_map[24]={12,15, 4, 7,18,  // 1-5
                          19, 9,16,13, 2,  // 6-10
                           5, 6,  // 11-12
                           8,10,  // 13-14
                          17,11,  // 15-16
                           0,14,  // 17-18
                           1, 3,  // 19-20
                          xx,xx,xx,xx // 21-24
                          };



/* Ring only 
byte light_index_map[24]={xx, 2, 3, 8, 9,  // 1-5
                          xx,xx,11, 0,xx,  // 6-10
                           5, 6,  // 11-12
                          xx,xx,  // 13-14
                          10,xx,  // 15-16
                          xx, 1,  // 17-18
                          xx,xx,  // 19-20
                          xx, 4, 7,xx // 21-24
                          };
*/                       
                          
#endif

// MAIN PIXEL STRIP definition

#ifndef MOCKUP_LIGHTS  

#define NUMCHAINS 3
#define NUMPIXELS 8
#define PIXEL_BRIGHTNESS 210

Adafruit_NeoPixel light_chain[3]={ Adafruit_NeoPixel(NUMPIXELS, CHAIN_PIN_1, NEO_RGB + NEO_KHZ400),
                                    Adafruit_NeoPixel(NUMPIXELS, CHAIN_PIN_2, NEO_RGB + NEO_KHZ400),
                                    Adafruit_NeoPixel(NUMPIXELS, CHAIN_PIN_3, NEO_RGB + NEO_KHZ400)};

/* This map translates the picture light index in to the physical light index 
   an must be adapted to the current physical setup
 */
byte light_index_map[24]={ 9, 4,11,22,18,  // 1-5
                          14,20,16, 2, 6,  // 6-10
                          13,15,  // 11-12
                          21,19,  // 13-14
                          17, 8,  // 15-16
                           1, 3,  // 17-18
                           5, 7,  // 19-20
                           0,12,23,10  // 21-24
                          };
#endif


void output_setup()
{
  for (int i=0;i<NUMCHAINS;i++) 
  {
    light_chain[i].begin(); 
    light_chain[i].setBrightness(PIXEL_BRIGHTNESS);                                 
  }
}

/* output_setLightColor takes RGB values, from 0,0,0 up to 255,255,255 */
void output_setLightColor(byte index,int red,int green, int blue)
{
  byte light_index=light_index_map[index];
  byte chain_index=light_index/NUMPIXELS;
  byte pixel_index=light_index%NUMPIXELS;
  #ifdef MOCKUP_LIGHTS
     if(chain_index<NUMCHAINS && pixel_index<NUMPIXELS)
  #endif  
  light_chain[chain_index].setPixelColor(pixel_index, light_chain[chain_index].Color(red,green,blue));
}

/* output_setLightColorUnmapped takes RGB values, from 0,0,0 up to 255,255,255 */
void output_setLightColorUnmapped(byte index,int red,int green, int blue)
{
  byte light_index=index;
  byte chain_index=light_index/NUMPIXELS;
  byte pixel_index=light_index%NUMPIXELS;
  #ifdef MOCKUP_LIGHTS
    if(chain_index<NUMCHAINS && pixel_index<NUMPIXELS)
  #endif  
  light_chain[chain_index].setPixelColor(pixel_index, light_chain[chain_index].Color(red,green,blue));
}

void output_show()
{
  #ifdef TRACE_OUTPUT_HIGH
      Serial.println(F(">output_show"));
  #endif
  for (int i=0;i<NUMCHAINS;i++) 
  {
    light_chain[i].show();                                  
  }
}




