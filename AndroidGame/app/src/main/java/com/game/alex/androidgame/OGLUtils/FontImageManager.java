package com.game.alex.androidgame.OGLUtils;

import com.game.alex.androidgame.OGLUtils.OGLDrawBuffer;
import com.game.alex.androidgame.OGLUtils.FontImage;

/**
 * Created by kag on 20.12.2017.
 */

public class FontImageManager {
    int								m_iWidth = 0;
    int								m_iHeight = 0;

    FontImage[]		                m_vFonts = new FontImage[1];
    int                             m_iCount = 0;
    int								m_iCurFont = -1;
    OGLDrawBuffer       			m_DrawBuffer;
    static FontImageManager	        m_instance = new FontImageManager();

    public static FontImageManager GetInstance()  {
        return m_instance;
    };
    //
    //public void		InitFonts(  FontImage.FontSerial Fonts, int	Count ) {}
    public void		AddFont( FontImage Font ) {
        m_vFonts[m_iCount] = Font;
        m_iCount++;
    }
   /* public void		AddFontSerial( FontImage.FontSerial Data ) {
        FontImage font = new FontImage();
        font.MakeFontSerial( Data );
        AddFont( font );
    }*/
    public int		GetFontsCount() { return m_iCount; }
    public FontImage	GetFont( int Num ) {
        if( Num >= 0 && Num < m_iCount)
            return m_vFonts[Num];
        else
            return null;
    };
    public void	    SetCurFont( int Num ) { m_iCurFont = Num; };
    public int		GetCurFont() { return m_iCurFont; };
}
