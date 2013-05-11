#include <inttypes.h> 
#include <zpuino-types.h>
#include <zpuino.h>
#include "Arduino.h"

#define PERFORMANCE_COUNT 8

struct PerformancePatch {
  char* name;
  
  int splitEnabled;
  int splitStart; // 0 - 127
  int splitEnd; // 9 - 127
  
  int splitChannel;
  int splitOctave;
  int splitTranspose;
  
  int normalChannel;
  int normalOctave;
  int normalTranspose;
  
  int octave;
  int transpose;
  int sidInstrument;
};

class Performance
{
  public:
    Performance();
    
    void joystickUp();
    void joystickDown();
    void joystickSelect();
    
    char* getSetting();
    int getSettingValue();
    char* getHeading();
    
    char* getPerformanceName();
    
    byte getActiveChannel(byte pitch);
    byte applyPitchTransposeOctave(byte pitch);
  private:
    int currentPerformance;
    int currentSetting;
    int editMode;
    
    void toggleEditMode();
    
    void patchNext();
    void patchPrevious();
    
    void settingNext();
    void settingPrevious();
    
    PerformancePatch performance[8];
};
