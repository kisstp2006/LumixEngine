#pragma once


#include "core.h"


namespace Lumix {


struct Vec2;
struct Vec3;
struct DVec3;
struct Matrix;


struct LUMIX_CORE_API IVec2 {
	IVec2() {}
	explicit IVec2(i32 i) : x(i), y(i) {}
	explicit IVec2(const Vec2& rhs);
	IVec2(i32 x, i32 y) : x(x), y(y) {}
	IVec2 operator +(const IVec2& v) const { return IVec2(x + v.x, y + v.y); }
	IVec2 operator -(const IVec2& v) const { return IVec2(x - v.x, y - v.y); }
	IVec2 operator *(i32 i) const { return IVec2(x * i, y * i); }
	IVec2 operator / (i32 rhs) const { return IVec2(x / rhs, y / rhs); }
	IVec2 operator / (const IVec2& rhs) const { return IVec2(x / rhs.x, y / rhs.y); }
	inline Vec2 operator / (const Vec2& rhs) const;
	bool operator !=(const IVec2& rhs) const { return rhs.x != x || rhs.y != y; }
	bool operator ==(const IVec2& rhs) const { return rhs.x == x && rhs.y == y; }

	i32 x;
	i32 y;
};

struct LUMIX_CORE_API IVec3 {
	IVec3() {}
	IVec3(i32 x, i32 y, i32 z) : x(x), y(y), z(z) {}
	IVec3 operator +(const IVec3& v) const { return IVec3(x + v.x, y + v.y, z + v.z); }
	explicit IVec3(const DVec3& rhs);
	explicit IVec3(const Vec3& rhs);
	bool operator==(const IVec3& rhs) const { return x == rhs.x && y == rhs.y && z == rhs.z; }
	DVec3 operator *(double i) const;
	IVec2 xy() const { return {x, y}; }

	i32 x;
	i32 y;
    i32 z;
};

struct LUMIX_CORE_API IVec4 {
	IVec4() {}
	IVec4(i32 x, i32 y, i32 z, i32 w) : x(x), y(y), z(z), w(w) {}
	IVec4(i32 v) : x(v), y(v), z(v), w(v) {}
	IVec4(const IVec2& a, const IVec2& b) : x(a.x), y(a.y), z(b.x), w(b.y) {}
	void operator += (const IVec4& rhs);
	IVec2 xy() const { return {x,y}; }
	IVec2 zw() const { return {z,w}; }

	i32 x, y, z, w;
};

struct LUMIX_CORE_API DVec2 { 
	DVec2() {}
	DVec2(double x, double y) : x(x), y(y) {}
	DVec2 operator -(const DVec2& rhs) const { return {x - rhs.x, y - rhs.y}; }
	DVec2 operator +(const DVec2& rhs) const { return {x + rhs.x, y + rhs.y}; }
	DVec2 operator *(double rhs) const { return {x * rhs, y * rhs}; }
	double x, y;
};

struct LUMIX_CORE_API Vec2 {
	Vec2() {}
	
	explicit Vec2(const IVec2& rhs);
	explicit Vec2(const DVec2& rhs);
	explicit Vec2(float a);
	Vec2(float a, float b);

	float& operator[](u32 i);
	float operator[](u32 i) const;
	bool operator==(const Vec2& rhs) const;
	bool operator!=(const Vec2& rhs) const;
	void operator/=(float rhs);
	void operator*=(float f);

	Vec2 operator *(const Vec2& v) const;
	Vec2 operator /(const Vec2& v) const;
	Vec2 operator *(float f) const;
	Vec2 operator /(float f) const;
	Vec2 operator +(const Vec2& v) const;
	Vec2 operator +(float v) const;
	void operator +=(const Vec2& v);
	Vec2 operator -(const Vec2& v) const;
	Vec2 operator -(float f) const;
	Vec2 operator -() const;
	Vec2 ortho() const { return {y, -x}; }

	float x, y;

	static const Vec2 MAX;
	static const Vec2 MIN;
	static const Vec2 ZERO;
};

struct LUMIX_CORE_API Vec3 {
	Vec3() {}
	Vec3(const Vec2& v, float c);
	Vec3(float a, float b, float c);
	explicit Vec3(float a);
	explicit Vec3(const DVec3& rhs);
	explicit Vec3(const IVec3& rhs);

