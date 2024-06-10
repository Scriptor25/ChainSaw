\inc "math.csh"
\inc "point3.csh"
\inc "vec3.csh"

def Ray {
	origin: Point3,
	direction: Vec3
}

@Ray(ray: Ray);
@Ray(origin: Point3, direction: Vec3);

@at:Ray(t: Float): Point3;
