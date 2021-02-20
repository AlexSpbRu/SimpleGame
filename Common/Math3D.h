#ifndef MATH3D_H_
#define MATH3D_H_

#define M_PIF  3.141592654f
#pragma pack(4)

struct SOGLColor {
	GLfloat		r = 0.0f;
	GLfloat		g = 0.0f;
	GLfloat		b = 0.0f;
	GLfloat		a = 0.0f;
	SOGLColor(GLfloat R, GLfloat G, GLfloat B, GLfloat A) : r(R), g(G), b(B), a(A) {};
	SOGLColor() = default;
};

struct Vec2 {
	GLfloat		x = 0.0f;
	GLfloat		y = 0.0f;
	Vec2( GLfloat X, GLfloat Y ) : x(X), y(Y) {}
	Vec2() {}
};


typedef struct MyD3DXPLANE {
	float a;
	float b;
	float c;
	float d;
} MyD3DXPLANE, *LPMyD3DXPLANE;

struct  Vec3 {
	GLfloat		x = 0.0f;
	GLfloat		y = 0.0f;
	GLfloat		z = 0.0f;
	Vec3(GLfloat X, GLfloat Y, GLfloat Z) : x(X), y(Y), z(Z) {}
	Vec3() {}

	// assignment operators
	Vec3& operator += (CONST Vec3& v) {
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
	}
};


typedef struct MyD3DXVECTOR4 {
	float x;
	float y;
	float z;
	float w;
} MyD3DXVECTOR4, *LPMyD3DXVECTOR4;
#define  MyD3DXQUATERNION MyD3DXVECTOR4

struct SOGLMatrix {
	union {
		struct {
			GLfloat        _11, _12, _13, _14;
			GLfloat        _21, _22, _23, _24;
			GLfloat        _31, _32, _33, _34;
			GLfloat        _41, _42, _43, _44;

		};
		GLfloat m[4][4];
	};
} ;


inline void MultiplyMatrices4by4OpenGL_FLOAT(GLfloat *result, GLfloat *matrix1, GLfloat *matrix2)
{
	result[0]=matrix1[0]*matrix2[0]+
		matrix1[4]*matrix2[1]+
		matrix1[8]*matrix2[2]+
		matrix1[12]*matrix2[3];
	result[4]=matrix1[0]*matrix2[4]+
		matrix1[4]*matrix2[5]+
		matrix1[8]*matrix2[6]+
		matrix1[12]*matrix2[7];
	result[8]=matrix1[0]*matrix2[8]+
		matrix1[4]*matrix2[9]+
		matrix1[8]*matrix2[10]+
		matrix1[12]*matrix2[11];
	result[12]=matrix1[0]*matrix2[12]+
		matrix1[4]*matrix2[13]+
		matrix1[8]*matrix2[14]+
		matrix1[12]*matrix2[15];

	result[1]=matrix1[1]*matrix2[0]+
		matrix1[5]*matrix2[1]+
		matrix1[9]*matrix2[2]+
		matrix1[13]*matrix2[3];
	result[5]=matrix1[1]*matrix2[4]+
		matrix1[5]*matrix2[5]+
		matrix1[9]*matrix2[6]+
		matrix1[13]*matrix2[7];
	result[9]=matrix1[1]*matrix2[8]+
		matrix1[5]*matrix2[9]+
		matrix1[9]*matrix2[10]+
		matrix1[13]*matrix2[11];
	result[13]=matrix1[1]*matrix2[12]+
		matrix1[5]*matrix2[13]+
		matrix1[9]*matrix2[14]+
		matrix1[13]*matrix2[15];

	result[2]=matrix1[2]*matrix2[0]+
		matrix1[6]*matrix2[1]+
		matrix1[10]*matrix2[2]+
		matrix1[14]*matrix2[3];
	result[6]=matrix1[2]*matrix2[4]+
		matrix1[6]*matrix2[5]+
		matrix1[10]*matrix2[6]+
		matrix1[14]*matrix2[7];
	result[10]=matrix1[2]*matrix2[8]+
		matrix1[6]*matrix2[9]+
		matrix1[10]*matrix2[10]+
		matrix1[14]*matrix2[11];
	result[14]=matrix1[2]*matrix2[12]+
		matrix1[6]*matrix2[13]+
		matrix1[10]*matrix2[14]+
		matrix1[14]*matrix2[15];

	result[3]=matrix1[3]*matrix2[0]+
		matrix1[7]*matrix2[1]+
		matrix1[11]*matrix2[2]+
		matrix1[15]*matrix2[3];
	result[7]=matrix1[3]*matrix2[4]+
		matrix1[7]*matrix2[5]+
		matrix1[11]*matrix2[6]+
		matrix1[15]*matrix2[7];
	result[11]=matrix1[3]*matrix2[8]+
		matrix1[7]*matrix2[9]+
		matrix1[11]*matrix2[10]+
		matrix1[15]*matrix2[11];
	result[15]=matrix1[3]*matrix2[12]+
		matrix1[7]*matrix2[13]+
		matrix1[11]*matrix2[14]+
		matrix1[15]*matrix2[15];
}

