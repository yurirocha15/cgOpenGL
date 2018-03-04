
//#pragma once

//###################################################################
// C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
// C++ STL
#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>
// windows
#if !defined(__GNUC__)&&(defined(_WIN32)||defined(_WIN64))
	#include <windows.h>
	#include <wchar.h>
#endif

// user types
typedef unsigned int	uint;
typedef unsigned char	uchar;
typedef unsigned short	ushort;
//###################################################################

template <class T> struct tvec2
{
	union{ struct { T r, g; }; struct { T x, y; }; };

	// constructor/set
	tvec2(){ x=y=0; }
	tvec2( T a ){ x=y=a; }						inline void set( T a ){ x=y=a; }
	tvec2( T a, T b ){ x=a;y=b; }				inline void set( T a, T b ){ x=a;y=b; }
	tvec2( const tvec2& v ){ x=v.x;y=v.y; }		inline void set( const tvec2& v ){ x=v.x;y=v.y; }
	tvec2( const T* a ){ x=a[0];y=a[1]; }		inline void set( const T* a ){ x=a[0];y=a[1]; }

	// assignment
	inline tvec2& operator=( T a ){ set(a); return *this; }
	inline tvec2& operator+=( const tvec2& v ){ x+=v.x; y+=v.y; return *this; }
	inline tvec2& operator-=( const tvec2& v ){ x-=v.x; y-=v.y; return *this; }
	inline tvec2& operator*=( const tvec2& v ){ x*=v.x; y*=v.y; return *this; }
	inline tvec2& operator/=( const tvec2& v ){ x/=v.x; y/=v.y; return *this; }
	inline tvec2& operator+=( T a ){ x+=a; y+=a; return *this; }
	inline tvec2& operator-=( T a ){ x-=a; y-=a; return *this; }
	inline tvec2& operator*=( T a ){ x*=a; y*=a; return *this; }
	inline tvec2& operator/=( T a ){ x/=a; y/=a; return *this; }

	// casting
	inline operator T*(){ return &x; }
	inline operator const T*() const { return &x; }

	// array operator
	inline T& operator[]( int i ){ return (&r)[i];}
	inline const T& operator[]( int i ) const { return (&r)[i];}
	inline T& at( int i ){ return (&r)[i]; }
	inline const T& at( int i ) const { return (&r)[i];}

	// unary operators
	inline tvec2 operator+(){ return *this; }
	inline tvec2 operator+() const { return *this; }
	inline tvec2 operator-(){ return tvec2(-r,-g); }
	inline tvec2 operator-() const { return tvec2(-r,-g); }

	// binary operators
	inline tvec2 operator+( const tvec2& v ) const { return tvec2(this->x + v.x, this->y + v.y); }
	inline tvec2 operator-( const tvec2& v ) const { return tvec2(this->x - v.x, this->y - v.y); }
	inline tvec2 operator*( const tvec2& v ) const { return tvec2(this->x * v.x, this->y * v.y); }
	inline tvec2 operator/( const tvec2& v ) const { return tvec2(this->x / v.x, this->y / v.y); }
	inline tvec2 operator+( T a ) const { return tvec2(this->x + a, this->y + a); }
	inline tvec2 operator-( T a ) const { return tvec2(this->x - a, this->y - a);  }
	inline tvec2 operator*( T a ) const { return tvec2(this->x * a, this->y * a);  }
	inline tvec2 operator/( T a ) const { return tvec2(this->x / a, this->y / a);  }

	// length, normalize, dot product
	inline T length(){ return sqrt(this->x * this->x + this->y * this->y); }
	inline T dot( const tvec2& v ) const { return this->x * v.x + this->y * v.y; }
	inline tvec2 normalize(){ return *this / length(); }
};

