#include "gtest/gtest.h"
#include "big_int.h"
#include "utils.h"



class BigIntTest : public testing::Test {
protected:
    BigIntTest() {
        small = BigInt("1234");
        big = BigInt("12837129837");
        overflow = BigInt("1928371982738917238712323123123124556756");
        negOverflow = BigInt("-987982734987234792749827394872938479143234");
        negative = BigInt("-1234");
        bigNegative = BigInt("-12837129837");
    }

    BigInt small;
    BigInt big;
    BigInt overflow;
    BigInt negOverflow;
    BigInt negative;
    BigInt bigNegative;
    BigInt zero;
};


TEST_F(BigIntTest, constructorTest) {


    ASSERT_TRUE(zero.isPositive());
    ASSERT_EQ(zero.getDigits(), "0");

    ASSERT_FALSE(negative.isPositive());
    ASSERT_EQ(negative.getDigits(), "1234");


    ASSERT_TRUE(big.isPositive());
    ASSERT_EQ(big.getDigits(), "12837129837");


    ASSERT_TRUE(overflow.isPositive());
    ASSERT_EQ(overflow.getDigits(), "1928371982738917238712323123123124556756");


    ASSERT_FALSE(negOverflow.isPositive());
    ASSERT_EQ(negOverflow.getDigits(), "987982734987234792749827394872938479143234");

    const BigInt leadingZeroes("-00000234234");
    ASSERT_FALSE(leadingZeroes.isPositive());
    ASSERT_EQ(leadingZeroes.getDigits(), "234234");

    const BigInt fromLongPos(183812398129);
    const BigInt fromLongNeg(-183812398129);

    ASSERT_EQ(fromLongPos.getDigits(), "183812398129");
    ASSERT_EQ(fromLongNeg.getDigits(), "183812398129");
    ASSERT_TRUE(fromLongPos.isPositive());
    ASSERT_FALSE(fromLongNeg.isPositive());


}

TEST_F(BigIntTest, equalityTest) {
    const BigInt num("18292");
    const BigInt num2("18292");

    ASSERT_TRUE(num == num2);
    ASSERT_FALSE(num != num2);

    ASSERT_FALSE(num == negative);
    ASSERT_FALSE(num2 == negative);
    ASSERT_TRUE(negative != num);
    ASSERT_TRUE(negative != num);

}

TEST_F(BigIntTest, inequalityTest) {
    const BigInt num2("812");
    const BigInt num3("812");

    ASSERT_TRUE(big >= num2);
    ASSERT_TRUE(big > num2);
    ASSERT_FALSE(big < num2);
    ASSERT_TRUE(num3 >= num2);
    ASSERT_TRUE(num2 <= num3);

    ASSERT_TRUE(negative < big);
    ASSERT_TRUE(negative <= big);

    ASSERT_TRUE(bigNegative < negative);
    ASSERT_TRUE(bigNegative < big);
}

TEST_F(BigIntTest, additionTest) {

    BigInt res1 = small + big;
    BigInt res2 = big + small;

    ASSERT_EQ(res1.getDigits(), "12837131071");
    ASSERT_EQ(res2.getDigits(), "12837131071");

    res1 = overflow + small;
    res2 = small + overflow;

    ASSERT_EQ(res1.getDigits(), "1928371982738917238712323123123124557990");
    ASSERT_EQ(res2.getDigits(), "1928371982738917238712323123123124557990");

    res1 += res2;

    ASSERT_EQ(res1.getDigits(), "3856743965477834477424646246246249115980");

    BigInt num1(2106);
    num1 += BigInt("9000");
    ASSERT_EQ(num1.getDigits(), "11106");

}

TEST_F(BigIntTest, subtractionTest) {


    BigInt res1 = small - big;
    ASSERT_FALSE(res1.isPositive());
    ASSERT_EQ(res1.getDigits(), "12837128603");

    res1 = big - small;
    ASSERT_TRUE(res1.isPositive());
    ASSERT_EQ(res1.getDigits(), "12837128603");

    res1 = big - big;
    ASSERT_EQ(res1.getDigits(), "0");

    res1 = negative - negative;
    ASSERT_EQ(res1.getDigits(), "0");

    res1 = negative - bigNegative;
    ASSERT_TRUE(res1.isPositive());
    ASSERT_EQ(res1.getDigits(), "12837128603");

    res1 = BigInt("123499") - BigInt("123490");
    ASSERT_EQ(res1.getDigits(), "9");
    ASSERT_TRUE(res1.isPositive());

    BigInt num(128);
    BigInt num2(123);
    num = num - num2;
    ASSERT_EQ(num.getDigits(), "5");
    ASSERT_TRUE(num.isPositive());
}

