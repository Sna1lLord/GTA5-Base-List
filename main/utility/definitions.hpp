#ifndef _DEFINITIONS_HPP
#define _DEFINITIONS_HPP

#include <common.hpp>

struct Vector4 {
    float x, y, z, w;
};

class Vector3 {
public:
    float x;
    float y;
    float z;
public:
    Vector3 operator+(const Vector3& other) {
        Vector3 result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;

		return result;
	}

    Vector3 operator-(const Vector3& other) {
        Vector3 result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;

		return result;
	}

    Vector3 operator*(const Vector3& other) {
        Vector3 result;
        result.x = x * other.x;
        result.y = y * other.y;
        result.z = z * other.z;
        
		return result;
	}

    Vector3 operator*(float other) {
        Vector3 result;
        result.x = x * other;
        result.y = y * other;
        result.z = z * other;
        
		return result;
	}
};

struct Vector2 {
    float x, y;
};

class NativeVector {
public:
    float x;
    unsigned long paddingX;
    float y;
    unsigned long paddingY;
    float z;
    unsigned long paddingZ;
public:
    NativeVector operator+(const NativeVector& other) {
        NativeVector result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;

		return result;
	}

    NativeVector operator-(const NativeVector& other) {
        NativeVector result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;

		return result;
	}

    NativeVector operator*(const NativeVector& other) {
        NativeVector result;
        result.x = x * other.x;
        result.y = y * other.y;
        result.z = z * other.z;
        
		return result;
	}

    NativeVector operator*(float other) {
        NativeVector result;
        result.x = x * other;
        result.y = y * other;
        result.z = z * other;
        
		return result;
	}

    int Distance(const NativeVector& other) {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;

        dx = std::abs(dx);
        dy = std::abs(dy);
        dz = std::abs(dz);

        float squaredDistance = dx * dx + dy * dy + dz * dz;
        int distanceInMeters = static_cast<int>(std::sqrt(squaredDistance));

        return distanceInMeters;
    }
};

class PaddingVector {
public:
    float x;
    float paddingX;
    float y;
    float paddingY;
    float z;
    float paddingZ;
public:
    NativeVector operator+(const NativeVector& other) {
        NativeVector result;
        result.x = x + other.x;
        result.y = y + other.y;
        result.z = z + other.z;

		return result;
	}

    NativeVector operator-(const NativeVector& other) {
        NativeVector result;
        result.x = x - other.x;
        result.y = y - other.y;
        result.z = z - other.z;

		return result;
	}

    NativeVector operator*(const NativeVector& other) {
        NativeVector result;
        result.x = x * other.x;
        result.y = y * other.y;
        result.z = z * other.z;
        
		return result;
	}

    NativeVector operator*(float other) {
        NativeVector result;
        result.x = x * other;
        result.y = y * other;
        result.z = z * other;
        
		return result;
	}
};

struct IntVector3 {
	int x, y, z;
};

#endif