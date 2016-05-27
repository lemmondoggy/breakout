/* Breakout game for Arduino
 *
 * Copyright (c) 2016 Erin Molton <erin.molton@gmail.com>
 *
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

bool bricks [6*25] = {
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

struct ball {
  int x;
  int y;
  int x_vel;
  int y_vel;
};


struct ball ball;

void start_game(void) {
  ball.x = SSD1306_LCDWIDTH / 2;
  ball.y = SSD1306_LCDHEIGHT * 3 / 4;
  ball.x_vel = 1;
  ball.y_vel = -1;
}

void draw_bricks() {
  int l, x, b=0;

  for (l=0; l<6; l++)
    for (x=0; x<25; x++) {
      if(bricks[b++]){
        display.drawLine(x*5 + 2, l*3 + 14, x*5 + 5, l*3 + 14, WHITE);
        display.drawLine(x*5 + 2, l*3 + 15, x*5 + 5, l*3 + 15, WHITE);
      }
    }
}

void draw_arena() {
  display.drawLine(0, 0, 127, 0, WHITE);
  display.drawLine(0, 0, 0, 63, WHITE);
  display.drawLine(127, 0, 127, 63, WHITE);
}

void draw_ball() {
  display.drawPixel(ball.x, ball.y, WHITE);
}

static int bat_x;

void move_ball() {
  ball.x += ball.x_vel;
  ball.y += ball.y_vel;

  if(ball.x <= 1 || ball.x >= SSD1306_LCDWIDTH-1)
    ball.x_vel = -ball.x_vel;

  if(ball.y <=1 || (ball.y == 59 && (ball.x >= bat_x && ball.x <= bat_x+10)))
    ball.y_vel = -ball.y_vel;

  if(ball.y >= 80)
    start_game();
}

void setup() {
  long sensorvalue;
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done

  start_game();

  while(1) {
    // Clear the buffer.
    display.clearDisplay();

    draw_arena();
    draw_bricks();
    draw_ball();
    move_ball();

    sensorvalue = analogRead(A0);

    bat_x = (sensorvalue * 115 / 1023) + 1;

    //Serial.println(bat_x);

    display.drawLine(bat_x, 60, bat_x + 10, 60, WHITE);

    // Show the display buffer on the hardware.
    // NOTE: You _must_ call display after making any drawing commands

    // to make them visible on the display hardware!
    display.display();
  }
}





void loop() {

}

