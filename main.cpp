#include <iostream>
#include <map>
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

class Instruction {
public:
  enum class Type {
    HALT,
    INC,
    DEC
  };
  Type type;
  int reg;
  int lbl1;
  int lbl2;
public:
  Instruction() {
    this->type = Type::HALT;
  }

  Instruction(int reg, int lbl1) {
    this->type = Type::INC;
    this->reg = reg;
    this->lbl1 = lbl1;
  }

  Instruction(int reg, int lbl1, int lbl2) {
    this->type = Type::DEC;
    this->reg = reg;
    this->lbl1 = lbl1;
    this->lbl2 = lbl2;
  }
};

class Program {
private:
  std::map<int, Instruction> instructions;
  std::map<int, int> registers;
public:
  /* Decode the program from a single number
   * Converts the number into a list, and then converts each item
   * into a {{x, y}} pair.
   * If x is 0, then the program halts
   * If x is even, then the program increments the register encoded as x/2 and jumps to the label encoded as y
   * If x is odd, then the program decrements the register encoded as (x-1)/2. If the
   * current value in the register is zero then it's not decremented (as we can't have negative values)
   * and the program jumps to L2. If it decrements successfully then it jumps to L1. Where {L1, L2} = y.
   */
  Program(int prog) {
    ListNumCoding list_of_instr = ListNumCoding(prog);
    std::vector<int> instr_nums = list_of_instr.getNums();
    for (int i = 0; i < instr_nums.size(); i++) {
      int instr = instr_nums[i];
      if (instr == 0) {
        this->instructions[i] = Instruction();
        continue;
      }

      DoubleNumCoding x = DoubleNumCoding(instr);

      if (x.getX() % 2 == 0) {
        this->instructions[i] = Instruction(x.getX(), x.getY());
      } else {
        SingleNumCoding label = SingleNumCoding(x.getY());
        this->instructions[i] = Instruction(x.getX(), label.getX(), label.getY());
      }
    }
  }

  void print() {
    for (int i = 0; i < this->instructions.size(); i++) {
      Instruction instr = this->instructions[i];
      if (instr.type == Instruction::Type::HALT) {
        std::cout << i << ": HALT" << std::endl;
      } else if (instr.type == Instruction::Type::INC) {
        std::cout << i << ": INC R" << instr.reg << " -> " << instr.lbl1 << std::endl;
      } else if (instr.type == Instruction::Type::DEC) {
        std::cout << i << ": DEC R" << instr.reg << " -> " << instr.lbl1 << ", " << instr.lbl2 << std::endl;
      }
    }
  }

  void run() {
    int cur_index = 0;
    while (true) {
      if (cur_index >= this->instructions.size()) break;

      Instruction cur_instr = this->instructions[cur_index];
      if (cur_instr.type == Instruction::Type::HALT) {
        break;
      } else if (cur_instr.type == Instruction::Type::INC) {
        this->registers[cur_instr.reg]++;
        cur_index = cur_instr.lbl1;
      } else if (cur_instr.type == Instruction::Type::DEC) {
        if (this->registers[cur_instr.reg] > 0) {
          this->registers[cur_instr.reg]--;
          cur_index = cur_instr.lbl1;
        } else {
          cur_index = cur_instr.lbl2;
        }
      }
      for (int i = 0; i < this->registers.size(); i++) {
        std::cout << "R" << i << ": " << this->registers[i] << std::endl;
      }
    }
    for (int i = 0; i < this->registers.size(); i++) {
      std::cout << "R" << i << ": " << this->registers[i] << std::endl;
    }
  }

  void run(std::map<int, int> starting_registers) {
    for (int i = 0; i < starting_registers.size(); i++) {
      this->registers[i] = starting_registers[i];
    }
    run();
  }
};

int main() {

  long long int program = 786432;;
  Program p = Program(program);
  p.print();
  // p.run({{0, 3}, {1, 5}});



  return 0;
}
