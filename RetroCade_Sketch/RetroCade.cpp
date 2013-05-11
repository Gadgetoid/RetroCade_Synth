/*!
 *  @file		RetroCade.h
 *  Project		RetroCade Library
 *	@brief		RetroCade Library for the ZPUino
 *	Version		1.0
 *  @author		Jack Gassett 
 *	@date		9/11/12
 *  License		GPL
 */
#include "RetroCade.h"
#include "LiquidCrystal.h"
#include "binary.h"
//#include "spaceinvaders.h"

#define FREQ 17000          //Freq for modplayer 
#define TIMEOUTMAX 7000    //Timeout for joystick 
#define INVADERSTIMERMAX 2000    //Timeout for LCD 

//Defines for lcdMode
#define WELCOME 0
#define PERFORMANCE 1
#define CHANNEL 2
#define INSTRUMENT 3
#define MODFILE 4
#define SMALLFSMODFILE 5
#define YMFILE 6
#define SMALLFSYMFILE 7
#define ABOUT 8
#define LCDMODEMAX 9

char smallfsModTrack[] = "track1.mod";
//char smallfsYmTrack[] = "track1.mod";

#define SIDINSTRUMENTS 16          
/*char sidInstrumentName[SIDINSTRUMENTS][20]=        //TODO: Goofy way to do this, change to struct or function when strcpy works.
 { "Calliope",                                              
 "Drum",
 "Accordian", 
 "Guitar",           
 "Harpsicord", 
 "Organ", 
 "Trumpet", 
 "Xylophone",           
 "Flute" };  */

LiquidCrystal lcd(WING_B_10, WING_B_9, WING_B_8, WING_B_7, WING_B_6, WING_B_5, WING_B_4);

void RETROCADE::setupMegaWing()
{   
  SIDVoiceSetting = 0;
  SIDVoiceEditMode = 0;
  SIDVoiceSettingEditMode = 0;
  activeChannel = 0;
  activeInstrument = 0;
  smallfsActiveTrack = 0;
  timeout = TIMEOUTMAX;
  smallFs = false;
  sdFs = false;
  invadersCurLoc = 0;
  invadersCurSeg = 1;
  invadersTimer = INVADERSTIMERMAX;
  lcdMode = WELCOME;
  buttonPressed = None;


  SIGMADELTACTL=0x3;
  //Move the audio output to the appropriate pins on the Papilio Hardware
  pinMode(AUDIO_J1_L,OUTPUT);
  digitalWrite(AUDIO_J1_L,HIGH);
  //outputPinForFunction(AUDIO_J1_L, IOPIN_SIGMADELTA0);
  outputPinForFunction(AUDIO_J1_L, 8);
  pinModePPS(AUDIO_J1_L, HIGH);

  pinMode(AUDIO_J1_R,OUTPUT);
  digitalWrite(AUDIO_J1_R,HIGH);
  outputPinForFunction(AUDIO_J1_R, 8);
  //outputPinForFunction(AUDIO_J1_R, IOPIN_SIGMADELTA1);
  pinModePPS(AUDIO_J1_R, HIGH);

  pinMode(AUDIO_J2_L,OUTPUT);
  digitalWrite(AUDIO_J2_L,HIGH);
  outputPinForFunction(AUDIO_J2_L, 8);
  pinModePPS(AUDIO_J2_L, HIGH);

  pinMode(AUDIO_J2_R,OUTPUT);
  digitalWrite(AUDIO_J2_R,HIGH);
  outputPinForFunction(AUDIO_J2_R, 8);
  pinModePPS(AUDIO_J2_R, HIGH);

  //Move the second serial port pin to where we need it, this is for MIDI input.
  pinMode(SERIAL1RXPIN,INPUT);
  inputPinForFunction(SERIAL1RXPIN, 1);
  pinMode(SERIAL1TXPIN,OUTPUT);
  //digitalWrite(SERIAL1TXPIN,HIGH);
  outputPinForFunction(SERIAL1TXPIN, 6);
  pinModePPS(SERIAL1TXPIN, HIGH);

  //Start SmallFS
  if (SmallFS.begin()<0) {
    Serial.println("No SmalLFS found.");
  }
  else{
    Serial.println("SmallFS Started.");
    smallFs = true; 
  }

  //Setup SD Card
  initSD();

  //Setup Joystick
  pinMode(JSELECT, INPUT); 
  pinMode(JUP, INPUT); 
  pinMode(JDOWN, INPUT); 
  pinMode(JLEFT, INPUT); 
  pinMode(JRIGHT, INPUT);  

  //Setup LCD
  pinMode(WING_C_14, OUTPUT);     //Set contrast to GND
  digitalWrite(WING_C_14, LOW);   //Set contrast to GND    
  // set up the LCD's number of columns and rows:
  lcd.begin(16,2);
  // clear the LCD screen:
  lcd.clear();

  //Setup timer for YM and mod players
  TMR0CTL = 0;
  TMR0CNT = 0;
  TMR0CMP = ((CLK_FREQ/2) / FREQ )- 1;
  TMR0CTL = _BV(TCTLENA)|_BV(TCTLCCM)|_BV(TCTLDIR)|
    _BV(TCTLCP0) | _BV(TCTLIEN);
  INTRMASK = BIT(INTRLINE_TIMER0); // Enable Timer0 interrupt
  INTRCTL=1;    

}

