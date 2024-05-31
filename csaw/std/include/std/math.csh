## MATH ##

## Types
def double_t = flt64;
def float_t  = flt32;

## Constants
math_errhandling: int32;
INFINITY: flt32;
NAN: flt32;
HUGE_VAL: flt16;
HUGE_VALF: flt32;
HUGE_VALL: flt64;

MATH_ERRNO:     int32;
MATH_ERREXCEPT: int32;

## Trigonometric functions
@[c]cos(x: flt64): flt64;
@[c]sin(x: flt64): flt64;
@[c]tan(x: flt64): flt64;
@[c]acos(x: flt64): flt64;
@[c]asin(x: flt64): flt64;
@[c]atan(x: flt64): flt64;
@[c]atan2(y: flt64, x: flt64): flt64;

## Hyperbolic functions
@[c]cosh(x: flt64): flt64;
@[c]sinh(x: flt64): flt64;
@[c]tanh(x: flt64): flt64;
@[c]acosh(x: flt64): flt64;
@[c]asinh(x: flt64): flt64;
@[c]atanh(x: flt64): flt64;

## Exponential and logarithmic functions
@[c]exp(x: flt64): flt64;
@[c]frexp(x: flt64, exp: int32*): flt64;
@[c]ldexp(x: flt64, exp: int32): flt64;
@[c]log(x: flt64): flt64;
@[c]log10(x: flt64): flt64;
@[c]modf(x: flt64, intpart: flt64*): flt64;
@[c]exp2(x: flt64): flt64;
@[c]expm1(x: flt64): flt64;
@[c]ilogb(x: flt64): int32;
@[c]log1p(x: flt64): flt64;
@[c]log2(x: flt64): flt64;
@[c]logb(x: flt64): flt64;
@[c]scalbn(x: flt64, n: int32): flt64;
@[c]scalbln(x: flt64, n: int64): flt64;

## Power functions
@[c]pow(base: flt64, exponent: flt64): flt64;
@[c]sqrt(x: flt64): flt64;
@[c]cbrt(x: flt64): flt64;
@[c]hypot(x: flt64, y: flt64): flt64;

## Error and gamma functions
@[c]erf(x: flt64): flt64;
@[c]erfc(x: flt64): flt64;
@[c]tgamma(x: flt64): flt64;
@[c]lgamma(x: flt64): flt64;

## Rounding and remainder functions
@[c]ceil(x: flt64): flt64;
@[c]floor(x: flt64): flt64;
@[c]fmod(numer: flt64, denom: flt64): flt64;
@[c]trunc(x: flt64): flt64;
@[c]round(x: flt64): flt64;
@[c]lround(x: flt64): int64;
@[c]llround(x: flt64): int128;
@[c]rint(x: flt64): flt64;
@[c]lrint(x: flt64): int64;
@[c]llrint(x: flt64): int128;
@[c]nearbyint(x: flt64): flt64;
@[c]remainder(numer: flt64, denom: flt64): flt64;
@[c]remquo(numer: flt64, denom: flt64, quot: int32*): flt64;

## Floating-point manipulation functions
@[c]copysign(x: flt64, y: flt64): flt64;
@[c]nan(tagp: int8*): flt64;
@[c]nextafter(x: flt64, y: flt64): flt64;
@[c]nexttoward(x: flt64, y: flt64): flt64;

## Minimum, maximum, difference functions
@[c]fdim(x: flt64, y: flt64): flt64;
@[c]fmax(x: flt64, y: flt64): flt64;
@[c]fmin(x: flt64, y: flt64): flt64;

## Other functions
@[c]fabs(x: flt64): flt64;
@[c]abs(x: flt64): flt64;
@[c]fma(x: flt64, y: flt64, z: flt64): flt64;

## Classification functions
@[c]fpclassify(x: flt64): int32;
@[c]isfinite(x: flt64): int1;
@[c]isinf(x: flt64): int1;
@[c]isnan(x: flt64): int1;
@[c]isnormal(x: flt64): int1;
@[c]signbit(x: flt64): int1;

## Comparison functions
@[c]isgreater(x: flt64, y: flt64): int1;
@[c]isgreaterequal(x: flt64, y: flt64): int1;
@[c]isless(x: flt64, y: flt64): int1;
@[c]islessequal(x: flt64, y: flt64): int1;
@[c]islessgreater(x: flt64, y: flt64): int1;
@[c]isunordered(x: flt64, y: flt64): int1;
