#pragma once
#include "include.h"

struct Vector2 {
	float x, y;

	float dist(Vector2 V) {
		return sqrt(pow(x - V.x, 2) + pow(y - V.y, 2));
	}
	Vector2 operator+(Vector2 Vec2_)
	{
		return { x + Vec2_.x,y + Vec2_.y };
	}
	Vector2 operator-(Vector2 Vec2_)
	{
		return { x - Vec2_.x,y - Vec2_.y };
	}
	Vector2 operator*(Vector2 Vec2_)
	{
		return { x * Vec2_.x,y * Vec2_.y };
	}
	Vector2 operator/(Vector2 Vec2_)
	{
		return { x / Vec2_.x,y / Vec2_.y };
	}
	Vector2 operator*(float n)
	{
		return { x / n,y / n };
	}
	Vector2 operator/(float n)
	{
		return { x / n,y / n };
	}
};

struct Vector3 {
	float x, y, z;

	float dist(Vector3 V) {
		return sqrt(pow(x - V.x, 2) + pow(y - V.y, 2) + pow(z - V.z, 2));
	}
	Vector3 operator+(Vector3 Vec3_)
	{
		return { x + Vec3_.x,y + Vec3_.y,z + Vec3_.z };
	}
	Vector3 operator-(Vector3 Vec3_)
	{
		return { x - Vec3_.x,y - Vec3_.y,z - Vec3_.z };
	}
	Vector3 operator*(Vector3 Vec3_)
	{
		return { x * Vec3_.x,y * Vec3_.y,z * Vec3_.z };
	}
	Vector3 operator/(Vector3 Vec3_)
	{
		return { x / Vec3_.x,y / Vec3_.y,z / Vec3_.z };
	}
	Vector3 operator*(float n)
	{
		return { x * n,y * n,z * n };
	}
	Vector3 operator/(float n)
	{
		return { x / n,y / n,z / n };
	}
};

struct Vector4 {
	float x, y, z, w;

	float dist(Vector4 V) {
		return sqrt(pow(x - V.x, 2) + pow(y - V.y, 2) + pow(z - V.z, 2) + pow(w - V.w, 2));
	}
	Vector4 operator+(Vector4 Vec4_)
	{
		return { x + Vec4_.x,y + Vec4_.y,z + Vec4_.z,w + Vec4_.w };
	}
	Vector4 operator-(Vector4 Vec4_)
	{
		return { x - Vec4_.x,y - Vec4_.y,z - Vec4_.z,w - Vec4_.w };
	}
	Vector4 operator*(Vector4 Vec4_)
	{
		return { x * Vec4_.x,y * Vec4_.y,z * Vec4_.z,w * Vec4_.w };
	}
	Vector4 operator/(Vector4 Vec4_)
	{
		return { x / Vec4_.x,y / Vec4_.y,z / Vec4_.z,w / Vec4_.w };
	}
	Vector4 operator*(float n)
	{
		return { x * n,y * n,z * n,w * n };
	}
	Vector4 operator/(float n)
	{
		return { x / n,y / n,z / n,w / n };
	}
};

struct C_UTL_VECTOR
{
	DWORD64 Count = 0;
	DWORD64 Data = 0;
};