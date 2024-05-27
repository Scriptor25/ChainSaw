\inc "std/stdbool.csh"

def interval {
    min: flt64,
    max: flt64
}

@interval;
@interval(min: flt64, max: flt64);
@interval(a: interval, b: interval);

@size:interval:flt64;
@contains:interval(x: flt64): bool;
@surrounds:interval(x: flt64): bool;
@clamp:interval(x: flt64): flt64;
@expand:interval(delta: flt64): interval;
