#pragma once
#include "../FastPix3D.h"
#include "vint2.h"
#include "vlong2.h"
#include "vfloat2.h"
#include "vint3.h"
#include "vfloat3.h"
#include "vint4.h"
#include "vlong4.h"
#include "vfloat4.h"
#include "vint8.h"
#include "vuint8.h"
#include "vfloat8.h"
#include "vshort16.h"
#include "vushort16.h"
#include "vsbyte32.h"
#include "vbyte32.h"

class FASTPIX3D_API VectorMath
{
public:
	VectorMath() = delete;

	__forceinline static vint4 CmpEq(const vint4 &a, const vint4 &b)
	{
		return _mm_cmpeq_epi32(a, b);
	}
	__forceinline static vint4 CmpEq(const vint4 &a, int32 b)
	{
		return _mm_cmpeq_epi32(a, _mm_set1_epi32(b));
	}
	__forceinline static vint4 CmpNeq(const vint4 &a, const vint4 &b)
	{
		return _mm_xor_si128(_mm_cmpeq_epi32(a, b), _mm_set1_epi32(-1));
	}
	__forceinline static vint4 CmpNeq(const vint4 &a, int32 b)
	{
		return _mm_xor_si128(_mm_cmpeq_epi32(a, _mm_set1_epi32(b)), _mm_set1_epi32(-1));
	}
	__forceinline static vint4 CmpLt(const vint4 &a, const vint4 &b)
	{
		return _mm_cmplt_epi32(a, b);
	}
	__forceinline static vint4 CmpLt(const vint4 &a, int32 b)
	{
		return _mm_cmplt_epi32(a, _mm_set1_epi32(b));
	}
	__forceinline static vint4 CmpGt(const vint4 &a, const vint4 &b)
	{
		return _mm_cmpgt_epi32(a, b);
	}
	__forceinline static vint4 CmpGt(const vint4 &a, int32 b)
	{
		return _mm_cmpgt_epi32(a, _mm_set1_epi32(b));
	}
	__forceinline static vint4 CmpLe(const vint4 &a, const vint4 &b)
	{
		return _mm_xor_si128(_mm_cmpgt_epi32(a, b), _mm_set1_epi32(-1));
	}
	__forceinline static vint4 CmpLe(const vint4 &a, int32 b)
	{
		return _mm_xor_si128(_mm_cmpgt_epi32(a, _mm_set1_epi32(b)), _mm_set1_epi32(-1));
	}
	__forceinline static vint4 CmpGe(const vint4 &a, const vint4 &b)
	{
		return _mm_xor_si128(_mm_cmplt_epi32(a, b), _mm_set1_epi32(-1));
	}
	__forceinline static vint4 CmpGe(const vint4 &a, int32 b)
	{
		return _mm_xor_si128(_mm_cmplt_epi32(a, _mm_set1_epi32(b)), _mm_set1_epi32(-1));
	}

