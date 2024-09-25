#pragma once

#include <string>

class BigInt {
public:
    BigInt();
    explicit BigInt(std::string number);
    BigInt(long long number);

    bool operator==(const BigInt &other) const;
    bool operator!=(const BigInt &other) const;

    bool operator<(const BigInt &rhs) const;
    bool operator>(const BigInt &rhs) const;
    bool operator<=(const BigInt &rhs) const;
    bool operator>=(const BigInt &rhs) const;

    BigInt& operator++();
    BigInt operator++(int);

    friend BigInt operator+(BigInt lhs, const BigInt &rhs);
    BigInt& operator+=(const BigInt &rhs);

    friend BigInt operator-(BigInt lhs, const BigInt &rhs);
    BigInt& operator-=(const BigInt &rhs);

    friend BigInt operator*(const BigInt& lhs, const BigInt &rhs);
    BigInt& operator*=(const BigInt &rhs);

    friend BigInt operator/(BigInt lhs, const BigInt &rhs);
    BigInt& operator/=(const BigInt &rhs);

    friend BigInt operator%(const BigInt &lhs, const BigInt &rhs);
    BigInt& operator%=(const BigInt &rhs);

    explicit operator long long() const;

    [[nodiscard]] const std::string& getDigits() const;
    [[nodiscard]] bool isPositive() const;
    [[nodiscard]] bool isEven() const;

    void setSign(bool sign);

    static BigInt abs(const BigInt &num);
    static BigInt gcd(const BigInt &lhs, const BigInt &rhs);
    static BigInt sqrt(const BigInt &num);
    static BigInt ceilSqrt(const BigInt &num);
    static BigInt exp(const BigInt &base, const BigInt &exponent, const BigInt &modulus);
    static BigInt log2(const BigInt &num);
    static BigInt modInverse(const BigInt &num, const BigInt &mod);
    static int stringComp(const std::string &lhs, const std::string &rhs);

private:
    std::string digits = "0";
    bool positive = true;

};

std::ostream &operator<<(std::ostream &os, const BigInt &obj);

BigInt multiplyDigit(const BigInt &lhs, char digit);
long long smallDivide(const BigInt &lhs, const BigInt &rhs);



