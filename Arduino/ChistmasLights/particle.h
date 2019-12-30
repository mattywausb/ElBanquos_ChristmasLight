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

class Particle
{
  public:
        Particle(void);
        
        /* operations */
        void init(PictureLamp *pictureLamp);

        /* Setup the particle */
        void start(byte *pathArray,bool mirror,int time_scale,t_color_hsv color_hsv, byte pathFadeStartIndex, float pathFadeRate);

        /* calculate progress and update lamps */
        void process();

        /* stop the particle */
        void end(void) {m_pathIndex=255;};

        /* information */
        bool inProgress(void) {return m_pathIndex!=255;}

  protected:
        PictureLamp *m_pictureLamp=0;
        t_color_hsv m_color_hsv;

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
