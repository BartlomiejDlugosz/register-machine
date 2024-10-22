#include <iostream>
#include <vector>

/* Representation of a {x, y} pair in the form of a single number
 * x is the number of 1s before the first 0
 * y is the number after the first 0
 * The number is represented as 2^x * (2y + 1) - 1
 */
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

/* Representation of a {x, y} pair in the form of a single number
 * x is the number of 0s before the first 1
 * y is the number after the first 1
 * The number is represented as 2^x * (2y + 1)
 */
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
    if (num == 0)
      throw std::runtime_error("Invalid number");
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

/* Representation of a list of numbers in the form of a single number
 * Each number in the list is represented by the number of consecutive 0s before the first 1
 */
class ListNumCoding {
private:
  std::vector<int> nums;
public:
  ListNumCoding(std::vector<int> nums) {
    this->nums = nums;
  }

  ListNumCoding(int num) {
    while (num > 0) {
      int n = 0;
      while ((num & 0b1) == 0) {
        num = num >> 1;
        n++;
      }
      nums.push_back(n);
      num = num >> 1;
    }
  }

  int num() {
    int res = 0;
    for (int i = 0; i < nums.size(); i++) {
      int new_num = 1 << nums[i];
      for (int j = 0; j < i; j++) {
        new_num = new_num << (nums[j] + 1);
      }
      res += new_num;
    }
    return res;
  }

  std::vector<int> getNums() { return nums; }
};

int main() {
  long long int program = 786432;
  ListNumCoding test = ListNumCoding(program);
  std::vector<int> nums = test.getNums();
  for (int i : nums) {
    if (i == 0) {
      std::cout << "HALT" << std::endl;
      continue;
    }

    DoubleNumCoding x = DoubleNumCoding(i);

    if (x.getX() % 2 == 0) {
      std::cout << "R" << x.getX() / 2 << "+ -> L" << x.getY() << std::endl;
    } else {
      SingleNumCoding label = SingleNumCoding(x.getY());
      std::cout << "R" << (x.getX() - 1) / 2 << "- -> L" << label.getX() << ", L" << label.getY() << std::endl;
    }
  }



  return 0;
}
