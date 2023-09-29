#include "iostream"
#include "string"
#include <algorithm>
#include <bitset>
#include <cstdint>
#include <cstring>
#include <sstream>
#include <vector>

#define bint_length 101

using namespace std;

string string_multiply(string nums1, string nums2) {
  int n = nums1.size();
  int m = nums2.size();
  string ans(n + m, '0');
  for (int i = n - 1; i >= 0; i--) {
    for (int j = m - 1; j >= 0; j--) {
      int p = (nums1[i] - '0') * (nums2[j] - '0') + (ans[i + j + 1] - '0');
      ans[i + j + 1] = p % 10 + '0';
      ans[i + j] += p / 10;
    }
  }
  for (int i = 0; i < m + n; i++) {
    if (ans[i] != '0')
      return ans.substr(i);
  }
  return "0";
}

string string_sum(string a, string b) {
  if (a.size() < b.size())
    swap(a, b);

  int j = a.size() - 1;
  for (int i = b.size() - 1; i >= 0; i--, j--)
    a[j] += (b[i] - '0');

  for (int i = a.size() - 1; i > 0; i--) {
    if (a[i] > '9') {
      int d = a[i] - '0';
      a[i - 1] = ((a[i - 1] - '0') + d / 10) + '0';
      a[i] = (d % 10) + '0';
    }
  }
  if (a[0] > '9') {
    string k;
    k += a[0];
    a[0] = ((a[0] - '0') % 10) + '0';
    k[0] = ((k[0] - '0') / 10) + '0';
    a = k + a;
  }
  return a;
}

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

bool full_adder(bool b1, bool b2, bool &carry) {
  bool sum = (b1 ^ b2) ^ carry;
  carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
  return sum;
}

template <uint8_t T> bitset<T> bitset_add(bitset<T> &x, bitset<T> &y) {
  bool carry = false;
  // bitset to store the sum of the two bitsets
  bitset<T> ans;
  for (int i = 0; i < T - 1; i++) {
    ans[T - i - 1] = full_adder(x[T - i - 1], y[T - i - 1], carry);
  }
  return ans;
}

class bint_t {
private:
  bitset<bint_length> bint;

public:
  explicit bint_t() {}
  bint_t(bitset<bint_length> bits) { this->bint = bits; }

  bint_t(string long_string) {
    if (long_string[0] == '+') {
      long_string[0] = '0';
      bint[0] = 0b0;
    }

    if (long_string[0] == '-') {
      long_string[0] = '0';
      bint[0] = 0b1;
    }

    uint8_t is_first_not_null_met = 0;
    string new_string;
    for (uint8_t i = 0; i < long_string.length(); i++) {
      if (long_string[i] != '0' && !is_first_not_null_met) {
        is_first_not_null_met = 1;
      }
      if (is_first_not_null_met) {
        new_string.push_back(long_string[i]);
      }
    }

    long_string = new_string;

    uint8_t counter = 0;
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
      bint[bint_length - 1 - counter] = (remainder);
      counter++;
    }

    if (long_string == "1") {
      bint[bint_length - 1 - counter] = (1);
    }

    print();
  }

  bint_t operator+(bint_t &right_bint) {
    bitset<bint_length> m = bitset_add<101>(this->bint, right_bint.bint);
    return bint_t(m);
  }

  virtual void print() {
    for (int i = 0; i < bint_length; i++) {
      cout << bint[i];
    }
    cout << endl;
  }

  bitset<bint_length> get_bitset() { return this->bint; }

  uint8_t get_length() {
    uint8_t length = 0;
    uint8_t sagnificant_index = 0;
    for (uint8_t i; i < bint_length; i++) {
      // cout << bint[i] << endl;
      if (bint[i] == 1) {
        sagnificant_index = 1;
      }
      if (sagnificant_index != 0) {
        length++;
      }
    }
    return length;
  }

  void print_dec() {
    string sum = "0";
    for (uint8_t i = 0; i < bint_length - 1; i++) {
      uint8_t binary_number = this->bint[bint_length - i - 1];
      string number = to_string(binary_number);
      for (uint8_t j = 0; j < i; j++) {
        if (i != 0) {
          number = string_multiply(number, "2");
        }
      }
      sum = string_sum(number, sum);
    }
    cout << sum << endl;
  }
};

#define sign_size 1
#define order_size 17
#define order_sign 1
#define mantissa_size 84

class bfloat_t {
private:
  // sign order mantissa 102bit
  bitset<sign_size + order_sign + order_size + mantissa_size> bfloat;

public:
  bfloat_t(string long_string) {
    if (long_string[0] == '-') {
      bfloat[0] = 0b1;
    }

    if (long_string[0] == '+') {
      bfloat[0] = 0b0;
    }

    std::string delimiter = ".";

    std::string left = {0};
    std::string right = {0};

    size_t pos = 0;

    pos = long_string.find(delimiter);
    left = long_string.substr(0, pos);
    std::cout << left << std::endl;
    long_string.erase(0, pos + delimiter.length());

    right = long_string;

    uint8_t index_to_write = 0;
    bint_t binary_left(left);
    for (uint8_t i = bint_length - binary_left.get_length(); i < bint_length;
         i++) {
      bfloat[index_to_write + sign_size + order_sign + order_size] =
          binary_left.get_bitset()[i];
      index_to_write++;
    }

    cout << "Right: " << right << endl;
    uint8_t string_prev_length = 0;
    uint8_t string_current_length = 0;
    while (right != "0" && index_to_write < 84) {
      string_prev_length = right.length();
      right = string_multiply(right, "2");
      cout << right << "-> ";
      string_current_length = right.length();
      if (string_prev_length < string_current_length) {
        bfloat[index_to_write + sign_size + order_sign + order_size] = 0b1;
        right.erase(0, 1);
      } else {
        bfloat[index_to_write + sign_size + order_sign + order_size] = 0b0;
      }
      index_to_write++;
    }

    cout << pos << endl;
    this->print();

    // короче я придумал, нужно умножать так как выше и сравнивать длины если
    // появлися разряд значит 1 если нет 0. Но это нужно выполнять только после
    // операдции приведения целой части к двоичному виду чтобы расчитать
    // оставшееся место.
  }

  void print() {
    for (int i = 0; i < sign_size + order_sign + order_size + mantissa_size;
         i++) {
      if (i == 19 || i == 1 || i == 2) {
        cout << ".";
      }
      cout << bfloat[i];
    }
    cout << endl;
  }
};

int main() {
  bfloat_t my_float("5.142");
  my_float.print();
  // bint_t my_bint("123");
  // my_bint.print();
}
