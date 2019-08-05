#if !defined(MATH_H)
#define MATH_H

union vec3_t 
{
	struct { r32 x, y, z; };
	struct { r32 r, g, b; };
	r32 data[3];
    
	r32 &operator[](int i) { return data[i]; }
	const r32 &operator[](int i) const { return data[i]; }
};

union vec3i_t 
{
	struct { s32 x, y, z; };
	struct { s32 r, g, b; };
	s32 data[3];
    
	s32 &operator[](int i) { return data[i]; }
	const s32 &operator[](int i) const { return data[i]; }
};

internal inline vec3_t 
MakeVec3(r32 x, r32 y, r32 z) 
{
	vec3_t v = {};
    
	v[0] = x;
	v[1] = y;
	v[2] = z;
    
	return v;
}

internal inline vec3_t 
MV3(r32 x, r32 y, r32 z) 
{
	vec3_t v = {};
    
	v[0] = x;
	v[1] = y;
	v[2] = z;
    
	return v;
}

internal inline vec3_t 
operator +(vec3_t a, vec3_t b) 
{
	vec3_t v = {};
    
	v[0] = a[0] + b[0];
	v[1] = a[1] + b[1];
	v[2] = a[2] + b[2];
    
	return v;
}

internal inline vec3i_t 
operator +(vec3i_t a, vec3i_t b) 
{
	vec3i_t v = {};
    
	v[0] = a[0] + b[0];
	v[1] = a[1] + b[1];
	v[2] = a[2] + b[2];
    
	return v;
}

internal inline vec3_t 
operator -(vec3_t a) 
{
	vec3_t v = {};
    
	v[0] = -a[0]; 
	v[1] = -a[1];
	v[2] = -a[2];
    
	return v;
}

internal inline vec3i_t 
operator -(vec3i_t a) 
{
	vec3i_t v = {};
    
	v[0] = -a[0]; 
	v[1] = -a[1];
	v[2] = -a[2];
    
	return v;
}

internal inline vec3_t 
operator -(vec3_t a, vec3_t b) 
{
	vec3_t v = {};
    
	v[0] = a[0] - b[0];
	v[1] = a[1] - b[1];
	v[2] = a[2] - b[2];
    
	return v;
}

internal inline vec3i_t 
operator -(vec3i_t a, vec3i_t b) 
{
	vec3i_t v = {};
    
	v[0] = a[0] - b[0];
	v[1] = a[1] - b[1];
	v[2] = a[2] - b[2];
    
	return v;
}

internal inline vec3_t 
operator +(vec3_t a, r32 s) 
{
	vec3_t v = {};
    
	v[0] = a[0] + s;
	v[1] = a[1] + s;
	v[2] = a[2] + s;
    
	return v;
}

internal inline vec3_t 
operator -(vec3_t a, r32 s) 
{
	vec3_t v = {};
    
	v[0] = a[0] - s;
	v[1] = a[1] - s;
	v[2] = a[2] - s;
    
	return v;
}

internal inline vec3_t 
operator *(vec3_t a, r32 s) 
{
	vec3_t v = {};
    
	v[0] = a[0] * s;
	v[1] = a[1] * s;
	v[2] = a[2] * s;
    
	return v;
}

internal inline vec3_t 
operator *(r32 s, vec3_t a) 
{
	vec3_t v = {};
    
	v[0] = a[0] * s;
	v[1] = a[1] * s;
	v[2] = a[2] * s;
    
	return v;
}

internal inline r32 
operator *(vec3_t a, vec3_t b) 
{
r32 result = {};
result = a[0]*b[0] + a[1]*b[1] + a[2]*b[2];

return result;
}

internal inline r32 
Dot3(vec3_t v1, vec3_t v2) 
{
	r32 result = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
    
	return result;
}

internal inline s32 
Dot3i(vec3i_t v1, vec3i_t v2) 
{
	s32 result = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
    
	return result;
}

internal inline r32 
Dot3(r32 *v1, r32 *v2) 
{
	r32 result = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
    
	return result;
}

internal inline r32 
Dot3(r32 *v1, vec3_t v2) 
{
	r32 result = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
    
	return result;
}

internal inline r32 
Dot3(vec3_t v1, r32 *v2) 
{
	r32 result = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];
    
	return result;
}

internal inline r32
Lerp(r32 a, r32 b, r32 t)
{
r32 result = (1.0f - t)*a + t*b;
return result;
}

typedef struct plane_t
{
vec3_t n;
r32 d;
} plane_t;
typedef struct world_t
{
} world_t;


#endif   // MATH_H