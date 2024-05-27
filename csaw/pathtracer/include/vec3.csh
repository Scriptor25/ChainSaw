\inc "std/stdbool.csh"

def vec3 {
    x: flt64,
    y: flt64,
    z: flt64
}

def point3 = vec3;

@vec3;
@vec3(s: flt64);
@vec3(x: flt64, y: flt64, z: flt64);

@"-":vec3:vec3;
@"[]":vec3(i: int32): flt64*;
@"+=":vec3(v: vec3): vec3;
@"*=":vec3(t: flt64): vec3;
@"/=":vec3(t: flt64): vec3;
@length_squared:vec3:flt64;
@length:vec3:flt64;
@near_zero:vec3:bool;

@"+"(u: vec3, v: vec3): vec3;
@"-"(u: vec3, v: vec3): vec3;
@"*"(u: vec3, v: vec3): vec3;
@"*"(t: flt64, v: vec3): vec3;
@"*"(v: vec3, t: flt64): vec3;
@"/"(v: vec3, t: flt64): vec3;
@dot(u: vec3, v: vec3): flt64;
@cross(u: vec3, v: vec3): vec3;
@unit_vector(v: vec3): vec3;
@random_vec3::vec3;
@random_vec3(min: flt64, max: flt64): vec3;
@random_in_unit_sphere::vec3;
@random_in_unit_disk::vec3;
@random_unit_vector::vec3;
@random_on_hemisphere(normal: vec3): vec3;
@reflect(v: vec3, n: vec3): vec3;
@refract(uv: vec3, n: vec3, etai_over_etat: flt64): vec3;
