\inc "math.csh"

def Vec4 { e: Float[4] }

@Vec4;
@Vec4(t: Float);
@Vec4(xyz: Vec3, w: Float);
@Vec4(v: Vec4);
@Vec4(x: Float, y: Float, z: Float, w: Float);

@x:Vec4:Float;
@y:Vec4:Float;
@z:Vec4:Float;
@w:Vec4:Float;

@"[]":Vec4(i: Int): Float*;
