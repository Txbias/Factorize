#include "big_int.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <cmath>


BigInt::BigInt() = default;

BigInt::BigInt(std::string number) {

    if(!number.empty() && number[0] == '-') {
        positive = false;
        number.erase(0, 1);
    }

    // Remove leading zeroes
    number.erase(0, number.find_first_not_of('0'));
    if(number.empty()) {
        number = "0";
    }

    this->digits = number;
}

BigInt::BigInt(const long long number) {
    if(number < 0) setSign(false);
    this->digits = std::to_string(std::abs(number));
}

std::ostream& operator<<(std::ostream& os, const BigInt& obj) {
    std::string res;
    if(!obj.isPositive()) {
        res = "-";
    }
    res.append(obj.getDigits());
    os.write(res.c_str(), res.size());
    return os;
}

const std::string& BigInt::getDigits() const {
    return digits;
}

bool BigInt::isPositive() const{
    return positive;
}

bool BigInt::isEven() const {
    const int lastDigit = digits.back() - '0';
    return (lastDigit % 2) == 0;
}


void BigInt::setSign(bool sign) {
    positive = sign;
}

BigInt::operator long long() const {
    assert(*this <= BigInt(INT64_MAX));
    return std::stoll(digits);
}



BigInt BigInt::abs(const BigInt &num) {
    BigInt result = num;
    result.positive = true;
    return std::move(result);
}

BigInt BigInt::gcd(const BigInt &lhs, const BigInt &rhs) {
    if(rhs == BigInt(0)) return lhs;
    return gcd(rhs, lhs % rhs);
}

BigInt BigInt::sqrt(const BigInt &num) {

    std::string digits = "1";
    for(int i = 0; i < num.getDigits().size() / 2; ++i) {
        digits.append("0");
    }
    digits.append("0");

    BigInt x = BigInt(digits);
    BigInt y = x + num/x;
    y /= BigInt(2);
    while(y < x) {
        x = std::move(y);
        y = x + num/x;
        y /= BigInt(2);
    }

    return std::move(x);
}

/**
 * Computes ceil(sqrt(num))
 */
BigInt BigInt::ceilSqrt(const BigInt &num) {
    auto res = sqrt(num);
    if(res*res < num) {
        return std::move(BigInt(1) + res);
    }
    return std::move(res);
}


BigInt BigInt::exp(const BigInt &base, const BigInt &exponent, const BigInt &modulus) {
    if(exponent <= BigInt(0)) return BigInt(1);
    if(exponent == BigInt(1)) return base % modulus;


    if((exponent % BigInt(2)) == BigInt(0)) {
        assert(((exponent.getDigits()[exponent.getDigits().size() - 1] - '0') % 2) == 0);

        BigInt res = exp(base, exponent / BigInt(2), modulus) % modulus;

        res *= res;
        res %= modulus;

        return std::move(res);
    }
    auto res = exp(base, exponent - 1, modulus) % modulus;
    res *= base;
    res %= modulus;
    return std::move(res);
}

/**
 * Approximates the value of log2(num)
 * reference: https://stackoverflow.com/questions/8232932/logarithm-of-the-very-very-large-number
 */
BigInt BigInt::log2(const BigInt &num) {
    assert(num > 0);
    if(num.getDigits().size() <= 8) {
        return static_cast<long long>(std::log(std::stoll(num.getDigits()))/std::log(2.0));
    }

    const auto msb = static_cast<long double>(num.getDigits()[0] - '0');
    const auto smsb = static_cast<long double>(num.getDigits()[1] - '0');
    long double res = std::log(msb*10.0l + smsb) / std::log(2.0l);
    res += 3.32192809l * static_cast<long double>(num.getDigits().size() - 2);
    const auto intRes = static_cast<long long>(res);
    return BigInt(intRes);
}

BigInt BigInt::modInverse(const BigInt &num, const BigInt &mod) {
    assert(num != 0);
    BigInt t = 0;
    BigInt r = mod;
    BigInt newT = 1;
    BigInt newR = num;

    while(newR != 0) {
        BigInt q = r / newR;
        BigInt updateT = t - q * newT;
        BigInt updateR = r - q * newR;
        t = std::move(newT);
        r = std::move(newR);
        newT = std::move(updateT);
        newR = std::move(updateR);
    }

    if(t < 0) t+= mod;

    return std::move(t);
}



bool BigInt::operator==(const BigInt &other) const {

    if(this->positive != other.positive) {
        return false;
    }

    return this->digits == other.digits;
}

bool BigInt::operator!=(const BigInt &other) const {
    return !(*this == other);
}

/**
 * Compares to digit strings.
 * @return -1 if lhs < rhs
 *          0 if lhs == rhs
 *          1 if lhs > rhs
 */