template <class T> struct tvec3
{
	union { struct { T r, g, b; }; struct { T x, y, z; }; };

	// constructor/set
	tvec3(){ x=y=z=0; }
	tvec3( T a ){ x=y=z=a; }								inline void set( T a ){ x=y=z=a; }
	tvec3( T a, T b, T c ){ x=a;y=b;z=c; }					inline void set( T a, T b, T c ){ x=a;y=b;z=c; }
	tvec3( const tvec3& v ){ x=v.x;y=v.y;z=v.z; }			inline void set( const tvec3& v ){ x=v.x;y=v.y;z=v.z; }
	tvec3( const T* a ){ x=a[0];y=a[1];z=a[2]; }			inline void set( const T* a ){ x=a[0];y=a[1];z=a[2]; }
	tvec3( const tvec2<T>& v, T c ){ x=v.x;y=v.y;z=c; }		inline void set( const tvec2<T>& v, T c ){ x=v.x;y=v.y;z=c; }
	tvec3( T a, const tvec2<T>& v ){ x=a;y=v.x;z=v.y; }		inline void set( T a, const tvec2<T>& v ){ x=a;y=v.x;z=v.y; }

	// assignment
	inline tvec3& operator=( T a ){ set(a); return *this; }
	inline tvec3& operator=( const T* a ){ set(a); return *this; }
	inline tvec3& operator+=( const tvec3& v ){ x+=v.x; y+=v.y; z+=v.z; return *this; }
	inline tvec3& operator-=( const tvec3& v ){ x-=v.x; y-=v.y; z-=v.z; return *this; }
	inline tvec3& operator*=( const tvec3& v ){ x*=v.x; y*=v.y; z*=v.z; return *this; }
	inline tvec3& operator/=( const tvec3& v ){ x/=v.x; y/=v.y; z/=v.z; return *this; }
	inline tvec3& operator+=( T a ){ x+=a; y+=a; z+=a; return *this; }
	inline tvec3& operator-=( T a ){ x-=a; y-=a; z-=a; return *this; }
	inline tvec3& operator*=( T a ){ x*=a; y*=a; z*=a; return *this; }
	inline tvec3& operator/=( T a ){ x/=a; y/=a; z/=a; return *this; }

	// casting
	inline operator T*(){ return &x; }
	inline operator const T*(){ return &x; }

	// array operator
	inline T& operator[]( int i ){ return (&r)[i]; }
	inline const T& operator[]( int i ) const { return (&r)[i]; }
	inline T& at( int i ){ return (&r)[i]; }
	inline const T& at( int i ) const { return (&r)[i]; }
	
	// unary operators
	inline tvec3 operator+(){ return *this; }
	inline tvec3 operator+() const { return *this; }
	inline tvec3 operator-(){ return tvec3(-r,-g,-b); }
	inline tvec3 operator-() const { return tvec3(-r,-g,-b); }

	// binary operators
	inline tvec3 operator+( const tvec3& v ) const { return tvec3(this->x + v.x, this->y + v.y, this->z + v.z); }
	inline tvec3 operator-( const tvec3& v ) const { return tvec3(this->x - v.x, this->y - v.y, this->z - v.z); }
	inline tvec3 operator*( const tvec3& v ) const { return tvec3(this->x * v.x, this->y * v.y, this->z * v.z); }
	inline tvec3 operator/( const tvec3& v ) const { return tvec3(this->x / v.x, this->y / v.y, this->z / v.z); }
	inline tvec3 operator+( T a ) const { return tvec3(this->x + a, this->y + a, this->z + a); }
	inline tvec3 operator-( T a ) const { return tvec3(this->x - a, this->y - a, this->z - a); }
	inline tvec3 operator*( T a ){ return tvec3(this->x * a, this->y * a, this->z * a); }
	inline tvec3 operator/( T a ){ return tvec3(this->x / a, this->y / a, this->z / a); }
		
	// length, normalize, dot product
	inline T length(){ return sqrt(this->x * this->x + this->y * this->y + this->z * this->z); }
	inline tvec3 normalize(){ return *this / length(); }
	inline T dot( const tvec3& v ) const { return this->x * v.x + this->y * v.y + this->z * v.z; }

	// tvec3 only: cross product
	inline tvec3 operator^( tvec3& v ){ return tvec3(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x); }
	inline tvec3 operator^( const tvec3& v ) const { return tvec3(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x); }
	inline tvec3 cross( tvec3& v ){ return tvec3(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x); }
	inline tvec3 cross( const tvec3& v ) const { return tvec3(this->y * v.z - this->z * v.y, this->z * v.x - this->x * v.z, this->x * v.y - this->y * v.x); }
};