inline void glhMultiplyMatrixByMatrix4by4f_1(GLfloat *leftMatrix, GLfloat *rightMatrix, GLfloat *result)
{
	result[0]=leftMatrix[0]*rightMatrix[0]+
		leftMatrix[4]*rightMatrix[1]+
		leftMatrix[8]*rightMatrix[2]+
		leftMatrix[12]*rightMatrix[3];
	result[4]=leftMatrix[0]*rightMatrix[4]+
		leftMatrix[4]*rightMatrix[5]+
		leftMatrix[8]*rightMatrix[6]+
		leftMatrix[12]*rightMatrix[7];
	result[8]=leftMatrix[0]*rightMatrix[8]+
		leftMatrix[4]*rightMatrix[9]+
		leftMatrix[8]*rightMatrix[10]+
		leftMatrix[12]*rightMatrix[11];
	result[12]=leftMatrix[0]*rightMatrix[12]+
		leftMatrix[4]*rightMatrix[13]+
		leftMatrix[8]*rightMatrix[14]+
		leftMatrix[12]*rightMatrix[15];

	result[1]=leftMatrix[1]*rightMatrix[0]+
		leftMatrix[5]*rightMatrix[1]+
		leftMatrix[9]*rightMatrix[2]+
		leftMatrix[13]*rightMatrix[3];
	result[5]=leftMatrix[1]*rightMatrix[4]+
		leftMatrix[5]*rightMatrix[5]+
		leftMatrix[9]*rightMatrix[6]+
		leftMatrix[13]*rightMatrix[7];
	result[9]=leftMatrix[1]*rightMatrix[8]+
		leftMatrix[5]*rightMatrix[9]+
		leftMatrix[9]*rightMatrix[10]+
		leftMatrix[13]*rightMatrix[11];
	result[13]=leftMatrix[1]*rightMatrix[12]+
		leftMatrix[5]*rightMatrix[13]+
		leftMatrix[9]*rightMatrix[14]+
		leftMatrix[13]*rightMatrix[15];

	result[2]=leftMatrix[2]*rightMatrix[0]+
		leftMatrix[6]*rightMatrix[1]+
		leftMatrix[10]*rightMatrix[2]+
		leftMatrix[14]*rightMatrix[3];
	result[6]=leftMatrix[2]*rightMatrix[4]+
		leftMatrix[6]*rightMatrix[5]+
		leftMatrix[10]*rightMatrix[6]+
		leftMatrix[14]*rightMatrix[7];
	result[10]=leftMatrix[2]*rightMatrix[8]+
		leftMatrix[6]*rightMatrix[9]+
		leftMatrix[10]*rightMatrix[10]+
		leftMatrix[14]*rightMatrix[11];
	result[14]=leftMatrix[2]*rightMatrix[12]+
		leftMatrix[6]*rightMatrix[13]+
		leftMatrix[10]*rightMatrix[14]+
		leftMatrix[14]*rightMatrix[15];

	result[3]=leftMatrix[3]*rightMatrix[0]+
		leftMatrix[7]*rightMatrix[1]+
		leftMatrix[11]*rightMatrix[2]+
		leftMatrix[15]*rightMatrix[3];
	result[7]=leftMatrix[3]*rightMatrix[4]+
		leftMatrix[7]*rightMatrix[5]+
		leftMatrix[11]*rightMatrix[6]+
		leftMatrix[15]*rightMatrix[7];
	result[11]=leftMatrix[3]*rightMatrix[8]+
		leftMatrix[7]*rightMatrix[9]+
		leftMatrix[11]*rightMatrix[10]+
		leftMatrix[15]*rightMatrix[11];
	result[15]=leftMatrix[3]*rightMatrix[12]+
		leftMatrix[7]*rightMatrix[13]+
		leftMatrix[11]*rightMatrix[14]+
		leftMatrix[15]*rightMatrix[15];
}

