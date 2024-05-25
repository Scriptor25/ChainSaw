def image {
    width: int32,
    height: int32,
    buffer: int32*
}

@image(width: int32, height: int32);

@pixel:image(x: int32, y: int32, color: int32): void;
@toppm:image(name: int8*): void;
@destroy:image:void;
