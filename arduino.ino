#if 1

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP = 6, XM = A2, YP = A1, YM = 7; //ID=0x9341
const int TS_LEFT = 175, TS_RT = 890, TS_TOP = 906, TS_BOT = 209;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);



// Cores
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define BUTTON_X 25
#define BUTTON_Y 200
#define BUTTON_W 40
#define BUTTON_H 40
#define BUTTON_SPACING_X 10
#define BUTTON_SPACING_Y 10
#define BUTTON_TEXTSIZE 2

#define TEXT_X 10
#define TEXT_Y 10
#define TEXT_W 220
#define TEXT_H 50
#define TEXT_TSIZE 3
#define TEXT_TCOLOR WHITE
#define TEXT_LEN 12
char textfield[TEXT_LEN+1] = "";
int textfield_i=0;

char buttonlabels[15][5] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    /*uint16_t buttoncolors[15] = {ILI9341_DARKGREEN, ILI9341_DARKGREY, ILI9341_RED, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_BLUE, ILI9341_BLUE, ILI9341_BLUE, 
                             ILI9341_ORANGE, ILI9341_BLUE, ILI9341_ORANGE};
                                */
Adafruit_GFX_Button buttons[15];


void setup(void)
{
    Serial.begin(9600);
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    Serial.println("Calibrate for your Touch Panel");
    if (ID == 0xD3D3) ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0);       //PORTRAIT
   
    tft.fillScreen(BLACK);    
    desenho();
    
    tft.drawRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, WHITE);

  
}


void loop(void)
{
  TSPoint p;
  p = ts.getPoint(); 
  if (p.z != -1) {
    p.x = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
    p.y = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
    //Serial.print("("); Serial.print(p.x); Serial.print(", "); 
    //Serial.print(p.y); Serial.print(", "); 
    //Serial.print(p.z); Serial.println(") ");
  }
  
  // go thru all the buttons, checking if they were pressed
  for (int b=0; b<15; b++) {
    if (buttons[b].contains(p.x, p.y)) {
      //Serial.print("Pressing: "); Serial.println(b);
      buttons[b].press(true);  // tell the button it is pressed
    } else {
      buttons[b].press(false);  // tell the button it is NOT pressed
    }
  }

  // now we can ask the buttons if their state has changed
  for (int b=0; b<15; b++) {
    if (buttons[b].justReleased()) {
      // Serial.print("Released: "); Serial.println(b);
      buttons[b].drawButton();  // draw normal
    }
    
   for (int b=0; b<15; b++) {
    if (buttons[b].justReleased()) {
      buttons[b].drawButton();
    }
    
    if (buttons[b].justPressed()) {


        
        // if a numberpad button, append the relevant # to the textfield
        
        /*
        if (b >= 3) {
          if (textfield_i < 12) {
            textfield[textfield_i] = buttonlabels[b][0];
            textfield_i++;
	          textfield[textfield_i] = 0; // zero terminate
          }
        }
        // clr button! delete char
        if (b == 1) {
          
          textfield[textfield_i] = 0;
          if (textfield > 0) {
            textfield_i--;
            textfield[textfield_i] = ' ';
          }
        }
        */
        
          textfield[textfield_i] = buttonlabels[b][0];
          textfield_i++;
          textfield[textfield_i] = 0;

        // update the current text field
        Serial.println(textfield);
       /* tft.setCursor(TEXT_X + 2, TEXT_Y+10);
        tft.setTextColor(TEXT_TCOLOR, BLACK);
        tft.setTextSize(TEXT_TSIZE);
        tft.print(textfield);
      */
        
        
      delay(1000); // UI debouncing
      return;
    }
   }
   
}
}



void desenho(void) {
  for (int row=0; row<5; row++) {
    for (int col=0; col<3; col++) {
      buttons[col + row*3].initButton(&tft, BUTTON_X+col*(BUTTON_W+BUTTON_SPACING_X), 
                 BUTTON_Y+row*(BUTTON_H+BUTTON_SPACING_Y),    // x, y, w, h, outline, fill, text
                  BUTTON_W, BUTTON_H, WHITE, CYAN, BLACK,
                  buttonlabels[col + row*3], BUTTON_TEXTSIZE); 
      buttons[col + row*3].drawButton();
    }
  }
}
#endif
