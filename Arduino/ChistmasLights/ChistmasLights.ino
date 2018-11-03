
#define NUMLIGHTS 8

int delayval = 100; // delay for half a second

void setup() {
 output_setup();
}

void loop() {
  for(int i=0;i<NUMLIGHTS;i++){
    output_setLightColor(i,0,150,0); // Moderately bright green color.
    output_show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
   for(int i=0;i<NUMLIGHTS;i++){
    output_setLightColor(i,150,0,0); // Moderately bright red color.
    output_show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).
  }
   for(int i=0;i<NUMLIGHTS;i++){
     output_setLightColor(i, 0,0,150); // Moderately bright blue color.
     output_show(); // This sends the updated pixel color to the hardware.
     delay(delayval); // Delay for a period of time (in milliseconds).
  } 
}
