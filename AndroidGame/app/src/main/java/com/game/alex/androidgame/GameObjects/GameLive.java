package com.game.alex.androidgame.GameObjects;

import static java.lang.Math.*;

import com.game.alex.androidgame.OGLGame;
import com.game.alex.androidgame.OGLUtils.OGLBuffer;
import com.game.alex.androidgame.OGLUtils.OGLDrawBuffer;
import com.game.alex.androidgame.GameObjects.GameStructs.*;
import com.game.alex.androidgame.OGLUtils.TextureManager;

/**
 * Created by kag on 25.12.2017.
 */

public class GameLive extends GameObject {
    enum EGameLiveState {
        Active,
        Fall,
        NotActive
    };
    final float                 m_StartScale = 2.0f;
    EGameLiveState				m_State = EGameLiveState.Active;
    float						m_iAlpha = 1.0f;
    float						m_fScale = m_StartScale;
    //
    Vec3 Offset = new Vec3();
    Vec3 RotAxis = new Vec3(0.0f, 0.0f, 1.0f);
    Vec3 Scale = new Vec3();
    //
    public  GameLive( OGLGame Game ){
        super(Game);
        m_eTextureType = TextureManager.EType.Live;
    }
    public void	Move(){
        if (m_State == EGameLiveState.Active) {
            m_fSpin += 2;
            if (m_fSpin > 360.0f)
                m_fSpin = 0.0f;
            m_fScale = (float)(1.5 + 0.5*sin(m_fSpin / 180 * PI));
        }
        else if (m_State == EGameLiveState.Fall) {
            m_fSpin += 2;
            if (m_fSpin > 360.0f)
                m_fSpin = 0.0f;
            m_fScale +=  0.03f;
            m_iY += 1;
            m_iAlpha -= 0.01f;
            if (m_iAlpha < 0.0f) {
                m_iAlpha = 1.0f;
                m_State = EGameLiveState.NotActive;
            }
        }
    }
    public void	SetPosition(int X, int Y)   { m_iX = X; 	m_iY = Y; };
    public void	ResetObject(boolean NewLvel) {
        m_fSpin = 0.0f;//(float)(random() * 180);
        m_State = EGameLiveState.Active;
        m_iAlpha = 1.0f;
        m_fScale = m_StartScale;
    }
    //   Рисование шейдерами
    public void	Draw(OGLDrawBuffer Buffer)  throws OGLBuffer.BufferExeption {
        if (m_State == EGameLiveState.NotActive)
            return;
        Offset.x = (float)m_iX;
        Offset.y = m_pGame.m_fMaxHeight - (float)m_iY;
        Offset.z = -0.4f;
        Scale.x = m_fScale;
        Scale.y = m_fScale;
        Scale.z = 1.0f;
        Buffer.AffineTransformation2D( Offset/*new Vec3( (float)m_iX, m_pGame.m_fMaxHeight - (float)m_iY, -0.4f )*/,
        m_fSpin, RotAxis/*new Vec3( 0.0f, 0.0f, 1.0f )*/, Scale/*new Vec3( m_fScale, m_fScale, 1.0f )*/);
        Buffer.AddVerticesAndTransform(m_fVertex, m_iVertAmount * 3, m_fTexCoord, m_iVertAmount * 2,
                new OGLColor( 1.0f, 1.0f, 1.0f, 0.75f ) );
        Buffer.LoadIdentity();
    }


}
