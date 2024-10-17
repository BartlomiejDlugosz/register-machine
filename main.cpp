#include <iostream>

class SingleNumCoding {
private:
  int x;
  int y;

public:
  SingleNumCoding(int x, int y) {
    this->x = x;
    this->y = y;
  }

  SingleNumCoding(int num): x(0), y(0) {
    while ((num & 0b1) == 1) {
      num = num >> 1;
      this->x++;
    }
    this->y = num / 2;
  }

  long long int num() {
    long long int a = 1 << x;
    long long int b = 2 * y + 1;
    return (a * b) - 1;
  }

  int getX() { return x; }
  int getY() { return y; }
};

class DoubleNumCoding {
private:
  int x;
  int y;

public:
  DoubleNumCoding(int x, int y) {
    this->x = x;
    this->y = y;
  }

  DoubleNumCoding(int num): x(0), y(0) {
    while ((num & 0b1) == 0) {
      num = num >> 1;
      this->x++;
    }
    this->y = (num - 1) / 2;
  }

  long long int num() {
    long long int a = 1 << x;
    long long int b = 2 * y + 1;
    return a * b;
  }

  int getX() { return x; }
  int getY() { return y; }
};

int main() {
  long long int program = 0;
  SingleNumCoding test = SingleNumCoding(27);
  std::cout << test.getX() << " " << test.getY() << std::endl;

  return 0;
}