inline void MultiplyMatrices4(SOGLMatrix *result, SOGLMatrix *matrix1, SOGLMatrix *matrix2)
{

	glhMultiplyMatrixByMatrix4by4f_1( (GLfloat *)matrix1, (GLfloat *)matrix2, (GLfloat *)result);
}

inline SOGLMatrix* MultiplyMatrices(SOGLMatrix *pout, const SOGLMatrix *pm1, const SOGLMatrix *pm2)
{
	int i,j;

	for (i=0; i<4; i++)  {
		for (j=0; j<4; j++)   {
			pout->m[i][j] = pm1->m[i][0] * pm2->m[0][j] + pm1->m[i][1] * pm2->m[1][j] + pm1->m[i][2] * pm2->m[2][j] + pm1->m[i][3] * pm2->m[3][j];
		}
	}

	return pout;
}



inline void glhScalef2(SOGLMatrix *matrix_, GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat *matrix = (GLfloat* )matrix_;
	matrix[0] *=x;
	matrix[4] *=y;
	matrix[8] *=z;
	
	matrix[1] *=x;
	matrix[5] *=y;
	matrix[9] *=z;

	matrix[2] *=x;
	matrix[6] *=y;
	matrix[10] *=z;

	matrix[3] *=x;
	matrix[7] *=y;
	matrix[11] *=z;
}


inline void glhTranslatef2(SOGLMatrix *matrix_, GLfloat x, GLfloat y, GLfloat z)
{
	GLfloat *matrix = (GLfloat*)matrix_;
	matrix[12]=matrix[0]*x+matrix[4]*y+matrix[8]*z+matrix[12];
	matrix[13]=matrix[1]*x+matrix[5]*y+matrix[9]*z+matrix[13];
	matrix[14]=matrix[2]*x+matrix[6]*y+matrix[10]*z+matrix[14];
	matrix[15]=matrix[3]*x+matrix[7]*y+matrix[11]*z+matrix[15];
}


inline void glhRotatef2(SOGLMatrix *matrix_, GLfloat angleInRadians, GLfloat x, GLfloat y, GLfloat z)
{
	if( angleInRadians == 0 )
		return;
	GLfloat *matrix = (GLfloat*)matrix_;
	//See page 191 of Elementary Linear Algebra by Howard Anton
	GLfloat m[16], rotate[16];
	GLfloat OneMinusCosAngle, CosAngle, SinAngle;
	GLfloat A_OneMinusCosAngle, C_OneMinusCosAngle;
	CosAngle=cosf(angleInRadians);			//Some stuff for optimizing code
	OneMinusCosAngle=1.0f-CosAngle;
	SinAngle=sinf(angleInRadians);
	A_OneMinusCosAngle=x*OneMinusCosAngle;
	C_OneMinusCosAngle=z*OneMinusCosAngle;
	//Make a copy
	m[0]=matrix[0];
	m[1]=matrix[1];
	m[2]=matrix[2];
	m[3]=matrix[3];
	m[4]=matrix[4];
	m[5]=matrix[5];
	m[6]=matrix[6];
	m[7]=matrix[7];
	m[8]=matrix[8];
	m[9]=matrix[9];
	m[10]=matrix[10];
	m[11]=matrix[11];
	m[12]=matrix[12];
	m[13]=matrix[13];
	m[14]=matrix[14];
	m[15]=matrix[15];

	rotate[ 0]=x*A_OneMinusCosAngle+CosAngle;
	rotate[ 1]=y*A_OneMinusCosAngle+z*SinAngle;
	rotate[ 2]=z*A_OneMinusCosAngle-y*SinAngle;
	rotate[ 3]=0.0;

	rotate[ 4]=y*A_OneMinusCosAngle-z*SinAngle;
	rotate[ 5]=y*y*OneMinusCosAngle+CosAngle;
	rotate[ 6]=y*C_OneMinusCosAngle+x*SinAngle;
	rotate[ 7]=0.0;

	rotate[ 8]=x*C_OneMinusCosAngle+y*SinAngle;
	rotate[ 9]=y*C_OneMinusCosAngle-x*SinAngle;
	rotate[10]=z*C_OneMinusCosAngle+CosAngle;
	rotate[11]=0.0;
	//The last column of rotate[] is {0 0 0 1}

	matrix[0]=m[0]*rotate[0]+
		m[4]*rotate[1]+
		m[8]*rotate[2];

	matrix[4]=m[0]*rotate[4]+
		m[4]*rotate[5]+
		m[8]*rotate[6];

	matrix[8]=m[0]*rotate[8]+
		m[4]*rotate[9]+
		m[8]*rotate[10];

	//matrix[12]=matrix[12];

	matrix[1]=m[1]*rotate[0]+
		m[5]*rotate[1]+
		m[9]*rotate[2];

	matrix[5]=m[1]*rotate[4]+
		m[5]*rotate[5]+
		m[9]*rotate[6];

	matrix[9]=m[1]*rotate[8]+
		m[5]*rotate[9]+
		m[9]*rotate[10];

	//matrix[13]=matrix[13];

	matrix[2]=m[2]*rotate[0]+
		m[6]*rotate[1]+
		m[10]*rotate[2];

	matrix[6]=m[2]*rotate[4]+
		m[6]*rotate[5]+
		m[10]*rotate[6];

	matrix[10]=m[2]*rotate[8]+
		m[6]*rotate[9]+
		m[10]*rotate[10];

	//matrix[14]=matrix[14];

	matrix[3]=m[3]*rotate[0]+
		m[7]*rotate[1]+
		m[11]*rotate[2];

	matrix[7]=m[3]*rotate[4]+
		m[7]*rotate[5]+
		m[11]*rotate[6];

	matrix[11]=m[3]*rotate[8]+
		m[7]*rotate[9]+
		m[11]*rotate[10];

	//matrix[15]=matrix[15];
}



