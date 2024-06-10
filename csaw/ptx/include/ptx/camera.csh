\inc "math/math.csh"
\inc "math/mat4.csh"
\inc "math/point3.csh"

def Camera {
	origin: Point3,
	
	proj: Mat4,
	view: Mat4,
	projInv: Mat4,
	viewInv: Mat4,
}

@Camera(eye: Point3, target: Point3, up: Vec3, fov: Float, aspect: Float, near: Float, far: Float);

@getRay:Camera(pDst: Ray*, sample: Vec2): void;
