\inc "std/stdbool.csh"

\inc "aabb.csh"
\inc "hittable.csh"
\inc "interval.csh"
\inc "material.csh"
\inc "ray.csh"
\inc "vec3.csh"

def sphere {
    type: int8,
    center1: point3,
    radius: flt64,
    mat: material*,
    is_moving: bool,
    center_vec: vec3,
    bbox: aabb
}

@sphere(center: point3, radius: flt64, mat: material*);
@sphere(center1: point3, center2: point3, radius: flt64, mat: material*);

@center:sphere(time: flt64): point3;
@hit:sphere(r: ray, ray_t: interval, rec: hit_record*): bool;
@bounding_box:sphere:aabb;
