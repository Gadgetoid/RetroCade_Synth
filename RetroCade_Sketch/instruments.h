
#ifndef LIB_INS_H_
#define LIB_INS_H_

#define SIDINSTRUMENTS 16 //  max number of slots for SID instruments

// Struct to store sid instrument details
struct SIDInstrument
{
    const char* name;
    int envAttack;
    int envDecay;
    int envSustain;
    int envRelease;
    int noiseWave;
    int squareWave;
    int sawtoothWave;
    int triangleWave;
    int pwm;
};

   //Calliope
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 0;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 0; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 15;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0;
//  SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 0;
//  SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 1;
   //sid.V1.setInstrument(0,0,15,0,0,0,0,1,0);  //Calliope

   //Drum
//  SID_REG_V2_ATTACK_DECAY.ATTACK = 0;
//  SID_REG_V2_ATTACK_DECAY.DECAY = 5; 
//  SID_REG_V2_SUSTAIN_RELEASE.SUSTAIN = 5;
//  SID_REG_V2_SUSTAIN_RELEASE.RELEASE = 0;
//  SID_REG_V2_CONTROLREG.NOISE_WAVE = 1;
//  SID_REG_V2_CONTROLREG.SQUARE_WAVE = 0;
//  SID_REG_V2_CONTROLREG.SAWTOOTH_WAVE = 0;
//  SID_REG_V2_CONTROLREG.TRIANGLE_WAVE = 0;
//   sid.V1.setInstrument(0,5,5,0,1,0,0,0,0);  //Drum

  //Accordian
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 12;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 0; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 12;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0;
//  SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 1;
//  SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 0;
   //sid.V1.setInstrument(12,0,12,0,0,0,1,0,0);  //Accordian
 
   //Guitar
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 0;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 9; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 2;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 1;
//  SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 1;
//  SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 0;
  //sid.V1.setInstrument(0,9,2,1,0,0,1,0,0);  //Guitar

  
   //Harpsicord
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 0;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 9; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 0;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0;
//  SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 1;
//  SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 0;
//  SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 0; 
//  set_pwm(SID_ADDR_V1_PW_LOW, 512);
//sid.V1.setInstrument(0,9,0,0,0,1,0,0,512);  //Harpsicord
  
   //Organ
//  SID_REG_V3_ATTACK_DECAY.ATTACK = 0;
//  SID_REG_V3_ATTACK_DECAY.DECAY = 9; 
//  SID_REG_V3_SUSTAIN_RELEASE.SUSTAIN = 9;
//  SID_REG_V3_SUSTAIN_RELEASE.RELEASE = 0;
//  SID_REG_V3_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V3_CONTROLREG.SQUARE_WAVE = 1;
//  SID_REG_V3_CONTROLREG.SAWTOOTH_WAVE = 0;
//  SID_REG_V3_CONTROLREG.TRIANGLE_WAVE = 0; 
//  set_pwm(SID_ADDR_V3_PW_LOW, 2048);   
//sid.V1.setInstrument(0,9,9,0,0,1,0,0,2048);  //Organ
  
   //Trumpet
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 8;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 9; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 4;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 1;
//  SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 1;
//  SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 0;
//  SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 0; 
//  set_pwm(SID_ADDR_V1_PW_LOW, 512);
//sid.V1.setInstrument(8,9,4,1,0,1,0,0,512);  //Trumpet

   //Xylophone
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 0;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 9; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 0;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0;
//  SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 0;
//  SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 1;
//sid.V1.setInstrument(0,9,0,0,0,0,0,1,0);  //Xylophone

   //Flute
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 9;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 4; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 4;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0;
//  SID_REG_V1_CONTROLREG.NOISE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SQUARE_WAVE = 0;
//  SID_REG_V1_CONTROLREG.SAWTOOTH_WAVE = 0;
//  SID_REG_V1_CONTROLREG.TRIANGLE_WAVE = 1;
//sid.V1.setInstrument(9,4,4,0,0,0,0,1,0);  //Flute


//sid.V1.setInstrument(0,0,15,0,0,0,0,1,0);  //Calliope
//sid.V1.setInstrument(0,5,5,0,1,0,0,0,0);  //Drum
//sid.V1.setInstrument(12,0,12,0,0,0,1,0,0);  //Accordian
//sid.V1.setInstrument(0,9,2,1,0,0,1,0,0);  //Guitar
//sid.V1.setInstrument(0,9,0,0,0,1,0,0,512);  //Harpsicord
//sid.V1.setInstrument(0,9,9,0,0,1,0,0,2048);  //Organ
//sid.V1.setInstrument(8,9,4,1,0,1,0,0,512);  //Trumpet
//sid.V1.setInstrument(0,9,0,0,0,0,0,1,0);  //Xylophone
//sid.V1.setInstrument(9,4,4,0,0,0,0,1,0);  //Flute  
  
  
  //Set ADSR Attack=10 Decay=8 Sustain=10 Release=9 For Violin 
  //Address 5 = 0xA8 for attack and decay
  //Address 6 = 0xA9 for sustain and release
//  write_data(0x05, 0xA8);
//  write_data(0x06, 0xA9);
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 0x0A;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 0x08; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 0x0A;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0x09;
  
  //Set ADSR Attack=0 Decay=9 Sustain=0 Release=9 For Cymbal 
  //Address 5 = 0xA8 for attack and decay
  //Address 6 = 0xA9 for sustain and release
//  write_data(0x05, 0x09);
//  write_data(0x06, 0x09);  
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 0x0;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 0x09; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 0x00;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0x09;
  
  
  //Set ADSR Attack=0 Decay=0 Sustain=F Release=0 For Organ 
  //Address 5 = 0xA8 for attack and decay
  //Address 6 = 0xA9 for sustain and release
//  SID_REG_V1_ATTACK_DECAY.ATTACK = 0x0;
//  SID_REG_V1_ATTACK_DECAY.DECAY = 0x00; 
//  SID_REG_V1_SUSTAIN_RELEASE.SUSTAIN = 0x00;
//  SID_REG_V1_SUSTAIN_RELEASE.RELEASE = 0x00;
#endif
