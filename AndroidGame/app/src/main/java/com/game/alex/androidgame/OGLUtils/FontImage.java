package com.game.alex.androidgame.OGLUtils;


import com.game.alex.androidgame.GameObjects.GameStructs.OGLColor;
import com.game.alex.androidgame.GameObjects.GameStructs.Vec3;
import com.game.alex.androidgame.OGLUtils.TextureUtils;
import com.game.alex.androidgame.OGLUtils.OGLDrawBuffer;
/**
 * Created by kag on 15.12.2017.
 */

class  GLGlyph {
    char m_cSymbol = 0;
    float m_fXpos = 0.0f;        //	положение символа на текстуре ( верхний левый угол )
    float m_fYpos = 0.0f;
    float m_fWidth;            //	размер прямоугольной области, занимаемой символом
    float m_fHeight;
    float m_fXoff = 0.0f;        //	смещение символа при выводе текста
    float m_fYoff = 0.0f;

    GLGlyph() {
    }

    GLGlyph(char Symbol, float Xpos, float Ypos, float Width, float Height, float fXoff, float Yoff) {
        m_cSymbol = Symbol;
        m_fXpos = Xpos;
        m_fYpos = Ypos;
        m_fWidth = Width;
        m_fHeight = Height;
        m_fXoff = fXoff;
        m_fYoff = Yoff;
    }

    float[] GetVertexes( int Xoff, int Yoff, float Z) {
        float[] Vertexes = new float[3 * 2 * 4];
        Vertexes[0] = m_fXpos + Xoff;
        Vertexes[1] = m_fYpos + Yoff;
        Vertexes[2] = Z;
        Vertexes[3] = m_fXpos + Xoff + m_fWidth;
        Vertexes[4] = m_fYpos + Yoff;
        Vertexes[5] = Z;

        Vertexes[6] = m_fXpos + Xoff + m_fWidth;
        Vertexes[7] = m_fYpos + Yoff;
        Vertexes[8] = Z;
        Vertexes[9] = m_fXpos + Xoff + m_fWidth;
        Vertexes[10] = m_fYpos + Yoff + m_fHeight;
        Vertexes[11] = Z;

        Vertexes[12] = m_fXpos + Xoff + m_fWidth;
        Vertexes[13] = m_fYpos + Yoff + m_fHeight;
        Vertexes[14] = Z;
        Vertexes[15] = m_fXpos + Xoff;
        Vertexes[16] = m_fYpos + Yoff + m_fHeight;
        Vertexes[17] = Z;

        Vertexes[18] = m_fXpos + Xoff;
        Vertexes[19] = m_fYpos + Yoff + m_fHeight;
        Vertexes[20] = Z;
        Vertexes[21] = m_fXpos + Xoff;
        Vertexes[22] = m_fYpos + Yoff;
        Vertexes[23] = Z;
        return Vertexes;
    }
}



public class FontImage {

    //
    int                         m_iResID;
    String					    m_sName;
    TextureUtils				m_Image = null;
    int							m_iWidth = 0;
    int							m_iHeight = 0;
    int							m_iXoff = 0;
    int							m_iYoff = 50;
    int							m_iGlyphHeight = 20;
    int							m_iGlyphWidth = 0;
    //
    GLGlyph[]		            m_Glyphs;

    //float						m_iScale = 1.0f;
    float						m_fXScale = 1.0f;
    float						m_fYScale = 1.0f;
    OGLColor					m_Color = new OGLColor(1.0f, 1.0f, 1.0f, 1.0f );

    public FontImage() {}

    public void		GetSize(int Width, int Height) { Width = m_iWidth; Height = m_iHeight; };

    public TextureUtils	GetTexture() { 	return m_Image;  };

    // Возвращает новое смещение символа по оси Х
    public float		GetSymbol( char Symb,			                    //	выводимый символ
                                   float Xoff, float Yoff, float Zoff,	//	смещение символа
                                   OGLDrawBuffer DrawBuffer)		        // при выводе в одну строчку , возвращается значение смещения X для выводв следующего символа
                                                                throws OGLBuffer.BufferExeption
    {
        GLGlyph pGl = null;
        for( GLGlyph gl :  m_Glyphs ) {
            if ( gl.m_cSymbol == Symb ) {
                pGl = gl;
                break;
            }
        }
        //
        if (pGl != null) {
            short indVert = (short)DrawBuffer.Count();

            DrawBuffer.AddVertex(  new float[]{ Xoff, Yoff + pGl.m_fYoff*m_fYScale, Zoff, Xoff + pGl.m_fWidth*m_fXScale, Yoff + pGl.m_fYoff*m_fYScale, Zoff,
                    Xoff + pGl.m_fWidth*m_fXScale, Yoff + ( pGl.m_fHeight  + pGl.m_fYoff)*m_fYScale, Zoff, Xoff, Yoff + ( pGl.m_fHeight  + pGl.m_fYoff)*m_fYScale, Zoff }, 12);
            float[] tex = { (pGl.m_fXpos) / m_iWidth,  (pGl.m_fYpos + pGl.m_fHeight )/ m_iHeight,
                    (pGl.m_fXpos + pGl.m_fWidth) / m_iWidth,  (pGl.m_fYpos + pGl.m_fHeight) / m_iHeight,
                    (pGl.m_fXpos + pGl.m_fWidth) / m_iWidth, pGl.m_fYpos/ m_iHeight,
                    (pGl.m_fXpos) / m_iWidth, pGl.m_fYpos / m_iHeight};
            DrawBuffer.AddTexture( tex, tex.length);

            DrawBuffer.AddIndex( new short[]{ indVert, (short)(indVert + 2), (short)(indVert + 1), indVert,
                    (short)(indVert + 3), (short)(indVert + 2) }, 6 );

            DrawBuffer.SetColor(m_Color);

            return	Xoff + pGl.m_fWidth*m_fXScale;
        }
        return	Xoff;
    }

    public void		TextDraw( String Text, float X, float Y, float Zoff, OGLDrawBuffer DrawBuffer )  throws OGLBuffer.BufferExeption
    {
        //DrawBuffer.AffineTransformation2D( new Vec3( 0.0f, 0.0f, 0.0f ), 0.0f,
        //        new Vec3( 0.0f, 0.0f, 1.0f ), new Vec3( 1.0f, 1.0f, 1.0f ) );
        for( int i = 0 ; i != Text.length() ; ++i  ) {
            float xoff = this.GetSymbol( Text.charAt(i), X, Y, Zoff, DrawBuffer);
            X = xoff;
        }
    }

    //public void		SetScale(float Scale) { m_iScale = Scale; }
    //public float	GetScale() {return  m_iScale; }
    public void		SetColor( OGLColor Color) { m_Color = Color; }
    public void		SetColor(float R, float G, float B, float A) { m_Color = new OGLColor(R,G,B,A); }
    //public void		SetHeight(float  Height) { m_iScale = Height / m_iGlyphHeight; }
    public void		SetSize( float Width, float  Height) {
        m_fYScale = Height / m_iGlyphHeight;
        if( m_iGlyphWidth == 0 ) {   //  вычисляем среднюю ширину символа
            for( GLGlyph gl : m_Glyphs ) {
                m_iGlyphWidth += gl.m_fWidth;
            }
            m_iGlyphWidth /= m_Glyphs.length;
        }
        if(m_iGlyphWidth != 0)
            m_fXScale = Width / m_iGlyphWidth;
        else
            m_fXScale = m_fYScale;
    }
}
