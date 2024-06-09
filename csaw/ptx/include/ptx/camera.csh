\inc "math/math.csh"

def Camera {
	lookfrom: Point3,
	lookat: Point3,
}

@getRay:Camera(pDst: Ray*, sample: Vec2): void;
