#include <iostream>
#include "bigint.h"
#include <math.h>

using namespace std;

// Write your implementation below
BigInt::BigInt(std::string s, int base) {
    for (size_t i = s.size(); i >= 1; i--) {
      int temp = static_cast<int>(s[i-1]);
      if (temp <= 57) {
        temp -= '0';
      }
      else {
          temp = temp + 10 - 'A';
      }
      myvec.push_back(temp);
    }
    ba = base;
}

void BigInt::removeLeadingZeroes() {
    for (size_t i = myvec.size(); i >= 1; i--) {
        if ((i != 1) && (myvec[i-1] == 0)) {
            myvec.pop_back();
        }
        else {
            break;
        }
    }
}

// returns the sum of this BigInt and rhs
BigInt BigInt::operator+(const BigInt& rhs) const {
    BigInt temp = *this;
    temp.add(rhs);
    return temp;
}

// returns the difference of this BigInt minus rhs
BigInt BigInt::operator-(const BigInt& rhs) const {
    BigInt temp = *this;
    for (size_t i = 0; i < rhs.myvec.size(); i++) {
        if ((i != temp.myvec.size() - 1) && (temp.myvec[i] - rhs.myvec[i] < 0)) {
            temp.myvec[i+1]--;
            temp.myvec[i] += 10;
        }
        temp.myvec[i] -= rhs.myvec[i];
    }
    return temp;
}
// returns the true if this BigInt is less than rhs
bool BigInt::operator<(const BigInt& rhs) const {
    BigInt temp = *this;
    if (temp.myvec.size() > rhs.myvec.size()) {
        return true;
    }
    else if (temp.myvec.size() < rhs.myvec.size()) {
        return false;
    }
    else {
        for (size_t i = rhs.myvec.size(); i >= 1; i--) {
            if ((i == rhs.myvec.size()) && (temp.myvec[i-1] - rhs.myvec[i-1] < 0)) {
              return true;
            }
            else if ((temp.myvec[i-1] - rhs.myvec[i-1] < 0)) {
                if (temp.myvec[i] > 0) {
                    temp.myvec[i]--;
                    temp.myvec[i-1] += 10;
                }
                else {
                    return true;
                }
            }
            temp.myvec[i-1] -= rhs.myvec[i-1];
        }
    }
    return false;
}

std::string BigInt::to_string() {
    string result = "";
    this->removeLeadingZeroes();
    for (size_t i = myvec.size(); i >= 1; i--) {
        if (myvec[i-1] < 10) {
            result += (char)(myvec[i-1]+'0');
        }
        else {
            result += (char)(myvec[i-1]-10+'A');
        }
    }
    return result;
}

void BigInt::add(BigInt b) {
    int addon = 0;
    int s;
    if (b.myvec.size() > myvec.size()) {
        s = b.to_string().size();
    }
    else {
        s = myvec.size();
    }
    for (size_t i = 0; i < s; i++) {
        int adig, bdig;
        if (i > myvec.size() - 1) {
            adig = 0;
        }
        else {
            adig = myvec[i];
        }
        if (i > b.myvec.size() - 1) {
            bdig = 0;
        }
        else {
            bdig = b.myvec[i];
        }
        if (i > myvec.size() - 1) {
            myvec.push_back(addon + adig + bdig);
        }
        else {
            myvec[i] = (addon + adig + bdig) % ba;
        }
        addon = (addon + adig + bdig) / ba;
    }
    if (addon != 0) {
        myvec.push_back(addon);
    }
}
