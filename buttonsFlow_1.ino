#include <FastLED.h>

#define LED_PIN     8
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    144

#define BRIGHTNESS  66
#define FRAMES_PER_SECOND 10

//-------time params etc
long flowtime = 30000;//120000 = 2 mins
//-------params end

int delayTime = 170;//starting point delay time
int counter = 0;
long t = 0;
int full = 0;

int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;
int allIn = false;

CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;


void setup() {
  Serial.begin(9600);
  delay(3000); // sanity delay

  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(12, OUTPUT);
    
  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  gPal = CRGBPalette16( CRGB::Red, CRGB::Black, CRGB::OrangeRed);
  ///bloodflow();//Start();
  // kaikki ledit mustaksi
  FastLED.clear();
  FastLED.show();
}

void loop()
{
  random16_add_entropy( random());
  buttonState2 = digitalRead(2);
  buttonState3 = digitalRead(3);
  buttonState4 = digitalRead(4);
  buttonState5 = digitalRead(5);

  //mainLoopEffect();
  
  if (buttonState2 == 0) {
    Serial.println(buttonState2);
  } else if (buttonState3 == 0) {

    Serial.println(buttonState3);
  } else if (buttonState4 == 0) {

    Serial.println(buttonState4);
  } else if (buttonState5 == 0) {

    Serial.println(buttonState5);
  } else {
    Serial.println("low");
    }

  if (buttonState2 == 0 && buttonState3 == 0 && buttonState4 == 0 && buttonState5 == 0) {
    Serial.println("all in");
    mainLoopEffect();
  }
  
}

void mainLoopEffect() {
    boolean fillUp = false;
  if (t <= flowtime) {
    counter++;
    if (counter > NUM_LEDS){  
      if (counter % 8) {
        //do nothing because fuck you
      } else {
        //speed up
        delayTime--;
      }
    }
    bubbleAdvance(fillUp);
  } else if (t > flowtime && full < NUM_LEDS) {
    bubbleAdvance(true);
    full++;
  } else {
    delayTime = 80;
    pulse();  
  }
  
  
  delay(delayTime);
  t = t + delayTime;
  //Serial.println(t);
  FastLED.show(); // display this frame
}

void pulse() {
  int b = 0;
  int g = 0;
  for (int i = 255; i > 0; i--) {
    /*g = i-200;
    if (g < 0) {g=0;}
    b=g;*/
    fill_solid( leds, NUM_LEDS, CRGB(i,g,b));
    delay(5);
    FastLED.show();
  }
  for (int i = 0; i < 255; i++) {
    /*g = i-200;
    if (g < 0) {g=0;}
    b=g;*/
    fill_solid( leds, NUM_LEDS, CRGB(i,g,b));
    delay(5);
    FastLED.show();
  }
}

void bubbleAdvance(boolean fillUp)
{
  CRGB color = chooseColor();
  if (fillUp) {
    color = CRGB (255,0,0);
  }
  shiftLeds();
  leds[0].r=color.r;
  leds[0].g=color.g;
  leds[0].b=color.b;
}

CRGB chooseColor()
{
  CRGB oldColor = leds[0]; // old color
  CRGB color = CRGB(0,0,0);
  // odds of flipping = 0.5;
  float r = (float)random8();
  float x = r/255.0;
      int red = 0;
  if (x <= 0.2) {

    // valkkaa uusi väri 
    // jos oli musta tai himmeä, niin vaihda kirkkaksi
    if (oldColor.r <= 40) {
      red = random8(127,245); 
    } else {
      // jos oli kirkas, niin himmeä
      red = random8(0,40); 
    } 
    // int red = random8(0,255);
    color = CRGB( red, 0, 0);
  } else {
    if (oldColor.r > 40) {
      red = random8(127,245); 
    } else {
      // jos oli kirkas, niin himmeä
      red = random8(0,40); 
    } 
    color = CRGB( red, 0, 0);
  }
  return color; 
}

void shiftLeds()
{
  // tämä siirtää kaikkia ledejä yhden oikealle, eli leds[1] = leds[0] jne 
  // n pituus
  int i = 0;
  for(i = NUM_LEDS - 1; i > 0; i--){
      leds[i] = leds[i-1];
  }
}