TEST_F(BigIntTest, absoluteValueTest) {

    ASSERT_EQ(BigInt::abs(small).getDigits(), "1234");
    ASSERT_TRUE(BigInt::abs(small).isPositive());
    ASSERT_EQ(BigInt::abs(bigNegative).getDigits(), "12837129837");
    ASSERT_TRUE(BigInt::abs(bigNegative).isPositive());
}

TEST_F(BigIntTest, isEvenTest) {
    ASSERT_TRUE(small.isEven());
    ASSERT_FALSE(big.isEven());
    ASSERT_TRUE(overflow.isEven());
    ASSERT_TRUE(BigInt(0).isEven());
    ASSERT_FALSE(BigInt(1).isEven());
}

TEST_F(BigIntTest, multiplyDigitTest) {

    BigInt result = multiplyDigit(small, '0');
    ASSERT_EQ(result.getDigits(), "0");

    result = multiplyDigit(small, '1');
    ASSERT_EQ(result.getDigits(), small.getDigits());

    result = multiplyDigit(small, '9');
    ASSERT_EQ(result.getDigits(), "11106");

    result = multiplyDigit(overflow, '7');
    ASSERT_EQ(result.getDigits(), "13498603879172420670986261861861871897292");
}

TEST_F(BigIntTest, karatsubaMultiplyTest) {

    BigInt res;
    BigInt res2;

    res = overflow * overflow;
    res2 = overflow;
    res2 *= overflow;
    ASSERT_EQ(res.getDigits(), "3718618503812422924845298023188983776999628915336117586444008994452361465243536");
    ASSERT_EQ(res.getDigits(), res2.getDigits());
    ASSERT_TRUE(res.isPositive());
    ASSERT_TRUE(res2.isPositive());

    res = big * small;
    res2 = big;
    res2 *= small;
    ASSERT_EQ(res.getDigits(), "15841018218858");
    ASSERT_EQ(res.getDigits(), res2.getDigits());
    ASSERT_TRUE(res.isPositive());
    ASSERT_TRUE(res2.isPositive());

    res = overflow * bigNegative;
    res2 = overflow;
    res2 *= bigNegative;
    ASSERT_EQ(res.getDigits(), "24754761516452603466147614623428886872199847528772");
    ASSERT_EQ(res.getDigits(), res2.getDigits());
    ASSERT_FALSE(res.isPositive());
    ASSERT_FALSE(res2.isPositive());

    res = small * big * negative * bigNegative;
    res2 = small;
    res2 *= big;
    res2 *= negative;
    res2 *= bigNegative;
    ASSERT_EQ(res.getDigits(), "250937858210191082786824164");
    ASSERT_EQ(res.getDigits(), res2.getDigits());
    ASSERT_TRUE(res.isPositive());
    ASSERT_TRUE(res2.isPositive());
}

TEST_F(BigIntTest, smallDivideTest) {

    auto res = smallDivide(BigInt(40), BigInt(20));
    ASSERT_EQ(res, 2);

    res = smallDivide(BigInt(10), BigInt(20));
    ASSERT_EQ(res, 0);

    res = smallDivide(small, small);
    ASSERT_EQ(res, 1);

    res = smallDivide(BigInt(90), BigInt(10));
    ASSERT_EQ(res, 9);

    res = smallDivide(overflow, overflow);
    ASSERT_EQ(res, 1);

    res = smallDivide(BigInt(0), overflow);
    ASSERT_EQ(res, 0);

}

