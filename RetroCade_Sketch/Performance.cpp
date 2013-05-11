
#include "Performance.h"

#define PREFPANE_COUNT 12

const char* settingsPanes[PREFPANE_COUNT] = {
"Split Enabled",
"Split Start",
"Split End",
"Split Channel",
"Split Octave",
"Split Transpose",
"Normal Channel",
"Normal Octave",
"Normal Transpose",
"Octave",
"Transpose",
"SID Instrument"
};

void Performance::next(){
  if( currentPerformance < PERFORMANCE_COUNT )
  {
    currentPerformance += 1;
  }
}

void Performance::previous(){
  if( currentPerformance > 1 )
  {
    currentPerformance -= 1;
  }
}

const char* Performance::settingNext(){
  currentSetting += 1;
  if( currentSetting > PREFPANE_COUNT )
  {
    currentSetting = 1;
  }
  return settingsPanes[currentSetting];
}

const char* Performance::settingPrevious(){
  currentSetting -= 1;  
  if( currentSetting < 1  )
  {
    currentSetting = PREFPANE_COUNT;
  }
  return settingsPanes[currentSetting];
}

char* Performance::getPerformanceName()
{
  return performance[currentPerformance].name;
}

Performance::Performance()
{
  currentPerformance = 1;
  currentSetting = 1;
  performance[0] = (PerformancePatch) {"Split",1,0,47,1,2,4};
  performance[1] = (PerformancePatch) {"Bass n Treble",1,0,47,1,2,4};
  performance[2] = (PerformancePatch) {"B",1,0,47,1,2,4};
  performance[2] = (PerformancePatch) {"B",1,0,63,1,2,4};
  
  performance[2] = (PerformancePatch) {"User Perf 1",1,0,63,1,2,4};
  performance[2] = (PerformancePatch) {"User Perf 2",1,0,47,1,2,4};
  performance[2] = (PerformancePatch) {"User Perf 3",1,0,47,1,2,4};
  performance[2] = (PerformancePatch) {"User Perf 4",1,0,47,1,2,4};
}
