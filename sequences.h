#define SCENES_COUNT  2

void demo_sequence() {
  for(int i = 0; i < STRIP_NUM_PIXELS; i++) {
    // First slide the led in one direction
    static uint8_t hue = 0;
    p_fill( CHSV(hue++, 255, 255), STRIP_NUM_PIXELS-i-1, 1);
    // Show the leds
    p_show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    wait(100);
    if (!playing) return; // return early if told to stop
  }
  return;
}

void white_strips() {
  //Serial.println("demo_sequence...");
  // do demo thing here
  int duration = 10000;


#define REPEAT 20  // number of times for animation to repeat
#define SEGMENT_WIDTH 8 // 10  // number of leds to use per repeating segment
#define NUM_SEGMENTS  (STRIP_NUM_PIXELS/SEGMENT_WIDTH)
//#define SEGMENT_WIDTH (STRIP_NUM_PIXELS/NUM_SEGMENTS)
#define FRAME_DELAY 50

  for (int count = 0 ; count < REPEAT ; count++) {  // total number of frames
    /*for (int i = 0 ; i < STRIP_NUM_PIXELS ; i++) {
      //leds[i] = CRGB::Red;
      p_fill(CRGB::Green, i, 1);
      //leds[i] = CRGB::Green;
      p_show();
      wait(10);
      /*leds[i] = CRGB::Blue;
      p_show();
      wait(10);
      p_clear();
    }*/
    //return;
    for (int anim_position = 0 ; anim_position < SEGMENT_WIDTH ; anim_position++) { // all of frame processed within this loop
      p_clear();  // clear frame ready for drawing

      for (int seg = 0 ; seg < NUM_SEGMENTS ; seg++) {
        p_fill(CRGB(255,255,255),(seg * SEGMENT_WIDTH) + anim_position, 2);
        p_fill(CRGB(255,255,255),(seg * SEGMENT_WIDTH) + (SEGMENT_WIDTH-anim_position-1), 2);
      }

      p_show(); // update the strip
      
      wait(FRAME_DELAY); // wait so the leds can be seen
      if (!playing) return; // return early if told to stop
    }
  }
}



void (*doScene[SCENES_COUNT])() {
  white_strips,
  demo_sequence
};
