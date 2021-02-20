#pragma once
#include "Math3d.h"
//  Класс Matrix - по аналогии с Java. По возможности используются теже прототипы и набор методов.
//  Все методы  статические.

class CMatrix
{
public:
	//		Sets matrix m to the identity matrix.
	//	Java : void setIdentityM( float[] sm, int smOffset ) - 
	//	sm 	float: returns the result
	//	smOffset 	int : index into sm where the result matrix starts - в нашем случае полагаем  = 0
	static void setIdentityM( SOGLMatrix& Matrix ) {
		glhLoadIdentityf2(&Matrix);
	}
	//
	static void orthoM(SOGLMatrix& matrix_, float left, float right, float bottom, float top, float znear, float zfar) {
		glhOrthof2( &matrix_, left, right, bottom, top, znear, zfar);
	}
	//	Translates matrix m by x, y, and z in place.
	//	Java : void translateM (float[] m, int mOffset, float x, float y, float z)
	//	m 	float: matrix
	//	mOffset 	int : index into m where the matrix starts  - в нашем случае полагаем  = 0
	//	x 	float : translation factor x
	//	y 	float : translation factor y
	//	z 	float : translation factor z
	static void translateM(SOGLMatrix& Matrix, float x, float y, float z) {
		glhTranslatef2(&Matrix, x, y, z);
	}
	//	Creates a matrix for rotation by angle a(in degrees) around the axis(x, y, z).
	//	Java : 	void setRotateM(float[] rm, int rmOffset, float a, float x, float y, float z)
	//		rm 	float: returns the result
	//		rmOffset 	int : index into rm where the result matrix starts - в нашем случае полагаем  = 0
	//		a 	float : angle to rotate in degrees
	//		x 	float : X axis component
	//		y 	float : Y axis component
	//		z 	float : Z axis component
	static void setRotateM(SOGLMatrix& Matrix, float a, float x, float y, float z) {
		glhRotatef2(&Matrix, a/180.0f*M_PIF, x, y, z);
	}

	//	Scales matrix m in place by sx, sy, and sz.
	//	Java : void scaleM (float[] m, int mOffset, float x, float y, float z)
	//		m 	float: matrix to scale
	//		mOffset 	int : index into m where the matrix starts - в нашем случае полагаем  = 0
	//		x 	float : scale factor x
	//		y 	float : scale factor y
	//		z 	float : scale factor z
	static void scaleM (SOGLMatrix& Matrix, float x, float y, float z)  {
		glhScalef2(&Matrix,  x,  y,  z);
	}

	//	Multiplies two 4x4 matrices together and stores the result in a third 4x4 matrix.In matrix notation : result = lhs x rhs.
	//	Due to the way matrix multiplication works, the result matrix will have the same effect as first multiplying by the rhs matrix, then multiplying by the lhs matrix.This is the opposite of what you might expect.
	//	Java : 	void multiplyMM(float[] result, int resultOffset, float[] lhs, int lhsOffset, float[] rhs, int rhsOffset)
	//		The same float array may be passed for result, lhs, and / or rhs.However, the result element values are undefined if the result
	//	elements overlap either the lhs or rhs elements.
	//		Parameters
	//		result 	float : The float array that holds the result.
	//		resultOffset 	int : The offset into the result array where the result is stored. - в нашем случае полагаем  = 0
	//		lhs 	float : The float array that holds the left - hand - side matrix.
	//		lhsOffset 	int : The offset into the lhs array where the lhs is stored - в нашем случае полагаем  = 0
	//		rhs 	float : The float array that holds the right - hand - side matrix.
	//		rhsOffset 	int : The offset into the rhs array where the rhs is stored. - в нашем случае полагаем  = 0
	//		Throws
	//		IllegalArgumentException 	if result, lhs, or rhs are null, or if resultOffset + 16 > result.length or lhsOffset + 16 > 
	//		lhs.length or rhsOffset + 16 > rhs.length.
	static void multiplyMM(SOGLMatrix& result, const SOGLMatrix& lhs,  const SOGLMatrix& rhs ) {
		MultiplyMatrices(&result, &lhs, &rhs);
	}
};