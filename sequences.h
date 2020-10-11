#define SCENES_COUNT  1 



void demo_sequence() {
  // do demo thing here
  int duration = 10000;

#define REPEAT 20  // number of times for animation to repeat
#define SEGMENT_WIDTH 10  // number of leds to use per repeating segment
#define NUM_SEGMENTS  8
#define SEGMENT_WIDTH (STRIP_NUM_PIXELS/NUM_SEGMENTS)
#define FRAME_DELAY 50

  for (int count = 0 ; count < REPEAT ; count++) {  // total number of frames
    for (int anim_position = 0 ; anim_position < SEGMENT_WIDTH ; anim_position++) { // all of frame processed within this loop
      p_clear();  // clear frame ready for drawing

      for (int seg = 0 ; seg < STRIP_NUM_PIXELS / SEGMENT_WIDTH ; seg++) {
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
  demo_sequence,
};
