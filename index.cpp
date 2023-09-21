#include "iostream"
#include "string"
#include <cstdint>
#include <sstream>
#include <vector>

using namespace std;

struct bfloat {
  uint32_t mantissa_h : 20;
  uint64_t mantissa_l : 64;
  uint32_t order : 17;

  uint8_t sign : 1;
};

// 101 bit
struct bint {
  // there must be 30 symbols
  uint64_t part_l : 64;
  uint64_t part_h : 36;

  uint8_t sign : 1;
};

class bint_t {
public:
  bint_t(const char *chars) {
    int counter = 0;
    bint my_int;

    char to_fixed[30] = {'B'};

    while (*chars) {

      if (counter == 0 && *chars == '-') {
        my_int.sign |= 0b1;
        continue;
      }

      if (counter == 0 && *chars == '+') {
        my_int.sign &= ~(0b1);
        continue;
      }

      if (48 <= *chars && *chars <= 57) {
        to_fixed[counter] = *chars;
      } else {
        cout << "NOT A NUMBER IN bint constructor" << endl;
      }
      chars++;
      counter++;
    }

    for (uint8_t i = 0; i < 30; i++) {
      cout << to_fixed[i] << endl;
    }

    cout << to_fixed[0] << endl;
    cout << to_fixed[1] << endl;
  }
};

string long_div(string number, int divisor) {
  // As result can be very large store it in string
  string ans;
  int idx = 0;
  int temp = number[idx] - '0';
  while (temp < divisor)
    temp = temp * 10 + (number[++idx] - '0');

  while (number.size() > idx) {
    ans += (temp / divisor) + '0';

    temp = (temp % divisor) * 10 + number[++idx] - '0';
  }

  if (ans.length() == 0)
    return "0";

  return ans;
}

int main() {
  stringstream geek;

  string long_string = long_div("999999999999999999999999999999", 1);
  string prev_long_string;

  vector<int> result = {};

  while (long_string != "0" && long_string != "1") {
    char last_char_of_long_string = long_string[long_string.length() - 1];
    uint8_t remainder = 0;

    switch (last_char_of_long_string) {
    case '1':
    case '3':
    case '5':
    case '7':
    case '9':
      remainder = 1;
    }

    long_string = long_div(long_string, 2);

    result.push_back(remainder);
  }

  if (long_string == "1") {
    result.push_back(1);
  } else {
    result.push_back(0);
  }

  for (auto item : result) {
    cout << item << endl;
  }
}
