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
    int currentPerformance;
    int currentSetting;
    char* getPerformanceName();
    void next();
    void previous();
    const char* settingNext();
    const char* settingPrevious();
  private:
    PerformancePatch performance[8];
};