	float& operator[](u32 i);
	float operator[](u32 i) const;
	bool operator==(const Vec3& rhs) const;
	bool operator!=(const Vec3& rhs) const;
	Vec3 operator+(const Vec3& rhs) const;
	Vec3 operator-() const;
	Vec3 operator-(const Vec3& rhs) const;
	void operator+=(const Vec3& rhs);
	void operator-=(const Vec3& rhs);
	Vec3 operator*(float s) const;
	Vec3 operator*(const Vec3& rhs) const;
	Vec3 operator*(const IVec3& rhs) const;
	Vec3 operator/(const Vec3& rhs) const;
	Vec3 operator/(const IVec3& rhs) const;
	Vec3 operator-(float s) const;
	Vec3 operator/(float s) const;
	void operator/=(float rhs);
	void operator*=(float rhs);

	Vec2 xz() const;
	Vec2 yz() const;
	Vec2 xy() const;

	union {
		struct {
			float x, y, z;
		};
		struct {
			float r, g, b;
		};
	};

	static const Vec3 MAX;
	static const Vec3 MIN;
	static const Vec3 ZERO;
};

struct LUMIX_CORE_API DVec3
{
	DVec3() {}
	DVec3(double a);
	DVec3(double x, double y, double z);
	DVec3(const DVec3& rhs) = default;
	explicit DVec3(const Vec3& rhs);

	DVec3 operator-() const;
	DVec3 operator*(float rhs) const;
	DVec3 operator*(const Vec3& rhs) const;
	DVec3 operator/(float rhs) const;
	DVec3 operator/(const Vec3& rhs) const;
	DVec3 operator/(const DVec3& rhs) const;
	DVec3 operator-(const DVec3& rhs) const;
	DVec3 operator+(const DVec3& rhs) const;
	DVec3 operator-(const Vec3& rhs) const;
	DVec3 operator+(const Vec3& rhs) const;
	void operator*=(const double& rhs);
	void operator/=(const double& rhs);
	void operator+=(const DVec3& rhs);
	void operator+=(const Vec3& rhs);
	void operator-=(const Vec3& rhs);
	DVec2 xz() const;

	double x, y, z;
};

struct LUMIX_CORE_API Vec4 {
	Vec4() {}

	explicit Vec4(float a);

	Vec4(float a, float b, float c, float d);
	Vec4(const Vec2& v1, const Vec2& v2);
	Vec4(const Vec3& v, float d);

	Vec2 xz() const;
	Vec2 xy() const;
	Vec3 xyz() const;
	Vec2 yz() const;
	Vec2 yw() const;
	Vec3 rgb() const;

	float& operator[](u32 i);
	float operator[](u32 i) const;
	bool operator==(const Vec4& rhs) const;
	bool operator!=(const Vec4& rhs) const;
	Vec4 operator+(const Vec4& rhs) const;
	Vec4 operator-() const;
	Vec4 operator-(const Vec4& rhs) const;

	void operator+=(const Vec4& rhs);
	void operator-=(const Vec4& rhs);
	void operator/=(float rhs);
	Vec4 operator/(float s);
	Vec4 operator/(const Vec4& v);
	Vec4 operator*(float s);
	Vec4 operator*(const Vec4& v);
	void operator*=(float rhs);

	union {
		struct {
			float x, y, z, w;
		};
		struct {
			float r, g, b, a;
		};
	};

	static const Vec4 MAX;
	static const Vec4 MIN;
	static const Vec4 ZERO;
};


LUMIX_CORE_API Vec3 operator *(float f, const Vec3& v);
LUMIX_CORE_API Vec4 operator *(float f, const Vec4& v);
LUMIX_CORE_API Vec4 operator*(const Vec4& v, float s);
LUMIX_CORE_API Vec4 operator/(const Vec4& v, float s);

struct LUMIX_CORE_API Quat
{
	Quat() {}
	Quat(const Vec3& axis, float angle);
	Quat(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; } 

	void fromEuler(const Vec3& euler);
	Vec3 toEuler() const;
	float toYaw() const;
	void set(float _x, float _y, float _z, float _w) { x = _x; y = _y; z = _z; w = _w; } 
	void conjugate();
	Quat conjugated() const;
	Matrix toMatrix() const;

