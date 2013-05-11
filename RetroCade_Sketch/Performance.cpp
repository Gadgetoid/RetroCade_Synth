
#include "Performance.h"

#define PREFPANE_COUNT 13

char* settingsPanes[PREFPANE_COUNT] = {
"S. Enabled",
"S. Start",
"S. End",
"S. Channel",
"S. Octave",
"S. Transpose",
"N. Channel",
"N. Octave",
"N. Transpose",
"Octave",
"Transpose",
"SID Ins",
"Patch"
};

    byte Performance::getActiveChannel(byte pitch){
      if (pitch >= performance[currentPerformance].splitStart && pitch <= performance[currentPerformance].splitEnd){
       return   performance[currentPerformance].splitChannel;
      }else
      {
        return   performance[currentPerformance].normalChannel;
      }
    }
    byte Performance::applyPitchTransposeOctave(byte pitch){
      if (pitch >= performance[currentPerformance].splitStart && pitch <= performance[currentPerformance].splitEnd){
       return   pitch + (performance[currentPerformance].splitOctave * 12) + performance[currentPerformance].splitTranspose;
      }else{
       return   pitch + (performance[currentPerformance].normalOctave * 12) + performance[currentPerformance].normalTranspose;
      }
    }

void Performance::joystickUp() {
  if( editMode == 0 )
  {
    settingPrevious();
  }else if( editMode == 1 && currentSetting == 12)
  {
    patchPrevious();
  }else if( editMode == 1)
  {
    
  switch(currentSetting){
    case 0:
        performance[currentPerformance].splitEnabled = 1;
    break;
    case 1:
      performance[currentPerformance].splitStart += 1;
    break;
    case 2:
      performance[currentPerformance].splitEnd += 1;
    break;
    case 3:
      performance[currentPerformance].splitChannel += 1;
    break;
    case 4:
      performance[currentPerformance].splitOctave += 1;
    break;
    case 5:
      performance[currentPerformance].splitTranspose += 1;
    break;
    case 6:
      performance[currentPerformance].normalChannel += 1;
    break;
    case 7:
      performance[currentPerformance].normalOctave += 1;
    break;
    case 8:
      performance[currentPerformance].normalTranspose += 1;
    break;
    case 9:
      performance[currentPerformance].octave += 1;
    break;
    case 10:
      performance[currentPerformance].transpose += 1;
    break;
    case 11:
      performance[currentPerformance].sidInstrument += 1;
    break;
  }
  }

}

void Performance::joystickDown() {
  if( editMode == 0 )
  {
    settingNext();
  }else if( editMode == 1 && currentSetting == 12)
  {
    patchNext();
  }else if( editMode == 1)
  {
    
  switch(currentSetting){
    case 0:
        performance[currentPerformance].splitEnabled = 0;
    break;
    case 1:
      performance[currentPerformance].splitStart -= 1;
    break;
    case 2:
      performance[currentPerformance].splitEnd -= 1;
    break;
    case 3:
      performance[currentPerformance].splitChannel -= 1;
    break;
    case 4:
      performance[currentPerformance].splitOctave -= 1;
    break;
    case 5:
      performance[currentPerformance].splitTranspose -= 1;
    break;
    case 6:
      performance[currentPerformance].normalChannel -= 1;
    break;
    case 7:
      performance[currentPerformance].normalOctave -= 1;
    break;
    case 8:
      performance[currentPerformance].normalTranspose -= 1;
    break;
    case 9:
      performance[currentPerformance].octave -= 1;
    break;
    case 10:
      performance[currentPerformance].transpose -= 1;
    break;
    case 11:
      performance[currentPerformance].sidInstrument -= 1;
    break;
  }
  }
}

void Performance::joystickSelect(){
  if( editMode == 1 ) {
   editMode = 0; 
  }else{
    editMode = 1;
  }
}

void Performance::patchNext(){
  if( currentPerformance < PERFORMANCE_COUNT )
  {
    currentPerformance += 1;
  }
}

void Performance::patchPrevious(){
  if( currentPerformance > 1 )
  {
    currentPerformance -= 1;
  }
}

void Performance::settingNext(){
  if (editMode==1)
    return;
  currentSetting += 1;
  if( currentSetting >= PREFPANE_COUNT )
  {
    currentSetting = 0;
  }
}

void Performance::settingPrevious(){
  if (editMode==1)
    return;
  currentSetting -= 1;  
  if( currentSetting < 0  )
  {
    currentSetting = PREFPANE_COUNT - 1;
  }
}


char* Performance::getHeading(){
  if( editMode == 1  ) {
    
    if( currentSetting == 12 ){
      return "Select Perf: ";
    }
    
    
    return "Editing: ";
  }
  
  return performance[currentPerformance].name;
}

char* Performance::getSetting(){
  if( currentSetting == 12 )
  {
    return performance[currentPerformance].name;
  }
  return settingsPanes[currentSetting];
}
  
int Performance::getSettingValue(){
  /*
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
*/
  int settingValue = 0;
  switch(currentSetting){
    case 0:
      settingValue = performance[currentPerformance].splitEnabled;
    break;
    case 1:
      settingValue = performance[currentPerformance].splitStart;
    break;
    case 2:
      settingValue = performance[currentPerformance].splitEnd;
    break;
    case 3:
      settingValue = performance[currentPerformance].splitChannel;
    break;
    case 4:
      settingValue = performance[currentPerformance].splitOctave;
    break;
    case 5:
      settingValue = performance[currentPerformance].splitTranspose;
    break;
    case 6:
      settingValue = performance[currentPerformance].normalChannel;
    break;
    case 7:
      settingValue = performance[currentPerformance].normalOctave;
    break;
    case 8:
      settingValue = performance[currentPerformance].normalTranspose;
    break;
    case 9:
      settingValue = performance[currentPerformance].octave;
    break;
    case 10:
      settingValue = performance[currentPerformance].transpose;
    break;
    case 11:
      settingValue = performance[currentPerformance].sidInstrument;
  }
  
  return settingValue;
}

char* Performance::getPerformanceName()
{
  return performance[currentPerformance].name;
}


  /*
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
*/
Performance::Performance()
{
  currentPerformance = 0;
  currentSetting = 12;
  editMode = 0;
  performance[0] = (PerformancePatch) {"Split",1,0,70,2,-1,0,0,0,0,0,0,0};
  performance[1] = (PerformancePatch) {"Bass n Treble",1,0,70,0,-2,0,0,0,0,0,0,0};
  performance[2] = (PerformancePatch) {"B",1,0,70,2,-1,0,0,0,0,0,0,0};
  performance[2] = (PerformancePatch) {"B",1,0,70,2,-1,0,0,0,0,0,0,0};
  /*
  performance[2] = (PerformancePatch) {"User Perf 1",1,0,63,1,2,4};
  performance[2] = (PerformancePatch) {"User Perf 2",1,0,47,1,2,4};
  performance[2] = (PerformancePatch) {"User Perf 3",1,0,47,1,2,4};
  performance[2] = (PerformancePatch) {"User Perf 4",1,0,47,1,2,4};
  */
}
