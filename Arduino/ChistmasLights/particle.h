#ifndef particle_h
#define particle_h
#include "Arduino.h"
#include "picturelamp.h"
#include "mainSettings.h"

typedef struct {
    float r;       // a fraction between 0 and 1
    float g;       // a fraction between 0 and 1
    float b;       // a fraction between 0 and 1
} t_color_rgb;

typedef struct {
    float h;       // angle in degrees
    float s;       // a fraction between 0 and 1
    float v;       // a fraction between 0 and 1
} t_color_hsv;

/* Particle class 
 * A particle initiates lamps on a defined path with a given color, that fades in a defined rate
 * Relative movement speed and fade rate are defined by the path and can be shiftet with parameters for every particle instance
 * The following parameters are set, to start a particle:
 * pathArray: Array, describing the path to follow (lamp sequence, time it should be lit, fade time for the lamp)
 * pathIndexMax: length of the path for this particle (to shorten the path individually)
 * mirror: boolea to mirror the path at the vertikal axis (allows more variation)
 * time_scale: Factor to modify time for fade and path progress (1 = 15ms
 * color_hsv: Initial color of the particle 
 * pathFadeStartIndex: Index on the path to start fading (255=never)
 * pathFadeRate: Factor to multiply the brightness for the next lamp in path
 */

class Particle
{
  public:
        Particle(void);
        
        /* operations */
        void init(PictureLamp *pictureLamp);

        /* Setup the particle */
        void start(byte *pathArray,byte m_pathIndexMax,bool mirror,int time_scale,t_color_hsv color_hsv, byte pathFadeStartIndex, float pathFadeRate);
        void start(byte *pathArray,bool mirror,int time_scale,t_color_hsv color_hsv) {start(pathArray,255,mirror,time_scale,color_hsv,255,1);};

        /* calculate progress and update lamps */
        void process();

        /* stop the particle */
        void end(void) {m_pathIndex=255;};

        /* information */
        bool inProgress(void) {return m_pathIndex!=255;}

  protected:
        PictureLamp *m_pictureLamp=0;
        t_color_hsv m_color_hsv;

        byte m_pathIndexMax=255; // Up to the final end
        byte m_pathFadeStartIndex;
        float m_pathFadeRate;
        byte *m_pathArray;
        byte m_pathIndex=255;
        unsigned long m_previousStepMillis=0;
        int m_stepDuration=0;
        int m_time_scale=1;
        bool m_mirror=false;

        /* internal color conversion function */
        t_color_rgb get_color_rgb();
};




#endif
