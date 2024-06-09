\inc "math.csh"

def Color { e: Float[3] }

@Color;
@Color(t: Float);
@Color(c: Color);
@Color(r: Float, g: Float, b: Float);

@r:Color:Float;
@g:Color:Float;
@b:Color:Float;

@pack:Color(gamma: int1): int32;

@"[]":Color(i: Int): Float*;

@"+"(a: Color, b: Color): Color;
@"*"(t: Float, c: Color): Color;