void RETROCADE::initSD()
{
  int i;
  USPICTL=BIT(SPICP1)|BIT(SPICPOL)|BIT(SPISRE)|BIT(SPIEN)|BIT(SPIBLOCK);
  outputPinForFunction( SDIPIN, IOPIN_USPI_MOSI );
  pinModePPS(SDIPIN,HIGH);
  pinMode(SDIPIN,OUTPUT);

  outputPinForFunction( SCKPIN, IOPIN_USPI_SCK);
  pinModePPS(SCKPIN,HIGH);
  pinMode(SCKPIN,OUTPUT);

  pinModePPS(CSPIN,LOW);
  pinMode(CSPIN,OUTPUT);

  inputPinForFunction( SDOPIN, IOPIN_USPI_MISO );
  pinMode(SDOPIN,INPUT);    

  Serial.println("Starting SD Card");

  digitalWrite(CSPIN,HIGH);

  for (i=0;i<51200;i++)
    USPIDATA=0xff;

  digitalWrite(CSPIN,LOW);

  for (i=0;i<51200;i++)
    USPIDATA=0xff;  

  if (!SD.begin(CSPIN)) {
    Serial.println("init failed!");
    Serial.println(SD.errorCode());
    sdFs = false;
  } 
  else {
    Serial.println("done.");
    sdFs = true;
  }  
  root = SD.open("/");
}

void RETROCADE::setTimeout()
{
  if (timeout!=0)
    timeout--;  
  if (invadersTimer!=0)
    invadersTimer--;      
}

byte RETROCADE::getActiveChannel()
{
  return activeChannel;
}

byte RETROCADE::getActiveChannel(byte pitch)
{
  return performance.getActiveChannel(pitch);
}

byte RETROCADE::applyPitchTransposeOctave(byte pitch){
  return performance.applyPitchTransposeOctave(pitch);
}

