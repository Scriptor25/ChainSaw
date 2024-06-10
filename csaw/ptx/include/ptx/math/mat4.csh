\inc "math.csh"
\inc "vec4.csh"

def Mat4 { e: Vec4[4] }

@Mat4;
@Mat4(t: Float);
@Mat4(m: Mat4);
@Mat4(e0: Vec4, e1: Vec4, e2: Vec4, e3: Vec4);

@"[]":Mat4(i: Int): Vec4*;

@perspective:Mat4(fov: Float, aspect: Float, near: Float, far: Float): void;
@perspectiveInv:Mat4(fov: Float, aspect: Float, near: Float, far: Float): void;

@lookAt:Mat4(eye: Point3, target: Point3, up: Vec3): void;
@lookAtInv:Mat4(eye: Point3, target: Point3, up: Vec3): void;

@"*"(m: Mat4, v: Vec4): Vec4;