	Vec3 rotate(const Vec3& v) const;
	DVec3 rotate(const DVec3& v) const;
	Quat operator*(const Quat& q) const;
	Quat operator-() const;
	Quat operator+(const Quat& q) const;
	Quat operator*(float m) const;
	Vec3 operator*(const Vec3& q) const;

	static Quat vec3ToVec3(const Vec3& a, const Vec3& b);

	float x, y, z, w;
	
	static const Quat IDENTITY;
};

struct LUMIX_CORE_API DualQuat {
	Quat r;
	Quat d;
};

struct LUMIX_CORE_API LocalRigidTransform {
	LocalRigidTransform inverted() const;
	LocalRigidTransform operator*(const LocalRigidTransform& rhs) const;
	Matrix toMatrix() const;
	DualQuat toDualQuat() const;
	LocalRigidTransform interpolate(const LocalRigidTransform& rhs, float t) const;
	Vec3 pos;
	Quat rot;
};

struct LUMIX_CORE_API RigidTransform {
	RigidTransform() {}
	RigidTransform(const DVec3& pos, const Quat& rot);

	RigidTransform inverted() const;
	RigidTransform operator*(const RigidTransform& rhs) const;
	RigidTransform operator*(const LocalRigidTransform& rhs) const;

	Quat rot;
	DVec3 pos;
};


// single precision position, uniform scale
struct LUMIX_CORE_API LocalTransform {
	LocalTransform() {}
	LocalTransform(const Vec3& pos, const Quat& rot, float scale);

	LocalTransform inverted() const;

	LocalTransform operator*(const LocalTransform& rhs) const;

	Vec3 pos;
	Quat rot;
	float scale;
};


// double precision position, quaternion rotation, nonuniform scale
// this is NOT the same as Matrix
// 	* it behaves like a matrix only when the scale is uniform
// 	* when composing multiple transforms with nonuniform scale, there's no skew
// 	* it behaves like transforms in most other engines (e.g. Unreal)
// 	* scale is lossy, i.e. when composing multiple transforms, "direction" of the original scale is lost
struct LUMIX_CORE_API Transform {
	Transform() {}
	Transform(const DVec3& pos, const Quat& rot, Vec3 scale);
	
	static Transform computeLocal(const Transform& parent, const Transform& child);

	Transform compose(const Transform& rhs) const;
	Transform compose(const LocalRigidTransform& rhs) const;
	DVec3 transform(const DVec3& value) const;
	DVec3 transform(const Vec3& value) const;
	DVec3 invTransform(const DVec3& value) const;
	DVec3 invTransform(const Vec3& value) const;
	Vec3 transformVector(const Vec3& value) const;
	Vec3 invTransformVector(const Vec3& value) const;
	RigidTransform getRigidPart() const;

	DVec3 pos;
	Quat rot;
	Vec3 scale;

	static const Transform IDENTITY;
};

struct alignas(16) LUMIX_CORE_API Matrix {
	static Matrix rotationX(float angle);
	static Matrix rotationY(float angle);
	static Matrix rotationZ(float angle);

	Matrix() {}

	Matrix(const Vec4& col0, const Vec4& col1, const Vec4& col2, const Vec4& col3);
	Matrix(const Vec3& pos, const Quat& rot);

	void decompose(Vec3& position, Quat& rotation, float& scale) const;
	void decompose(Vec3& position, Quat& rotation, Vec3& scale) const;

	float operator[](int index) const { return (&columns[0].x)[index]; }
	float& operator[](int index) { return (&columns[0].x)[index]; }

	Matrix operator*(const Matrix& rhs) const;
	Matrix operator+(const Matrix& rhs) const;
	Matrix operator*(float rhs) const;
	Vec4 operator*(const Vec4& rhs) const { return columns[0] * rhs.x + columns[1] * rhs.y + columns[2] * rhs.z + columns[3] * rhs.w; }

	Vec3 getZVector() const { return columns[2].xyz(); }
	Vec3 getYVector() const { return columns[1].xyz(); }
	Vec3 getXVector() const { return columns[0].xyz(); }

	void setXVector(const Vec3& v);
	void setYVector(const Vec3& v);
	void setZVector(const Vec3& v);

