#ifndef PICTURELAMP_h
#define PICTURELAMP_h

enum transition_type_t {TT_NONE,TT_ON,TT_OFF,TT_BLEND};

class PictureLamp
{
  public:
      PictureLamp();

      /* ---- Operations ---- */
      void updateOutput(byte light_index);
      void setCurrentColor(float red, float green, float blue);
      void setTargetColor(float red, float green, float blue);
      void setCurrentColor_int(int red, int green, int blue);
      void setTargetColor_int(int red, int green, int blue);
      void startTransition(unsigned long duration);
      void endTransition();

      /* ---- State information ---- */
      bool is_in_transition();
      bool is_transition_pending();
      transition_type_t getTransitionType() {return transition_type;};

  protected:
      unsigned long start_transition_time=0;
      unsigned long transition_duration=0; /* 0 = not in transition */
      int current_red,current_green,current_blue;
      int target_red,target_green,target_blue;
      transition_type_t transition_type;
};



#endif /* End of Header file */
