#project(factorizeLib)


set(HEADER_FILES utils.h number.h factorize.h big_int.h quadratic_sieve.h)
set(SOURCE_FILES utils.cpp factorize.cpp big_int.cpp quadratic_sieve.cpp)

add_library(factorize STATIC ${HEADER_FILES} ${SOURCE_FILES})