	float determinant() const;
	Matrix inverted() const;
	// orthonormal
	Matrix fastInverted() const;
	void copy3x3(const Matrix& mtx);
	void translate(const Vec3& t);
	void translate(float x, float y, float z);
	void setTranslation(const Vec3& t);

	void setOrtho(float left, float right, float bottom, float top, float z_near, float z_far, bool reversed_z);
	// reversed z, infinite far plane
	void setPerspective(float vertical_fov, float xy_ratio, float near_plane);
	void fromEuler(float yaw, float pitch, float roll);
	void lookAt(const Vec3& eye, const Vec3& at, const Vec3& up);
	void normalizeScale();

	Vec3 getTranslation() const { return columns[3].xyz(); }
	Quat getRotation() const;
	void transpose();
	Vec3 transformPoint(const Vec3& pos) const;
	Vec3 transformVector(const Vec3& pos) const;
	void multiply3x3(float scale);
	void multiply3x3(const Vec3& scale);
	void setIdentity();

	Vec4 columns[4];

	static const Matrix IDENTITY;
};

struct Matrix3x4 {
	Matrix3x4() {}
	explicit Matrix3x4(const Matrix& rhs);

	Vec4 columns[3];
};

struct LUMIX_CORE_API Matrix4x3 {
	Matrix4x3() {}
	explicit Matrix4x3(const Matrix& rhs);
	Matrix3x4 transposed() const;

