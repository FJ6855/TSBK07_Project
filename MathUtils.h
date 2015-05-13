#ifndef MATH_UTILS
#define MATH_UTILS

#include <math.h>
#include <GL/gl.h>

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif

struct vec2
{
vec2() : x(0), y(0) {}
vec2(GLfloat x2, GLfloat y2) : x(x2), y(y2) {}
    
    GLfloat x;
    GLfloat y;
};

struct vec3
{
	vec3() : x(0), y(0), z(0) {}
	vec3(GLfloat x2, GLfloat y2, GLfloat z2) : x(x2), y(y2), z(z2) {}
	
	GLfloat x;
	GLfloat y;
	GLfloat z;

};

struct mat4
{
	GLfloat m[16];
};

//Vector
vec3 SetVector(GLfloat x, GLfloat y, GLfloat z);
vec3 VectorSub(vec3 a, vec3 b);
vec3 VectorAdd(vec3 a, vec3 b);
vec3 CrossProduct(vec3 a, vec3 b);
GLfloat DotProduct(vec3 a, vec3 b);
vec3 ScalarMult(vec3 a, GLfloat s);
GLfloat Norm(vec3 a);
vec3 Normalize(vec3 a);
vec3 CalcNormalVector(vec3 a, vec3 b, vec3 c);
bool PointIsOnPositiveSide(vec3 point, vec3 normal, vec3 pointOnLine);
bool LinePlaneIntersection(vec3 l, vec3 l0, vec3 planeNormal, vec3 p0);
bool LinePlaneIntersection(vec3 l, vec3 l0, vec3 planeNormal, vec3 p0, vec3& intersection);

//Matrix
mat4 IdentityMatrix();
mat4 Rx(GLfloat a);
mat4 Ry(GLfloat a);
mat4 Rz(GLfloat a);
mat4 T(GLfloat tx, GLfloat ty, GLfloat tz);
mat4 TVec3(vec3 t);
mat4 S(GLfloat sx, GLfloat sy, GLfloat sz);
mat4 Mult(mat4 a, mat4 b);
vec3 MultVec3(mat4 a, vec3 b);
mat4 Transpose(mat4 m);
mat4 ArbRotate(vec3 axis, GLfloat fi);
mat4 MatrixAdd(mat4 a, mat4 b);

mat4 lookAtv(vec3 p, vec3 l, vec3 v);
mat4 lookAt(GLfloat px, GLfloat py, GLfloat pz,
	GLfloat lx, GLfloat ly, GLfloat lz,
	GLfloat vx, GLfloat vy, GLfloat vz);

mat4 perspective(float fovyInDegrees, float aspectRatio,
	float znear, float zfar);
mat4 frustum(float left, float right, float bottom, float top,
	float znear, float zfar);


// --- vec3 operations ---
inline
vec3 operator+(const vec3 &a, const vec3 &b) // vec3+vec3
{
	return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline
vec3 operator-(const vec3 &a, const vec3 &b) // vec3-vec3
{
	return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

// Questionable, not like GLSL
inline
float operator*(const vec3 &a, const vec3 &b) // vec3 dot vec3
{
	return (a.x*b.x + a.y*b.y + a.z*b.z);
}

inline
vec3 operator*(const vec3 &b, double a) // vec3 * scalar
{
	return vec3(a*b.x, a*b.y, a*b.z);
}

inline
vec3 operator*(double a, const vec3 &b) // scalar * vec3
{
	return vec3(a*b.x, a*b.y, a*b.z);
}

inline
vec3 operator/(const vec3 &b, double a) // vec3 / scalar
{
	return vec3(b.x / a, b.y / a, b.z / a);
}

inline
void operator+=(vec3 &a, const vec3 &b) // vec3+=vec3
{
	a = a + b;
}

inline
void operator-=(vec3 &a, const vec3 &b) // vec3-=vec3
{
	a = a - b;
}

inline
void operator*=(vec3 &a, const float &b) // vec3*=scalar
{
	a = a * b;
}

inline
void operator/=(vec3 &a, const float &b) // vec3/=scalar
{
	a = a / b;
}

// --- Matrix multiplication ---

// mat4 * mat4
inline
mat4 operator*(const mat4 &a, const mat4 &b)
{
	return (Mult(a, b));
}

// mat4 * vec3
inline
vec3 operator*(const mat4 &a, const vec3 &b)
{
	return MultVec3(a, b); // result = a * b
}

#endif
