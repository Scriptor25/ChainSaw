\inc "math.csh"

def Ray {
	origin: Point3,
	direction: Vec3
}

@Ray;
@Ray(ray: Ray);
@Ray(origin: Point3, direction: Vec3);

@at(t: Float): Point3;