int BigInt::stringComp(const std::string &lhs, const std::string &rhs) {
    if(lhs.size() < rhs.size()) {
        return -1;
    }

    if(rhs.size() < lhs.size()) {
        return 1;
    }

    for(int i = 0; i < lhs.size(); ++i) {
        if(lhs[i] < rhs[i]) {
            return -1;
        }
        if(lhs[i] > rhs[i]) {
            return 1;
        }
    }
    return 0;
}


bool BigInt::operator<(const BigInt &rhs) const {

    if(this->positive != rhs.positive) {
        return !this->positive;
    }

    if(this->digits.size() != rhs.digits.size()) {
        if(this->digits.size() > rhs.digits.size()) {
            return !this->positive;
        }
        return this->positive;
    }

    const int result = stringComp(digits, rhs.digits);
    if(positive) {
        return result == -1;
    }
    return result == 1;
}

bool BigInt::operator>(const BigInt &rhs) const {
    return rhs < *this;
}

bool BigInt::operator<=(const BigInt &rhs) const {
    return !(*this > rhs);
}

bool BigInt::operator>=(const BigInt &rhs) const {
    return !(*this < rhs);
}

BigInt &BigInt::operator++() {
    *this += BigInt(1);
    return *this;
}

BigInt BigInt::operator++(int) {
    BigInt old = *this;
    operator++();
    return std::move(old);
}

BigInt operator+(BigInt lhs, const BigInt &rhs) {
    lhs += rhs;
    return std::move(lhs);
}

BigInt &BigInt::operator+=(const BigInt &rhs) {

    if(!rhs.isPositive()) {
        BigInt copy = rhs;
        copy.positive = true;
        *this -= copy;
        return *this;
    }

    if(!isPositive()) {
        BigInt copy = rhs;
        positive = true;
        copy -= *this;
        *this = copy;
        return *this;
    }

    const unsigned long long maxLen = std::max(rhs.digits.size(), digits.size());

    std::string additionalDigits;

    int carry = 0;
    for(int i = 0; i < maxLen; ++i) {

        int leftDigit = 0;
        if(i < digits.size()) {
            leftDigit = digits[digits.size() - i - 1] - '0';
        }
        int rightDigit = 0;
        if(i < rhs.digits.size()) {
            rightDigit = rhs.digits[rhs.digits.size() - i - 1] - '0';
        }

        const int sum = leftDigit + rightDigit + carry;
        carry = sum / 10;


        if(i < digits.size()) {
            digits[digits.size() - i - 1] = (sum % 10) + '0';
        } else {
            additionalDigits += (sum % 10) + '0';
        }
    }

    if(carry != 0) {
        additionalDigits += '1';
    }

    std::ranges::reverse(additionalDigits);
    this->digits.insert(0, additionalDigits);


    return *this;
}

BigInt &BigInt::operator-=(const BigInt &rhs) {

    if(!rhs.isPositive()) {
        BigInt copy = rhs;
        copy.positive = true;
        *this += copy;
        return *this;
    }

    if(!isPositive()) {
        positive = true;
        *this += rhs;
        positive = false;
        return *this;
    }

    if(*this < rhs) {
        BigInt result = rhs;
        result -= *this;
        result.positive = false;
        *this = std::move(result);
        return *this;
    }

    std::string resDigits;
    int carry = 0;
    for(int i = 0; i < digits.size(); ++i) {

        int leftDigit = digits[digits.size() - i - 1] - '0';

        int rightDigit = 0;
        if(i < rhs.digits.size()) {
            rightDigit = rhs.digits[rhs.digits.size() - i - 1] - '0';
        }

        int res = leftDigit - rightDigit - carry;
        carry = 0;
        if(res < 0) {
            carry = 1;
            res += 10;
        }

        resDigits += res + '0';
    }

    while(!resDigits.empty() && resDigits.back() == '0') {
        resDigits.pop_back();
    }
    if(resDigits.empty()) {
        resDigits = "0";
    }
    std::ranges::reverse(resDigits);
    digits = std::move(resDigits);



    return *this;
}

BigInt operator-(BigInt lhs, const BigInt &rhs) {
    lhs -= rhs;
    return std::move(lhs);
}


BigInt multiplyDigit(const BigInt &lhs, const char digit) {
    BigInt result;

    const int digitNum = digit - '0';
    std::string padding;
    for(int i = lhs.getDigits().size() - 1; i >= 0; --i) {

        const int lhsDigit = lhs.getDigits()[i] - '0';
        std::string res = std::to_string(lhsDigit * digitNum);

        res.append(padding);
        result += BigInt(res);

        padding += '0';
    }
    return std::move(result);
}

/**
 * Helper function for multiplication
 */
inline std::string extractHighNumber(const int middle, const int maxLen, const BigInt &number) {
    std::string high = "0";
    const int digitSize = number.getDigits().size();
    for(int i = 0; i < middle - (maxLen - digitSize); ++i) {
        high += number.getDigits()[i];
    }
    return std::move(high);
}


