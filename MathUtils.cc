#include "MathUtils.h"

//Vector
vec3 SetVector(GLfloat x, GLfloat y, GLfloat z)
{
	vec3 v;

	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

vec3 VectorSub(vec3 a, vec3 b)
{
	vec3 result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return result;
}

vec3 VectorAdd(vec3 a, vec3 b)
{
	vec3 result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return result;
}

vec3 CrossProduct(vec3 a, vec3 b)
{
	vec3 result;

	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;

	return result;
}

GLfloat DotProduct(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

vec3 ScalarMult(vec3 a, GLfloat s)
{
	vec3 result;

	result.x = a.x * s;
	result.y = a.y * s;
	result.z = a.z * s;

	return result;
}

GLfloat Norm(vec3 a)
{
	GLfloat result;

	result = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	return result;
}

vec3 Normalize(vec3 a)
{
	GLfloat norm;
	vec3 result;

	norm = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	result.x = a.x / norm;
	result.y = a.y / norm;
	result.z = a.z / norm;
	return result;
}

vec3 CalcNormalVector(vec3 a, vec3 b, vec3 c)
{
	vec3 n;

	n = CrossProduct(VectorSub(a, b), VectorSub(a, c));
	n = ScalarMult(n, 1 / Norm(n));

	return n;
}

bool PointIsOnPositiveSide(vec3 point, vec3 normal, vec3 pointOnLine)
{
    //vec3 n = Normalize(vec3(-lineDir.z, 0.0f, lineDir.x));

    float D = DotProduct(ScalarMult(normal, -1), pointOnLine);
    
    return DotProduct(point, normal) + D > 0;
}

bool LinePlaneIntersection(vec3 l, vec3 l0, vec3 planeNormal, vec3 p0)
{
    return DotProduct(l, planeNormal) != 0;
}

bool LinePlaneIntersection(vec3 l, vec3 l0, vec3 planeNormal, vec3 p0, vec3& intersection)
{
    if (DotProduct(l, planeNormal) != 0)
    {
	GLfloat d = DotProduct(VectorSub(p0, l0), planeNormal) / DotProduct(l, planeNormal);

	intersection = VectorAdd(ScalarMult(l, d), l0);
	
	return true;
    }    
    else
    {
	return false;
    }
}

//Matrix

mat4 IdentityMatrix()
{
	mat4 m;
	int i;

	for (i = 0; i <= 15; i++)
		m.m[i] = 0;
	for (i = 0; i <= 3; i++)
		m.m[i * 5] = 1; // 0,5,10,15
	return m;
}

mat4 Rx(GLfloat a)
{
	mat4 m;
	m = IdentityMatrix();
	m.m[5] = cos(a);
	m.m[9] = sin(a);
	m.m[6] = -m.m[9]; //sin(a);
	m.m[10] = m.m[5]; //cos(a);
	return m;
}

mat4 Ry(GLfloat a)
{
	mat4 m;
	m = IdentityMatrix();
	m.m[0] = cos(a);
	m.m[8] = sin(a);
	m.m[2] = -m.m[8]; //sin(a);
	m.m[10] = m.m[0]; //cos(a);
	return m;
}

mat4 Rz(GLfloat a)
{
	mat4 m;
	m = IdentityMatrix();
	m.m[0] = cos(a);
	m.m[4] = sin(a);
	m.m[1] = -m.m[4]; //sin(a);
	m.m[5] = m.m[0]; //cos(a);
	return m;
}

mat4 T(GLfloat tx, GLfloat ty, GLfloat tz)
{
	mat4 m;
	m = IdentityMatrix();
	m.m[3] = tx;
	m.m[7] = ty;
	m.m[11] = tz;
	
	return m;
}

mat4 TVec3(vec3 t)
{
    return T(t.x, t.y, t.z);
}

mat4 S(GLfloat sx, GLfloat sy, GLfloat sz)
{
	mat4 m;
	m = IdentityMatrix();
	m.m[0] = sx;
	m.m[5] = sy;
	m.m[10] = sz;
	return m;
}

mat4 Mult(mat4 a, mat4 b)
{
	mat4 m;

	int x, y;
	for (x = 0; x <= 3; x++)
		for (y = 0; y <= 3; y++)
		{
			m.m[y * 4 + x] = a.m[y * 4 + 0] * b.m[0 * 4 + x] +
							a.m[y * 4 + 1] * b.m[1 * 4 + x] +
							a.m[y * 4 + 2] * b.m[2 * 4 + x] +
							a.m[y * 4 + 3] * b.m[3 * 4 + x];
		}

	return m;
}

vec3 MultVec3(mat4 a, vec3 b)
{
	vec3 r;

	r.x = a.m[0] * b.x + a.m[1] * b.y + a.m[2] * b.z + a.m[3];
	r.y = a.m[4] * b.x + a.m[5] * b.y + a.m[6] * b.z + a.m[7];
	r.z = a.m[8] * b.x + a.m[9] * b.y + a.m[10] * b.z + a.m[11];
	
	return r;
}

mat4 Transpose(mat4 m)
{
	mat4 a;

	a.m[0] = m.m[0]; a.m[4] = m.m[1]; a.m[8] = m.m[2];      a.m[12] = m.m[3];
	a.m[1] = m.m[4]; a.m[5] = m.m[5]; a.m[9] = m.m[6];      a.m[13] = m.m[7];
	a.m[2] = m.m[8]; a.m[6] = m.m[9]; a.m[10] = m.m[10];    a.m[14] = m.m[11];
	a.m[3] = m.m[12]; a.m[7] = m.m[13]; a.m[11] = m.m[14];    a.m[15] = m.m[15];

	return a;
}

mat4 ArbRotate(vec3 axis, GLfloat fi)
{
	vec3 x, y, z;
	mat4 R, Rt, Raxel, m;

	// Check if parallel to Z
	if (axis.x < 0.0000001) // Below some small value
		if (axis.x > -0.0000001)
			if (axis.y < 0.0000001)
				if (axis.y > -0.0000001)
				{
					if (axis.z > 0)
					{
						m = Rz(fi);
						return m;
					}
					else
					{
						m = Rz(-fi);
						return m;
					}
				}

	x = Normalize(axis);
	z = SetVector(0, 0, 1); // Temp z
	y = Normalize(CrossProduct(z, x)); // y' = z^ x x'
	z = CrossProduct(x, y); // z' = x x y

	
	R.m[0] = x.x; R.m[1] = x.y; R.m[2] = x.z;  R.m[3] = 0.0;
	R.m[4] = y.x; R.m[5] = y.y; R.m[6] = y.z;  R.m[7] = 0.0;
	R.m[8] = z.x; R.m[9] = z.y; R.m[10] = z.z;  R.m[11] = 0.0;

	R.m[12] = 0.0; R.m[13] = 0.0; R.m[14] = 0.0;  R.m[15] = 1.0;

	Rt = Transpose(R); // Transpose = Invert -> felet ej i Transpose, och det Šr en ortonormal matris

	Raxel = Rx(fi); // Rotate around x axis

	// m := Rt * Rx * R
	m = Mult(Mult(Rt, Raxel), R);

	return m;
}

mat4 MatrixAdd(mat4 a, mat4 b)
{
	mat4 dest;

	int i;
	for (i = 0; i < 16; i++)
		dest.m[i] = a.m[i] + b.m[i];

	return dest;
}


mat4 lookAtv(vec3 p, vec3 l, vec3 v)
{
	vec3 n, u;

	n = Normalize(VectorSub(p, l));
	u = Normalize(CrossProduct(v, n));
	v = CrossProduct(n, u);

	mat4 rot = { { u.x, u.y, u.z, 0,
		v.x, v.y, v.z, 0,
		n.x, n.y, n.z, 0,
		0, 0, 0, 1 } };
	mat4 trans;
	trans = T(-p.x, -p.y, -p.z);
	return Mult(rot, trans);
}

mat4 lookAt(GLfloat px, GLfloat py, GLfloat pz,
	GLfloat lx, GLfloat ly, GLfloat lz,
	GLfloat vx, GLfloat vy, GLfloat vz)
{
	vec3 p, l, v;

	p = SetVector(px, py, pz);
	l = SetVector(lx, ly, lz);
	v = SetVector(vx, vy, vz);

	return lookAtv(p, l, v);
}

mat4 perspective(float fovyInDegrees, float aspectRatio,
	float znear, float zfar)
{
	float ymax, xmax;
	ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
	if (aspectRatio < 1.0)
	{
		ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
		xmax = ymax * aspectRatio;
	}
	else
	{
		xmax = znear * tanf(fovyInDegrees * M_PI / 360.0);
		ymax = xmax / aspectRatio;
	}

	return frustum(-xmax, xmax, -ymax, ymax, znear, zfar);
}

mat4 frustum(float left, float right, float bottom, float top,
	float znear, float zfar)
{
	float temp, temp2, temp3, temp4;
	mat4 matrix;

	temp = 2.0 * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	matrix.m[0] = temp / temp2; // 2*near/(right-left)
	matrix.m[1] = 0.0;
	matrix.m[2] = 0.0;
	matrix.m[3] = 0.0;
	matrix.m[4] = 0.0;
	matrix.m[5] = temp / temp3; // 2*near/(top - bottom)
	matrix.m[6] = 0.0;
	matrix.m[7] = 0.0;
	matrix.m[8] = (right + left) / temp2; // A = r+l / r-l
	matrix.m[9] = (top + bottom) / temp3; // B = t+b / t-b
	matrix.m[10] = (-zfar - znear) / temp4; // C = -(f+n) / f-n
	matrix.m[11] = -1.0;
	matrix.m[12] = 0.0;
	matrix.m[13] = 0.0;
	matrix.m[14] = (-temp * zfar) / temp4; // D = -2fn / f-n
	matrix.m[15] = 0.0;

	matrix = Transpose(matrix);

	return matrix;
}
