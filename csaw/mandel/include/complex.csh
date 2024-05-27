def complex {
    real: flt64,
    imaginary: flt64,
}

@complex(real: flt64, imaginary: flt64);

@"+=":complex(z: complex): complex;
@"-=":complex(z: complex): complex;
@"*=":complex(z: complex): complex;
@"/=":complex(z: complex): complex;

@conjugate:complex:complex;
@mod:complex:flt64;
@square:complex:complex;
@arg:complex:flt64;
@inverse:complex:complex;
@abs:complex:flt64;
@print:complex:void;

@"+"(z1: complex, z2: complex): complex;
@"-"(z1: complex, z2: complex): complex;
@"*"(z1: complex, z2: complex): complex;
@"/"(z1: complex, z2: complex): complex;

@exp(z: complex): complex;
@sin(z: complex): complex;
@cos(z: complex): complex;
@tan(z: complex): complex;
@cot(z: complex): complex;
@sec(z: complex): complex;
@cosec(z: complex): complex;
