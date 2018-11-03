#ifndef PICTURELAMP_h
#define PICTURELAMP_h

class PictureLamp
{
  public:
      PictureLamp();

      /* ---- Operations ---- */
      void updateOutput(byte light_index);
      void setCurrentColor(float red, float green, float blue);
      void setTargetColor(float red, float green, float blue);
      void startTransition(unsigned long duration);
      void endTransition();

      /* ---- State information ---- */
      bool is_in_transition();
      bool is_transition_pending();

  protected:
      unsigned long start_transition_time=0;
      float transition_duration=0.0; /* 0 = not in transition */
      float current_red,current_green,current_blue;
      float target_red,target_green,target_blue;
};



#endif /* End of Header file */