static GLfloat GlobalIdentityMatrixf[]=
	{1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0};

inline SOGLMatrix * glhLoadIdentityf2(SOGLMatrix *matrix)
{
	memcpy(matrix, GlobalIdentityMatrixf, sizeof(GLfloat)*16);
	return matrix;
}


inline SOGLMatrix* MyD3DXMatrixRotationQuaternion(SOGLMatrix *pout, const MyD3DXQUATERNION *pq)
{
    glhLoadIdentityf2(pout);
    pout->m[0][0] = 1.0f - 2.0f * (pq->y * pq->y + pq->z * pq->z);
    pout->m[0][1] = 2.0f * (pq->x *pq->y + pq->z * pq->w);
    pout->m[0][2] = 2.0f * (pq->x * pq->z - pq->y * pq->w);
    pout->m[1][0] = 2.0f * (pq->x * pq->y - pq->z * pq->w);
    pout->m[1][1] = 1.0f - 2.0f * (pq->x * pq->x + pq->z * pq->z);
    pout->m[1][2] = 2.0f * (pq->y *pq->z + pq->x *pq->w);
    pout->m[2][0] = 2.0f * (pq->x * pq->z + pq->y * pq->w);
    pout->m[2][1] = 2.0f * (pq->y *pq->z - pq->x *pq->w);
    pout->m[2][2] = 1.0f - 2.0f * (pq->x * pq->x + pq->y * pq->y);
    return pout;
}
#define   D3DXMatrixRotationQuaternion MyD3DXMatrixRotationQuaternion

inline SOGLMatrix*  MyD3DXMatrixTransformation2D(
	SOGLMatrix *pout, GLfloat scalingX, GLfloat scalingY,
    CONST Vec2 *protationcenter, GLfloat rotation,
    CONST Vec2 *ptranslation)
{
	SOGLMatrix m1, m2, m3, m4, m5;
	glhLoadIdentityf2( &m1 );
	glhLoadIdentityf2( &m2 );
	glhLoadIdentityf2( &m3 );
	glhLoadIdentityf2( &m4 );
	glhLoadIdentityf2( &m5 );
	
    MyD3DXQUATERNION rot;
    Vec3 rot_center, trans;

    rot.w=cos(rotation/2.0f);
    rot.x=0.0f;
    rot.y=0.0f;
    rot.z=sin(rotation/2.0f);

    if ( protationcenter )
    {
        rot_center.x = protationcenter->x;
        rot_center.y = protationcenter->y;
        rot_center.z = 0.0f;
    }
    else
    {
        rot_center.x=0.0f;
        rot_center.y=0.0f;
        rot_center.z=0.0f;
    }

    if ( ptranslation )
    {
        trans.x = ptranslation->x;
        trans.y = ptranslation->y;
        trans.z = 0.0f;
    }
    else
    {
        trans.x = 0.0f;
        trans.y = 0.0f;
        trans.z = 0.0f;
    }

    glhScalef2(&m1, scalingX, scalingY, 1.0f);
    glhTranslatef2(&m2, -rot_center.x, -rot_center.y, -rot_center.z);
    glhTranslatef2(&m4, rot_center.x, rot_center.y, rot_center.z);
    MyD3DXMatrixRotationQuaternion(&m3, &rot);
    glhTranslatef2(&m5, trans.x, trans.y, trans.z);

    SOGLMatrix   m1_ = m1 , m2_ = m2, m3_ = m3, m4_ = m4, m5_ =  m5;
    
	MultiplyMatrices(&m1_, &m1, &m2_);
	m1 = m1_;
	MultiplyMatrices(&m1_, &m1, &m3_);
	m1 = m1_;
	MultiplyMatrices(&m1_, &m1, &m4_);
    m1 = m1_;
	MultiplyMatrices(pout, &m1, &m5_);

     

    return pout;
}


