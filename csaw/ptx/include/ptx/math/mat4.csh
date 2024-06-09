\inc "math.csh"
\inc "vec4.csh"

def Mat4 { e: Vec4[4] }

@Mat4;
@Mat4(t: Float);
@Mat4(m: Mat4);
@Mat4(e0: Vec4, e1: Vec4, e2: Vec4, e3: Vec4);

@"[]":Mat4(i: Int): Vec4*;