template <class T> struct tvec4
{
	union { struct { T r, g, b, a; }; struct { T x, y, z, w; }; };

	// constructor/set
	tvec4(){ x=y=z=w=0; }
	tvec4( T a ){ x=y=z=w=a; }										inline void set( T a ){ x=y=z=w=a; }
	tvec4( T a, T b, T c, T d ){ x=a;y=b;z=c;w=d; }					inline void set( T a, T b, T c, T d ){ x=a;y=b;z=c;w=d; }
	tvec4( const tvec4& v ){ x=v.x;y=v.y;z=v.z;w=v.w; }				inline void set( const tvec4& v ){ x=v.x;y=v.y;z=v.z;w=v.w; }
	tvec4( const T* a ){ x=a[0];y=a[1];z=a[2];w=a[3]; }				inline void set( const T* a ){ x=a[0];y=a[1];z=a[2];w=a[3]; }
	tvec4( const tvec2<T>& v, T c, T d ){ x=v.x;y=v.y;z=c;w=d; }	inline void set( const tvec2<T>& v, T c, T d ){ x=v.x;y=v.y;z=c;w=d; }
	tvec4( T a, T b, const tvec2<T>& v ){ x=a;y=b;z=v.x;w=v.y; }	inline void set( T a, T b, const tvec2<T>& v ){ x=a;y=b;z=v.x;w=v.y; }	
	tvec4( const tvec3<T>& v, T d ){ x=v.x;y=v.y;z=v.z;w=d; }		inline void set( const tvec3<T>& v, T d ){ x=v.x;y=v.y;z=v.z;w=d; }
	tvec4( T a, const tvec3<T>& v ){ x=a;y=v.x;z=v.y;w=v.z; }		inline void set( T a, const tvec3<T>& v ){ x=a;y=v.x;z=v.y;w=v.z; }
	tvec4( const tvec2<T>& v1, const tvec2<T>& v2 ){ x=v1.x;y=v1.y;z=v2.x;w=v2.y; }
	inline void set( const tvec2<T>& v1, const tvec2<T>& v2 ){ x=v1.x;y=v1.y;z=v2.x;w=v2.y; }

	// assignment
	inline tvec4& operator=( T a ){ set(a); return *this; }
	inline tvec4& operator=( const T* a ){ set(a); return *this; }
	inline tvec4& operator+=( const tvec4& v ){ x+=v.x; y+=v.y; z+=v.z; w+=v.w; return *this; }
	inline tvec4& operator-=( const tvec4& v ){ x-=v.x; y-=v.y; z-=v.z; w-=v.w; return *this; }
	inline tvec4& operator*=( const tvec4& v ){ x*=v.x; y*=v.y; z*=v.z; w*=v.w; return *this; }
	inline tvec4& operator/=( const tvec4& v ){ x/=v.x; y/=v.y; z/=v.z; w/=v.w; return *this; }
	inline tvec4& operator+=( T a ){ x+=a; y+=a; z+=a; w+=a; return *this; }
	inline tvec4& operator-=( T a ){ x-=a; y-=a; z-=a; w-=a; return *this; }
	inline tvec4& operator*=( T a ){ x*=a; y*=a; z*=a; w*=a; return *this; }
	inline tvec4& operator/=( T a ){ x/=a; y/=a; z/=a; w/=a; return *this; }
	
	// casting
	inline operator T*(){ return &x; }
	inline operator const T*(){ return &x; }

	// array operator
	inline T& operator[]( int i ){ return (&r)[i]; }
	inline const T& operator[]( int i ) const { return (&r)[i]; }
	inline T& at( int i ){ return (&r)[i]; }
	inline const T& at( int i ) const { return (&r)[i]; }

	// unary operators
	inline tvec4 operator+(){ return *this; }
	inline tvec4 operator+() const { return *this; }
	inline tvec4 operator-(){ return tvec4(-r,-g,-b,-a); }
	inline tvec4 operator-() const { return tvec4(-r,-g,-b,-a); }

	// binary operators
	inline tvec4 operator+( const tvec4& v ) const { return tvec4(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w); }
	inline tvec4 operator-( const tvec4& v ) const { return tvec4(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w); }
	inline tvec4 operator*( const tvec4& v ) const { return tvec4(this->x * v.x, this->y * v.y, this->z * v.z, this->w * v.w); }
	inline tvec4 operator/( const tvec4& v ) const { return tvec4(this->x / v.x, this->y / v.y, this->z / v.z, this->w / v.w); }
	inline tvec4 operator+( T a ) const { return tvec4(this->x + a, this->y + a, this->z + a, this->w + a); }
	inline tvec4 operator-( T a ) const { return tvec4(this->x - a, this->y - a, this->z - a, this->w - a); }
	inline tvec4 operator*( T a ){ return tvec4(this->x * a, this->y * a, this->z * a, this->w * a); }
	inline tvec4 operator/( T a ){ return tvec4(this->x / a, this->y / a, this->z / a, this->w / a); }

	// length, normalize, dot product
	inline T length(){ return sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w); }
	inline tvec4 normalize(){ return *this/length(); }
	inline T dot( const tvec4& v ) const { return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w; }
};