inline SOGLMatrix*  MyD3DXMatrixAffineTransformation2D(
    SOGLMatrix *pout, FLOAT scaling,
    CONST Vec2 *protationcenter, FLOAT rotation,
    CONST Vec2 *ptranslation)
{
	return MyD3DXMatrixTransformation2D( pout, scaling, scaling, protationcenter, rotation, ptranslation);
    /*SOGLMatrix m1, m2, m3, m4, m5;
	glhLoadIdentityf2( &m1 );
	glhLoadIdentityf2( &m2 );
	glhLoadIdentityf2( &m3 );
	glhLoadIdentityf2( &m4 );
	glhLoadIdentityf2( &m5 );
	
    MyD3DXQUATERNION rot;
    Vec3 rot_center, trans;

    rot.w=cos(rotation/2.0f);
    rot.x=0.0f;
    rot.y=0.0f;
    rot.z=sin(rotation/2.0f);

    if ( protationcenter )
    {
        rot_center.x=protationcenter->x;
        rot_center.y=protationcenter->y;
        rot_center.z=0.0f;
    }
    else
    {
        rot_center.x=0.0f;
        rot_center.y=0.0f;
        rot_center.z=0.0f;
    }

    if ( ptranslation )
    {
        trans.x=ptranslation->x;
        trans.y=ptranslation->y;
        trans.z=0.0f;
    }
    else
    {
        trans.x=0.0f;
        trans.y=0.0f;
        trans.z=0.0f;
    }

    glhScalef2(&m1, scaling, scaling, 1.0f);
    glhTranslatef2(&m2, -rot_center.x, -rot_center.y, -rot_center.z);
    glhTranslatef2(&m4, rot_center.x, rot_center.y, rot_center.z);
    MyD3DXMatrixRotationQuaternion(&m3, &rot);
    glhTranslatef2(&m5, trans.x, trans.y, trans.z);

    SOGLMatrix   m1_ = m1 , m2_ = m2, m3_ = m3, m4_ = m4, m5_ =  m5;
    
	MultiplyMatrices(&m1_, &m1, &m2_);
	m1 = m1_;
	MultiplyMatrices(&m1_, &m1, &m3_);
	m1 = m1_;
	MultiplyMatrices(&m1_, &m1, &m4_);
    m1 = m1_;
	MultiplyMatrices(pout, &m1, &m5_);

     

    return pout;*/
}
#define D3DXMatrixAffineTransformation2D MyD3DXMatrixAffineTransformation2D



inline SOGLMatrix*  MyD3DXMatrixOrthoOffCenterLH(SOGLMatrix *pout, float l, float r, float b, float t, float zn, float zf)
{
    glhLoadIdentityf2( pout );
    pout->m[0][0] = 2.0f / (r - l);
    pout->m[1][1] = 2.0f / (t - b);
    pout->m[2][2] = 1.0f / (zf -zn);
    pout->m[3][0] = -1.0f -2.0f *l / (r - l);
    pout->m[3][1] = 1.0f + 2.0f * t / (b - t);
    pout->m[3][2] = zn / (zn -zf);
    return pout;
}

