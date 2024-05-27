def vector {
    size: int64,
    reserved: int64,
    buffer: void**
}

@vector;

@"[]":vector(i: int64): void**;

@push_back:vector(void*): void;
@sort:vector(start: int64, num: int64, comp: (void*, void*)(int32)*): void;
@destroy:vector:void;
