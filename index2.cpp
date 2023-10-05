#include "iostream"
#include <algorithm>
#include <cstdint>

using namespace std;

string delete_duplicates(string str) {
  for (string::iterator it1 = str.begin(); it1 != str.end(); ++it1) {
    for (string::iterator it2 = it1 + 1; it2 != str.end();) {
      if (*it1 == *it2) {
        str.erase(it2);
      } else {
        ++it2;
      }
    }
  }
  return str;
}

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

class bint_t {
private:
  string bint;

public:
  bint_t(string bint) : bint(bint) {}

  bint_t operator*(string val) {

    if (delete_duplicates(val) == "0" || delete_duplicates(bint) == "0") {
      return bint_t("0");
    }

    string sign = "";

    if ((val.find("-") + this->bint.find("-")) % 2 == 1) {
      sign = "-";
    }

    bint.erase(std::remove(bint.begin(), bint.end(), '-'), bint.end());
    val.erase(std::remove(val.begin(), val.end(), '-'), val.end());

    std::string delimiter = ".";

    std::string left = {'0'};
    std::string right = {'0'};

    size_t pos = 0;

    pos = val.find(delimiter);
    left = val.substr(0, pos);
    // std::cout << left << std::endl;
    val.erase(0, pos);

    bool is_right_exists = 0;
    if (val.find(delimiter) != std::string::npos) {
      val.erase(0, 1);
      right = val;
      is_right_exists = 1;
    }

    cout << "Left: " << left << endl;
    cout << "Right: " << right << endl;

    string counter = {'1'};

    string new_left = left;
    string new_right = right;

    while (counter != this->bint) {

      uint32_t string_size = 0;

      string_size = new_right.length();

      new_left = string_sum(new_left, left);
      new_right = string_sum(new_right, right);

      if (new_right.length() > string_size) {
        string carry = new_right.substr(0, 1);
        new_left = string_sum(new_left, carry);
        new_right.erase(0, 1);
      }

      counter = string_sum("1", counter);
    }

    return bint_t(sign + new_left + "." + new_right);
    this->bint = string_multiply(this->bint, val);
    return *this;
  };

  string get_bint() { return this->bint; }
};

int main() {
  // 12201913.888888888821475
  bint_t bint("2");
  cout << (bint * "-2.0").get_bint();
  // cout << bint.get_bint() << endl;

  return 0;
}