void RETROCADE::handleJoystick()
{ 
  if (timeout==0) {
    if (!digitalRead(JUP)) {
      timeout = TIMEOUTMAX;
      buttonPressed = Up;
    } 
    else if (!digitalRead(JDOWN)) {
      timeout = TIMEOUTMAX;
      buttonPressed = Down;
    } 
    else if (!digitalRead(JRIGHT)) {
      if (lcdMode<LCDMODEMAX)
        lcdMode++;
      timeout = TIMEOUTMAX;
      buttonPressed = Right;
    } 
    else if (!digitalRead(JLEFT)) {
      if (lcdMode!=0)
        lcdMode--;
      timeout = TIMEOUTMAX; 
      buttonPressed = Left;
    } 
    else if (!digitalRead(JSELECT)) {
      timeout = TIMEOUTMAX; 
      buttonPressed = Select;      
    }         
  }  

  if (buttonPressed < 5) {
    switch (lcdMode) {
    case WELCOME:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("RetroCade Synth");        
      break;
    case PERFORMANCE:
    
      char* setting;    
      int settingValue;
      char* heading;
      
      if (buttonPressed == Up) {
        performance.joystickUp();
      }
      if (buttonPressed == Down) {
        performance.joystickDown();
      }
      if (buttonPressed == Select) {
        performance.joystickSelect();
      }
      
      setting = performance.getSetting();
      settingValue = performance.getSettingValue();
      heading = performance.getHeading();
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(heading);
      lcd.setCursor(0,1);
      lcd.print(setting);
      lcd.print(": ");
      lcd.print(settingValue);    
      
      break;
    case CHANNEL:
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Channel");
      if (buttonPressed == Up) {
        if (activeChannel<7)
          activeChannel++;  
      }        
      if (buttonPressed == Down) {
        if (activeChannel!=0)
          activeChannel--; 
      }            
      lcd.setCursor(0,1);
      lcd.print(activeChannel);        
      break;
    case INSTRUMENT:
      instrumentJoystick();
      break;
    case MODFILE:
      smallfsActiveTrack = 0;
      modFileJoystick();
      break;
    case SMALLFSMODFILE:
      smallfsModFileJoystick(0);
      break;        
    case YMFILE:
      ymFileJoystick();
      break; 
    case SMALLFSYMFILE:   
      smallfsModFileJoystick(1);
      break;           
    case ABOUT:
      smallfsActiveTrack = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("RetroCade Synth");          
      lcd.setCursor(0,1);
      lcd.print("Version: 1.02");          
      break;        
    default:
      //return;
      break;       
    }   
    buttonPressed = None; 
  }  
}

void RETROCADE::ymFileJoystick() 
{
  smallfsActiveTrack = 0;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("YMD File SD Card");
  if (buttonPressed == Down) {
    printFile("YMD");
  }      
  if (buttonPressed == Up) {
    ymplayer.play(false);
  }            
  if (buttonPressed == Select) {
    //Serial.println("Select Pressed");
    //Serial.println(fileName);
    ymplayer.loadFile(fileName);
    ymplayer.play(true);  
    lcd.setCursor(0,1);   
    lcd.print(fileName);
    lcd.print(" ");
    lcd.print(curFile.size(), DEC);
  }     
}

void RETROCADE::modFileJoystick() 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MOD File SD Card");
  if (buttonPressed == Down) {
    printFile("MOD");
  }      
  if (buttonPressed == Up) {
    modplayer.play(false);
  }            
  if (buttonPressed == Select) {
    //Serial.println("Select Pressed");
    //Serial.println(fileName);
    modplayer.loadFile(fileName);
    modplayer.play(true);  
    lcd.setCursor(0,1);   
    lcd.print(fileName);
    lcd.print(" ");
    lcd.print(curFile.size(), DEC);
  }    
}

