package com.game.alex.androidgame.OGLUtils;

import android.opengl.Matrix;

import static android.opengl.GLES20.GL_TRIANGLES;
import static android.opengl.GLES20.glDrawElements;
import static android.opengl.GLES20.glGenBuffers;
import static android.opengl.GLES20.glBindBuffer;
import static android.opengl.GLES20.glBufferData;
import static android.opengl.GLES20.glDrawArrays;
import static android.opengl.GLES20.glDeleteBuffers;
import static android.opengl.GLES20.glEnable;
import static android.opengl.GLES20.glDisable;
import static android.opengl.GLES20.GL_ARRAY_BUFFER;
import static android.opengl.GLES20.GL_TEXTURE_2D;
import static android.opengl.GLES20.GL_TEXTURE0;
import static android.opengl.GLES20.GL_STATIC_DRAW;
import static android.opengl.GLES20.GL_FLOAT;
import static android.opengl.GLES20.GL_UNSIGNED_SHORT;
import static android.opengl.GLES20.GL_TEXTURE_MIN_FILTER;
import static android.opengl.GLES20.GL_LINEAR;
import static android.opengl.GLES20. GL_TEXTURE_MAG_FILTER;
import static android.opengl.GLES20.glGetAttribLocation;
import static android.opengl.GLES20.glGetUniformLocation;
import static android.opengl.GLES20.glVertexAttribPointer;
import static android.opengl.GLES20.glEnableVertexAttribArray;
import static android.opengl.GLES20.glActiveTexture;
import static android.opengl.GLES20.glUseProgram;
import static android.opengl.GLES20.glGetError;
import static android.opengl.GLES20.glUniform1i;
import static android.opengl.GLES20.glUniformMatrix4fv;
import static android.opengl.GLES20.glTexParameteri;
import static android.opengl.GLES20.glDisableVertexAttribArray;
import static android.opengl.GLES20.glDeleteProgram;

import com.game.alex.androidgame.GameObjects.GameStructs.OGLColor;
import com.game.alex.androidgame.GameObjects.GameStructs.Vec3;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;


public class OGLDrawBuffer {
    OGLBuffer	            	m_vVertex;
    OGLBuffer	    			m_vTexture;
    OGLBuffer	            	m_vColor;
    OGLBuffer           		m_vIndex;

    int							m_iVBO = -1;
    int							m_iTBO = -1;
    int							m_iCBO = -1;
    FloatBuffer                 m_ModelMatrix = ByteBuffer.allocateDirect( 16 * 4 ).order(ByteOrder.nativeOrder()).asFloatBuffer();
    int					        m_iShaderId;
    int							m_iVertShAttr = 0;			//  координата вершины
    int							m_iTexCoordLoc = 0;			//	текстурная координгата
    int							m_iColorLoc = 0;			//	цвет вершины
    int							m_iSamplerLoc = 0;			//	номер текстуры
    int							m_iProjMatrix = 0;			//	матрица преобразования кординат координгата
    int							m_iModelMatrix = 0;			//	матрица преобразования кординат координгата
    int							m_ibUseTexture = 0;			//  использовать текстуру или нет
    FloatBuffer                 m_ProjMatrix = ByteBuffer.allocateDirect( 16 * 4 ).order(ByteOrder.nativeOrder()).asFloatBuffer();
    //
    public int	               VertexDimension;
    public int	               ColorDimension;
    public int	               TextureDimension = 2;
    //
    int                         m_iPositionLocation = -1;
    //
    public OGLDrawBuffer( int VertexDim, int ColorDim, int MaxSize ) {
       VertexDimension = VertexDim;
       ColorDimension = ColorDim;
       m_vVertex = new OGLBuffer(MaxSize, VertexDimension, 0.0f);
       m_vTexture = new OGLBuffer(MaxSize, TextureDimension, 0.0f);
       m_vColor = new OGLBuffer(MaxSize, ColorDimension, 0.0f);
       m_vIndex = new OGLBuffer( 2*MaxSize, 1, 0);
       float[] ModelMatrix = new float[16];
       Matrix.setIdentityM( ModelMatrix, 0 );

       m_ModelMatrix.put(ModelMatrix).position(0);
   };

    public void		LoadIdentity()   {
        float[] matrix = new float[16];
        Matrix.setIdentityM(matrix, 0);
        m_ModelMatrix.position(0);
        m_ModelMatrix.put(matrix).position(0);
    };

