\inc "std/stdbool.csh"

\inc "interval.csh"
\inc "ray.csh"
\inc "vec3.csh"

def aabb {
    x: interval,
    y: interval,
    z: interval
}

@aabb;
@aabb(x: interval, y: interval, z: interval);
@aabb(a: point3, b: point3);
@aabb(box1: aabb, box2: aabb);

@longest_axis:aabb:int32;
@axis_interval:aabb(n: int32): interval;
@hit:aabb(r: ray, ray_t: interval): bool;
