#define BUTTON_PIN  A0
#define ENABLE_PIXELS

#include <MIDI.h>

#include "pixels.h"
//#include "sequences.h"

#ifdef BUTTON_PIN
#include <DebounceEvent.h>
#endif

// MIDI library setup

MIDI_CREATE_DEFAULT_INSTANCE();


bool playing = true;

byte current_scene = 0;


void handleButtonPressed(uint8_t pin, uint8_t event, uint8_t count, uint16_t length) {
    if (event == EVENT_RELEASED) {
      playing = !playing; // toggle playing status
    }
}

#ifdef BUTTON_PIN
DebounceEvent button = DebounceEvent(BUTTON_PIN, handleButtonPressed, BUTTON_PUSHBUTTON | BUTTON_DEFAULT_HIGH | BUTTON_SET_PULLUP);
#endif


void wait(int time) {
  for (int i = 0 ; i < time ; i++) {
    MIDI.read();
    delay(1);
  }
}


#include "sequences.h"


void handleNoteOn(byte channel, byte pitch, byte velocity) {
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

  if (channel==10 // use channel 10 (drums) to trigger the preprogrammed sequences
      && pitch < sizeof (SCENES_COUNT)  // and is a valid sequence number
      )
  {
        if (!playing) {
          current_scene = pitch;
          playing = true;
        } else {
          playing = false;  // stop if already ongoing
        }
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
}


void setup() {
  Serial.begin(9600);
  Serial.println("resetting");
  
  // put your setup code here, to run once:

#ifdef ENABLE_PIXELS
  setup_pixels();
#endif

#ifdef BUTTON_PIN
  //pinMode(BUTTON_PIN, INPUT_PULLUP);
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

  playing = true;
  if (playing) {
    Serial.println("playing!");
    doScene[current_scene]();
    playing = false;  // stop playing after one interation of sequence
    p_clear();
    p_show();
  } else {
    //Serial.println("looping..");
  }

}
