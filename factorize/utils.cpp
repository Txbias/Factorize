#include "utils.h"

#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>


std::vector<BigInt> generatePrimes(long long limit) {

    std::vector<BigInt> primes;
    std::vector<bool> isPrime(limit, true);
    for(long long i = 2LL; i < limit; ++i) {
        if(isPrime[i]) {
            primes.push_back(i);
        }
        for(long long j = 2LL*i; j < limit; j += i) {
            isPrime[j] = false;
        }
    }

    return std::move(primes);
}

/**
 * Approximates the value of the nth prime
 * reference: https://en.wikipedia.org/wiki/Prime_number_theorem
 */
long long nthPrime(const long long n) {

    long double logn = std::log(n);
    long double loglogn = std::log(logn);

    long double res = logn;
    res += loglogn - 1.0l;
    res += (loglogn - 2.0l) / logn;
    res -= (loglogn*loglogn - 6.0l*loglogn+ 11.0) / (2.0*logn*logn);
    res *= n;
    assert(res < INT64_MAX);
    return static_cast<long long>(res);
}

std::vector<BigInt> generateFactorBase(const long long amount, const BigInt& number) {

    const long long limit = nthPrime(amount);

    std::vector<BigInt> primes = generatePrimes(limit);
    std::vector<BigInt> factorBase;

    for(const auto & prime : primes) {
        if(prime != 2 && isQuadraticResidue(number, prime)) {
            factorBase.emplace_back(prime);
        }
    }
    return std::move(factorBase);
}

bool isQuadraticResidue(const BigInt& number, const BigInt& prime) {
    const BigInt exponent = (prime - 1) / 2;

    BigInt res = BigInt::exp(number, exponent, prime);
    return res == BigInt(1);
}

BigInt tonelliShanks(const BigInt& number, const BigInt& prime) {
    if(!isQuadraticResidue(number, prime)) {
        std::cerr << "Not a quadratic residue" << std::endl;
        return BigInt("-1");
    }

    BigInt q = prime - BigInt(1);
    long s = 0;
    while((q % BigInt(2)) == BigInt(0)) {
        q /= BigInt(2);
        s++;
    }

    BigInt z;
    for(z = BigInt(1); z < prime; ++z) {
        if(!isQuadraticResidue(z, prime)) {
            break;
        }
    }


    BigInt m = s;
    BigInt c = BigInt::exp(z, q, prime);
    BigInt t = BigInt::exp(number, q, prime);
    BigInt r = BigInt::exp(number, (BigInt(1) + q) / BigInt(2), prime);

    while(t != BigInt(0) && t != BigInt(1)) {
        auto exponent = BigInt(1);
        BigInt i;

        for(i = BigInt(1); i < m; ++i) {
            exponent *= BigInt(2);

            if(BigInt::exp(t, exponent, prime) == BigInt(1)) {
                break;
            }
        }


        exponent = BigInt::exp(BigInt(2), m - i - BigInt(1), BigInt(0));
        BigInt b = BigInt::exp(c, exponent, prime);
        m = i;

        c = (b * b) % prime;
        t *= c;
        t %= prime;
        r *= b;
        r %= prime;
    }

    if(t == BigInt(0)) return {0};
    return r;
}

