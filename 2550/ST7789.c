///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                              ST7789.c                             ////
////                                                                   ////
////               ST7789 display driver for CCS C compiler            ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////               This is a free software with NO WARRANTY.           ////
////                     https://simple-circuit.com/                   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#include <stdint.h>
#ifndef bool
#define bool int1
#endif

#define HEIGHT 240
#define WIDTH  240

#define TX_COLOR ST7789_WHITE
#define BG_COLOR ST7789_BLACK

#define ST_CMD_DELAY      0x80    // special signifier for command lists

#define ST77XX_NOP        0x00
#define ST77XX_SWRESET    0x01
#define ST77XX_RDDID      0x04
#define ST77XX_RDDST      0x09

#define ST77XX_SLPIN      0x10
#define ST77XX_SLPOUT     0x11
#define ST77XX_PTLON      0x12
#define ST77XX_NORON      0x13

#define ST77XX_INVOFF     0x20
#define ST77XX_INVON      0x21
#define ST77XX_DISPOFF    0x28
#define ST77XX_DISPON     0x29
#define ST77XX_CASET      0x2A
#define ST77XX_RASET      0x2B
#define ST77XX_RAMWR      0x2C
#define ST77XX_RAMRD      0x2E

#define ST77XX_PTLAR      0x30
#define ST77XX_COLMOD     0x3A
#define ST77XX_MADCTL     0x36

#define ST77XX_MADCTL_MY  0x80
#define ST77XX_MADCTL_MX  0x40
#define ST77XX_MADCTL_MV  0x20
#define ST77XX_MADCTL_ML  0x10
#define ST77XX_MADCTL_RGB 0x00

#define ST77XX_RDID1      0xDA
#define ST77XX_RDID2      0xDB
#define ST77XX_RDID3      0xDC
#define ST77XX_RDID4      0xDD

// Some ready-made 16-bit ('565') color settings:
#define   ST7789_BLACK   0x0000
#define   ST7789_BLUE    0x001F
#define   ST7789_RED     0xF800
#define   ST7789_GREEN   0x07E0
#define   ST7789_CYAN    0x07FF
#define   ST7789_MAGENTA 0xF81F
#define   ST7789_YELLOW  0xFFE0
#define   ST7789_WHITE   0xFFFF

#ifndef ST7789_SPI_XFER
#define ST7789_SPI_XFER(x) SPI_XFER(ST7789, x)
#endif

uint8_t
  _width,     ///< Display width as modified by current rotation
  _height,    ///< Display height as modified by current rotation
  _xstart,    ///< Internal framebuffer X offset
  _ystart,    ///< Internal framebuffer Y offset
  _colstart,  ///< Some displays need this changed to offset
  _rowstart,  ///< Some displays need this changed to offset
  rotation,   ///< Display rotation (0 thru 3)
  X_MAX,
  X_BEGIN,
  Y_MAX,
  Y_BEGIN;

uint8_t X;
uint8_t Y;
uint8_t CARRIAGEX;
uint8_t CARRIAGEY;


// SCREEN INITIALIZATION ***************************************************

// Rather than a bazillion writecommand() and writedata() calls, screen
// initialization commands and arguments are organized in these tables
// stored in PROGMEM.  The table may look bulky, but that's mostly the
// formatting -- storage-wise this is hundreds of bytes more compact
// than the equivalent code.  Companion function follows.

#define ST7789_240x240_XSTART 0
#define ST7789_240x240_YSTART 80     //80

