package com.game.alex.androidgame.GameObjects;

/**
 * Created by kag on 21.09.2017.
 */

public class GameStructs {
    public enum  EDirection {
        None ,
        Right,
        Left,
        Top,
        Bottom
    }
    //      Цвет
    static public class OGLColor {
        public float		r = 0.0f;
        public float		g = 0.0f;
        public float		b = 0.0f;
        public float		a = 0.0f;
        public OGLColor(float R, float G, float B, float A) {
            r = R; g = G; b = B; a = A; };
        public OGLColor() { this( 1.0f, 1.0f, 1.0f, 1.0f );};
        public void set(float R, float G, float B, float A) {
            r = R; g = G; b = B; a = A; };
    }


    static public class Vec3 {
        public float		x = 0.0f;
        public float		y = 0.0f;
        public float		z = 0.0f;
        public Vec3(float X, float Y, float Z) {
            x = X;
            y = Y;
            z = Z;
        }
        public Vec3() {}

        // assignment operators
      /*  Vec3 operator += ( Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
        }
        Vec3& operator -= (CONST Vec3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
        }
        Vec3& operator *= (float f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
        }
        Vec3& operator /= (float f) {
        float fInv = 1.0f / f;
        x *= fInv;
        y *= fInv;
        z *= fInv;
        return *this;
        }


        // unary operators
        Vec3 operator + () const {
        return *this;
        }
        Vec3 operator - () const {
        return Vec3(-x, -y, -z);
        }


        // binary operators
        Vec3 operator + (CONST Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
        }
        Vec3 operator - (CONST Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
        }
        Vec3 operator * (float f) const {
        return Vec3(x * f, y * f, z * f);
        }
        Vec3 operator / (float f) const {
        float fInv = 1.0f / f;
        return Vec3(x * fInv, y * fInv, z * fInv);
        }  */
    };
}



