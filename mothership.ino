//#define BUTTON_PIN  A0
#define ENABLE_PIXELS

#define MIDI_NOTE_START 60

#include <MIDI.h>

#include "pixels.h"
//#include "sequences.h"

#ifdef BUTTON_PIN
#include <DebounceEvent.h>
#endif

// MIDI library setup

MIDI_CREATE_DEFAULT_INSTANCE();


bool playing = false;

byte current_scene = 0;

#ifdef BUTTON_PIN
void handleButtonPressed(uint8_t pin, uint8_t event, uint8_t count, uint16_t length) {
    if (event == EVENT_RELEASED) {
      playing = !playing; // toggle playing status
    }
}

DebounceEvent button = DebounceEvent(BUTTON_PIN, handleButtonPressed, BUTTON_PUSHBUTTON | BUTTON_DEFAULT_HIGH | BUTTON_SET_PULLUP);
#endif


void wait(long time) {
  unsigned long mils = millis();
  while (millis()-mils < time) {
    MIDI.read();
  }
}


#include "sequences.h"


void handleNoteOn(byte channel, byte pitch, byte velocity) {
  static int notes_received = 0;
  /*byte p = pitch;
  byte v = velocity;

  if (velocity==0) 
    handleNoteOff(channel, pitch, velocity);

  activeNotes++;
  if (!process_triggers_for_pitch(pitch, velocity, true)) {
    p = convert_drum_pitch(pitch);
    if (p>=MUSO_NOTE_MINIMUM && p<=MUSO_NOTE_MAXIMUM) {
      trigger_status[p - MUSO_NOTE_MINIMUM] = velocity>0; // TRIGGER_IS_ON;
    }
    
    MIDI.sendNoteOn(p, v, MUSO_GATE_CHANNEL); //CHANNEL_DRUMS);  // output channel that the midimuso expects its triggers on
    last_input_at = millis();
  }*/
  // check if the incoming message is a trigger for a sequence, if so then 
  //  switch to it, or stop it if its already running

  if (velocity==0)
    return;  // ignore if this is actually a note off

  //pitch = velocity;
  
  if (pitch>=MIDI_NOTE_START) {
    pitch -= MIDI_NOTE_START;  // so we can use C5 upwards as 0/1/2 are a bit fiddly...
    //wait(1000);
    if (//true || 
        //channel==10 && // use channel 10 (drums) to trigger the preprogrammed sequences
        pitch < SCENES_COUNT  // and is a valid sequence number
        ) {
          if (!playing) {
            current_scene = pitch;
            playing = true;
          } else {
            playing = false;  // stop if already ongoing
          }
    }
  } else {
    // turn the light on
    notes_received++;
    //p_fill(CHSV(velocity*2, velocity, notes_received%255), notes_received++, 2); //channel + (pitch%12), 1); 
    if (velocity==255) 
      p_fill(CRGB::White, pitch%STRIP_NUM_PIXELS, 1); //++, 2); //channel + (pitch%12), 1); 
    else
      p_fill(CHSV(velocity*2, 255, 255), pitch%STRIP_NUM_PIXELS, 1); //++, 2); //channel + (pitch%12), 1); 
    //if (!playing) {
      p_show();
    //}
  }
}

void handleNoteOff(byte channel, byte pitch, byte velocity) {
  /*byte p = pitch;
  byte v = velocity;

  activeNotes--;
  if (!process_triggers_for_pitch(pitch, velocity, false)) {
    p = convert_drum_pitch(pitch);
    if (p>=MUSO_NOTE_MINIMUM && p<=MUSO_NOTE_MAXIMUM) {
      trigger_status[p - MUSO_NOTE_MINIMUM] = TRIGGER_IS_OFF;
    }
    
    MIDI.sendNoteOff(p, v, MUSO_GATE_CHANNEL);   // hardcoded channel 16 for midimuso
  }

  last_input_at = millis();*/

  if (pitch>=MIDI_NOTE_START) {
    /*pitch -= MIDI_NOTE_START;  // so we can use C5 upwards as 0/1/2 are a bit fiddly...
    //wait(1000);
    if (//true || 
        //channel==10 && // use channel 10 (drums) to trigger the preprogrammed sequences
        pitch < SCENES_COUNT  // and is a valid sequence number
        ) {
          if (!playing) {
            current_scene = pitch;
            playing = true;
          } else {
            playing = false;  // stop if already ongoing
          }
    }*/
  } else {
    // turn the light off
    //notes_received++;
    //p_fill(CHSV(velocity*2, velocity, notes_received%255), notes_received++, 2); //channel + (pitch%12), 1); 
    p_fill(CRGB::Black, pitch%STRIP_NUM_PIXELS, 1); //++, 2); //channel + (pitch%12), 1); 
    //if (!playing) {
      p_show();
    //}
  }
}


void setup() {
  //Serial.begin(38400);
  //Serial.println("resetting");
  
  // put your setup code here, to run once:

#ifdef ENABLE_PIXELS
  setup_pixels();
#endif

  // Initiate MIDI communications, listen to all channels
  MIDI.begin(MIDI_CHANNEL_OMNI);

  MIDI.turnThruOff();

  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  /*MIDI.setHandleControlChange(handleControlChange);

  MIDI.setHandleStop(handleStop);
  MIDI.setHandleStart(handleStart);
  MIDI.setHandleStart(handleContinue);

  MIDI.setHandleClock(handleClock);

  MIDI.setHandleSongPosition(handleSongPosition);*/

}

void loop() {
  // put your main code here, to run repeatedly:

  MIDI.read();

  //playing = true;
  if (playing) {
    //Serial.println("playing!");
    //wait(1000);
    //delay(1000);
    doScene[current_scene]();
    //demo_sequence();
    playing = false;  // stop playing after one iteration of sequence
    p_clear();
    p_show();
  } else {
    //Serial.println("looping..");
  }

}
