\inc "std/stdbool.csh"

\inc "aabb.csh"
\inc "interval.csh"
\inc "material.csh"
\inc "ray.csh"
\inc "vec3.csh"

def hit_record {
    p: point3,
    normal: vec3,
    mat: material*,
    t: flt64,
    front_face: bool
}

@set_face_normal:hit_record(r: ray, outward_normal: vec3): void;

def hittable {
    type: int8
}

@hit:hittable(r: ray, ray_t: interval, rec: hit_record*): bool;
@bounding_box:hittable:aabb;