//*******************************************************************
// typedefs
typedef tvec2<float>	vec2;
typedef tvec3<float>	vec3;
typedef tvec4<float>	vec4;
typedef tvec2<int>		ivec2;
typedef tvec3<int>		ivec3;
typedef tvec4<int>		ivec4;

//*******************************************************************
// matrix 4x4: uses a standard row-major notation.
struct mat4
{
	union { float a[16]; struct {float _11,_12,_13,_14,_21,_22,_23,_24,_31,_32,_33,_34,_41,_42,_43,_44;}; };

	mat4(){ _12=_13=_14=_21=_23=_24=_31=_32=_34=_41=_42=_43=0.0f;_11=_22=_33=_44=1.0f; }
	mat4( float* f ){ memcpy(a,f,sizeof(float)*16); }
	mat4( float f11, float f12, float f13, float f14, float f21, float f22, float f23, float f24, float f31, float f32, float f33, float f34, float f41, float f42, float f43, float f44 ){_11=f11;_12=f12;_13=f13;_14=f14;_21=f21;_22=f22;_23=f23;_24=f24;_31=f31;_32=f32;_33=f33;_34=f34;_41=f41;_42=f42;_43=f43;_44=f44;}
	
	// casting
	inline operator float*(){ return a; }
	inline operator const float*() const { return a; }

	// array access operator
	inline float& operator[]( unsigned i ){ return a[i]; }
	inline float& operator[]( int i ){ return a[i]; }
	inline const float& operator[]( unsigned i ) const { return a[i]; }
	inline const float& operator[]( int i ) const { return a[i]; }
	
	// basic operation
	inline mat4& setIdentity(){ _12=_13=_14=_21=_23=_24=_31=_32=_34=_41=_42=_43=0.0f;_11=_22=_33=_44=1.0f; }
	inline mat4 transpose() const { return mat4(this->_11, this->_21, this->_31, this->_41, this->_12, this->_22, this->_32, this->_42, this->_13, this->_23, this->_33, this->_43, this->_14, this->_24, this->_34, this->_44); }

