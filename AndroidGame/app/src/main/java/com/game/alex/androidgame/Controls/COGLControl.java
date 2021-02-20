package com.game.alex.androidgame.Controls;


import com.game.alex.androidgame.GameObjects.GameStructs.Vec3;
import com.game.alex.androidgame.GameObjects.GameStructs.OGLColor;
import com.game.alex.androidgame.OGLUtils.OGLDrawBuffer;
import com.game.alex.androidgame.OGLUtils.TextureManager;

/**
 * Created by kag on 14.03.2018.
 */

public class COGLControl {

    public enum ERotation {
        R0,		//		без поворота
        R90,	//		поворот на 90 градусов
        R180,	//		поворот на 180 градусов
        R270,	//		поворот на 270 градусов
    };

    public enum  EHorAlign {
        No,
        Left,
        Center,
        Right,
        Full,
        RightBind,
    };

    public enum  EVertAlign {
        No,
        Up,
        Center,
        Down,
        Full,
        DownBind
    };

    boolean		m_bIsClicable = false;
    boolean		m_bVisible = true;
    boolean		m_bDisable = false;
    //
    EHorAlign	m_eHorAlign = EHorAlign.No;
    EVertAlign	m_eVertAlign = EVertAlign.No;
    //
    Vec3		m_LeftTop;				// относительные  координаты левого верхнего угла контрола ( для текста линия над которой расположен текст )
    float		m_fWidth = 0.0f;		// относительный размер контрола
    float		m_fHeight = 0.0f;
    //
    Vec3		m_visLeftTop;			//  фактическое положение контрола  - для рисования
    float		m_fvisWidth = 0.0f;		//  размер контрола
    float		m_fvisHeight = 0.0f;
    //
    OGLColor	m_Color = new OGLColor( 1.0f, 1.0f, 1.0f, 1.0f );
    OGLColor	m_ActiveColor =  new OGLColor( 1.0f, 1.0f, 1.0f, 1.0f );
    OGLColor	m_CurrentColor = new OGLColor( 1.0f, 1.0f, 1.0f, 1.0f );
    ERotation	m_eRottation = ERotation.R0;
    TextureManager.EType	m_iTextureType = TextureManager.EType.None;
    //
    COGLControl				m_pParent = null;

    //

    COGLControl( ) {   }
    COGLControl( TextureManager.EType  Type ) {     }
    //
    void SetAlign( EHorAlign HorAlign, EVertAlign VertAlign ) {
        m_eHorAlign = HorAlign;
        m_eVertAlign = VertAlign;
    }
    //
    void	SetParent(COGLControl Parent) { m_pParent = Parent; };
    COGLControl 	GetParent()  {
        return m_pParent;
    }
    //
    void	SetPosition(  Vec3 LeftTop, float Width, float Height, ERotation  Rotation )  {
        m_LeftTop = LeftTop;
        m_fWidth = Width;
        m_fHeight = Height;
        m_eRottation = Rotation;
    }
    //
    void	SetPosition( Vec3 LeftTop, float Size,  ERotation  Rotation)  {
        m_LeftTop = LeftTop;
        m_fWidth = Size;
        m_fHeight = -1.0f;
        m_eRottation = Rotation;
    }
    void	SetColor( OGLColor Color, OGLColor ActiveColor /*= { 1.0f, 1.0f, 1.0f, 1.0 }*/ )  {
        m_Color = Color;
        m_CurrentColor = Color;
        m_ActiveColor = ActiveColor;
    }
    //
    void	SetVisible( boolean Vis )  { m_bVisible = Vis; }
    boolean	GetVisible()  { return m_bVisible; }
    //
    void		SetDisable(boolean Disable)  { m_bDisable = Disable; }
    boolean		GetDisable()  { return m_bDisable; }
    //
    float Left()  { return m_LeftTop.x;	};
    float Right()   { return m_LeftTop.x + m_fWidth;  }
    float Top()  { return m_LeftTop.y; }
    float Bottom()  { return m_LeftTop.y + m_fHeight; }

    float Width()  { return  m_fWidth; }
    float Height()  { return  m_fHeight; }

    float visLeft()  { return m_visLeftTop.x;	}
    float visRight()  { return m_visLeftTop.x + m_fvisWidth;  }
    float visTop()  { return m_visLeftTop.y; };
    float visBottom()  { return m_visLeftTop.y + m_fvisHeight; }
    //
    float Z()  { return m_LeftTop.z; }
    void	GetPosition(float Width, float Height ) {

    }
    //
    ERotation	GetRotation()   { return m_eRottation; }
    OGLColor	GetCurrentColor()   { return m_CurrentColor; }
    //
    TextureManager.EType	GetTexType()   { return m_iTextureType; }
    //
    boolean	IsClickable()  { return m_bIsClicable; }

    void	OnLMouseDown() {}
    void	OnLMouseUp() {}
    void	OnMouseMove() {}
    void	OnMouseLeave() {}
    //
    void	DrawES(OGLDrawBuffer Buffer, float Height ) {
        if (m_iTextureType != TextureManager.EType.None) {
            glEnable(GL_TEXTURE_2D);
            TextureManager.GetInstance().GetTexture(m_iTextureType).Bind();
        }
	else
        glDisable(GL_TEXTURE_2D);

        //
        VertexArrayType verOut;
        GetQuadVertecis(Vec3{ m_visLeftTop.x, m_visLeftTop.y, m_visLeftTop.z/*Z*/ }, Vec2{ m_fvisWidth, m_fvisHeight },
        m_eRottation, verOut);
        for (auto i = 1; i < iVerteAmount * 3; i += 3 ) verOut[i] = Height - verOut[i];
        Buffer.AddVertex(verOut, verOut + iVerteAmount * Buffer.VertexDimension);
        //Buffer.AddVerticesAndTransform(verOut, iVerteAmount * Buffer.VertexDimension, TextureCoords, iVerteAmount * Buffer.TextureDimension, m_CurrentColor);
        if (m_iTextureType != CTextureManager::EType::None)
        Buffer.AddTexture( TextureCoords, TextureCoords + iVerteAmount * Buffer.TextureDimension);
        Buffer.SetColor(m_CurrentColor);
        Buffer.AddIndex(Indexes, Indexes + Indexes_size);
    }
    //
    void OnResizeWindow(float Width, float Height)    {
        GetPosition(Width, Height);
    }
}
