#include "gtest/gtest.h"
#include "quadratic_sieve.h"


TEST(QuadraticSieveTest, isQuadraticResidueTest) {

    bool res = isQuadraticResidue(15347, 3);
    ASSERT_FALSE(res);

}


TEST(QuadraticSieveTest, tonelliShanksTest) {

    BigInt res = tonelliShanks(5, 41);
    ASSERT_EQ(res.getDigits(), "28");
    ASSERT_TRUE(res.isPositive());

    res = tonelliShanks(11, 19);
    ASSERT_EQ(res.getDigits(), "7");
    ASSERT_TRUE(res.isPositive());

    res = tonelliShanks(909, 19927);
    ASSERT_EQ(res.getDigits(), "1231");
    ASSERT_TRUE(res.isPositive());

    res = tonelliShanks(BigInt("19641285"), BigInt("39916801"));
    ASSERT_EQ(res.getDigits(), "231232");
    ASSERT_TRUE(res.isPositive());

    res = tonelliShanks(BigInt("14491491703446199"), BigInt("99194853094755497"));
    ASSERT_EQ(res.getDigits(), "1283182731827323");
    ASSERT_TRUE(res.isPositive());

    res = tonelliShanks(BigInt("5977434972254961629160107345"),
        BigInt("10888869450418352160768000001"));
    ASSERT_EQ(res.getDigits(), "10888741079134632929536768770");
    ASSERT_TRUE(res.isPositive());

    res = tonelliShanks(BigInt("21009439513550995"), BigInt("99194853094755497"));
    ASSERT_EQ(res.getDigits(), "1231237812382892");
    ASSERT_TRUE(res.isPositive());

}

TEST(QuadraticSieveTest, computeFactorsTest) {
    const std::vector<BigInt> factorBase = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};

    BigInt num = 1;
    for(int i = 0; i < factorBase.size(); ++i) {
        num *= factorBase[i];

        auto res = computeFactors(num, factorBase);
        ASSERT_EQ(res.size(), factorBase.size());

        for(int j = 0; j < res.size(); ++j) {
            if(j <= i) ASSERT_EQ(res[j], 1);
            else ASSERT_EQ(res[j], 0);
        }
    }

    for(int i = 0; i < factorBase.size(); ++i) {
        num *= factorBase[i];

        auto res = computeFactors(num, factorBase);
        ASSERT_EQ(res.size(), factorBase.size());

        for(int j = 0; j < res.size(); ++j) {
            if(j <= i) ASSERT_EQ(res[j], 2);
            else ASSERT_EQ(res[j], 1);
        }
    }
}

TEST(QuadraticSieveTest, computeLinearDependencyTest) {
    std::vector<std::vector<int>> factorizationExponents = {
        {1, 1, 1},
        {0, 1, 0},
        {1, 0, 0}
    };

    auto res = computeLinearDependency(factorizationExponents);
    ASSERT_EQ(res.size(), 0);

    factorizationExponents.push_back({0, 0, 1});

    res = computeLinearDependency(factorizationExponents);
    ASSERT_EQ(res.size(), 4);

    for(int i = 0; i < 4; ++i) {
        ASSERT_TRUE(res.contains(i));
    }

    factorizationExponents = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 1, 0, 0, 0, 1, 0, 0},
    };

    res = computeLinearDependency(factorizationExponents);
    ASSERT_EQ(res.size(), 3);
    ASSERT_TRUE(res.contains(2));
    ASSERT_TRUE(res.contains(6));
    ASSERT_TRUE(res.contains(9));

    factorizationExponents.pop_back();
    factorizationExponents.push_back({1, 1, 1, 1, 1, 1, 1, 1, 1});
    res = computeLinearDependency(factorizationExponents);

    ASSERT_EQ(res.size(), 10);
    for(int i = 0; i < 10; ++i) {
        ASSERT_TRUE(res.contains(i));
    }
}