rom uint8_t
  cmd_240x240[] =  {                // Init commands for 7789 screens
    9,                              //  9 commands in list:
    ST77XX_SWRESET,   ST_CMD_DELAY, //  1: Software reset, no args, w/delay
      150,                          //    150 ms delay
    ST77XX_SLPOUT ,   ST_CMD_DELAY, //  2: Out of sleep mode, no args, w/delay
      255,                          //     255 = 500 ms delay
    ST77XX_COLMOD , 1+ST_CMD_DELAY, //  3: Set color mode, 1 arg + delay:
      0x55,                         //     16-bit color
      10,                           //     10 ms delay
    ST77XX_MADCTL , 1,              //  4: Mem access ctrl (directions), 1 arg:
      0x08,                         //     Row/col addr, bottom-top refresh
    ST77XX_CASET  , 4,              //  5: Column addr set, 4 args, no delay:
      0x00,
      ST7789_240x240_XSTART,        //     XSTART = 0
      ((long)WIDTH+ST7789_240x240_XSTART)>>8,
      (WIDTH+ST7789_240x240_XSTART)&0xFF,  //     XEND = 240
    ST77XX_RASET  , 4,              //  6: Row addr set, 4 args, no delay:
      0x00,
      0,//ST7789_240x240_YSTART,             //     YSTART = 0
      (HEIGHT+ST7789_240x240_YSTART)>>8,
      (HEIGHT+ST7789_240x240_YSTART)&0xFF,  //     YEND = 240
    ST77XX_INVON  ,   ST_CMD_DELAY,  //  7: hack
      10,
    ST77XX_NORON  ,   ST_CMD_DELAY, //  8: Normal display on, no args, w/delay
      10,                           //     10 ms delay
    ST77XX_DISPON ,   ST_CMD_DELAY, //  9: Main screen turn on, no args, delay
    255 };                          //     255 = max (500 ms) delay

//*************************** User Functions ***************************//
void tft_init(void);

