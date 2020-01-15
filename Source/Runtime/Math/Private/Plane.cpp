#include "Precompiled.h"
#include "Plane.h"

// 외적
// 내적 == 0
// ax + by + zc + d = 0
// 투영벡터
// 임의의 벡터가 2개 존재하고 직선으로 투영했을 때, 나오는 벡터
// a * b = ||a|| * ||b|| * cosR
// p = b / ||b|| * ||p||
// p = a*b / ||b^2|| * b
