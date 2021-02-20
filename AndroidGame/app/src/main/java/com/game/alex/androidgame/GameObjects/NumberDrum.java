package com.game.alex.androidgame.GameObjects;

import com.game.alex.androidgame.OGLGame;
import com.game.alex.androidgame.OGLUtils.OGLBuffer;
import com.game.alex.androidgame.OGLUtils.OGLDrawBuffer;
import com.game.alex.androidgame.OGLUtils.TextureManager;

/**
 * Created by kag on 09.01.2018.
 */

class SlidingTexture {
    public int m_iNumSign = 11;
    public float m_fWidth = 1.0f;
    public float m_fHeight = 1.0f;
    public float m_fX = 1.0f;     //  положение верхней левой точки
    public float m_fY = 1.0f;
    public float m_fZ = 1.0f;
    //
    public float m_fSlider = 10.0f / m_iNumSign;        //  координата верхнего края текстуры цифры
    public float m_fStopSlider = 10.0f / m_iNumSign;    //  координата верхнего края текстуры цифры
    public boolean m_bDown = false;
    //
    public int m_iSymbol = 10;        // 0 - 0, 1 -1 , .... 10 - пустое поле

    public SlidingTexture() {
    }


    public SlidingTexture(float Height, float Width, float X, float Y, float Z) {
        m_fWidth = Width;
        m_fX = X;
        m_fY = Y;
        m_fZ = Z;
        m_fHeight = Height;
    }

    public void SetSymbol(int Symbol, boolean Animation ) {
        assert (m_iSymbol >= 0 && m_iSymbol < m_iNumSign);
        m_iSymbol = Symbol;
        m_fStopSlider = 1.0f / m_iNumSign * m_iSymbol;
        if (m_fSlider > m_fStopSlider)
            m_bDown = true;
        if( !Animation )
            m_fSlider = m_fStopSlider;
    }

    public TextureManager.EType GetTexture() {
        return TextureManager.EType.Digits;
    }

    public void GetVertexTexture( OGLDrawBuffer Buffer )   throws OGLBuffer.BufferExeption  {
        short IndexStart = (short)Buffer.Count();
        Buffer.AddVertex( m_fX, m_fY, m_fZ);
        Buffer.AddVertex(m_fX + m_fWidth, m_fY, m_fZ);
        Buffer.AddVertex( m_fX, m_fY - m_fHeight, m_fZ);
        Buffer.AddVertex( m_fX + m_fWidth, m_fY - m_fHeight, m_fZ);
        Buffer.SetColor( 1.0f, 1.0f, 1.0f, 0.8f );

        Buffer.AddTexture(0.0f, m_fSlider);
        Buffer.AddTexture(1.0f, m_fSlider);
        Buffer.AddTexture(0.0f, m_fSlider + 1.0f / m_iNumSign );
        Buffer.AddTexture(1.0f, m_fSlider + 1.0f / m_iNumSign );

        Buffer.AddQuadIndex( IndexStart, (short)(IndexStart + 2), (short)(IndexStart + 1),
                (short)(IndexStart + 2), (short)(IndexStart + 3), (short)(IndexStart + 1) );
    };

    public void Move() {
        if (m_bDown) {
            m_fSlider += 0.01f;
            if (m_fSlider >= 10.0f / m_iNumSign) {
                m_fSlider = 0.0f;
                m_bDown = false;
            }
        } else if (m_fSlider < m_fStopSlider) {
            m_fSlider += 0.01f;
            m_fSlider = Math.max(m_fSlider, m_fStopSlider);
        }
    } ;
};

public class NumberDrum extends GameObject {
    SlidingTexture[]	m_Drums;
    int                 m_iNumDigits = 0;
    float				m_fWidthDigit = 10.0f;
    float				m_fHeightDigit = 16.0f;
    boolean             m_bAnimate = true;

    public NumberDrum(OGLGame Game, int NumDigits ) {
        super(Game);
        m_eTextureType = TextureManager.EType.Digits;
        m_iNumDigits = NumDigits;
        m_Drums = new SlidingTexture[ NumDigits ];
    };

    public void Move() {
        for( SlidingTexture st : m_Drums ) {
            st.Move();
        }
    };

    public void ResetObject(boolean NewLvel) {
    };

    public void SetAnimation(boolean Animate) {
        m_bAnimate = Animate;
    };

    public void SetPos( float X, float Y, float Z,  //  координаты верхнего левого угла
                float Width, float Height) {
        float x = X;
        Y = m_pGame.m_fMaxHeight - Y;
        for (int i = 0; i < m_iNumDigits; ++i) {
            if( m_Drums[ i ] == null )
                m_Drums[ i ] = new SlidingTexture( Height,  Width,  x,  Y,  Z);
            else {
                m_Drums[ i ].m_fHeight = Height;
                m_Drums[ i ].m_fWidth = Width;
                m_Drums[ i ].m_fX = x;
                m_Drums[ i ].m_fY = Y;
                m_Drums[ i ].m_fZ = Z;
             }
            x += Width;
        }
    };

    public void SetNumber(int Num) {
        int number = Num;
        for (int i = 0; i < m_iNumDigits; ++i) {
            int nn = number - (number / 10) * 10;
            if (number == 0)
                nn = 10;
            m_Drums[m_iNumDigits - i - 1].SetSymbol(nn,m_bAnimate);
            number = number / 10;
        }
    };

    //   Рисование шейдерами
    public void	Draw(OGLDrawBuffer Buffer)    throws OGLBuffer.BufferExeption   {
        for (SlidingTexture dr : m_Drums) {
            dr.GetVertexTexture(Buffer);
        }
    };
}
