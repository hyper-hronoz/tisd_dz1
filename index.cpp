#include "iostream"
#include "string"
#include <algorithm>
#include <bitset>
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

bool fullAdder(bool b1, bool b2, bool &carry) {
  bool sum = (b1 ^ b2) ^ carry;
  carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
  return sum;
}

bitset<101> bitsetAdd(bitset<101> &x, bitset<101> &y) {
  bool carry = false;
  // bitset to store the sum of the two bitsets
  bitset<101> ans;
  for (int i = 0; i < 100; i++) {
    ans[101 - i - 1] = fullAdder(x[101 - i - 1], y[101 - i - 1], carry);
  }
  return ans;
}

class bint_t {
private:
  bitset<101> bint;

public:
  explicit bint_t() {}
  bint_t(bitset<101> bits) { this->bint = bits; }

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
      bint[101 - 1 - counter] = (remainder);
      counter++;
    }

    if (long_string == "1") {
      bint[101 - 1 - counter] = (1);
    }

    print();
  }

  bint_t operator+(bint_t &right_bint) {
    bitset<101> m = bitsetAdd(this->bint, right_bint.bint);
    return bint_t(m);
  }

  void print() {
    for (int i = 0; i < 101; i++) {
      cout << bint[i];
    }
    cout << endl;
  }

  void print_dec() {
    string sum = "0";
    for (uint8_t i = 0; i < 100; i++) {
      uint8_t binary_number = this->bint[101 - i - 1];
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

int main() {
  bint_t by_n1("-00000000001");
  by_n1.print();
  // bint_t by_n2("2");

  // bint_t by_n3 = by_n1 + by_n2;
  // by_n3.print();
  // by_n3.print_dec();
}
