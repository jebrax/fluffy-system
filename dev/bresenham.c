#include "sys.h"
#include "base.h"
#include "pixels.h"
#include "bmp.h"

#define WIDTH 1920
#define HEIGHT 1080

typedef struct {
  int width;
  int height;
  int x0, y0, x1, y1;
  float k;
  float b;
} jx_uniform;

void jxswap(int* a, int* b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int jxround(float a)
{
  a += a - (int) a;
  return (int) a;
}

void jxprecalculate(jx_uniform* u)
{
  if (u->x0 > u->x1)
    jxswap(&(u->x0), &(u->x1));

  if (u->y0 > u->y1)
    jxswap(&(u->y0), &(u->y1));

  if (u->x1 - u->x0 < u->y1 - u->y0) {
    jxswap(&(u->x0), &(u->y0));
    jxswap(&(u->x1), &(u->y1));
  }

  u->k = (float) (u->y1 - u->y0) / (u->x1 - u->x0);
  u->b = u->y0 - u->x0 * u->k;
}

jx_pixel24bit jxcalculate_pixel_color(int x, int y, jx_uniform* u)
{
  jx_pixel24bit result = {0, 0, 0};

  if (u->x1 - u->x0 < u->y1 - u->y0)
    jxswap(&x, &y);

  if (x >= u->x0 && x < u->x1) {
    int yline = jxround(u->k * x + u->b);
    if (y == yline)
      result.r = 255;
  }

  return result;
}

int main()
{
  jx_pixel24bit pixels[WIDTH*HEIGHT];

  jx_uniform uniform;
  uniform.width = WIDTH;
  uniform.height = HEIGHT;
  uniform.x0 = 300;
  uniform.x1 = 1400;
  uniform.y0 = 200;
  uniform.y1 = 400;

  jxprecalculate(&uniform);

  for (register int x = 0; x < WIDTH; x++)
    for (register int y = 0; y < HEIGHT; y++)
      pixels[y*WIDTH + x] = jxcalculate_pixel_color(x, y, &uniform);

  jxsavebmpfile(pixels, WIDTH, HEIGHT);

  return 0;
}