void RETROCADE::instrumentJoystick() 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SIDV1 Instrument");
  
 /*
   SIDVoiceSetting
   0 NAME
   1 SID_REG_ATTACK_DECAY.ATTACK = sidInstrument[instrument].envAttack; //[0];
   2 SID_REG_ATTACK_DECAY.DECAY = sidInstrument[instrument].envDecay; //[1];
   3 SID_REG_SUSTAIN_RELEASE.SUSTAIN = sidInstrument[instrument].envSustain; //[2];
   4 SID_REG_SUSTAIN_RELEASE.RELEASE = sidInstrument[instrument].envRelease; //[3];
   5 SID_REG_CONTROLREG.NOISE_WAVE = sidInstrument[instrument].noiseWave; //[4];
   6 SID_REG_CONTROLREG.SQUARE_WAVE = sidInstrument[instrument].squareWave; //[5];
   7 SID_REG_CONTROLREG.SAWTOOTH_WAVE = sidInstrument[instrument].sawtoothWave; //[6];
   8 SID_REG_CONTROLREG.TRIANGLE_WAVE = sidInstrument[instrument].triangleWave; //[7];
   9 PWM
   10 DONE
 */
  
  if( SIDVoiceEditMode == 0)
  {
     if (buttonPressed == Select){
        SIDVoiceEditMode = 1;
        buttonPressed = None;
      }  
    
  if (buttonPressed == Up) {
    if (activeInstrument < (SIDINSTRUMENTS -1)) {
      activeInstrument++;      
      sid.V1.loadInstrument(activeInstrument);  
    }
  }        
  if (buttonPressed == Down) {
    if (activeInstrument!=0) {
      activeInstrument--; 
      sid.V1.loadInstrument(activeInstrument);  
    }
  }            
  lcd.setCursor(0,1);
  //lcd.print(sidInstrumentName[activeInstrument]);

  int instrumentIdx = sid.V1.getCurrentInstrument();

  lcd.print(sid.V1.sidInstrument[instrumentIdx].name);  
  
  }else{ 
   
       if (buttonPressed == Select && SIDVoiceSetting == 10){
        SIDVoiceEditMode = 0;
        SIDVoiceSettingEditMode = 0;
        buttonPressed = None;
      }   
      if (buttonPressed == Select && SIDVoiceSettingEditMode == 1 )
      {
        SIDVoiceSettingEditMode = 0;
        sid.V1.reloadCurrentInstrument();
        buttonPressed = None;
      }
         
      if (buttonPressed == Select && SIDVoiceSettingEditMode == 0 )
      {
        SIDVoiceSettingEditMode = 1;
        //buttonPressed = None;
      }     if (buttonPressed == Up && SIDVoiceSettingEditMode == 0) {
                SIDVoiceSetting -= 1;
               if(SIDVoiceSetting < 0)
              {
                SIDVoiceSetting = 10;
              }
              
              }
              if (buttonPressed == Down && SIDVoiceSettingEditMode == 0) {
                SIDVoiceSetting += 1;
               if(SIDVoiceSetting > 10)
              {
                SIDVoiceSetting = 0;
              }
              }
              
       int instrumentIdx = sid.V1.getCurrentInstrument();
       int displayVal;
       char* displayName;
       displayVal = -1;
       displayName = "Err";
       
       switch(SIDVoiceSetting)
       {
         case 0: // name
            displayName = sid.V1.sidInstrument[instrumentIdx].name;
         break;
         case 1: //ATTACK
              if (buttonPressed == Up && SIDVoiceSettingEditMode == 1) {
                sid.V1.sidInstrument[instrumentIdx].envAttack += 1;
              }
              if (buttonPressed == Down && SIDVoiceSettingEditMode == 1) {
                sid.V1.sidInstrument[instrumentIdx].envAttack -= 1;
              }
              displayName = "Attack";
              displayVal = sid.V1.sidInstrument[instrumentIdx].envAttack;
         break;
         case 2: //DECAY
              if (buttonPressed == Up && SIDVoiceSettingEditMode == 1) {
                sid.V1.sidInstrument[instrumentIdx].envDecay += 1;
              }
              if (buttonPressed == Down && SIDVoiceSettingEditMode == 1) {
                sid.V1.sidInstrument[instrumentIdx].envDecay -= 1;
              }
              displayName = "Decay";
              displayVal = sid.V1.sidInstrument[instrumentIdx].envDecay;
         break;
         case 3: //SUSTAIN
              if (buttonPressed == Up && SIDVoiceSettingEditMode == 1) {
                sid.V1.sidInstrument[instrumentIdx].envSustain += 1;
              }
              if (buttonPressed == Down && SIDVoiceSettingEditMode == 1) {
                sid.V1.sidInstrument[instrumentIdx].envSustain -= 1;
              }
              displayName = "Sustain";
              displayVal = sid.V1.sidInstrument[instrumentIdx].envSustain;
         break;
         case 4: //RELEASE
              if (buttonPressed == Up && SIDVoiceSettingEditMode == 1) {
                sid.V1.sidInstrument[instrumentIdx].envRelease += 1;
              }
              if (buttonPressed == Down && SIDVoiceSettingEditMode == 1) {
                sid.V1.sidInstrument[instrumentIdx].envRelease -= 1;
              }
              displayName = "Release";
              displayVal = sid.V1.sidInstrument[instrumentIdx].envRelease;
         break;
         case 5: //NOISE WAVE
           if (buttonPressed == Select && SIDVoiceSettingEditMode == 1) {
             
                 if(sid.V1.sidInstrument[instrumentIdx].noiseWave==1){
                   sid.V1.sidInstrument[instrumentIdx].noiseWave = 0;
                 }
                 else
                 {
                   sid.V1.sidInstrument[instrumentIdx].noiseWave = 1;
                 }
                
                SIDVoiceSettingEditMode = 0;
                sid.V1.reloadCurrentInstrument();
                buttonPressed = None;
                
              }
              displayName = "Noise W.";
              displayVal = sid.V1.sidInstrument[instrumentIdx].noiseWave;
         break;
         case 6: //SQUARE WAVE
           if (buttonPressed == Select && SIDVoiceSettingEditMode == 1) {
             
                 if(sid.V1.sidInstrument[instrumentIdx].squareWave==1){
                   sid.V1.sidInstrument[instrumentIdx].squareWave = 0;
                 }
                 else
                 {
                   sid.V1.sidInstrument[instrumentIdx].squareWave = 1;
                 }
                
                SIDVoiceSettingEditMode = 0;
                sid.V1.reloadCurrentInstrument();
                buttonPressed = None;
                
              }
              displayName = "Square W.";
              displayVal = sid.V1.sidInstrument[instrumentIdx].squareWave;
         break;
         case 7: //NOISE WAVE
           if (buttonPressed == Select && SIDVoiceSettingEditMode == 1) {
             
                 if(sid.V1.sidInstrument[instrumentIdx].sawtoothWave==1){
                   sid.V1.sidInstrument[instrumentIdx].sawtoothWave = 0;
                 }
                 else
                 {
                   sid.V1.sidInstrument[instrumentIdx].sawtoothWave = 1;
                 }
                
                SIDVoiceSettingEditMode = 0;
                sid.V1.reloadCurrentInstrument();
                buttonPressed = None;
                
              }
              displayName = "Sawtooth W.";
              displayVal = sid.V1.sidInstrument[instrumentIdx].sawtoothWave;
         break;
         case 8: //NOISE WAVE
           if (buttonPressed == Select && SIDVoiceSettingEditMode == 1) {
             
                 if(sid.V1.sidInstrument[instrumentIdx].triangleWave==1){
                   sid.V1.sidInstrument[instrumentIdx].triangleWave = 0;
                 }
                 else
                 {
                   sid.V1.sidInstrument[instrumentIdx].triangleWave = 1;
                 }
                
                SIDVoiceSettingEditMode = 0;
                sid.V1.reloadCurrentInstrument();
                buttonPressed = None;
                
              }
              
              displayName = "Triangle W.";
              displayVal = sid.V1.sidInstrument[instrumentIdx].triangleWave;      
         break;
         case 9: //PWM
             if (buttonPressed == Up && SIDVoiceSettingEditMode == 1) {
                sid.V1.sidInstrument[instrumentIdx].pwm += 10;
              }
              if (buttonPressed == Down && SIDVoiceSettingEditMode == 1) {
                sid.V1.sidInstrument[instrumentIdx].pwm -= 10;
              }
              displayName = "PWM";
              displayVal = sid.V1.sidInstrument[instrumentIdx].pwm;
   
         break;
         
         case 10: // exit
            displayName = "Done";
         break;
       }
       
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Edit SIDV1 Ins.");
            lcd.setCursor(0,1);
            lcd.print(displayName);
            if( displayVal > -1 )
            {
            lcd.print(": ");
            lcd.print(displayVal);
            }
       
       
  }
}

