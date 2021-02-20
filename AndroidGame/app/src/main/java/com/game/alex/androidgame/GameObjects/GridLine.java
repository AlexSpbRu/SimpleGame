package com.game.alex.androidgame.GameObjects;

import com.game.alex.androidgame.OGLGame;
import com.game.alex.androidgame.GameObjects.GameStructs;
import com.game.alex.androidgame.GameObjects.GameObject;
import com.game.alex.androidgame.OGLUtils.OGLBuffer;
import com.game.alex.androidgame.OGLUtils.OGLDrawBuffer;
import android.opengl.Matrix;


public class GridLine  extends GameObject {

    boolean			m_bChecked = false;
    float			m_fZ     = -3.0f;
    float			m_fXstop = 0.0f;	// координаты конечной точки отрезка
    float			m_fYstop = 0.0f;	//  координаты начальной точки лежат в CGameObject - m_fFx,	m_fFy
    GameStructs.OGLColor    m_StartColor = new GameStructs.OGLColor( 1.0f, 1.0f, 1.0f, 1.0f );
    GameStructs.OGLColor	m_StopColor = new GameStructs.OGLColor( 1.0f, 1.0f, 1.0f, 1.0f );

    public GridLine(OGLGame Game)  {
        super(Game);
    };
    //
    public void Init(GameStructs.EDirection	Direction, float Z, float Xstart, float Ystart, float Length ) {
        m_bChecked = false;
        m_eDirection = Direction;
        m_fZ = Z;
        m_fFx = Xstart;	m_fFy = m_pGame.m_fMaxHeight - Ystart;
        switch (Direction) {
            case Bottom	: m_fXstop = m_fFx;             m_fYstop = m_fFy - Length; break;
            case Left	: m_fXstop = m_fFx + Length;	m_fYstop = m_fFy; break;
            case Right	: m_fXstop = m_fFx - Length;	m_fYstop = m_fFy; break;
            case Top	: m_fXstop = m_fFx;	            m_fYstop = m_fFy + Length; break;
            case None	: assert(false); break;
        }
    };
    //
    public void SetColor( float R, float G, float B, float A ){
        m_StartColor.r =  m_StopColor.r = R;
        m_StartColor.g =  m_StopColor.g = G;
        m_StartColor.b =  m_StopColor.b = B;
        m_StartColor.a =  m_StopColor.a = A;
    }
    //
    public void ResetObject( boolean NewLvel ){ m_bChecked = false; };
    //
    public void	Move() {};
    //
    public boolean GetCheck () {  return m_bChecked; };
    //
    public void SetCheck (boolean Check) {  m_bChecked = Check; };
    //  новое рисование через буферы
    public void	Draw(OGLDrawBuffer Buffer)  throws OGLBuffer.BufferExeption {
        Buffer.AddVertex(m_fFx, m_fFy, m_fZ);
        Buffer.AddVertex(m_fXstop, m_fYstop, m_fZ);
        Buffer.AddColor(m_StartColor);
        Buffer.AddColor(m_StopColor);
    };

}
