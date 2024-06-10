\inc "math.csh"

def Vec3 { e: Float[3] }

@Vec3(t: Float);
@Vec3(v: Vec3);
@Vec3(x: Float, y: Float, z: Float);

@x:Vec3:Float;
@y:Vec3:Float;
@z:Vec3:Float;

@"[]":Vec3(i: Int): Float*;

@length(v: Vec3): Float;
@normalize(v: Vec3): Vec3;

@"*"(t: Float, v: Vec3): Vec3;
@"/"(v: Vec3, t: Float): Vec3;