void RETROCADE::smallfsModFileJoystick(byte type) 
{
  //  char smallfsModTrack[] = "track1.mod";
  lcd.clear();
  lcd.setCursor(0,0);
  if (type == 0){
    lcd.print("MOD File SmallFS");
    smallfsModTrack[7] = 0x6d;  //m
    smallfsModTrack[8] = 0x6f;  //o
    smallfsModTrack[9] = 0x64;  //d
  }
  if (type == 1){
    lcd.print("YMD File SmallFS");
    smallfsModTrack[7] = 0x79;  //y
    smallfsModTrack[8] = 0x6d;  //m
    smallfsModTrack[9] = 0x64;  //d
  }
  if (buttonPressed == Down) {          
    if (smallfsActiveTrack<=8) {
      smallfsActiveTrack++;
      smallfsModTrack[5] = (0x30 + smallfsActiveTrack );
    }
    else {
      smallfsActiveTrack=1;
      smallfsModTrack[5] = (0x30 + smallfsActiveTrack );
    }
    lcd.setCursor(0,1); 
    lcd.print(smallfsModTrack);
  }      
  if (buttonPressed == Up) {
    //    if (type == 0)    
    modplayer.play(false);
    //    if (type == 1)
    ymplayer.play(false);  
    lcd.setCursor(0,1);   
    lcd.print("Stop Track");    
  }            
  if (buttonPressed == Select) {
    Serial.println("Select Pressed");
    Serial.println(smallfsModTrack);
    if (type == 0){    
      modplayer.loadFile(smallfsModTrack);
      modplayer.play(true);  
    }
    if (type == 1){
      ymplayer.loadFile(smallfsModTrack);
      ymplayer.play(true);  
    }    
    lcd.setCursor(0,1);   
    lcd.print(smallfsModTrack);
  }    
}