inline SOGLMatrix*  MyD3DXMatrixOrthoLH(SOGLMatrix *pout, float r, float t, float zn, float zf)
{
    glhLoadIdentityf2( pout );
    pout->m[0][0] = 2.0f / r;
    pout->m[1][1] = 2.0f / t;
    pout->m[2][2] = 1.0f / (zf -zn);
    pout->m[3][0] = 0.0f;
    pout->m[3][1] = 0.0f;
    pout->m[3][2] = zn / (zn -zf);
	return pout;
}
#define D3DXMatrixOrthoOffCenterLH MyD3DXMatrixOrthoOffCenterLH


inline void glhOrthof2(SOGLMatrix *matrix_, GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat znear, GLfloat zfar)
{
	GLfloat *matrix = (GLfloat* )matrix_;
	GLfloat matrix2[16], temp2, temp3, temp4, resultMatrix[16];
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=2.0f/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=2.0f/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=0.0;
	matrix2[9]=0.0;
	matrix2[10]=-2.0f/temp4;
	matrix2[11]=0.0;
	matrix2[12]=(-right-left)/temp2;
	matrix2[13]=(-top-bottom)/temp3;
	matrix2[14]=(-zfar-znear)/temp4;
	matrix2[15]=1.0;
	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);
	memcpy(matrix, resultMatrix, 16*sizeof(GLfloat));
}

inline Vec3* glhVec3TransformCoord(Vec3* pOut, CONST Vec3* pV, CONST SOGLMatrix* pM)
{
	float x, y, z, w;

	x = pV->x * pM->_11 + pV->y * pM->_21 + pV->z * pM->_31 + pM->_41;
	y = pV->x * pM->_12 + pV->y * pM->_22 + pV->z * pM->_32 + pM->_42;
	z = pV->x * pM->_13 + pV->y * pM->_23 + pV->z * pM->_33 + pM->_43;
	w = pV->x * pM->_14 + pV->y * pM->_24 + pV->z * pM->_34 + pM->_44;

	pOut->x = x / w;
	pOut->y = y / w;
	pOut->z = z / w;
	return pOut;
}
 

inline void glhVec3TransformCoord( GLfloat* pOut, const GLfloat* pV, const SOGLMatrix* pM)
{
	float x, y, z, w;

	x = pV[0] * pM->_11 + pV[1] * pM->_21 + pV[2] * pM->_31 + pM->_41;
	y = pV[0] * pM->_12 + pV[1] * pM->_22 + pV[2] * pM->_32 + pM->_42;
	z = pV[0] * pM->_13 + pV[1] * pM->_23 + pV[2] * pM->_33 + pM->_43;
	w = pV[0] * pM->_14 + pV[1] * pM->_24 + pV[2] * pM->_34 + pM->_44;

	pOut[0] = x / w;
	pOut[1] = y / w;
	pOut[2] = z / w;
}

inline GLvoid glhVec3TransformCoord( GLfloat& x, GLfloat& y, GLfloat& z, CONST SOGLMatrix* pM)
{
	float x_, y_, z_, w_;

	x_ = x * pM->_11 + y * pM->_21 + z * pM->_31 + pM->_41;
	y_ = x * pM->_12 + y * pM->_22 + z * pM->_32 + pM->_42;
	z_ = x * pM->_13 + y * pM->_23 + z * pM->_33 + pM->_43;
	w_ = x * pM->_14 + y * pM->_24 + z * pM->_34 + pM->_44;

	x = x_ / w_;
	y = y_ / w_;
	z = z_ / w_;
}


inline MyD3DXVECTOR4* MyD3DXVec4Cross(MyD3DXVECTOR4 *pout, CONST MyD3DXVECTOR4 *pv1, CONST MyD3DXVECTOR4 *pv2, CONST MyD3DXVECTOR4 *pv3)
{
    pout->x = pv1->y * (pv2->z * pv3->w - pv3->z * pv2->w) - pv1->z * (pv2->y * pv3->w - pv3->y * pv2->w) + pv1->w * (pv2->y * pv3->z - pv2->z *pv3->y);
    pout->y = -(pv1->x * (pv2->z * pv3->w - pv3->z * pv2->w) - pv1->z * (pv2->x * pv3->w - pv3->x * pv2->w) + pv1->w * (pv2->x * pv3->z - pv3->x * pv2->z));
    pout->z = pv1->x * (pv2->y * pv3->w - pv3->y * pv2->w) - pv1->y * (pv2->x *pv3->w - pv3->x * pv2->w) + pv1->w * (pv2->x * pv3->y - pv3->x * pv2->y);
    pout->w = -(pv1->x * (pv2->y * pv3->z - pv3->y * pv2->z) - pv1->y * (pv2->x * pv3->z - pv3->x *pv2->z) + pv1->z * (pv2->x * pv3->y - pv3->x * pv2->y));
    return pout;
}
#define   D3DXVec4Cross MyD3DXVec4Cross