    public void	AffineTransformation2D( Vec3 Offset, float Angle, Vec3 RotAxis, Vec3 Scale)
    {
        float[] matrix = new float[16];
        Matrix.setIdentityM( matrix, 0);//glhLoadIdentityf2();
        Matrix.translateM(matrix, 0, Offset.x, /*m_fMaxHeight - Offset.y*/ Offset.y, Offset.z);
        Matrix.scaleM(matrix, 0,  Scale.x, Scale.y, Scale.z);
        Matrix.rotateM(matrix, 0, Angle, RotAxis.x, RotAxis.y, RotAxis.z);
        m_ModelMatrix.put(matrix).position(0);

    }
   //  добавить текстурные координаты
   public int  AddTexture(float	U, float	V)  throws OGLBuffer.BufferExeption {
        int tSize = m_vTexture.Add(U, V);
        return tSize;
    };
    public int  AddTexture( float[] Data,  int Size ) throws OGLBuffer.BufferExeption {
        int cSize = m_vTexture.Add(Data, Size);
        return cSize;
    };
    //  добавить координаты  вертексов
    public int  AddVertex(float X, float Y) throws OGLBuffer.BufferExeption {
        int vSize = m_vVertex.Add(X, Y);
        return vSize;
    };
    public  int  AddVertex(float X, float Y, float Z) throws OGLBuffer.BufferExeption  {
        int vSize = m_vVertex.Add(X, Y, Z);
        return vSize;
    };

    public int  AddVertex( float[] Data,  int Size ) throws OGLBuffer.BufferExeption  {
        int cSize = m_vVertex.Add(Data,  Size);
        return cSize;
    };
     //  добавить цвет
     public int  AddColor(float R, float G, float B) throws OGLBuffer.BufferExeption  {
        int cSize = m_vColor.Add(R, G, B);
        return cSize;
    };
    public int  AddColor(float R, float G, float B, float A) throws OGLBuffer.BufferExeption  {
        int cSize = m_vColor.Add( R, G, B, A );
        return cSize;
    };
    public int  AddColor( float[] Data,  int Size )  throws OGLBuffer.BufferExeption {
        int cSize = m_vColor.Add(Data,  Size);
        return cSize;
    };
    public int  AddColor( OGLColor Color)  throws OGLBuffer.BufferExeption {
        return  m_vColor.Add(Color.r, Color.g, Color.b, Color.a);
    };
    //  Вызывать после того как заполнены остальные составляющие ( Vertex , Texture ). Добавляется столько элементов, чтобы размер буфера соответствовал размерам остальных
    public int  SetColor(float R, float G, float B)  throws OGLBuffer.BufferExeption {
        int count = m_vVertex.Count();
        int cSize;
        while( ( cSize = m_vColor.Add(R, G, B) ) < count );
        return cSize;
    };
    public int  SetColor(float R, float G, float B, float A)  throws OGLBuffer.BufferExeption {

        int count = m_vVertex.Count();
        int cSize;
        while ((cSize = m_vColor.Add(R, G, B, A)) < count);
        return cSize;
    };
    public int  SetColor( OGLColor Color )  throws OGLBuffer.BufferExeption {
        return  SetColor(Color.r, Color.g, Color.b, Color.a);
    };
    //

    /*int AddIndex( int Index) {
        int cSize = m_vIndex.Add(Index);
        return cSize;
    };*/
    //
    public int AddQuadIndex( short I1,  short I2,  short I3,  short I4,  short I5,  short I6 )  throws OGLBuffer.BufferExeption {
        return m_vIndex.AddQuadIndex(I1, I2, I3, I4, I5, I6);
    }
    //
    public int  AddIndex(  short[] Data,  int Size)  throws OGLBuffer.BufferExeption  {
        int cSize = m_vIndex.Add(Data, Size);
        return cSize;
    };
    //
    public void	AddVerticesAndTransform( float[] Vertises, int VertSize, float[] Texture, int TexSize, OGLColor Color)  throws OGLBuffer.BufferExeption
    {
        float[] verOut = new float[VertSize];
        float[] matrix = new float[16];
        m_ModelMatrix.get(matrix);
        float[]  vertIn = new float[4];
        vertIn[3] = 1.0f;
        float[]  vertOut = new float[4];
        for ( int i = 0; i < VertSize; i += VertexDimension ) {
            vertIn[0] = Vertises[i];
            vertIn[1] = Vertises[i+1];
            vertIn[2] = Vertises[i+2];
            Matrix.multiplyMV(vertOut, 0, matrix, 0, vertIn, 0);
            verOut[i] = vertOut[0];
            verOut[i+1] = vertOut[1];
            verOut[i+2] = vertOut[2];
        }
        //
        //    glhVec3TransformCoord(verOut + i, Vertises + i, &m_ModelMatrix);
        AddVertex(verOut, VertSize);
        AddTexture(Texture, TexSize);
        SetColor(Color);
    };

    //  проверка что размер всех буферов совпадает
    public void Check()   {
        assert( ( m_vTexture.Count() == m_vVertex.Count() || m_vTexture.Count() == 0 ) &&
                ( m_vVertex.Count() == m_vColor.Count() || m_vColor.Count() == 0) );
    };
    //
    public void	Clear()  {
        m_vVertex.Clear();
        m_vTexture.Clear();
        m_vColor.Clear();
        m_vIndex.Clear();
    };
    //
    public int	Count()   {
        Check();
        return m_vVertex.Count();
    };
    public int	VertexSize()   {
        return m_vVertex.Size();
    };
    public int	TextureSize()   {
        return m_vTexture.Size();
    };
    public int	ColorSize()   {
        return m_vColor.Size();
    };
    public int	IndexSize()   {
        return m_vIndex.Size();
    };
    //
    public float[]  GetVertexData() { return m_vVertex.GetData(); };
    public float[]  GetTextureData() { return m_vTexture.GetData(); };
    public float[]  GetColorData() { return m_vColor.GetData(); };
    public short[]  GetIndexData() { return m_vIndex.GetDataI(); };

