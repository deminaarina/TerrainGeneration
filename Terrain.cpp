#include <iostream>
#include <cstdlib>
#include <cmath>

const int WIDTH = 100;
const int HEIGHT = 100;

double noise(double x, double y) {
  int n = (int)x + (int)y * 57;
  n = (n << 13) ^ n;
  int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
  return 1.0 - ((double)nn / 1073741824.0);
}

double smoothNoise(double x, double y) {
  double corners = (noise(x - 1, y - 1) + noise(x + 1, y - 1) + noise(x - 1, y + 1) + noise(x + 1, y + 1)) / 16;
  double sides = (noise(x - 1, y) + noise(x + 1, y) + noise(x, y - 1) + noise(x, y + 1)) / 8;
  double center = noise(x, y) / 4;
  return corners + sides + center;
}

double interpolate(double a, double b, double x) {
  double ft = x * 3.1415927;
  double f = (1 - cos(ft)) * 0.5;
  return  a*(1-f) + b*f;
}

double interpolatedNoise(double x, double y) {
  int intX = (int)x;
  double fracX = x - intX;

  int intY = (int)y;
  double fracY = y - intY;

  double v1 = smoothNoise(intX, intY);
  double v2 = smoothNoise(intX + 1, intY);
  double v3 = smoothNoise(intX, intY + 1);
  double v4 = smoothNoise(intX + 1, intY + 1);

  double i1 = interpolate(v1, v2, fracX);
  double i2 = interpolate(v3, v4, fracX);

  return interpolate(i1, i2, fracY);
}

int main() {
  double amplitude = 30;
  double frequency = 0.05;

  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      double value = interpolatedNoise(x * frequency, y * frequency) * amplitude;
      int height = (int)value;
      for (int i = 0; i < height; i++) {
        std::cout << "*";
      }
      std::cout << std::endl;
    }
  }
  return 0;
}