inline float MyD3DXMatrixfDeterminant(CONST SOGLMatrix *pm)
{
    MyD3DXVECTOR4 minor, v1, v2, v3;
    float det;

    v1.x = pm->m[0][0]; v1.y = pm->m[1][0]; v1.z = pm->m[2][0]; v1.w = pm->m[3][0];
    v2.x = pm->m[0][1]; v2.y = pm->m[1][1]; v2.z = pm->m[2][1]; v2.w = pm->m[3][1];
    v3.x = pm->m[0][2]; v3.y = pm->m[1][2]; v3.z = pm->m[2][2]; v3.w = pm->m[3][2];
    MyD3DXVec4Cross(&minor,&v1,&v2,&v3);
    det =  - (pm->m[0][3] * minor.x + pm->m[1][3] * minor.y + pm->m[2][3] * minor.z + pm->m[3][3] * minor.w);
    return det;
}
#define   D3DXMatrixfDeterminant MyD3DXMatrixfDeterminant

inline SOGLMatrix* MyD3DXMatrixInverse(SOGLMatrix *pout, float *pdeterminant, CONST SOGLMatrix *pm)
{
    int a, i, j;
    MyD3DXVECTOR4 v, vec[3];
    float cofactor, det;

    det = MyD3DXMatrixfDeterminant(pm);
    if ( !det ) return NULL;
    if ( pdeterminant ) *pdeterminant = det;
    for (i=0; i<4; i++)
    {
     for (j=0; j<4; j++)
     {
      if (j != i )
      {
       a = j;
       if ( j > i ) a = a-1;
       vec[a].x = pm->m[j][0];
       vec[a].y = pm->m[j][1];
       vec[a].z = pm->m[j][2];
       vec[a].w = pm->m[j][3];
      }
     }
    MyD3DXVec4Cross(&v, &vec[0], &vec[1], &vec[2]);
    for (j=0; j<4; j++)
    {
     switch(j)
     {
      case 0: cofactor = v.x; break;
      case 1: cofactor = v.y; break;
      case 2: cofactor = v.z; break;
      case 3: cofactor = v.w; break;
     }
    pout->m[j][i] = pow(-1.0f, i) * cofactor / det;
    }
   }
    return pout;
}

#define   D3DXMatrixInverse MyD3DXMatrixInverse


inline float MyD3DXVec3Dot(CONST Vec3 *pv1, CONST Vec3 *pv2)
{
    if ( !pv1 || !pv2 ) return 0.0f;
    return (pv1->x) * (pv2->x) + (pv1->y) * (pv2->y) + (pv1->z) * (pv2->z);
}

#define   D3DXVec3Dot MyD3DXVec3Dot

inline MyD3DXPLANE* MyD3DXPlaneFromPointNormal(MyD3DXPLANE *pout, CONST Vec3 *pvpoint, CONST Vec3 *pvnormal)
{
    pout->a = pvnormal->x;
    pout->b = pvnormal->y;
    pout->c = pvnormal->z;
    pout->d = -MyD3DXVec3Dot(pvpoint, pvnormal);
    return pout;
}
#define   D3DXPlaneFromPointNormal MyD3DXPlaneFromPointNormal

inline Vec3* MyD3DXPlaneIntersectLine(Vec3 *pout, CONST MyD3DXPLANE *pp, CONST Vec3 *pv1, CONST Vec3 *pv2)
{
    Vec3 direction, normal;
    float dot, temp;

    normal.x = pp->a;
    normal.y = pp->b;
    normal.z = pp->c;
    direction.x = pv2->x - pv1->x;
    direction.y = pv2->y - pv1->y;
    direction.z = pv2->z - pv1->z;
    dot = MyD3DXVec3Dot(&normal, &direction);
    if ( !dot ) return NULL;
    temp = ( pp->d + MyD3DXVec3Dot(&normal, pv1) ) / dot;
    pout->x = pv1->x - temp * direction.x;
    pout->y = pv1->y - temp * direction.y;
    pout->z = pv1->z - temp * direction.z;
    return pout;
}




#endif