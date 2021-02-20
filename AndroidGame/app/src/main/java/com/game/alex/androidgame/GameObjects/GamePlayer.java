package com.game.alex.androidgame.GameObjects;

import com.game.alex.androidgame.OGLGame;
import com.game.alex.androidgame.OGLUtils.OGLBuffer;
import com.game.alex.androidgame.OGLUtils.OGLDrawBuffer;
import com.game.alex.androidgame.OGLUtils.TextureManager;

/**
 * Created by kag on 23.01.2018.
 */

public class GamePlayer extends GameObject {
    public  boolean			m_bKeyRIGHT = false;
    public  boolean			m_bKeyLEFT = false;
    public  boolean			m_bKeyDOWN = false;
    public  boolean			m_bKeyUP = false;
    //
    public  GamePlayer( OGLGame Game )    {
        super(Game);
        m_eTextureType = TextureManager.EType.Player;
    }

    //   Рисование шейдерами
    public  void	Draw(OGLDrawBuffer Buffer)  throws OGLBuffer.BufferExeption {
        float spin = 0.0f;
        switch (m_eDirection) {
            case  None:
            case  Right:
            case  Left: spin = 180.0f;
                break;
            case  Top: spin = -90.0f;
                break;
            case  Bottom: spin = 90.0f;
                break;
        }

        //Buffer.AffineTransformation2D(Vec3{ GLfloat(m_fFx + m_pGame->m_fGameFieldX), m_pGame->m_fMaxHeight - GLfloat(m_fFy + m_pGame->m_fGameFieldY), -0.4f },
        //spin, Vec3{ 0.0f, 0.0f, 1.0f }, Vec3{ 2.0f, 2.0f, 1.0f});
        //Buffer.AddVertices(m_fVertex, m_iVertAmount*3, m_fTexCoord, m_iVertAmount*2, { 1.0f, 1.0f, 1.0f, 1.0f } );

        Buffer.AffineTransformation2D( new GameStructs.Vec3( (float)(m_fFx + m_pGame.m_fGameFieldX),
                        m_pGame.m_fMaxHeight - (float)(m_fFy + m_pGame.m_fGameFieldY), -0.4f ),
                m_fSpin, new GameStructs.Vec3( 0.0f, 0.0f, 1.0f ), new GameStructs.Vec3(  2.0f, 2.0f, 1.0f ));
        Buffer.AddVerticesAndTransform(m_fVertex, m_iVertAmount * 3, m_fTexCoord, m_iVertAmount * 2,
                new GameStructs.OGLColor( 1.0f, 1.0f, 1.0f, 0.75f ) );
        Buffer.LoadIdentity();
    }

    public void	Move()    {
        if (m_pGame == null) return;

        if ( ( m_bKeyRIGHT  ) && m_iX < m_pGame.m_iHorSize - 1  && m_fFy == m_iY * m_pGame.m_SellHeight &&
                (m_fFx == m_iX * m_pGame.m_SellWidth || m_eDirection == GameStructs.EDirection.Left)) {
            m_pGame.ActivatePrize(m_iX, m_iY, true);
            m_iX++;										// Move The Player Right
            m_eDirection = GameStructs.EDirection.Right;
        }
        if ( ( m_bKeyLEFT  )  && m_iX > 0 && ( m_fFx == m_iX * m_pGame.m_SellWidth  || m_eDirection == GameStructs.EDirection.Right )&&
        m_fFy == m_iY * m_pGame.m_SellHeight) {
            m_iX--;									// Move The Player Left
            m_pGame.ActivatePrize(m_iX, m_iY, true);
            m_eDirection = GameStructs.EDirection.Left;
        }
        if ( ( m_bKeyDOWN  )  && m_iY < m_pGame.m_iVertSize - 1 && m_fFx == m_iX * m_pGame.m_SellWidth &&
                ( m_fFy == m_iY * m_pGame.m_SellHeight || m_eDirection == GameStructs.EDirection.Top) ) {
            m_pGame.ActivatePrize(m_iX, m_iY, false);
            m_iY++;											// Move The Player Down
            m_eDirection = GameStructs.EDirection.Bottom;
        }
        if ( ( m_bKeyUP  )  && m_iY > 0 && m_fFx == m_iX * m_pGame.m_SellWidth &&
                ( m_fFy == m_iY * m_pGame.m_SellHeight || m_eDirection == GameStructs.EDirection.Bottom ) ) {
            m_iY--;										// Move The Player Up
            m_pGame.ActivatePrize(m_iX, m_iY, false);
            m_eDirection = GameStructs.EDirection.Top;
        }


        if (m_fFx < m_iX * m_pGame.m_SellWidth) {						// Is Fine Position On X Axis Lower Than Intended Position?
            m_fFx += m_pGame.m_iSteps[m_pGame.m_iSpeed];				// If So, Increase The Fine X Position
            m_fFx = Math.min(m_fFx, (int)(m_iX * m_pGame.m_SellWidth));
        }

        if (m_fFx > m_iX * m_pGame.m_SellWidth) {						// Is Fine Position On X Axis Greater Than Intended Position?
            m_fFx -= m_pGame.m_iSteps[m_pGame.m_iSpeed];				// If So, Decrease The Fine X Position
            m_fFx = Math.max(m_fFx, (int)(m_iX * m_pGame.m_SellWidth));
        }

        if (m_fFy < m_iY * m_pGame.m_SellHeight) {				// Is Fine Position On Y Axis Lower Than Intended Position?
            m_fFy += m_pGame.m_iSteps[m_pGame.m_iSpeed];		// If So, Increase The Fine Y Position
            m_fFy = Math.min(m_fFy, (int)(m_iY * m_pGame.m_SellHeight));
        }

        if (m_fFy > m_iY * m_pGame.m_SellHeight) {				// Is Fine Position On Y Axis Lower Than Intended Position?
            m_fFy -= m_pGame.m_iSteps[m_pGame.m_iSpeed];		// If So, Decrease The Fine Y Position
            m_fFy = Math.max(m_fFy, (int)(m_iY * m_pGame.m_SellHeight));
        }
    };

    public void    ResetObject( boolean NewLvel )   {
        m_iX = 0;				// Reset Player X Position To Far Left Of The Screen
        m_iY = 0;				// Reset Player Y Position To The Top Of The Screen
        m_fFx = 0;				// Set Fine X Position To Match
        m_fFy = 0;				// Set Fine Y Position To Match
        m_fSpin = 0.0f;
    };
}