BigInt karatsubaMultiply(const BigInt &lhs, const BigInt &rhs) {

    if(lhs.getDigits().size() <= 9 && rhs.getDigits().size() <= 9) {
        const long long result = std::stoll(lhs.getDigits()) * std::stoll(rhs.getDigits());
        return BigInt(result);
    }

    if(lhs.getDigits().size() == 1) {
        BigInt res = multiplyDigit(rhs, lhs.getDigits()[0]);
        return std::move(res);
    }
    if(rhs.getDigits().size() == 1) {
        BigInt res = multiplyDigit(lhs, rhs.getDigits()[0]);
        return std::move(res);
    }

    int maxLen = std::max(lhs.getDigits().size(), rhs.getDigits().size());
    int middle = maxLen / 2;


    std::string high1 = extractHighNumber(middle, maxLen, lhs);
    std::string high2 = extractHighNumber(middle, maxLen, rhs);

    int lhsDigitSize = lhs.getDigits().size();
    int rhsDigitSIze = rhs.getDigits().size();
    int begin1 = std::max(0, middle - (maxLen - lhsDigitSize));
    int begin2 = std::max(0, middle - (maxLen - rhsDigitSIze));

    std::string low1 = lhs.getDigits().substr(begin1);
    std::string low2 = rhs.getDigits().substr(begin2);


    BigInt rhs1(std::move(high2));
    BigInt rhs0(std::move(low2));
    BigInt lhs1(std::move(high1));
    BigInt lhs0(std::move(low1));


    BigInt z3 = karatsubaMultiply(rhs1 + rhs0, lhs1 + lhs0);
    BigInt z2 = karatsubaMultiply(rhs1, lhs1);
    BigInt z0 = karatsubaMultiply(rhs0, lhs0);

    std::string padding;

    for(int i = 0; i < maxLen - middle; ++i) {
        padding += '0';
    }

    BigInt result;

    result += z0;

    BigInt z1 = z3 - z2 - z0;
    std::string digits = z1.getDigits();
    digits.append(padding);
    result += BigInt(std::move(digits));


    digits = z2.getDigits();
    padding += padding;
    digits.append(padding);
    result += BigInt(std::move(digits));

    return result;
}


BigInt operator*(const BigInt& lhs, const BigInt &rhs) {
    BigInt result = karatsubaMultiply(lhs, rhs);
    if(lhs.isPositive() != rhs.isPositive()) {
        result.setSign(false);
    }

    return std::move(result);
}

BigInt &BigInt::operator*=(const BigInt &rhs) {
    *this = std::move(*this * rhs);
    return *this;
}

/**
 * Divides lhs by rhs. Assumes, that rhs has at least as many digits as lhs.
 * Therefore, the result can be at most 9
 */
long long smallDivide(const BigInt &lhs, const BigInt &rhs) {
    if(lhs.getDigits().size() < rhs.getDigits().size()) {
        return 0;
    }

    long long result = 0;
    BigInt rhsCopy = rhs;
    while(BigInt::stringComp(rhsCopy.getDigits(), lhs.getDigits()) <= 0) {
        result++;
        rhsCopy += rhs;
    }

    return result;
}

BigInt &BigInt::operator/=(BigInt rhs) {
    const bool sign = isPositive() == rhs.isPositive();

    // calculations are done with the absolute values
    setSign(true);
    rhs.setSign(true);

    if(*this < rhs) {
        *this = BigInt(0);
        return *this;
    }

    if(getDigits() == rhs.getDigits()) {
        *this = BigInt(1);
        return *this;
    }

    std::string resultDigits;

    int index = rhs.getDigits().size();

    auto newDigits = BigInt(getDigits().substr(0, index));
    index--;

    while(index < getDigits().size()) {
        std::string digits = newDigits.getDigits();
        bool added = false;
        while(index + 1 < getDigits().size() && stringComp(digits, rhs.getDigits()) == -1) {

            if(added) resultDigits.append("0");

            digits += getDigits()[++index];
            added = true;
        }


        newDigits = BigInt(std::move(digits));
        const long long cnt = smallDivide(newDigits, rhs);
        newDigits = newDigits - rhs * BigInt(cnt);


        resultDigits += std::to_string(cnt);
        if(index == getDigits().size() - 1) break;
    }

    digits = std::move(resultDigits);

    // Set sign to expected value
    setSign(sign);

    return *this;
}

BigInt operator/(BigInt lhs, const BigInt &rhs) {
    lhs /= rhs;
    return std::move(lhs);
}


BigInt &BigInt::operator%=(const BigInt &rhs) {
    *this = *this % rhs;
    return *this;
}


BigInt operator%(const BigInt &lhs, const BigInt &rhs) {
    if(rhs == 0) return lhs;
    BigInt res = lhs - rhs * (lhs/rhs);
    if(!res.isPositive()) {
        // Make result positive
        res += ((res/rhs) + BigInt(1)) * rhs;
    }
    return std::move(res);
}