void drawPixel(uint16_t x, uint8_t y, uint16_t color);
void drawHLine(uint16_t x, uint8_t y, uint8_t w, uint16_t color);
void drawVLine(uint16_t x, uint8_t y, uint8_t h, uint16_t color);
void fillRect(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void fillScreen(uint16_t color);
void setRotation(uint8_t m);
void invertDisplay(bool i);

void tft_putc(char c);
void printDisp(uint8_t x, uint8_t y, uint16_t color, uint16_t b_color, int ch[5]);
void tft_gotoxy(uint8_t x, uint8_t y);

//************************* Non User Functions *************************//
void startWrite(void);
void endWrite(void);
void displayInit(rom uint8_t *addr);
void writeCommand(uint8_t cmd);
void setAddrWindow(uint8_t x, uint8_t y, uint8_t w, uint8_t h);

/**************************************************************************/
/*!
    @brief  Call before issuing command(s) or data to display. Performs
            chip-select (if required). Required
            for all display types; not an SPI-specific function.
*/
/**************************************************************************/
void startWrite(void) {
  #ifdef TFT_CS
    output_low(TFT_CS);
  #endif
}

/**************************************************************************/
/*!
    @brief  Call after issuing command(s) or data to display. Performs
            chip-deselect (if required). Required
            for all display types; not an SPI-specific function.
*/
/**************************************************************************/
void endWrite(void) {
  #ifdef TFT_CS
    output_high(TFT_CS);
  #endif
}

/**************************************************************************/
/*!
    @brief  Write a single command byte to the display. Chip-select and
            transaction must have been previously set -- this ONLY sets
            the device to COMMAND mode, issues the byte and then restores
            DATA mode. There is no corresponding explicit writeData()
            function -- just use ST7789_SPI_XFER().
    @param  cmd  8-bit command to write.
*/
/**************************************************************************/
void writeCommand(uint8_t cmd) {
  output_low(TFT_DC);
  ST7789_SPI_XFER(cmd);
  output_high(TFT_DC);
}

/**************************************************************************/
/*!
    @brief  Companion code to the initiliazation tables. Reads and issues
            a series of LCD commands stored in ROM byte array.
    @param  addr  Flash memory array with commands and data to send
*/
/**************************************************************************/
void displayInit(rom uint8_t *addr){
  uint8_t  numCommands, numArgs;
  uint16_t ms;
  startWrite();

  numCommands = *addr++;   // Number of commands to follow
  
  while(numCommands--) {                 // For each command...

    writeCommand(*addr++); // Read, issue command
    numArgs  = *addr++;    // Number of args to follow
    ms       = numArgs & ST_CMD_DELAY;   // If hibit set, delay follows args
    numArgs &= ~ST_CMD_DELAY;            // Mask out delay bit
    while(numArgs--) {                   // For each argument...
      ST7789_SPI_XFER(*addr++);   // Read, issue argument
    }

    if(ms) {
      ms = *addr++; // Read post-command delay time (ms)
      if(ms == 255) ms = 500;     // If 255, delay for 500 ms
      delay_ms(ms);
    }
  }
  endWrite();
}

/**************************************************************************/
/*!
    @brief  Initialization code for ST7789 display
*/
/**************************************************************************/
void tft_init() {
  #ifdef TFT_RST
    output_high(TFT_RST);
    output_drive(TFT_RST);
    delay_ms(100);
    output_low(TFT_RST);
    delay_ms(100);
    output_high(TFT_RST);
    delay_ms(200);
  #endif

  #ifdef TFT_CS
    output_high(TFT_CS);
    output_drive(TFT_CS);
  #endif

  output_drive(TFT_DC);

  displayInit(cmd_240x240);

  _colstart = 53;//ST7789_240x240_XSTART;
  _rowstart = 40;//ST7789_240x240_YSTART;
  _height   = 135;//HEIGHT;
  _width    = 240;//WIDTH;
  
  X = X_BEGIN;
  Y= Y_BEGIN;
  CARRIAGEX = X;
  CARRIAGEY = Y;
  
  setRotation(3);
  //invertDisplay(0);
  delay_ms(1);
  fillScreen(BG_COLOR);
  
  
}

/**************************************************************************/
/*!
  @brief  SPI displays set an address window rectangle for blitting pixels
  @param  x  Top left corner x coordinate
  @param  y  Top left corner x coordinate
  @param  w  Width of window
  @param  h  Height of window
*/
/**************************************************************************/
void setAddrWindow(uint16_t x, uint8_t y, uint8_t w, uint8_t h) {
  x += _xstart;
  uint8_t xHi = x>>8;
  uint8_t xLo = x&0xFF;
  x+=(w-1);
  uint8_t wHi = x>>8;
  uint8_t wLo = x&0xFF;
  y += _ystart;

  writeCommand(ST77XX_CASET); // Column addr set
  ST7789_SPI_XFER(xHi);
  ST7789_SPI_XFER(xLo);
  ST7789_SPI_XFER(wHi);
  ST7789_SPI_XFER(wLo);

  writeCommand(ST77XX_RASET); // Row addr set
  ST7789_SPI_XFER(0);
  ST7789_SPI_XFER(y);
  ST7789_SPI_XFER(0);
  ST7789_SPI_XFER(y+h-1);

  writeCommand(ST77XX_RAMWR); // write to RAM
}

/**************************************************************************/
/*!
    @brief  Set origin of (0,0) and orientation of TFT display
    @param  m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void setRotation(uint8_t m) {
  uint8_t madctl = 0;

  rotation = m & 3; // can't be higher than 3

  switch (rotation) {
   case 0:
     madctl  = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
     _xstart = _colstart;
     _ystart = _rowstart;
     _width = WIDTH;
     _height = HEIGHT;
     break;
   case 1:
     madctl  = ST77XX_MADCTL_MY | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
     _xstart = _rowstart;
     _ystart = _colstart;
     _width = HEIGHT;
     _height = WIDTH;
     
     break;
  case 2:
     madctl  = ST77XX_MADCTL_RGB;
     _xstart = 0;
     _ystart = 0;
     _width = WIDTH;
     _height = HEIGHT;
     break;
   case 3:
     madctl  = ST77XX_MADCTL_MX | ST77XX_MADCTL_MV | ST77XX_MADCTL_RGB;
     _xstart = _rowstart;
     _ystart = _colstart;
     _width = 240;
     _height = 135;
     break;
  }
  X_MAX = ((_width/6)-2);
  X_BEGIN = (_width/X_MAX)/2;
  Y_MAX = ((_height/8)-1);
  Y_BEGIN = (_height/Y_MAX)+1;
  startWrite();
  writeCommand(ST77XX_MADCTL);
  ST7789_SPI_XFER(madctl);
  endWrite();
}

void drawPixel(uint16_t x, uint8_t y, uint16_t color) {
  if((x < _width) && (y < _height)) {
    startWrite();
    setAddrWindow(x, y, 1, 1);
    ST7789_SPI_XFER(color >> 8);
    ST7789_SPI_XFER(color & 0xFF);
    endWrite();
  }
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly horizontal line (this is often optimized in a subclass!)
    @param    x   Left-most x coordinate
    @param    y   Left-most y coordinate
    @param    w   Width in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void drawHLine(uint16_t x, uint8_t y, uint8_t w, uint16_t color) {
  if( (x < _width) && (y < _height) && w) {   
    uint8_t hi = color >> 8, lo = color;

    if((x >= _width) || (y >= _height))
      return;
    if((x + w - 1) >= _width)  
      w = _width  - x;
    startWrite();
    setAddrWindow(x, y, w, 1);
    while (w--) {
      ST7789_SPI_XFER(hi);
      ST7789_SPI_XFER(lo);
    }
    endWrite();
  }
}

/**************************************************************************/
/*!
   @brief    Draw a perfectly vertical line (this is often optimized in a subclass!)
    @param    x   Top-most x coordinate
    @param    y   Top-most y coordinate
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void drawVLine(uint16_t x, uint8_t y, uint8_t h, uint16_t color) {
  if( (x < _width) && (y < _height) && h) {  
    uint8_t hi = color >> 8, lo = color;
    if((y + h - 1) >= _height) 
      h = _height - y;
    startWrite();
    setAddrWindow(x, y, 1, h);
    while (h--) {
      ST7789_SPI_XFER(hi);
      ST7789_SPI_XFER(lo);
    }
    endWrite();
  }
}

/**************************************************************************/
/*!
   @brief    Fill a rectangle completely with one color. Update in subclasses if desired!
    @param    x   Top left corner x coordinate
    @param    y   Top left corner y coordinate
    @param    w   Width in pixels
    @param    h   Height in pixels
   @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void fillRect(uint16_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
  if(w && h) {                            // Nonzero width and height?  
    uint8_t hi = color >> 8, lo = color;
    if((x >= _width) || (y >= _height))
      return;
    if((x + w - 1) >= _width)  
      w = _width  - x;
    if((y + h - 1) >= _height) 
      h = _height - y;
    startWrite();
    setAddrWindow(x, y, w, h);
    uint32_t px = (uint32_t)w * h;
    while (px--) {
      ST7789_SPI_XFER(hi);
      ST7789_SPI_XFER(lo);
    }
    endWrite();
  }
}

/**************************************************************************/
/*!
   @brief    Fill the screen completely with one color. Update in subclasses if desired!
    @param    color 16-bit 5-6-5 Color to fill with
*/
/**************************************************************************/
void fillScreen(uint16_t color) {
    fillRect(0, 0, _width, _height, color);
}

/**************************************************************************/
/*!
    @brief  Invert the colors of the display (if supported by hardware).
            Self-contained, no transaction setup required.
    @param  i  true = inverted display, false = normal display.
*/
/**************************************************************************/
void invertDisplay(bool i) {
    startWrite();
    writeCommand(i ? ST77XX_INVON : ST77XX_INVOFF);
    endWrite();
}

void printDisp(uint8_t x, uint8_t y, uint16_t f_color, uint16_t b_color, int ch[5]){
   for(int i=0;i<5;i++){
      for(int j=0;j<8;j++){
         if((ch[i]>>j)&1)  drawPixel(x+i, y-7+j, f_color);
         else              drawPixel(x+i, y-7+j, b_color);
      }
   }
}

void tft_putc(char c){
   if(c=='\f'){
      fillRect(0, 0, CARRIAGEX, CARRIAGEY, BG_COLOR);
      CARRIAGEX=X_BEGIN;
      CARRIAGEY=Y_BEGIN;
      tft_gotoxy(0,0);
   }else if(c=='\n'){
      Y+=8;
      X=X_BEGIN;
   }else{
      if(c>122){
         if(c>205){
            if(c<212) c=(c%3)+59;
            else if(c<216) c=(c%2)+94;
            else if(c==217) c=40;
         }
         if(c<220 && c>122) c=(c%11)+33;
         if(c==220) c=45;
         else if(c==222) c=47;
         else if(c==223) c=64;
      }
      printDisp(X, Y, TX_COLOR, BG_COLOR, letra[c-32]);
      X+=6;
      if(X>CARRIAGEX) CARRIAGEX=X;
      if(Y>CARRIAGEY) CARRIAGEY=Y;
      if((X-X_BEGIN)/6>X_MAX){
         X=X_BEGIN;
         Y+=8;
      }
      if((Y-Y_BEGIN)/8>Y_MAX) Y=Y_BEGIN;
   }
}

void tft_gotoxy(uint8_t _x, uint8_t _y){
   X=X_BEGIN + 6*_x;
   Y=Y_BEGIN + 8*_y;
}
// end of code.
