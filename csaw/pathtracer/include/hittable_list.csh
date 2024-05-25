\inc "std/stdbool.csh"

\inc "aabb.csh"
\inc "hittable.csh"
\inc "interval.csh"
\inc "ray.csh"
\inc "vector.csh"

def hittable_list {
    type: int8,
    objects: vector,
    bbox: aabb
}

@hittable_list;
@hittable_list(object: hittable*);

@add:hittable_list(object: hittable*): void;
@hit:hittable_list(r: ray, ray_t: interval, rec: hit_record*): bool;
@bounding_box:hittable_list:aabb;
@destroy:hittable_list:void;
