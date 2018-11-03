#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#ifdef TRACE_ON
#define TRACE_OUTPUT
#define TRACE_OUTPUT_HIGH
#endif


#define CHAIN_PIN_1 12
#define CHAIN_PIN_2 11
#define CHAIN_PIN_3 10
#define NUMCHAINS 3
#define NUMPIXELS 8


Adafruit_NeoPixel light_chain[3]={ Adafruit_NeoPixel(NUMPIXELS, CHAIN_PIN_1, NEO_RGB + NEO_KHZ800),
                                    Adafruit_NeoPixel(NUMPIXELS, CHAIN_PIN_2, NEO_RGB + NEO_KHZ800),
                                    Adafruit_NeoPixel(NUMPIXELS, CHAIN_PIN_3, NEO_RGB + NEO_KHZ800)};

/* This map translates the picture light index in to the physical light index 
   an must be adapted to the current physical setup
 */
byte light_index_map[24]={ 0, 1, 2, 3, 4, 5, 6, 7
                         , 8, 9,10,11,12,13,14,15
                         ,16,17,18,19,20,21,22,23 };

void output_setup()
{
  for (int i=0;i<NUMCHAINS;i++) 
  {
    light_chain[i].begin();                                  
  }
}

/* output_setLightColor takes RGB values, from 0,0,0 up to 255,255,255 */
void output_setLightColor(byte index,int red,int green, int blue)
{
  byte light_index=light_index_map[index];
  byte chain_index=light_index/NUMPIXELS;
  byte pixel_index=light_index%NUMPIXELS;
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




