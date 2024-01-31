

#include <TFT_eSPI.h>
#include "box2d-lite/World.h"
#include "box2d-lite/Body.h"
#include "box2d-lite/Joint.h"
// A library for interfacing with LCD displays
//
// Can be installed from the library manager (Search for "TFT_eSPI")
//https://github.com/Bodmer/TFT_eSPI

TFT_eSPI tft = TFT_eSPI();
Vec2 gravity(0.0f, 20.0f);
int iterations = 10;
World world(gravity, iterations);
Body b[3];

uint8_t buffer[TFT_WIDTH * TFT_HEIGHT];

void setup() {
  // Start the tft display and set it to black
  tft.init();
  tft.setRotation(3); //This is the display in landscape
  
  // Clear the screen before writing to it
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  int x = 5;
  int y = 10;
  int fontNum = 2; 
  tft.drawString("Hello", x, y, fontNum); // Left Aligned
  x = 320 /2;
  y += 16;
  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.drawCentreString("World", x, y, fontNum);

  b[0].Set(Vec2(100.0f, 20.0f), FLT_MAX);
  b[0].position.Set(50.0f, 4.0f * b[0].width.y);
  world.Add(&b[0]);

  b[1].Set(Vec2(20.0f, 20.0f), 200.0f);
  b[1].position.Set(10.0f, 0.0f);
  world.Add(&b[1]);

  
  b[2].Set(Vec2(20.0f, 20.0f), 200.0f);
  b[2].position.Set(20.0f, -30.0f);
  world.Add(&b[2]);


}

void bufferSetPixel(int x, int y, uint16_t color) {
    if (x >= 0 && x < TFT_WIDTH && y >= 0 && y < TFT_HEIGHT) {
        buffer[y * TFT_WIDTH + x] = color;
    }
}

void drawLineToBuffer(int x0, int y0, int x1, int y1, uint16_t color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        if (x0 >= 0 && x0 < TFT_WIDTH && y0 >= 0 && y0 < TFT_HEIGHT) {
            buffer[y0 * TFT_WIDTH + x0] = color; // Set pixel in buffer
        }

        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

static void DrawBody(Body* body)
{
	Mat22 R(body->rotation);
	Vec2 x = body->position;
	Vec2 h = 0.5f * body->width;

	Vec2 v1 = x + R * Vec2(-h.x, -h.y);
	Vec2 v2 = x + R * Vec2( h.x, -h.y);
	Vec2 v3 = x + R * Vec2( h.x,  h.y);
	Vec2 v4 = x + R * Vec2(-h.x,  h.y);


  drawLineToBuffer(v1.x, v1.y, v2.x, v2.y, TFT_WHITE);
  drawLineToBuffer(v2.x, v2.y, v3.x, v3.y, TFT_WHITE);
  drawLineToBuffer(v3.x, v3.y, v4.x, v4.y, TFT_WHITE);
  drawLineToBuffer(v4.x, v4.y, v1.x, v1.y, TFT_WHITE);
}

void clearBuffer(uint16_t color) {
    for (int i = 0; i < TFT_WIDTH * TFT_HEIGHT; ++i) {
        buffer[i] = color;
    }
}



void loop() {
  clearBuffer(TFT_BLACK);
  world.Step(1.0f / 16.0f);


  for (int i = 0; i < 3; ++i)
    DrawBody(b + i);

  tft.pushImage(0, 0, TFT_WIDTH, TFT_HEIGHT, buffer);


  // delay(16);
  // tft.fillScreen(TFT_BLACK);

}