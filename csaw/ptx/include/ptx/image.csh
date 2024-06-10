IMAGE_WRITE_SUCCESS:       [internal] int32 = 0;
IMAGE_WRITE_FILENAME_NULL: [internal] int32 = 1;
IMAGE_WRITE_STREAM_NULL:   [internal] int32 = 2;

def Image {
	width: int32,
	height: int32,
	pBuffer: int32*
}

@Image(width: int32, height: int32);
@set:Image(x: int32, y: int32, color: int32): void;
@write:Image(pFilename: int8*): int32;
@free:Image:void;