    public void SetShaderProgrammId( int  Id ) {
        m_iShaderId = Id;
        m_iVertShAttr = glGetAttribLocation(m_iShaderId, "vPosition");
        m_iTexCoordLoc = glGetAttribLocation(m_iShaderId, "vtexCoord");
        m_iColorLoc = glGetAttribLocation(m_iShaderId, "vColor");
        // Get the sampler location
        m_iSamplerLoc = glGetUniformLocation(m_iShaderId, "s_texture");
        //
        m_iProjMatrix = glGetUniformLocation(m_iShaderId, "u_ProjMatrix");
        m_iModelMatrix = glGetUniformLocation(m_iShaderId, "u_ModelMatrix");
        m_ibUseTexture = glGetUniformLocation(m_iShaderId, "b_UseTexture");
    };
    public void SetProjectMatrix(float[]	ProjMatrix )
    {
        m_ProjMatrix.put(ProjMatrix).position(0);
    };
    public void DrawBuffer(TextureUtils Texture, int	Primitive)
    {
        if (VertexSize() == 0)
            return;
        // Bind the texture
        glActiveTexture(GL_TEXTURE0);
        boolean bindText = false;
        if( Texture != null )
            bindText = Texture.Bind();
        int useTexture = 0;
        if( bindText && m_vTexture.Size() != 0 )   useTexture = 1;

        //
        glUseProgram(m_iShaderId);
        int err = glGetError();
        // Load the vertex position
        FloatBuffer VertBuff = ByteBuffer.allocateDirect(m_vVertex.m_iSize * 4 ).order(ByteOrder.nativeOrder()).asFloatBuffer();
        VertBuff.put(m_vVertex.m_fData, 0, m_vVertex.m_iSize).position(0);
        //FloatBuffer.wrap( m_vVertex.GetData(), 0, m_vVertex.Size() );
        glVertexAttribPointer(m_iVertShAttr, VertexDimension, GL_FLOAT, false, 0, VertBuff);
        glEnableVertexAttribArray(m_iVertShAttr);
        // Load the texture coordinate
        FloatBuffer TextBuff = ByteBuffer.allocateDirect(m_vTexture.m_iSize * 4 ).order(ByteOrder.nativeOrder()).asFloatBuffer();
        TextBuff.put(m_vTexture.m_fData, 0, m_vTexture.m_iSize).position(0);
        glVertexAttribPointer(m_iTexCoordLoc, TextureDimension, GL_FLOAT, false, 0, TextBuff);
        glEnableVertexAttribArray(m_iTexCoordLoc);
        // Load the texture coordinate
        FloatBuffer ColorBuff = ByteBuffer.allocateDirect(m_vColor.m_iSize * 4 ).order(ByteOrder.nativeOrder()).asFloatBuffer();
        ColorBuff.put(m_vColor.m_fData, 0, m_vColor.m_iSize).position(0);
        glVertexAttribPointer(m_iColorLoc, ColorDimension, GL_FLOAT, false, 0, ColorBuff);
        glEnableVertexAttribArray(m_iColorLoc);
        //
        glUniform1i(m_ibUseTexture, useTexture);
        //
        // Set the texture sampler to texture unit to 0
        glUniform1i(m_iSamplerLoc, 0);
        glUniformMatrix4fv(m_iProjMatrix, 1, false,  m_ProjMatrix);
        //
        glUniformMatrix4fv(m_iModelMatrix, 1, false, m_ModelMatrix);
        // Draw quad with nearest sampling
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR/*GL_NEAREST*/);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR/*GL_NEAREST*/);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
        if (m_vIndex.m_iSize == 0)
            glDrawArrays(Primitive/*GL_TRIANGLES*/, 0, Count());
        else {
            ShortBuffer indBuff = ByteBuffer.allocateDirect(m_vIndex.m_iSize * 2 ).order(ByteOrder.nativeOrder()).asShortBuffer();
            indBuff.put(m_vIndex.m_iData, 0, m_vIndex.m_iSize);
            indBuff.position(0);
            glDrawElements(Primitive/*GL_TRIANGLES*/,			// режим
                    m_vIndex.m_iSize,					// количество
                    GL_UNSIGNED_SHORT,					// тип
                    indBuff               	//
            );
            int err1 = glGetError();
            err = err1;
        }

        glDisableVertexAttribArray(m_iVertShAttr);
        glDisableVertexAttribArray(m_iTexCoordLoc);
        glDisableVertexAttribArray(m_iColorLoc);

        glDeleteProgram(m_iShaderId);

        Clear();
    };

}
