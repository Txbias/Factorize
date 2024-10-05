#include "gtest/gtest.h"
#include "poly_generator.h"

#include "utils.h"

TEST(PolyGeneratorTest, basic) {
    const auto number = BigInt(291);
    const std::vector<BigInt> basePrimes = {5, 7, 11};

    PolyGenerator generator(number, basePrimes, basePrimes);

    std::vector<Polynomial> res;
    while(generator.hasNext()) {
        res.emplace_back(generator.next());
    }

    ASSERT_EQ(res.size(), 4);

    for(auto & re : res) {
        ASSERT_EQ(re.a, 385);
    }

    ASSERT_EQ(res[0].b, 334);
    ASSERT_EQ(res[1].b, 26);
    ASSERT_EQ(res[2].b, -194);
    ASSERT_EQ(res[3].b, 114);
}

TEST(PolyGeneratorTest, findSolutionTest) {

    const auto number = BigInt(291);
    const std::vector<BigInt> basePrimes = {5, 7, 11, 19, 29};
    const std::vector<BigInt> factorBase = {17, 41, 47, 61, 67, 73};

    for(const auto &pr : basePrimes) {
        ASSERT_TRUE(isQuadraticResidue(number, pr));
    }

    for(const auto &pr : factorBase) {
        ASSERT_TRUE(isQuadraticResidue(number, pr));
    }


    PolyGenerator generator(number, basePrimes, factorBase);

    std::vector<std::pair<BigInt, BigInt>> lastSolutions(factorBase.size(), {-1, -1});

    while(generator.hasNext()) {
        auto polynomial = generator.next();
        ASSERT_EQ(polynomial.a, 212135);
        BigInt b = polynomial.b;

        auto nextSolutions = generator.findSolutions(lastSolutions, polynomial);

        for(int i = 0; i < factorBase.size(); i++) {

            auto sol = tonelliShanks(number, factorBase[i]);
            auto solI = sol - b;
            auto sol2 = sol * BigInt(-1) - b;
            auto aInv = BigInt::modInverse(polynomial.a, factorBase[i]);
            ASSERT_TRUE(aInv >= 0);
            solI *= aInv;
            sol2 *= aInv;
            solI %= factorBase[i];
            sol2 %= factorBase[i];

            ASSERT_EQ(polynomial(solI) % factorBase[i], 0);
            ASSERT_EQ(polynomial(sol2) % factorBase[i], 0);

            if(lastSolutions[i].first == -1) {
                lastSolutions[i].first = solI;
                lastSolutions[i].second = sol2;
                continue;
            }

            ASSERT_EQ(nextSolutions[i].first, solI);
            ASSERT_EQ(nextSolutions[i].second, sol2);
            ASSERT_EQ(polynomial(nextSolutions[i].first) % factorBase[i], 0);
            ASSERT_EQ(polynomial(nextSolutions[i].second) % factorBase[i], 0);
            lastSolutions[i] = {solI, sol2};
        }
    }

}