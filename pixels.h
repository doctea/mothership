#include <FastLED.h>

//#define RGBW_MODE

#ifdef RGBW_MODE
#include "FastLED_RGBW.h"
#endif

#define STRIP_PIN A0
#define STRIP_NUM_PIXELS 128 //144

#ifdef RGBW_MODE
CRGBW leds[STRIP_NUM_PIXELS];
#else
CRGB leds[STRIP_NUM_PIXELS];
#endif
CRGB leds_last[STRIP_NUM_PIXELS];
CRGB *ledsRGB = (CRGB *) &leds[0];

void fadeall() { for(int i = 0; i < STRIP_NUM_PIXELS; i++) { leds[i].nscale8(250); } }

void p_clear();
void p_show();

void setup_pixels() {
  //LEDS.addLeds<WS2812,STRIP_PIN,RGB>(leds,STRIP_NUM_PIXELS);
#ifdef RGBW_MODE
  LEDS.addLeds<WS2812B, STRIP_PIN, RGB>(ledsRGB, getRGBWsize(STRIP_NUM_PIXELS));  //use both the CRGB pointer and the size function "getRGBWsize" in the FastLED_RGBW helper file
#else
  LEDS.addLeds<NEOPIXEL,STRIP_PIN>(leds,STRIP_NUM_PIXELS);
#endif
  LEDS.setBrightness(255);

  FastLED.clear(true);

  // First slide the led in one direction
  static uint8_t hue = 0;

  for(int i = 0; i < STRIP_NUM_PIXELS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
  p_clear();
  p_show();
}

void update_pixel(byte index, CRGB rgb) {
  leds[index] = rgb;
}

void p_fill(CRGB colour, int position, byte span) {
  for (int i = position ; i < constrain(position + span,0,STRIP_NUM_PIXELS) ; i++) {
    leds[i] = colour;
  }
}

void p_clear() {  
  p_fill(CRGB::Black,0,STRIP_NUM_PIXELS);  // probably faster ways to do this
}

// only update strip when necessary
void p_show() {
  bool needs_updating = false;
  for (byte i = 0 ; i < STRIP_NUM_PIXELS ; i++) {
    // check if each pixel has updated
    if (leds[i] != leds_last[i]) {
      needs_updating = true;
      leds_last[i] = leds[i];
      //break;
    }
  }
  if (needs_updating) {
    FastLED.show();
  }
}
