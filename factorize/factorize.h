#pragma once
#include "number.h"
#include "big_int.h"


void runFactorization(const BigInt &number);

Number preprocessNumber(const BigInt &num);

Number pollardRho(Number number);