	// binary operator overloading
	inline vec4 operator*( const vec4& v )
	{
		return vec4(this->_11 * v.x + this->_12 * v.y + this->_13 * v.z + this->_14 * v.w, 
					this->_21 * v.x + this->_22 * v.y + this->_23 * v.z + this->_24 * v.w, 
					this->_31 * v.x + this->_32 * v.y + this->_33 * v.z + this->_34 * v.w, 
					this->_41 * v.x + this->_42 * v.y + this->_43 * v.z + this->_44 * v.w); 
	}
	inline mat4 operator*( mat4& M )
	{
		return mat4(this->_11 * M._11 + this->_12 * M._21 + this->_13 * M._31 + this->_14 * M._41,
					this->_11 * M._12 + this->_12 * M._22 + this->_13 * M._32 + this->_14 * M._42,
					this->_11 * M._13 + this->_12 * M._23 + this->_13 * M._33 + this->_14 * M._43,
					this->_11 * M._14 + this->_12 * M._24 + this->_13 * M._34 + this->_14 * M._44,	//Line 1
					this->_21 * M._11 + this->_22 * M._21 + this->_23 * M._31 + this->_24 * M._41,
					this->_21 * M._12 + this->_22 * M._22 + this->_23 * M._32 + this->_24 * M._42,
					this->_21 * M._13 + this->_22 * M._23 + this->_23 * M._33 + this->_24 * M._43,
					this->_21 * M._14 + this->_22 * M._24 + this->_23 * M._34 + this->_24 * M._44,	//Line 2
					this->_31 * M._11 + this->_32 * M._21 + this->_33 * M._31 + this->_34 * M._41,
					this->_31 * M._12 + this->_32 * M._22 + this->_33 * M._32 + this->_34 * M._42,
					this->_31 * M._13 + this->_32 * M._23 + this->_33 * M._33 + this->_34 * M._43,
					this->_31 * M._14 + this->_32 * M._24 + this->_33 * M._34 + this->_34 * M._44,	//Line 3
					this->_41 * M._11 + this->_42 * M._21 + this->_43 * M._31 + this->_44 * M._41,
					this->_41 * M._12 + this->_42 * M._22 + this->_43 * M._32 + this->_44 * M._42,
					this->_41 * M._13 + this->_42 * M._23 + this->_43 * M._33 + this->_44 * M._43,
					this->_41 * M._14 + this->_42 * M._24 + this->_43 * M._34 + this->_44 * M._44);	//Line 4
	}

	inline float determinant() const 
	{
		return 	this->_14 * this->_23 * this->_32 * this->_41 - this->_13 * this->_24 * this->_32 * this->_41 -
				this->_14 * this->_22 * this->_33 * this->_41 + this->_12 * this->_24 * this->_33 * this->_41 +
				this->_13 * this->_22 * this->_34 * this->_41 - this->_12 * this->_23 * this->_34 * this->_41 -
				this->_14 * this->_23 * this->_31 * this->_42 + this->_13 * this->_24 * this->_31 * this->_42 +
				this->_14 * this->_21 * this->_33 * this->_42 - this->_11 * this->_24 * this->_33 * this->_42 -
				this->_13 * this->_21 * this->_34 * this->_42 + this->_11 * this->_23 * this->_34 * this->_42 +
				this->_14 * this->_22 * this->_31 * this->_43 - this->_12 * this->_24 * this->_31 * this->_43 -
				this->_14 * this->_21 * this->_32 * this->_43 + this->_11 * this->_24 * this->_32 * this->_43 +
				this->_12 * this->_21 * this->_34 * this->_43 - this->_11 * this->_22 * this->_34 * this->_43 -
				this->_13 * this->_22 * this->_31 * this->_44 + this->_12 * this->_23 * this->_31 * this->_44 +
				this->_13 * this->_21 * this->_32 * this->_44 - this->_11 * this->_23 * this->_32 * this->_44 -
				this->_12 * this->_21 * this->_33 * this->_44 + this->_11 * this->_22 * this->_33 * this->_44;
	}