	Vec3 columns[4];
};

constexpr float PI = 3.14159265f;
constexpr float HALF_PI = 3.14159265f * 0.5f;
constexpr float SQRT2 = 1.41421356237f;
constexpr float SQRT3 = 1.73205080757f;


template <typename T> LUMIX_FORCE_INLINE void swap(T& a, T& b) {
	T tmp = static_cast<T&&>(a);
	a = static_cast<T&&>(b);
	b = static_cast<T&&>(tmp);
}

template <typename T> LUMIX_FORCE_INLINE T minimum(T a) {
	return a;
}

template <typename T> LUMIX_FORCE_INLINE T minimum(T a, T b) {
	return a < b ? a : b;
}

LUMIX_FORCE_INLINE Vec2 minimum(const Vec2& a, const Vec2& b) {
	return {
		minimum(a.x, b.x),
		minimum(a.y, b.y),
	};
}

LUMIX_FORCE_INLINE IVec2 minimum(const IVec2& a, const IVec2& b) {
	return {
		minimum(a.x, b.x),
		minimum(a.y, b.y),
	};
}

LUMIX_FORCE_INLINE DVec3 minimum(const DVec3& a, const DVec3& b) {
	return {
		minimum(a.x, b.x),
		minimum(a.y, b.y),
		minimum(a.z, b.z)
	};
}

LUMIX_FORCE_INLINE Vec3 minimum(const Vec3& a, const Vec3& b) {
	return {
		minimum(a.x, b.x),
		minimum(a.y, b.y),
		minimum(a.z, b.z)
	};
}

LUMIX_FORCE_INLINE Vec4 minimum(const Vec4& a, const Vec4& b) {
	return {
		minimum(a.x, b.x),
		minimum(a.y, b.y),
		minimum(a.z, b.z),
		minimum(a.w, b.w)
	};
}

template <typename T1, typename... T2> LUMIX_FORCE_INLINE T1 minimum(T1 a, T2... b) {
	T1 min_b = minimum(b...);
	return minimum(a, min_b);
}

template <typename T> LUMIX_FORCE_INLINE T maximum(T a) {
	return a;
}

template <typename T1, typename... T2> LUMIX_FORCE_INLINE T1 maximum(T1 a, T2... b) {
	T1 min_b = maximum(b...);
	return a > min_b ? a : min_b;
}

LUMIX_FORCE_INLINE Vec2 maximum(const Vec2& a, const Vec2& b) {
	return {
		maximum(a.x, b.x),
		maximum(a.y, b.y),
	};
}

LUMIX_FORCE_INLINE IVec2 maximum(const IVec2& a, const IVec2& b) {
	return {
		maximum(a.x, b.x),
		maximum(a.y, b.y),
	};
}

LUMIX_FORCE_INLINE DVec3 maximum(const DVec3& a, const DVec3& b) {
	return {
		maximum(a.x, b.x),
		maximum(a.y, b.y),
		maximum(a.z, b.z)
	};
}

LUMIX_FORCE_INLINE Vec3 maximum(const Vec3& a, const Vec3& b) {
	return {
		maximum(a.x, b.x),
		maximum(a.y, b.y),
		maximum(a.z, b.z)
	};
}

LUMIX_FORCE_INLINE Vec4 maximum(const Vec4& a, const Vec4& b) {
	return {
		maximum(a.x, b.x),
		maximum(a.y, b.y),
		maximum(a.z, b.z),
		maximum(a.w, b.w)
	};
}

template <typename T> LUMIX_FORCE_INLINE T signum(T a) {
	return a > 0 ? (T)1 : (a < 0 ? (T)-1 : 0);
}

template <typename T1, typename T2, typename T3> LUMIX_FORCE_INLINE T1 clamp(T1 value, T2 min_value, T3 max_value) {
	return minimum(maximum(value, min_value), max_value);
}


template <typename T> bool isPowOfTwo(T n)
{
	return (n) && !(n & (n - 1));
}

LUMIX_CORE_API float dot(const Vec4& op1, const Vec4& op2);
LUMIX_CORE_API float dot(const Vec3& op1, const Vec3& op2);
LUMIX_CORE_API float dot(const Vec2& op1, const Vec2& op2);
LUMIX_CORE_API Vec3 cross(const Vec3& op1, const Vec3& op2);
LUMIX_CORE_API DVec3 cross(const DVec3& op1, const DVec3& op2);

LUMIX_CORE_API Vec4 lerp(const Vec4& op1, const Vec4& op2, float t);
LUMIX_CORE_API float lerp(float a, float b, float t);
LUMIX_CORE_API Vec3 lerp(const Vec3& op1, const Vec3& op2, float t);
LUMIX_CORE_API DVec3 lerp(const DVec3& op1, const DVec3& op2, float t);
LUMIX_CORE_API Vec2 lerp(const Vec2& op1, const Vec2& op2, float t);
LUMIX_CORE_API Vec3 slerp(const Vec3& a, const Vec3& b, float t);
LUMIX_CORE_API Quat nlerp(const Quat& q1, const Quat& q2, float t);

LUMIX_CORE_API u32 nextPow2(u32 v);
LUMIX_CORE_API u32 log2(u32 v);
LUMIX_CORE_API float degreesToRadians(float angle);
LUMIX_CORE_API double degreesToRadians(double angle);
LUMIX_CORE_API float degreesToRadians(int angle);
LUMIX_CORE_API float radiansToDegrees(float angle);
LUMIX_CORE_API Vec2 radiansToDegrees(const Vec2& v);
LUMIX_CORE_API Vec3 radiansToDegrees(const Vec3& v);
LUMIX_CORE_API Vec2 degreesToRadians(const Vec2& v);
LUMIX_CORE_API Vec3 degreesToRadians(const Vec3& v);
LUMIX_CORE_API float easeInOut(float t);
LUMIX_CORE_API float angleDiff(float a, float b);
LUMIX_CORE_API u64 randGUID();
LUMIX_CORE_API u32 rand();
LUMIX_CORE_API u32 rand(u32 from, u32 to);
LUMIX_CORE_API float randFloat();
LUMIX_CORE_API float randFloat(float from, float to);
LUMIX_CORE_API DVec2 normalize(const DVec2& value);
LUMIX_CORE_API Vec2 normalize(const Vec2& value);
LUMIX_CORE_API Vec3 normalize(const Vec3& value);
LUMIX_CORE_API Quat normalize(const Quat& value);
LUMIX_CORE_API float length(const Vec2& value);
LUMIX_CORE_API float length(const Vec3& value);
LUMIX_CORE_API double length(const DVec3& value);
LUMIX_CORE_API float squaredLength(const Vec2& value);
LUMIX_CORE_API float squaredLength(const Vec3& value);
LUMIX_CORE_API double squaredLength(const DVec2& value);
LUMIX_CORE_API double squaredLength(const DVec3& value);
LUMIX_CORE_API float halton(u32 index, i32 base);

struct LUMIX_CORE_API RandomGenerator {
	RandomGenerator(u32 u = 521288629, u32 v = 362436069);
	u32 rand();
	float randFloat(float from, float to);
	float randFloat();
private:
	u32 u;
	u32 v;
};

} // namespace Lumix
