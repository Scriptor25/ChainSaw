#
 PCG Random Number Generation for C.

 Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 For additional information about the PCG random number generation scheme,
 including its license and other licensing options, visit

     http://www.pcg-random.org
#

def pcg32_random_t {
    state: int64,
    inc: int64,
}

PCG32_INITIALIZER_STATE: [internal] int64 = 0x853c49e6748fea9b;
PCG32_INITIALIZER_INC:   [internal] int64 = 0xda3e39cb94b95bdb;

@[c]pcg32_srandom_r(rngptr: pcg32_random_t*, initstate: int64, initseq: int64): void;
@[c]pcg32_random_r(rngptr: pcg32_random_t*): int32;
@[c]pcg32_boundedrand_r(rngptr: pcg32_random_t*, bound: int32): int32;

@[c]pcg32_srandom(initstate: int64, initseq: int64): void;
@[c]pcg32_random::int32;
@[c]pcg32_boundedrand(bound: int32): int32;