void RETROCADE::printFile(const char* ext) {
  curFile =  root.openNextFile();
  if (! curFile) {
    Serial.println("No File Opened");
    root.rewindDirectory();
    return;
  }
  fileName = curFile.name();
  //Serial.println(fileName);
  if (fileExtension(fileName,ext,3)) {
    lcd.setCursor(0,1);   
    lcd.print(fileName);
    lcd.print(" ");
    lcd.print(curFile.size(), DEC); 
  } 
  else
    printFile(ext);
}

void RETROCADE::printDirectory(File dir, int numTabs) {
  while(true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      //Serial.println("**nomorefiles**");
      break;
    }
    for (uint8_t i=0; i<numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs+1);
    } 
    else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
  }
}

boolean RETROCADE::smallFsActive() {
  return smallFs;
}

boolean RETROCADE::sdFsActive() {
  return sdFs;
}

int RETROCADE::fileExtension(const char* name, const char* extension, size_t length)
{
  //Serial.println(extension);
  const char* ldot = strrchr(name, '.');
  if (ldot != NULL)
  {
    if (length == 0)
      length = strlen(extension);
    return strncmp(ldot + 1, extension, length) == 0;
  }
  return 0;
}
#ifdef SPACEINV
void RETROCADE::spaceInvadersLCD(){
  /* 
   This Space Invaders alien crawling along the RetroCade LCD was created by JO3RI and adapted to the RetroCade by Jack Gassett.
   Please take a look at JO3RI's website for more cool hacks!
   
   Demo by JO3RI
   
   http://www.JO3RI.be/arduino/arduino-projects/lcd-16x2-demo
   
   */

  if (invadersTimer == 0 && lcdMode == WELCOME)
  {
    if (invadersCurSeg == 10){
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("RetroCade Synth");
      invadersCurSeg = 1;
      if (invadersCurLoc < 13)
        invadersCurLoc+=2;
      else
        invadersCurLoc = 0;         
    }
    else
      invadersCurSeg++;
    invadersTimer = INVADERSTIMERMAX;
    int a=invadersCurLoc;
    int b=a+1;
    int c=a+2;
    int d=a+3;
    int e=a+4;   
    switch (invadersCurSeg) {  
    case 1:
      lcd.createChar(0, charSP12a);
      lcd.createChar(1, charSP7a);
      lcd.createChar(2, charSP2a);
      lcd.createChar(3, charSP0);
      lcd.setCursor(a,0);
      lcd.write((byte)0);
      lcd.setCursor(b,0);
      lcd.write((byte)1);
      lcd.setCursor(c,0);
      lcd.write((byte)2);
      lcd.setCursor(d,0);
      lcd.write((byte)3);
      lcd.setCursor(e,0);
      lcd.write((byte)3);
      break;
    case 2:
      lcd.createChar(0, charSP0);
      lcd.createChar(1, charSP8b);
      lcd.createChar(2, charSP3b);
      lcd.createChar(3, charSP0);
      lcd.setCursor(a,0);
      lcd.write((byte)0);
      lcd.setCursor(b,0);
      lcd.write((byte)1);
      lcd.setCursor(c,0);
      lcd.write((byte)2);
      lcd.setCursor(d,0);
      lcd.write((byte)3);
      lcd.setCursor(e,0);
      lcd.write((byte)3);
      break;
    case 3:
      lcd.createChar(0, charSP0);
      lcd.createChar(1, charSP9a);
      lcd.createChar(2, charSP4a);
      lcd.setCursor(a,0);
      lcd.write((byte)0);
      lcd.setCursor(b,0);
      lcd.write((byte)1);
      lcd.setCursor(c,0);
      lcd.write((byte)2);
      lcd.setCursor(d,0);
      lcd.write((byte)0);
      lcd.setCursor(e,0);
      lcd.write((byte)0);
      break;
    case 4:
      lcd.createChar(0, charSP0);
      lcd.createChar(1, charSP10b);
      lcd.createChar(2, charSP5b);
      lcd.setCursor(a,0);
      lcd.write((byte)0);
      lcd.setCursor(b,0);
      lcd.write((byte)1);
      lcd.setCursor(c,0);
      lcd.write((byte)2);
      lcd.setCursor(d,0);
      lcd.write((byte)0);
      lcd.setCursor(e,0);
      lcd.write((byte)0);
      break;  
    case 5:
      lcd.createChar(0, charSP0);
      lcd.createChar(1, charSP11a);
      lcd.createChar(2, charSP6a);
      lcd.createChar(3, charSP1a);
      lcd.setCursor(a,0);
      lcd.write((byte)0);
      lcd.setCursor(b,0);
      lcd.write((byte)1);
      lcd.setCursor(c,0);
      lcd.write((byte)2);
      lcd.setCursor(d,0);
      lcd.write((byte)3);
      lcd.setCursor(e,0);
      lcd.write((byte)0);
      break;   
    case 6:
      lcd.createChar(0, charSP0);
      lcd.createChar(1, charSP12b);
      lcd.createChar(2, charSP7b);
      lcd.createChar(3, charSP2b);
      lcd.setCursor(a,0);
      lcd.write((byte)0);
      lcd.setCursor(b,0);
      lcd.write((byte)1);
      lcd.setCursor(c,0);
      lcd.write((byte)2);
      lcd.setCursor(d,0);
      lcd.write((byte)3);
      lcd.setCursor(e,0);
      lcd.write((byte)0);
      break;   
    case 7:
      lcd.createChar(0, charSP0);
      lcd.createChar(1, charSP0);
      lcd.createChar(2, charSP8a);
      lcd.createChar(3, charSP3a);
      lcd.setCursor(a,0);
      lcd.write((byte)0);
      lcd.setCursor(b,0);
      lcd.write((byte)1);
      lcd.setCursor(c,0);
      lcd.write((byte)2);
      lcd.setCursor(d,0);
      lcd.write((byte)3);
      lcd.setCursor(e,0);
      lcd.write((byte)0);
      break;   
    case 8:
      lcd.createChar(0, charSP0);
      lcd.createChar(2, charSP9b);
      lcd.createChar(3, charSP4b);
      lcd.setCursor(a,0);
      lcd.write((byte)0);
      lcd.setCursor(b,0);
      lcd.write((byte)0);
      lcd.setCursor(c,0);
      lcd.write((byte)2);
      lcd.setCursor(d,0);
      lcd.write((byte)3);
      lcd.setCursor(e,0);
      lcd.write((byte)0);
      break;   
    case 9:
      lcd.createChar(0, charSP0);
      lcd.createChar(2, charSP10a);
      lcd.createChar(3, charSP5a);
      lcd.setCursor(a,0);
      lcd.write((byte)0);
      lcd.setCursor(b,0);
      lcd.write((byte)0);
      lcd.setCursor(c,0);
      lcd.write((byte)2);
      lcd.setCursor(d,0);
      lcd.write((byte)3);
      lcd.setCursor(e,0);
      lcd.write((byte)0);
      break;  
    case 10:
      lcd.createChar(0, charSP0);
      lcd.createChar(2, charSP11b);
      lcd.createChar(3, charSP6b);
      lcd.createChar(4, charSP1b);
      lcd.setCursor(a,0);
      lcd.write((byte)0);
      lcd.setCursor(b,0);
      lcd.write((byte)0);
      lcd.setCursor(c,0);
      lcd.write((byte)2);
      lcd.setCursor(d,0);
      lcd.write((byte)3);
      lcd.setCursor(e,0);
      lcd.write((byte)4);
      break;        
    default:
      lcd.print("oops");
      break;  
    }  
  }
}

  #endif 

