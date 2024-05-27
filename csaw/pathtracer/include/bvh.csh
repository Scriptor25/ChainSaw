\inc "std/stdbool.csh"

\inc "aabb.csh"
\inc "hittable.csh"
\inc "hittable_list.csh"
\inc "interval.csh"
\inc "ray.csh"
\inc "vector.csh"

def bvh_node {
    type: int8,
    left: hittable*,
    right: hittable*,
    bbox: aabb
}

@bvh_node(list: hittable_list);
@bvh_node(objects: vector, start: int64, end: int64);

@hit:bvh_node(r: ray, ray_t: interval, rec: hit_record*): bool;
@bounding_box:bvh_node:aabb;