	inline mat4 inverse() const 
	{
		float det=determinant(); if( det==0 ) printf( "mat4::inverse() might be singular.\n" );
		else
		{
			mat4 inv(
				this->a[5]  * this->a[10] * this->a[15] - 
				this->a[5]  * this->a[11] * this->a[14] - 
				this->a[9]  * this->a[6]  * this->a[15] + 
				this->a[9]  * this->a[7]  * this->a[14] +
				this->a[13] * this->a[6]  * this->a[11] - 
				this->a[13] * this->a[7]  * this->a[10],		//_11

				-this->a[1]  * this->a[10] * this->a[15] + 
				this->a[1]  * this->a[11] * this->a[14] + 
				this->a[9]  * this->a[2] * this->a[15] - 
				this->a[9]  * this->a[3] * this->a[14] - 
				this->a[13] * this->a[2] * this->a[11] + 
				this->a[13] * this->a[3] * this->a[10],			//_12

				this->a[1]  * this->a[6] * this->a[15] - 
				this->a[1]  * this->a[7] * this->a[14] - 
				this->a[5]  * this->a[2] * this->a[15] + 
				this->a[5]  * this->a[3] * this->a[14] + 
				this->a[13] * this->a[2] * this->a[7] - 
				this->a[13] * this->a[3] * this->a[6],			//_13

				-this->a[1] * this->a[6] * this->a[11] + 
				this->a[1] * this->a[7] * this->a[10] + 
				this->a[5] * this->a[2] * this->a[11] - 
				this->a[5] * this->a[3] * this->a[10] - 
				this->a[9] * this->a[2] * this->a[7] + 
				this->a[9] * this->a[3] * this->a[6],			//_14

				-this->a[4]  * this->a[10] * this->a[15] + 
				this->a[4]  * this->a[11] * this->a[14] + 
				this->a[8]  * this->a[6]  * this->a[15] - 
				this->a[8]  * this->a[7]  * this->a[14] - 
				this->a[12] * this->a[6]  * this->a[11] + 
				this->a[12] * this->a[7]  * this->a[10],		//_21

				this->a[0]  * this->a[10] * this->a[15] - 
				this->a[0]  * this->a[11] * this->a[14] - 
				this->a[8]  * this->a[2] * this->a[15] + 
				this->a[8]  * this->a[3] * this->a[14] + 
				this->a[12] * this->a[2] * this->a[11] - 
				this->a[12] * this->a[3] * this->a[10],			//_22

				-this->a[0]  * this->a[6] * this->a[15] + 
				this->a[0]  * this->a[7] * this->a[14] + 
				this->a[4]  * this->a[2] * this->a[15] - 
				this->a[4]  * this->a[3] * this->a[14] - 
				this->a[12] * this->a[2] * this->a[7] + 
				this->a[12] * this->a[3] * this->a[6],			//_23

				this->a[0] * this->a[6] * this->a[11] - 
				this->a[0] * this->a[7] * this->a[10] - 
				this->a[4] * this->a[2] * this->a[11] + 
				this->a[4] * this->a[3] * this->a[10] + 
				this->a[8] * this->a[2] * this->a[7] - 
				this->a[8] * this->a[3] * this->a[6],			//_24

				this->a[4]  * this->a[9] * this->a[15] - 
				this->a[4]  * this->a[11] * this->a[13] - 
				this->a[8]  * this->a[5] * this->a[15] + 
				this->a[8]  * this->a[7] * this->a[13] + 
				this->a[12] * this->a[5] * this->a[11] - 
				this->a[12] * this->a[7] * this->a[9],			//_31

				-this->a[0]  * this->a[9] * this->a[15] + 
				this->a[0]  * this->a[11] * this->a[13] + 
				this->a[8]  * this->a[1] * this->a[15] - 
				this->a[8]  * this->a[3] * this->a[13] - 
				this->a[12] * this->a[1] * this->a[11] + 
				this->a[12] * this->a[3] * this->a[9],			//_32

				this->a[0]  * this->a[5] * this->a[15] - 
				this->a[0]  * this->a[7] * this->a[13] - 
				this->a[4]  * this->a[1] * this->a[15] + 
				this->a[4]  * this->a[3] * this->a[13] + 
				this->a[12] * this->a[1] * this->a[7] - 
				this->a[12] * this->a[3] * this->a[5],			//_33

				-this->a[0] * this->a[5] * this->a[11] + 
				this->a[0] * this->a[7] * this->a[9] + 
				this->a[4] * this->a[1] * this->a[11] - 
				this->a[4] * this->a[3] * this->a[9] - 
				this->a[8] * this->a[1] * this->a[7] + 
				this->a[8] * this->a[3] * this->a[5],			//_34

				-this->a[4]  * this->a[9] * this->a[14] + 
				this->a[4]  * this->a[10] * this->a[13] +
				this->a[8]  * this->a[5] * this->a[14] - 
				this->a[8]  * this->a[6] * this->a[13] - 
				this->a[12] * this->a[5] * this->a[10] + 
				this->a[12] * this->a[6] * this->a[9],			//_41

				this->a[0]  * this->a[9] * this->a[14] - 
				this->a[0]  * this->a[10] * this->a[13] - 
				this->a[8]  * this->a[1] * this->a[14] + 
				this->a[8]  * this->a[2] * this->a[13] + 
				this->a[12] * this->a[1] * this->a[10] - 
				this->a[12] * this->a[2] * this->a[9],			//_42

				-this->a[0]  * this->a[5] * this->a[14] + 
				this->a[0]  * this->a[6] * this->a[13] + 
				this->a[4]  * this->a[1] * this->a[14] - 
				this->a[4]  * this->a[2] * this->a[13] - 
				this->a[12] * this->a[1] * this->a[6] + 
				this->a[12] * this->a[2] * this->a[5],			//_43

				this->a[0] * this->a[5] * this->a[10] - 
				this->a[0] * this->a[6] * this->a[9] - 
				this->a[4] * this->a[1] * this->a[10] + 
				this->a[4] * this->a[2] * this->a[9] + 
				this->a[8] * this->a[1] * this->a[6] - 
				this->a[8] * this->a[2] * this->a[5]);			//_44

            float invDet = 1.0f / det;

            for(int i = 0; i < 16; i++)
            	inv[i] = inv[i] * invDet;

			return inv;
		}


	}
};