	__forceinline static vlong4 CmpEq(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_cmpeq_epi64(a, b);
	}
	__forceinline static vlong4 CmpEq(const vlong4 &a, int64 b)
	{
		return _mm256_cmpeq_epi64(a, _mm256_set1_epi64x(b));
	}
	__forceinline static vlong4 CmpNeq(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi64(a, b), _mm256_set1_epi64x(-1));
	}
	__forceinline static vlong4 CmpNeq(const vlong4 &a, int64 b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi64(a, _mm256_set1_epi64x(b)), _mm256_set1_epi64x(-1));
	}
	__forceinline static vlong4 CmpLt(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_cmpgt_epi64(b, a);
	}
	__forceinline static vlong4 CmpLt(const vlong4 &a, int64 b)
	{
		return _mm256_cmpgt_epi64(_mm256_set1_epi64x(b), a);
	}
	__forceinline static vlong4 CmpGt(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_cmpgt_epi64(a, b);
	}
	__forceinline static vlong4 CmpGt(const vlong4 &a, int64 b)
	{
		return _mm256_cmpgt_epi64(a, _mm256_set1_epi64x(b));
	}
	__forceinline static vlong4 CmpLe(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi64(a, b), _mm256_set1_epi64x(-1));
	}
	__forceinline static vlong4 CmpLe(const vlong4 &a, int64 b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi64(a, _mm256_set1_epi64x(b)), _mm256_set1_epi64x(-1));
	}
	__forceinline static vlong4 CmpGe(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi64(b, a), _mm256_set1_epi64x(-1));
	}
	__forceinline static vlong4 CmpGe(const vlong4 &a, int64 b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi64(_mm256_set1_epi64x(b), a), _mm256_set1_epi64x(-1));
	}

	__forceinline static vint4 CmpEq(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_castps_si128(_mm_cmpeq_ps(a, b));
	}
	__forceinline static vint4 CmpEq(const vfloat4 &a, float b)
	{
		return _mm_castps_si128(_mm_cmpeq_ps(a, _mm_set1_ps(b)));
	}
	__forceinline static vint4 CmpNeq(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_castps_si128(_mm_cmpneq_ps(a, b));
	}
	__forceinline static vint4 CmpNeq(const vfloat4 &a, float b)
	{
		return _mm_castps_si128(_mm_cmpneq_ps(a, _mm_set1_ps(b)));
	}
	__forceinline static vint4 CmpLt(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_castps_si128(_mm_cmplt_ps(a, b));
	}
	__forceinline static vint4 CmpLt(const vfloat4 &a, float b)
	{
		return _mm_castps_si128(_mm_cmplt_ps(a, _mm_set1_ps(b)));
	}
	__forceinline static vint4 CmpGt(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_castps_si128(_mm_cmpgt_ps(a, b));
	}
	__forceinline static vint4 CmpGt(const vfloat4 &a, float b)
	{
		return _mm_castps_si128(_mm_cmpgt_ps(a, _mm_set1_ps(b)));
	}
	__forceinline static vint4 CmpLe(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_castps_si128(_mm_cmple_ps(a, b));
	}
	__forceinline static vint4 CmpLe(const vfloat4 &a, float b)
	{
		return _mm_castps_si128(_mm_cmple_ps(a, _mm_set1_ps(b)));
	}
	__forceinline static vint4 CmpGe(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_castps_si128(_mm_cmpge_ps(a, b));
	}
	__forceinline static vint4 CmpGe(const vfloat4 &a, float b)
	{
		return _mm_castps_si128(_mm_cmpge_ps(a, _mm_set1_ps(b)));
	}

	__forceinline static vuint8 CmpEq(const vint8 &a, const vint8 &b)
	{
		return _mm256_cmpeq_epi32(a, b);
	}
	__forceinline static vuint8 CmpEq(const vint8 &a, int32 b)
	{
		return _mm256_cmpeq_epi32(a, _mm256_set1_epi32(b));
	}
	__forceinline static vuint8 CmpNeq(const vint8 &a, const vint8 &b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi32(a, b), _mm256_set1_epi32(-1));
	}
	__forceinline static vuint8 CmpNeq(const vint8 &a, int32 b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi32(a, _mm256_set1_epi32(b)), _mm256_set1_epi32(-1));
	}
	__forceinline static vuint8 CmpLt(const vint8 &a, const vint8 &b)
	{
		return _mm256_cmpgt_epi32(b, a);
	}
	__forceinline static vuint8 CmpLt(const vint8 &a, int32 b)
	{
		return _mm256_cmpgt_epi32(_mm256_set1_epi32(b), a);
	}
	__forceinline static vuint8 CmpGt(const vint8 &a, const vint8 &b)
	{
		return _mm256_cmpgt_epi32(a, b);
	}
	__forceinline static vuint8 CmpGt(const vint8 &a, int32 b)
	{
		return _mm256_cmpgt_epi32(a, _mm256_set1_epi32(b));
	}
	__forceinline static vuint8 CmpLe(const vint8 &a, const vint8 &b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi32(a, b), _mm256_set1_epi32(-1));
	}
	__forceinline static vuint8 CmpLe(const vint8 &a, int32 b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi32(a, _mm256_set1_epi32(b)), _mm256_set1_epi32(-1));
	}
	__forceinline static vuint8 CmpGe(const vint8 &a, const vint8 &b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi32(b, a), _mm256_set1_epi32(-1));
	}
	__forceinline static vuint8 CmpGe(const vint8 &a, int32 b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi32(_mm256_set1_epi32(b), a), _mm256_set1_epi32(-1));
	}

	__forceinline static vuint8 CmpEq(const vuint8 &a, const vuint8 &b)
	{
		return _mm256_cmpeq_epi32(a, b);
	}
	__forceinline static vuint8 CmpEq(const vuint8 &a, const uint32 b)
	{
		return _mm256_cmpeq_epi32(a, _mm256_set1_epi32(b));
	}
	__forceinline static vuint8 CmpNeq(const vuint8 &a, const vuint8 &b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi32(a, b), _mm256_set1_epi32(-1));
	}
	__forceinline static vuint8 CmpNeq(const vuint8 &a, const uint32 b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi32(a, _mm256_set1_epi32(b)), _mm256_set1_epi32(-1));
	}

	__forceinline static vuint8 CmpEq(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_EQ_OQ));
	}
	__forceinline static vuint8 CmpEq(const vfloat8 &a, float b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_EQ_OQ));
	}
	__forceinline static vuint8 CmpNeq(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_NEQ_OQ));
	}
	__forceinline static vuint8 CmpNeq(const vfloat8 &a, float b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_NEQ_OQ));
	}
	__forceinline static vuint8 CmpLt(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_LT_OQ));
	}
	__forceinline static vuint8 CmpLt(const vfloat8 &a, float b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_LT_OQ));
	}
	__forceinline static vuint8 CmpGt(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_GT_OQ));
	}
	__forceinline static vuint8 CmpGt(const vfloat8 &a, float b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_GT_OQ));
	}
	__forceinline static vuint8 CmpLe(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_LE_OQ));
	}
	__forceinline static vuint8 CmpLe(const vfloat8 &a, float b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_LE_OQ));
	}
	__forceinline static vuint8 CmpGe(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_GE_OQ));
	}
	__forceinline static vuint8 CmpGe(const vfloat8 &a, float b)
	{
		return _mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_GE_OQ));
	}

	__forceinline static vuint8 CmpEq(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_cmpeq_epi16(a, b);
	}
	__forceinline static vuint8 CmpEq(const vshort16 &a, int16 b)
	{
		return _mm256_cmpeq_epi16(a, _mm256_set1_epi16(b));
	}
	__forceinline static vuint8 CmpNeq(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi16(a, b), _mm256_set1_epi16(-1));
	}
	__forceinline static vuint8 CmpNeq(const vshort16 &a, int16 b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi16(a, _mm256_set1_epi16(b)), _mm256_set1_epi16(-1));
	}
	__forceinline static vuint8 CmpLt(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_cmpgt_epi16(b, a);
	}
	__forceinline static vuint8 CmpLt(const vshort16 &a, int16 b)
	{
		return _mm256_cmpgt_epi16(_mm256_set1_epi16(b), a);
	}
	__forceinline static vuint8 CmpGt(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_cmpgt_epi16(a, b);
	}
	__forceinline static vuint8 CmpGt(const vshort16 &a, int16 b)
	{
		return _mm256_cmpgt_epi16(a, _mm256_set1_epi16(b));
	}
	__forceinline static vuint8 CmpLe(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi16(a, b), _mm256_set1_epi16(-1));
	}
	__forceinline static vuint8 CmpLe(const vshort16 &a, int16 b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi16(a, _mm256_set1_epi16(b)), _mm256_set1_epi16(-1));
	}
	__forceinline static vuint8 CmpGe(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi16(b, a), _mm256_set1_epi16(-1));
	}
	__forceinline static vuint8 CmpGe(const vshort16 &a, int16 b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi16(_mm256_set1_epi16(b), a), _mm256_set1_epi16(-1));
	}

	__forceinline static vuint8 CmpEq(const vushort16 &a, const vushort16 &b)
	{
		return _mm256_cmpeq_epi16(a, b);
	}
	__forceinline static vuint8 CmpEq(const vushort16 &a, const uint16 b)
	{
		return _mm256_cmpeq_epi16(a, _mm256_set1_epi16(b));
	}
	__forceinline static vuint8 CmpNeq(const vushort16 &a, const vushort16 &b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi16(a, b), _mm256_set1_epi16(-1));
	}
	__forceinline static vuint8 CmpNeq(const vushort16 &a, const uint16 b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi16(a, _mm256_set1_epi16(b)), _mm256_set1_epi16(-1));
	}

	__forceinline static vuint8 CmpEq(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_cmpeq_epi8(a, b);
	}
	__forceinline static vuint8 CmpEq(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_cmpeq_epi8(a, _mm256_set1_epi8(b));
	}
	__forceinline static vuint8 CmpNeq(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi8(a, b), _mm256_set1_epi8(-1));
	}
	__forceinline static vuint8 CmpNeq(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi8(a, _mm256_set1_epi8(b)), _mm256_set1_epi8(-1));
	}
	__forceinline static vuint8 CmpLt(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_cmpgt_epi8(b, a);
	}
	__forceinline static vuint8 CmpLt(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_cmpgt_epi8(_mm256_set1_epi8(b), a);
	}
	__forceinline static vuint8 CmpGt(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_cmpgt_epi8(a, b);
	}
	__forceinline static vuint8 CmpGt(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_cmpgt_epi8(a, _mm256_set1_epi8(b));
	}
	__forceinline static vuint8 CmpLe(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi8(a, b), _mm256_set1_epi8(-1));
	}
	__forceinline static vuint8 CmpLe(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi8(a, _mm256_set1_epi8(b)), _mm256_set1_epi8(-1));
	}
	__forceinline static vuint8 CmpGe(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi8(b, a), _mm256_set1_epi8(-1));
	}
	__forceinline static vuint8 CmpGe(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_xor_si256(_mm256_cmpgt_epi8(_mm256_set1_epi8(b), a), _mm256_set1_epi8(-1));
	}

	__forceinline static vuint8 CmpEq(const vbyte32 &a, const vbyte32 &b)
	{
		return _mm256_cmpeq_epi8(a, b);
	}
	__forceinline static vuint8 CmpEq(const vbyte32 &a, const byte b)
	{
		return _mm256_cmpeq_epi8(a, _mm256_set1_epi8(b));
	}
	__forceinline static vuint8 CmpNeq(const vbyte32 &a, const vbyte32 &b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi8(a, b), _mm256_set1_epi8(-1));
	}
	__forceinline static vuint8 CmpNeq(const vbyte32 &a, const byte b)
	{
		return _mm256_xor_si256(_mm256_cmpeq_epi8(a, _mm256_set1_epi8(b)), _mm256_set1_epi8(-1));
	}

	__forceinline static uint32 CmpEqMask(const vint4 &a, const vint4 &b)
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi32(a, b));
	}
	__forceinline static uint32 CmpEqMask(const vint4 &a, int32 b)
	{
		return _mm_movemask_epi8(_mm_cmpeq_epi32(a, _mm_set1_epi32(b)));
	}
	__forceinline static uint32 CmpNeqMask(const vint4 &a, const vint4 &b)
	{
		return _mm_movemask_epi8(_mm_xor_si128(_mm_cmpeq_epi32(a, b), _mm_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpNeqMask(const vint4 &a, int32 b)
	{
		return _mm_movemask_epi8(_mm_xor_si128(_mm_cmpeq_epi32(a, _mm_set1_epi32(b)), _mm_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpLtMask(const vint4 &a, const vint4 &b)
	{
		return _mm_movemask_epi8(_mm_cmplt_epi32(a, b));
	}
	__forceinline static uint32 CmpLtMask(const vint4 &a, int32 b)
	{
		return _mm_movemask_epi8(_mm_cmplt_epi32(a, _mm_set1_epi32(b)));
	}
	__forceinline static uint32 CmpGtMask(const vint4 &a, const vint4 &b)
	{
		return _mm_movemask_epi8(_mm_cmpgt_epi32(a, b));
	}
	__forceinline static uint32 CmpGtMask(const vint4 &a, int32 b)
	{
		return _mm_movemask_epi8(_mm_cmpgt_epi32(a, _mm_set1_epi32(b)));
	}
	__forceinline static uint32 CmpLeMask(const vint4 &a, const vint4 &b)
	{
		return _mm_movemask_epi8(_mm_xor_si128(_mm_cmpgt_epi32(a, b), _mm_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpLeMask(const vint4 &a, int32 b)
	{
		return _mm_movemask_epi8(_mm_xor_si128(_mm_cmpgt_epi32(a, _mm_set1_epi32(b)), _mm_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpGeMask(const vint4 &a, const vint4 &b)
	{
		return _mm_movemask_epi8(_mm_xor_si128(_mm_cmplt_epi32(a, b), _mm_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpGeMask(const vint4 &a, int32 b)
	{
		return _mm_movemask_epi8(_mm_xor_si128(_mm_cmplt_epi32(a, _mm_set1_epi32(b)), _mm_set1_epi32(-1)));
	}

	__forceinline static uint32 CmpEqMask(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi64(a, b));
	}
	__forceinline static uint32 CmpEqMask(const vlong4 &a, int64 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi64(a, _mm256_set1_epi64x(b)));
	}
	__forceinline static uint32 CmpNeqMask(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi64(a, b), _mm256_set1_epi64x(-1)));
	}
	__forceinline static uint32 CmpNeqMask(const vlong4 &a, int64 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi64(a, _mm256_set1_epi64x(b)), _mm256_set1_epi64x(-1)));
	}
	__forceinline static uint32 CmpLtMask(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(b, a));
	}
	__forceinline static uint32 CmpLtMask(const vlong4 &a, int64 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(_mm256_set1_epi64x(b), a));
	}
	__forceinline static uint32 CmpGtMask(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(a, b));
	}
	__forceinline static uint32 CmpGtMask(const vlong4 &a, int64 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi64(a, _mm256_set1_epi64x(b)));
	}
	__forceinline static uint32 CmpLeMask(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi64(a, b), _mm256_set1_epi64x(-1)));
	}
	__forceinline static uint32 CmpLeMask(const vlong4 &a, int64 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi64(a, _mm256_set1_epi64x(b)), _mm256_set1_epi64x(-1)));
	}
	__forceinline static uint32 CmpGeMask(const vlong4 &a, const vlong4 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi64(b, a), _mm256_set1_epi64x(-1)));
	}
	__forceinline static uint32 CmpGeMask(const vlong4 &a, int64 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi64(_mm256_set1_epi64x(b), a), _mm256_set1_epi64x(-1)));
	}

	__forceinline static uint32 CmpEqMask(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmpeq_ps(a, b)));
	}
	__forceinline static uint32 CmpEqMask(const vfloat4 &a, float b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmpeq_ps(a, _mm_set1_ps(b))));
	}
	__forceinline static uint32 CmpNeqMask(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmpneq_ps(a, b)));
	}
	__forceinline static uint32 CmpNeqMask(const vfloat4 &a, float b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmpneq_ps(a, _mm_set1_ps(b))));
	}
	__forceinline static uint32 CmpLtMask(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmplt_ps(a, b)));
	}
	__forceinline static uint32 CmpLtMask(const vfloat4 &a, float b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmplt_ps(a, _mm_set1_ps(b))));
	}
	__forceinline static uint32 CmpGtMask(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmpgt_ps(a, b)));
	}
	__forceinline static uint32 CmpGtMask(const vfloat4 &a, float b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmpgt_ps(a, _mm_set1_ps(b))));
	}
	__forceinline static uint32 CmpLeMask(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmple_ps(a, b)));
	}
	__forceinline static uint32 CmpLeMask(const vfloat4 &a, float b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmple_ps(a, _mm_set1_ps(b))));
	}
	__forceinline static uint32 CmpGeMask(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmpge_ps(a, b)));
	}
	__forceinline static uint32 CmpGeMask(const vfloat4 &a, float b)
	{
		return _mm_movemask_epi8(_mm_castps_si128(_mm_cmpge_ps(a, _mm_set1_ps(b))));
	}

	__forceinline static uint32 CmpEqMask(const vint8 &a, const vint8 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(a, b));
	}
	__forceinline static uint32 CmpEqMask(const vint8 &a, int32 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(a, _mm256_set1_epi32(b)));
	}
	__forceinline static uint32 CmpNeqMask(const vint8 &a, const vint8 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi32(a, b), _mm256_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpNeqMask(const vint8 &a, int32 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi32(a, _mm256_set1_epi32(b)), _mm256_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpLtMask(const vint8 &a, const vint8 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(b, a));
	}
	__forceinline static uint32 CmpLtMask(const vint8 &a, int32 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(_mm256_set1_epi32(b), a));
	}
	__forceinline static uint32 CmpGtMask(const vint8 &a, const vint8 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(a, b));
	}
	__forceinline static uint32 CmpGtMask(const vint8 &a, int32 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi32(a, _mm256_set1_epi32(b)));
	}
	__forceinline static uint32 CmpLeMask(const vint8 &a, const vint8 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi32(a, b), _mm256_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpLeMask(const vint8 &a, int32 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi32(a, _mm256_set1_epi32(b)), _mm256_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpGeMask(const vint8 &a, const vint8 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi32(b, a), _mm256_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpGeMask(const vint8 &a, int32 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi32(_mm256_set1_epi32(b), a), _mm256_set1_epi32(-1)));
	}

	__forceinline static uint32 CmpEqMask(const vuint8 &a, const vuint8 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(a, b));
	}
	__forceinline static uint32 CmpEqMask(const vuint8 &a, const uint32 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi32(a, _mm256_set1_epi32(b)));
	}
	__forceinline static uint32 CmpNeqMask(const vuint8 &a, const vuint8 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi32(a, b), _mm256_set1_epi32(-1)));
	}
	__forceinline static uint32 CmpNeqMask(const vuint8 &a, const uint32 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi32(a, _mm256_set1_epi32(b)), _mm256_set1_epi32(-1)));
	}

	__forceinline static uint32 CmpEqMask(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_EQ_OQ)));
	}
	__forceinline static uint32 CmpEqMask(const vfloat8 &a, float b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_EQ_OQ)));
	}
	__forceinline static uint32 CmpNeqMask(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_NEQ_OQ)));
	}
	__forceinline static uint32 CmpNeqMask(const vfloat8 &a, float b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_NEQ_OQ)));
	}
	__forceinline static uint32 CmpLtMask(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_LT_OQ)));
	}
	__forceinline static uint32 CmpLtMask(const vfloat8 &a, float b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_LT_OQ)));
	}
	__forceinline static uint32 CmpGtMask(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_GT_OQ)));
	}
	__forceinline static uint32 CmpGtMask(const vfloat8 &a, float b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_GT_OQ)));
	}
	__forceinline static uint32 CmpLeMask(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_LE_OQ)));
	}
	__forceinline static uint32 CmpLeMask(const vfloat8 &a, float b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_LE_OQ)));
	}
	__forceinline static uint32 CmpGeMask(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, b, _CMP_GE_OQ)));
	}
	__forceinline static uint32 CmpGeMask(const vfloat8 &a, float b)
	{
		return _mm256_movemask_epi8(_mm256_castps_si256(_mm256_cmp_ps(a, _mm256_set1_ps(b), _CMP_GE_OQ)));
	}

	__forceinline static uint32 CmpEqMask(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(a, b));
	}
	__forceinline static uint32 CmpEqMask(const vshort16 &a, int16 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(a, _mm256_set1_epi16(b)));
	}
	__forceinline static uint32 CmpNeqMask(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi16(a, b), _mm256_set1_epi16(-1)));
	}
	__forceinline static uint32 CmpNeqMask(const vshort16 &a, int16 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi16(a, _mm256_set1_epi16(b)), _mm256_set1_epi16(-1)));
	}
	__forceinline static uint32 CmpLtMask(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(b, a));
	}
	__forceinline static uint32 CmpLtMask(const vshort16 &a, int16 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(_mm256_set1_epi16(b), a));
	}
	__forceinline static uint32 CmpGtMask(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(a, b));
	}
	__forceinline static uint32 CmpGtMask(const vshort16 &a, int16 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi16(a, _mm256_set1_epi16(b)));
	}
	__forceinline static uint32 CmpLeMask(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi16(a, b), _mm256_set1_epi16(-1)));
	}
	__forceinline static uint32 CmpLeMask(const vshort16 &a, int16 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi16(a, _mm256_set1_epi16(b)), _mm256_set1_epi16(-1)));
	}
	__forceinline static uint32 CmpGeMask(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi16(b, a), _mm256_set1_epi16(-1)));
	}
	__forceinline static uint32 CmpGeMask(const vshort16 &a, int16 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi16(_mm256_set1_epi16(b), a), _mm256_set1_epi16(-1)));
	}

	__forceinline static uint32 CmpEqMask(const vushort16 &a, const vushort16 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(a, b));
	}
	__forceinline static uint32 CmpEqMask(const vushort16 &a, const uint16 b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi16(a, _mm256_set1_epi16(b)));
	}
	__forceinline static uint32 CmpNeqMask(const vushort16 &a, const vushort16 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi16(a, b), _mm256_set1_epi16(-1)));
	}
	__forceinline static uint32 CmpNeqMask(const vushort16 &a, const uint16 b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi16(a, _mm256_set1_epi16(b)), _mm256_set1_epi16(-1)));
	}

	__forceinline static uint32 CmpEqMask(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(a, b));
	}
	__forceinline static uint32 CmpEqMask(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(a, _mm256_set1_epi8(b)));
	}
	__forceinline static uint32 CmpNeqMask(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi8(a, b), _mm256_set1_epi8(-1)));
	}
	__forceinline static uint32 CmpNeqMask(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi8(a, _mm256_set1_epi8(b)), _mm256_set1_epi8(-1)));
	}
	__forceinline static uint32 CmpLtMask(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(b, a));
	}
	__forceinline static uint32 CmpLtMask(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(_mm256_set1_epi8(b), a));
	}
	__forceinline static uint32 CmpGtMask(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(a, b));
	}
	__forceinline static uint32 CmpGtMask(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_movemask_epi8(_mm256_cmpgt_epi8(a, _mm256_set1_epi8(b)));
	}
	__forceinline static uint32 CmpLeMask(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi8(a, b), _mm256_set1_epi8(-1)));
	}
	__forceinline static uint32 CmpLeMask(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi8(a, _mm256_set1_epi8(b)), _mm256_set1_epi8(-1)));
	}
	__forceinline static uint32 CmpGeMask(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi8(b, a), _mm256_set1_epi8(-1)));
	}
	__forceinline static uint32 CmpGeMask(const vsbyte32 &a, const sbyte b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpgt_epi8(_mm256_set1_epi8(b), a), _mm256_set1_epi8(-1)));
	}

	__forceinline static uint32 CmpEqMask(const vbyte32 &a, const vbyte32 &b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(a, b));
	}
	__forceinline static uint32 CmpEqMask(const vbyte32 &a, const byte b)
	{
		return _mm256_movemask_epi8(_mm256_cmpeq_epi8(a, _mm256_set1_epi8(b)));
	}
	__forceinline static uint32 CmpNeqMask(const vbyte32 &a, const vbyte32 &b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi8(a, b), _mm256_set1_epi8(-1)));
	}
	__forceinline static uint32 CmpNeqMask(const vbyte32 &a, const byte b)
	{
		return _mm256_movemask_epi8(_mm256_xor_si256(_mm256_cmpeq_epi8(a, _mm256_set1_epi8(b)), _mm256_set1_epi8(-1)));
	}

	__forceinline static vint2 Min(const vint2 &a, const vint2 &b)
	{
		return vint2(a.X < b.X ? a.X : b.X, a.Y < b.Y ? a.Y : b.Y);
	}
	__forceinline static vint2 Min(const vint2 &a, int32 b)
	{
		return vint2(a.X < b ? a.X : b, a.Y < b ? a.Y : b);
	}
	__forceinline static vlong2 Min(const vlong2 &a, const vlong2 &b)
	{
		return vlong2(a.X < b.X ? a.X : b.X, a.Y < b.Y ? a.Y : b.Y);
	}
	__forceinline static vlong2 Min(const vlong2 &a, int64 b)
	{
		return vlong2(a.X < b ? a.X : b, a.Y < b ? a.Y : b);
	}
	__forceinline static vfloat2 Min(const vfloat2 &a, const vfloat2 &b)
	{
		return vfloat2(a.X < b.X ? a.X : b.X, a.Y < b.Y ? a.Y : b.Y);
	}
	__forceinline static vfloat2 Min(const vfloat2 &a, float b)
	{
		return vfloat2(a.X < b ? a.X : b, a.Y < b ? a.Y : b);
	}
	__forceinline static vint3 Min(const vint3 &a, const vint3 &b)
	{
		return _mm_min_epi32(a, b);
	}
	__forceinline static vint3 Min(const vint3 &a, int32 b)
	{
		return _mm_min_epi32(a, _mm_set1_epi32(b));
	}
	__forceinline static vfloat3 Min(const vfloat3 &a, const vfloat3 &b)
	{
		return _mm_min_ps(a, b);
	}
	__forceinline static vfloat3 Min(const vfloat3 &a, float b)
	{
		return _mm_min_ps(a, _mm_set1_ps(b));
	}
	__forceinline static vint4 Min(const vint4 &a, const vint4 &b)
	{
		return _mm_min_epi32(a, b);
	}
	__forceinline static vint4 Min(const vint4 &a, int32 b)
	{
		return _mm_min_epi32(a, _mm_set1_epi32(b));
	}
	__forceinline static vfloat4 Min(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_min_ps(a, b);
	}
	__forceinline static vfloat4 Min(const vfloat4 &a, float b)
	{
		return _mm_min_ps(a, _mm_set1_ps(b));
	}
	__forceinline static vint8 Min(const vint8 &a, const vint8 &b)
	{
		return _mm256_min_epi32(a, b);
	}
	__forceinline static vint8 Min(const vint8 &a, int32 b)
	{
		return _mm256_min_epi32(a, _mm256_set1_epi32(b));
	}
	__forceinline static vuint8 Min(const vuint8 &a, const vuint8 &b)
	{
		return _mm256_min_epu32(a, b);
	}
	__forceinline static vuint8 Min(const vuint8 &a, uint32 b)
	{
		return _mm256_min_epu32(a, _mm256_set1_epi32(b));
	}
	__forceinline static vfloat8 Min(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_min_ps(a, b);
	}
	__forceinline static vfloat8 Min(const vfloat8 &a, float b)
	{
		return _mm256_min_ps(a, _mm256_set1_ps(b));
	}
	__forceinline static vshort16 Min(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_min_epi16(a, b);
	}
	__forceinline static vshort16 Min(const vshort16 &a, int16 b)
	{
		return _mm256_min_epi16(a, _mm256_set1_epi16(b));
	}
	__forceinline static vushort16 Min(const vushort16 &a, const vushort16 &b)
	{
		return _mm256_min_epu16(a, b);
	}
	__forceinline static vushort16 Min(const vushort16 &a, uint16 b)
	{
		return _mm256_min_epu16(a, _mm256_set1_epi16(b));
	}
	__forceinline static vsbyte32 Min(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_min_epi8(a, b);
	}
	__forceinline static vsbyte32 Min(const vsbyte32 &a, sbyte b)
	{
		return _mm256_min_epi8(a, _mm256_set1_epi8(b));
	}
	__forceinline static vbyte32 Min(const vbyte32 &a, const vbyte32 &b)
	{
		return _mm256_min_epu8(a, b);
	}
	__forceinline static vbyte32 Min(const vbyte32 &a, byte b)
	{
		return _mm256_min_epu8(a, _mm256_set1_epi8(b));
	}

	__forceinline static vint2 Max(const vint2 &a, const vint2 &b)
	{
		return vint2(a.X > b.X ? a.X : b.X, a.Y > b.Y ? a.Y : b.Y);
	}
	__forceinline static vint2 Max(const vint2 &a, int32 b)
	{
		return vint2(a.X > b ? a.X : b, a.Y > b ? a.Y : b);
	}
	__forceinline static vlong2 Max(const vlong2 &a, const vlong2 &b)
	{
		return vlong2(a.X > b.X ? a.X : b.X, a.Y > b.Y ? a.Y : b.Y);
	}
	__forceinline static vlong2 Max(const vlong2 &a, int64 b)
	{
		return vlong2(a.X > b ? a.X : b, a.Y > b ? a.Y : b);
	}
	__forceinline static vfloat2 Max(const vfloat2 &a, const vfloat2 &b)
	{
		return vfloat2(a.X > b.X ? a.X : b.X, a.Y > b.Y ? a.Y : b.Y);
	}
	__forceinline static vfloat2 Max(const vfloat2 &a, float b)
	{
		return vfloat2(a.X > b ? a.X : b, a.Y > b ? a.Y : b);
	}
	__forceinline static vint3 Max(const vint3 &a, const vint3 &b)
	{
		return _mm_max_epi32(a, b);
	}
	__forceinline static vint3 Max(const vint3 &a, int32 b)
	{
		return _mm_max_epi32(a, _mm_set1_epi32(b));
	}
	__forceinline static vfloat3 Max(const vfloat3 &a, const vfloat3 &b)
	{
		return _mm_max_ps(a, b);
	}
	__forceinline static vfloat3 Max(const vfloat3 &a, float b)
	{
		return _mm_max_ps(a, _mm_set1_ps(b));
	}
	__forceinline static vint4 Max(const vint4 &a, const vint4 &b)
	{
		return _mm_max_epi32(a, b);
	}
	__forceinline static vint4 Max(const vint4 &a, int32 b)
	{
		return _mm_max_epi32(a, _mm_set1_epi32(b));
	}
	__forceinline static vfloat4 Max(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_max_ps(a, b);
	}
	__forceinline static vfloat4 Max(const vfloat4 &a, float b)
	{
		return _mm_max_ps(a, _mm_set1_ps(b));
	}
	__forceinline static vint8 Max(const vint8 &a, const vint8 &b)
	{
		return _mm256_max_epi32(a, b);
	}
	__forceinline static vint8 Max(const vint8 &a, int32 b)
	{
		return _mm256_max_epi32(a, _mm256_set1_epi32(b));
	}
	__forceinline static vuint8 Max(const vuint8 &a, const vuint8 &b)
	{
		return _mm256_max_epu32(a, b);
	}
	__forceinline static vuint8 Max(const vuint8 &a, uint32 b)
	{
		return _mm256_max_epu32(a, _mm256_set1_epi32(b));
	}
	__forceinline static vfloat8 Max(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_max_ps(a, b);
	}
	__forceinline static vfloat8 Max(const vfloat8 &a, float b)
	{
		return _mm256_max_ps(a, _mm256_set1_ps(b));
	}
	__forceinline static vshort16 Max(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_max_epi16(a, b);
	}
	__forceinline static vshort16 Max(const vshort16 &a, int16 b)
	{
		return _mm256_max_epi16(a, _mm256_set1_epi16(b));
	}
	__forceinline static vushort16 Max(const vushort16 &a, const vushort16 &b)
	{
		return _mm256_max_epu16(a, b);
	}
	__forceinline static vushort16 Max(const vushort16 &a, uint16 b)
	{
		return _mm256_max_epu16(a, _mm256_set1_epi16(b));
	}
	__forceinline static vsbyte32 Max(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_max_epi8(a, b);
	}
	__forceinline static vsbyte32 Max(const vsbyte32 &a, sbyte b)
	{
		return _mm256_max_epi8(a, _mm256_set1_epi8(b));
	}
	__forceinline static vbyte32 Max(const vbyte32 &a, const vbyte32 &b)
	{
		return _mm256_max_epu8(a, b);
	}
	__forceinline static vbyte32 Max(const vbyte32 &a, byte b)
	{
		return _mm256_max_epu8(a, _mm256_set1_epi8(b));
	}

	__forceinline static vint2 Clamp(const vint2 &value, const vint2 &min, const vint2 &max)
	{
		return Min(Max(value, min), max);
	}
	__forceinline static vint2 Clamp(const vint2 &value, int32 min, int32 max)
	{
		return Min(Max(value, min), max);
	}
	__forceinline static vlong2 Clamp(const vlong2 &value, const vlong2 &min, const vlong2 &max)
	{
		return Min(Max(value, min), max);
	}
	__forceinline static vlong2 Clamp(const vlong2 &value, int64 min, int64 max)
	{
		return Min(Max(value, min), max);
	}
	__forceinline static vfloat2 Clamp(const vfloat2 &value, const vfloat2 &min, const vfloat2 &max)
	{
		return Min(Max(value, min), max);
	}
	__forceinline static vfloat2 Clamp(const vfloat2 &value, float min, float max)
	{
		return Min(Max(value, min), max);
	}
	__forceinline static vint3 Clamp(const vint3 &value, const vint3 &min, const vint3 &max)
	{
		return _mm_min_epi32(_mm_max_epi32(value, min), max);
	}
	__forceinline static vint3 Clamp(const vint3 &value, int32 min, int32 max)
	{
		return _mm_min_epi32(_mm_max_epi32(value, _mm_set1_epi32(min)), _mm_set1_epi32(max));
	}
	__forceinline static vfloat3 Clamp(const vfloat3 &value, const vfloat3 &min, const vfloat3 &max)
	{
		return _mm_min_ps(_mm_max_ps(value, min), max);
	}
	__forceinline static vfloat3 Clamp(const vfloat3 &value, float min, float max)
	{
		return _mm_min_ps(_mm_max_ps(value, _mm_set1_ps(min)), _mm_set1_ps(max));
	}
	__forceinline static vint4 Clamp(const vint4 &value, const vint4 &min, const vint4 &max)
	{
		return _mm_min_epi32(_mm_max_epi32(value, min), max);
	}
	__forceinline static vint4 Clamp(const vint4 &value, int32 min, int32 max)
	{
		return _mm_min_epi32(_mm_max_epi32(value, _mm_set1_epi32(min)), _mm_set1_epi32(max));
	}
	__forceinline static vfloat4 Clamp(const vfloat4 &value, const vfloat4 &min, const vfloat4 &max)
	{
		return _mm_min_ps(_mm_max_ps(value, min), max);
	}
	__forceinline static vfloat4 Clamp(const vfloat4 &value, float min, float max)
	{
		return _mm_min_ps(_mm_max_ps(value, _mm_set1_ps(min)), _mm_set1_ps(max));
	}
	__forceinline static vint8 Clamp(const vint8 &value, const vint8 &min, const vint8 &max)
	{
		return _mm256_min_epi32(_mm256_max_epi32(value, min), max);
	}
	__forceinline static vint8 Clamp(const vint8 &value, int32 min, int32 max)
	{
		return _mm256_min_epi32(_mm256_max_epi32(value, _mm256_set1_epi32(min)), _mm256_set1_epi32(max));
	}
	__forceinline static vuint8 Clamp(const vuint8 &value, const vuint8 &min, const vuint8 &max)
	{
		return _mm256_min_epu32(_mm256_max_epu32(value, min), max);
	}
	__forceinline static vuint8 Clamp(const vuint8 &value, uint32 min, uint32 max)
	{
		return _mm256_min_epu32(_mm256_max_epu32(value, _mm256_set1_epi32(min)), _mm256_set1_epi32(max));
	}
	__forceinline static vfloat8 Clamp(const vfloat8 &value, const vfloat8 &min, const vfloat8 &max)
	{
		return _mm256_min_ps(_mm256_max_ps(value, min), max);
	}
	__forceinline static vfloat8 Clamp(const vfloat8 &value, float min, float max)
	{
		return _mm256_min_ps(_mm256_max_ps(value, _mm256_set1_ps(min)), _mm256_set1_ps(max));
	}
	__forceinline static vshort16 Clamp(const vshort16 &value, const vshort16 &min, const vshort16 &max)
	{
		return _mm256_min_epi16(_mm256_max_epi16(value, min), max);
	}
	__forceinline static vshort16 Clamp(const vshort16 &value, int16 min, int16 max)
	{
		return _mm256_min_epi16(_mm256_max_epi16(value, _mm256_set1_epi16(min)), _mm256_set1_epi16(max));
	}
	__forceinline static vushort16 Clamp(const vushort16 &value, const vushort16 &min, const vushort16 &max)
	{
		return _mm256_min_epu16(_mm256_max_epu16(value, min), max);
	}
	__forceinline static vushort16 Clamp(const vushort16 &value, uint16 min, uint16 max)
	{
		return _mm256_min_epu16(_mm256_max_epu16(value, _mm256_set1_epi16(min)), _mm256_set1_epi16(max));
	}
	__forceinline static vsbyte32 Clamp(const vsbyte32 &value, const vsbyte32 &min, const vsbyte32 &max)
	{
		return _mm256_min_epi8(_mm256_max_epi8(value, min), max);
	}
	__forceinline static vsbyte32 Clamp(const vsbyte32 &value, sbyte min, sbyte max)
	{
		return _mm256_min_epi8(_mm256_max_epi8(value, _mm256_set1_epi8(min)), _mm256_set1_epi8(max));
	}
	__forceinline static vbyte32 Clamp(const vbyte32 &value, const vbyte32 &min, const vbyte32 &max)
	{
		return _mm256_min_epu8(_mm256_max_epu8(value, min), max);
	}
	__forceinline static vbyte32 Clamp(const vbyte32 &value, byte min, byte max)
	{
		return _mm256_min_epu8(_mm256_max_epu8(value, _mm256_set1_epi8(min)), _mm256_set1_epi8(max));
	}

	__forceinline static vint2 Abs(const vint2 &value)
	{
		return vint2(abs(value.X), abs(value.Y));
	}
	__forceinline static vlong2 Abs(const vlong2 &value)
	{
		return vlong2(llabs(value.X), llabs(value.Y));
	}
	__forceinline static vfloat2 Abs(const vfloat2 &value)
	{
		return vfloat2(fabsf(value.X), fabsf(value.Y));
	}
	__forceinline static vint3 Abs(const vint3 &value)
	{
		return _mm_abs_epi32(value);
	}
	__forceinline static vfloat3 Abs(const vfloat3 &value)
	{
		return _mm_and_ps(value, _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff)));
	}
	__forceinline static vint4 Abs(const vint4 &value)
	{
		return _mm_abs_epi32(value);
	}
	__forceinline static vfloat4 Abs(const vfloat4 &value)
	{
		return _mm_and_ps(value, _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff)));
	}
	__forceinline static vint8 Abs(const vint8 &value)
	{
		return _mm256_abs_epi32(value);
	}
	__forceinline static vfloat8 Abs(const vfloat8 &value)
	{
		return _mm256_and_ps(value, _mm256_castsi256_ps(_mm256_set1_epi32(0x7fffffff)));
	}
	__forceinline static vshort16 Abs(const vshort16 &value)
	{
		return _mm256_abs_epi16(value);
	}
	__forceinline static vsbyte32 Abs(const vsbyte32 &value)
	{
		return _mm256_abs_epi8(value);
	}

	__forceinline static vfloat2 Floor(const vfloat2 &value)
	{
		return vfloat2(floorf(value.X), floorf(value.Y));
	}
	__forceinline static vfloat3 Floor(const vfloat3 &value)
	{
		return _mm_floor_ps(value);
	}
	__forceinline static vfloat4 Floor(const vfloat4 &value)
	{
		return _mm_floor_ps(value);
	}
	__forceinline static vfloat8 Floor(const vfloat8 &value)
	{
		return _mm256_floor_ps(value);
	}

	__forceinline static vfloat2 Ceil(const vfloat2 &value)
	{
		return vfloat2(ceilf(value.X), ceilf(value.Y));
	}
	__forceinline static vfloat3 Ceil(const vfloat3 &value)
	{
		return _mm_ceil_ps(value);
	}
	__forceinline static vfloat4 Ceil(const vfloat4 &value)
	{
		return _mm_ceil_ps(value);
	}
	__forceinline static vfloat8 Ceil(const vfloat8 &value)
	{
		return _mm256_ceil_ps(value);
	}

	__forceinline static vfloat2 Round(const vfloat2 &value)
	{
		return vfloat2(roundf(value.X), roundf(value.Y));
	}
	__forceinline static vfloat3 Round(const vfloat3 &value)
	{
		return _mm_round_ps(value, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
	}
	__forceinline static vfloat4 Round(const vfloat4 &value)
	{
		return _mm_round_ps(value, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
	}
	__forceinline static vfloat8 Round(const vfloat8 &value)
	{
		return _mm256_round_ps(value, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
	}

	__forceinline static vfloat2 Pow(const vfloat2 &value, const vfloat2 &exponent)
	{
		return vfloat2(powf(value.X, exponent.X), powf(value.Y, exponent.Y));
	}
	__forceinline static vfloat2 Pow(const vfloat2 &value, float exponent)
	{
		return vfloat2(powf(value.X, exponent), powf(value.Y, exponent));
	}
	__forceinline static vfloat3 Pow(const vfloat3 &value, const vfloat3 &exponent)
	{
		return _mm_pow_ps(value, exponent);
	}
	__forceinline static vfloat3 Pow(const vfloat3 &value, float exponent)
	{
		return _mm_pow_ps(value, _mm_set1_ps(exponent));
	}
	__forceinline static vfloat4 Pow(const vfloat4 &value, const vfloat4 &exponent)
	{
		return _mm_pow_ps(value, exponent);
	}
	__forceinline static vfloat4 Pow(const vfloat4 &value, float exponent)
	{
		return _mm_pow_ps(value, _mm_set1_ps(exponent));
	}
	__forceinline static vfloat8 Pow(const vfloat8 &value, const vfloat8 &exponent)
	{
		return _mm256_pow_ps(value, exponent);
	}
	__forceinline static vfloat8 Pow(const vfloat8 &value, float exponent)
	{
		return _mm256_pow_ps(value, _mm256_set1_ps(exponent));
	}

	__forceinline static vfloat2 Sqrt(const vfloat2 &value)
	{
		return vfloat2(sqrtf(value.X), sqrtf(value.Y));
	}
	__forceinline static vfloat3 Sqrt(const vfloat3 &value)
	{
		return _mm_sqrt_ps(value);
	}
	__forceinline static vfloat4 Sqrt(const vfloat4 &value)
	{
		return _mm_sqrt_ps(value);
	}
	__forceinline static vfloat8 Sqrt(const vfloat8 &value)
	{
		return _mm256_sqrt_ps(value);
	}

	__forceinline static vshort16 AddSaturation(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_adds_epi16(a, b);
	}
	__forceinline static vushort16 AddSaturation(const vushort16 &a, const vushort16 &b)
	{
		return _mm256_adds_epu16(a, b);
	}
	__forceinline static vsbyte32 AddSaturation(const vsbyte32 &a, const vsbyte32 &b)
	{
		return _mm256_adds_epi8(a, b);
	}
	__forceinline static vbyte32 AddSaturation(const vbyte32 &a, const vbyte32 &b)
	{
		return _mm256_adds_epu8(a, b);
	}
	__forceinline static vshort16 AddSaturation8(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_adds_epi8(a, b);
	}
	__forceinline static vushort16 AddSaturation8(const vushort16 &a, const vushort16 &b)
	{
		return _mm256_adds_epu8(a, b);
	}

	__forceinline static vushort16 MultiplyFixQ15(const vushort16 &a, const vushort16 &b)
	{
		return _mm256_mulhrs_epi16(a, b);
	}

	template<int32 index0, int32 index1, int32 index2, int32 index3>
	__forceinline static vint4 Shuffle(const vint4 &value)
	{
		return _mm_shuffle_epi32(value, _MM_SHUFFLE(index3, index2, index1, index0));
	}
	template<int32 a0, int32 a1, int32 b0, int32 b1>
	__forceinline static vfloat4 Shuffle(const vfloat4 &a, const vfloat4 &b)
	{
		return _mm_shuffle_ps(a, b, _MM_SHUFFLE(b1, b0, a1, a0));
	}
	template<int32 index0, int32 index1, int32 index2, int32 index3>
	__forceinline static vfloat4 Shuffle(const vfloat4 &value)
	{
		return _mm_shuffle_ps(value, value, _MM_SHUFFLE(index3, index2, index1, index0));
	}
	template<int32 a0, int32 a1, int32 b0, int32 b1>
	__forceinline static vfloat8 Shuffle(const vfloat8 &a, const vfloat8 &b)
	{
		return _mm256_shuffle_ps(a, b, _MM_SHUFFLE(b1, b0, a1, a0));
	}
	template<int32 index0, int32 index1, int32 index2, int32 index3>
	__forceinline static vfloat8 Shuffle(const vfloat8 &value)
	{
		return _mm256_shuffle_ps(value, value, _MM_SHUFFLE(index3, index2, index1, index0));
	}
	__forceinline static vfloat8 Shuffle(const vfloat8 &value, const vuint8 &indices)
	{
		return _mm256_permutevar8x32_ps(value, indices);
	}
	__forceinline static vsbyte32 Shuffle(const vsbyte32 &value, const vsbyte32 &mask)
	{
		return _mm256_shuffle_epi8(value, mask);
	}
	__forceinline static vbyte32 Shuffle(const vbyte32 &value, const vbyte32 &mask)
	{
		return _mm256_shuffle_epi8(value, mask);
	}

	__forceinline static vint3 Select(const vint3 &a, const vint3 &b, const vint3 &mask)
	{
		return _mm_blendv_epi8(a, b, mask);
	}
	__forceinline static vfloat3 Select(const vfloat3 &a, const vfloat3 &b, const vint3 &mask)
	{
		return _mm_blendv_ps(a, b, _mm_castsi128_ps(mask));
	}
	__forceinline static vint4 Select(const vint4 &a, const vint4 &b, const vint4 &mask)
	{
		return _mm_blendv_epi8(a, b, mask);
	}
	__forceinline static vlong4 Select(const vlong4 &a, const vlong4 &b, const vlong4 &mask)
	{
		return _mm256_blendv_epi8(a, b, mask);
	}
	__forceinline static vfloat4 Select(const vfloat4 &a, const vfloat4 &b, const vint4 &mask)
	{
		return _mm_blendv_ps(a, b, _mm_castsi128_ps(mask));
	}
	__forceinline static vint8 Select(const vint8 &a, const vint8 &b, const vint8 &mask)
	{
		return _mm256_blendv_epi8(a, b, mask);
	}
	__forceinline static vuint8 Select(const vuint8 &a, const vuint8 &b, const vuint8 &mask)
	{
		return _mm256_blendv_epi8(a, b, mask);
	}
	__forceinline static vfloat8 Select(const vfloat8 &a, const vfloat8 &b, const vint8 &mask)
	{
		return _mm256_blendv_ps(a, b, _mm256_castsi256_ps(mask));
	}
	__forceinline static vshort16 Select(const vshort16 &a, const vshort16 &b, const vshort16 &mask)
	{
		return _mm256_blendv_epi8(a, b, mask);
	}
	__forceinline static vushort16 Select(const vushort16 &a, const vushort16 &b, const vushort16 &mask)
	{
		return _mm256_blendv_epi8(a, b, mask);
	}
	__forceinline static vsbyte32 Select(const vsbyte32 &a, const vsbyte32 &b, const vsbyte32 &mask)
	{
		return _mm256_blendv_epi8(a, b, mask);
	}
	__forceinline static vbyte32 Select(const vbyte32 &a, const vbyte32 &b, const vbyte32 &mask)
	{
		return _mm256_blendv_epi8(a, b, mask);
	}

	__forceinline static vint8 Pack(const vshort16 &a, const vshort16 &b)
	{
		return _mm256_packs_epi16(a, b);
	}
	__forceinline static vuint8 Pack(const vushort16 &a, const vushort16 &b)
	{
		return _mm256_packus_epi16(a, b);
	}

	template<class T>
	__forceinline static T Reinterpret(const vfloat8 value)
	{
		return _mm256_castps_si256(value);
	}
	template<class T>
	__forceinline static vfloat8 Reinterpret(const T value)
	{
		return _mm256_castsi256_ps(value);
	}
};