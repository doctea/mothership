#include <FastLED.h>

#define STRIP_PIN 2
#define STRIP_NUM_PIXELS 144

CRGB leds[STRIP_NUM_PIXELS];
CRGB leds_last[STRIP_NUM_PIXELS];

void setup_pixels() {
  LEDS.addLeds<NEOPIXEL,STRIP_PIN>(leds,STRIP_NUM_PIXELS);
  LEDS.setBrightness(255);

  FastLED.clear(true);
}

void update_pixel(byte index, CRGB rgb) {
  leds[index] = rgb;
}

void p_fill(CRGB colour, byte position, byte span) {
  for (int i = position ; i < span ; i++) {
    leds[i] = colour;
  }
}

void p_clear() {  
  p_fill(CRGB(0, 0, 0),0,STRIP_NUM_PIXELS);  // probably faster ways to do this
}

// only update strip when necessary
void p_show() {
  bool needs_updating = false;
  for (byte i = 0 ; i < STRIP_NUM_PIXELS ; i++) {
    // check if each pixel has updated
    if (leds[i] != leds_last[i]) {
      needs_updating = true;
      leds_last[i] = leds[i];
      break;
    }
  }
  if (needs_updating) {
    FastLED.show();
  }
}