//*******************************************************************
// additional utility functions
inline vec2 operator+( float f, vec2& v ){ return v+f; }
inline vec3 operator+( float f, vec3& v ){ return v+f; }
inline vec4 operator+( float f, vec4& v ){ return v+f; }
inline vec2 operator-( float f, vec2& v ){ return -v+f; }
inline vec3 operator-( float f, vec3& v ){ return -v+f; }
inline vec4 operator-( float f, vec4& v ){ return -v+f; }
inline vec2 operator*( float f, vec2& v ){ return v*f; }
inline vec3 operator*( float f, vec3& v ){ return v*f; }
inline vec4 operator*( float f, vec4& v ){ return v*f; }
inline float dot(vec2& v1,vec2& v2){ return v1.dot(v2); }
inline float dot(vec3& v1,vec3& v2){ return v1.dot(v2); }
inline float dot(vec4& v1,vec4& v2){ return v1.dot(v2); }

//*******************************************************************
// common macros
#ifndef _CRT_SECURE_NO_WARNINGS
	#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef PI
	#define PI 3.141592653589793f
#endif
#ifndef max
	#define max(a,b) ((a)>(b)?(a):(b))
#endif
#ifndef min
	#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef clamp
	#define clamp(value,vmin,vmax) (max(vmin,min(vmax,value)))
#endif
#ifndef isnan
	#define	isnan(x) (x!=x)
#endif
