package com.game.alex.androidgame.GameObjects;

import com.game.alex.androidgame.GameObjects.GameStructs;
import com.game.alex.androidgame.OGLGame;
import com.game.alex.androidgame.OGLUtils.TextureManager;


public abstract class GameObject {
    //	буферы вертексов и текстур
    static float[] m_fVertex = { -10.0f , +10.0f, -2.0f,
            -10.0f , -10.0f, -2.0f,
            +10.0f , +10.0f, -2.0f,
            -10.0f , -10.0f, -2.0f,
            +10.0f , -10.0f, -2.0f,
            +10.0f , +10.0f, -2.0f,
    };
    static float[] m_fTexCoord = { 0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
    };
    static  int	m_iVertAmount = 6;
    OGLGame		    m_pGame;
    float			m_fFx = 0.0f;				// Fine Movement Position
    float			m_fFy = 0.0f;
    int				m_iX = 0;				// Current Player Position
    int				m_iY = 0;
    float			m_fSpin = 0.0f;			// Spin Direction
    GameStructs.EDirection		m_eDirection = GameStructs.EDirection.None;
    TextureManager.EType        m_eTextureType = TextureManager.EType.None;
    // для рисования с помощью буферов
    int			m_iStartPos = 0;
    int			m_iAmount = 0;
    //
    public GameObject(OGLGame Game)  {
        m_pGame = Game;
    };

    public TextureManager.EType GetTexture() { return m_eTextureType;};
//    void	ProcessKeyboard() {};
    //
    abstract public void	Move();
    abstract public void    ResetObject( boolean NewLvel );
}

// buildToolsVersion "25.0.3"