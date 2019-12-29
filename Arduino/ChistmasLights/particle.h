#ifndef particle_h
#define particle_h
#include "Arduino.h"
#include "picturelamp.h"
#include "mainSettings.h"

class Particle
{
  public:
        Particle(void);
        
        /* operations */
        void init(PictureLamp *pictureLamp);

        /* Setup the particle */
        void start(byte *pathArray,int time_scale,float red, float green, float blue, byte pathFadeStartIndex, float pathFadeRate);

        /* calculate progress and update lamps */
        void process();

        /* stop the particle */
        void end(void) {m_pathIndex=255;};

        /* information */
        bool inProgress(void) {return m_pathIndex!=255;}

  protected:
        PictureLamp *m_pictureLamp=0;
        float m_color_red,m_color_green,m_color_blue;

        byte m_pathFadeStartIndex;
        float m_pathFadeRate;
        byte *m_pathArray;
        byte m_pathIndex=255;
        unsigned long m_previousStepMillis=0;
        int m_stepDuration=0;
        int m_time_scale=1;
};

#endif