TEST_F(BigIntTest, divisionTest) {

    BigInt res;

    res = BigInt(400) / BigInt(20);
    ASSERT_EQ(res.getDigits(), "20");
    ASSERT_TRUE(res.isPositive());

    res = BigInt(-100) / BigInt(2);
    ASSERT_EQ(res.getDigits(), "50");
    ASSERT_FALSE(res.isPositive());

    res = small / big;
    ASSERT_EQ(res.getDigits(), "0");

    res = small / small;
    ASSERT_EQ(res.getDigits(), "1");
    ASSERT_TRUE(res.isPositive());

    res = big / small;
    ASSERT_EQ(res.getDigits(), "10402860");
    ASSERT_TRUE(res.isPositive());


    res = overflow / big;
    ASSERT_EQ(res.getDigits(), "150218312599818042855580967753");
    ASSERT_TRUE(res.isPositive());

    res = BigInt(128736) / BigInt(-123);
    ASSERT_EQ(res.getDigits(), "1046");
    ASSERT_FALSE(res.isPositive());

    res = BigInt(-18231238348) / BigInt(-20932);
    ASSERT_EQ(res.getDigits(), "870974");
    ASSERT_TRUE(res.isPositive());

    res = BigInt("-823987123") / BigInt("1232312");
    ASSERT_EQ(res.getDigits(), "668");
    ASSERT_FALSE(res.isPositive());

    res = BigInt("980000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    res /= BigInt("100000");
    ASSERT_EQ(res.getDigits(),
                "9800000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    res = small / BigInt(1233);
    ASSERT_EQ(res.getDigits(), "1");
    ASSERT_TRUE(res.isPositive());

    res = small / BigInt(1235);
    ASSERT_EQ(res.getDigits(), "0");

    res = BigInt(4) / BigInt(2);
    ASSERT_EQ(res.getDigits(), "2");
    ASSERT_TRUE(res.isPositive());

    res = BigInt(3) / BigInt(1);
    ASSERT_EQ(res.getDigits(), "3");
    ASSERT_TRUE(res.isPositive());
}

TEST_F(BigIntTest, moduloTest) {

    BigInt res;

    res = BigInt(10) % BigInt(2);
    ASSERT_EQ(res.getDigits(), "0");
    ASSERT_TRUE(res.isPositive());

    res = big % small;
    BigInt copy = big;
    copy %= small;
    ASSERT_EQ(res.getDigits(), "597");
    ASSERT_EQ(copy, res);
    ASSERT_TRUE(res.isPositive());

    res = overflow % big;
    copy = overflow;
    copy %= big;
    ASSERT_EQ(res.getDigits(), "11753410495");
    ASSERT_EQ(copy, res);
    ASSERT_TRUE(res.isPositive());

    res = BigInt(2) % BigInt(1);
    ASSERT_EQ(res.getDigits(), "0");


    for(long long i = 1; i <= 200; ++i) {
        for(long long j = 1; j <= 200; ++j) {
            long long resNum = i % j;
            res = BigInt(i) % BigInt(j);
            ASSERT_EQ(res.getDigits(), std::to_string(resNum));
            ASSERT_EQ(res.isPositive(), resNum >= 0);
        }
    }

}

TEST_F(BigIntTest, sqrtTest) {

    BigInt res = BigInt::sqrt(small);
    ASSERT_EQ(res.getDigits(), "35");
    ASSERT_TRUE(res.isPositive());

    res = BigInt::sqrt(big);
    ASSERT_EQ(res.getDigits(), "113301");
    ASSERT_TRUE(res.isPositive());

    res = BigInt::sqrt(overflow);
    ASSERT_EQ(res.getDigits(), "43913232433275932081");
    ASSERT_TRUE(res.isPositive());
}

TEST_F(BigIntTest, ceilSqrtTest) {

    BigInt res = BigInt::ceilSqrt(1);
    ASSERT_EQ(res.getDigits(), "1");
    ASSERT_TRUE(res.isPositive());

    res = BigInt::ceilSqrt(2);
    ASSERT_EQ(res.getDigits(), "2");
    ASSERT_TRUE(res.isPositive());

    res = BigInt::ceilSqrt(4);
    ASSERT_EQ(res.getDigits(), "2");
    ASSERT_TRUE(res.isPositive());

    res = BigInt::ceilSqrt(BigInt("12312312319384"));
    ASSERT_EQ(res.getDigits(), "3508891");
    ASSERT_TRUE(res.isPositive());

}

TEST_F(BigIntTest, expTest) {

    BigInt res = BigInt::exp(BigInt(2), BigInt(10), overflow);
    ASSERT_EQ(res.getDigits(), "1024");
    ASSERT_TRUE(res.isPositive());

    res = BigInt::exp(small, BigInt(3), overflow);
    ASSERT_EQ(res.getDigits(), "1879080904");
    ASSERT_TRUE(res.isPositive());

    res = BigInt::exp(small, 0, overflow);
    ASSERT_EQ(res.getDigits(), "1");
    ASSERT_TRUE(res.isPositive());

    res = BigInt::exp(overflow, 1, overflow + small);
    ASSERT_EQ(res.getDigits(), overflow.getDigits());
    ASSERT_TRUE(res.isPositive());

    res = BigInt::exp(big, small, 87178291199);
    ASSERT_EQ(res.getDigits(), "19272719186");
    ASSERT_TRUE(res.isPositive());

    res = BigInt::exp(182, BigInt("912392"),
        BigInt("10888869450418352160768000001"));
    ASSERT_EQ(res.getDigits(), "1412547413236019664049634774");
    ASSERT_TRUE(res.isPositive());
}

TEST_F(BigIntTest, modInverseTest) {

    const std::vector<BigInt> primes = generatePrimes(1000);

    for(const auto &prime : primes) {
        for(int i = 1; i < static_cast<long long>(prime); ++i) {
            BigInt inv = BigInt::modInverse(i, prime);
            ASSERT_EQ((i * inv) % prime, 1);
        }
    }

}

