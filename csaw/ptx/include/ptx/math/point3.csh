\inc "math.csh"

def Point3 { e: Float[3] }

@Point3;
@Point3(p: Point3);
@Point3(x: Float, y: Float, z: Float);

@x:Point3:Float;
@y:Point3:Float;
@z:Point3:Float;

@"[]":Point3(i: Int): Float*;
