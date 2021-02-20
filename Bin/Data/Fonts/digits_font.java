package com.game.alex.androidgame.OGLUtils;

import com.game.alex.androidgame.R;

public class EngSimpleFont   extends FontImage {
	static EngSimpleFont    font = new EngSimpleFont()
		{ {
			m_iResID = R.drawable.digits_font;
           m_Image = TextureManager.GetInstance().GetTexture(TextureManager.EType.digits_font);
           m_sName = "digits_font";
           m_iWidth = 800;
           m_iHeight = 402;
           m_iXoff = 0;
           m_iYoff = 0;
           m_iGlyphHeight = 121;
           m_Glyphs = new GLGlyph[11];
           m_Glyphs[0] = new GLGlyph(' ', 15.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[1] = new GLGlyph(' ', 145.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[2] = new GLGlyph(' ', 285.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[3] = new GLGlyph(' ', 421.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[4] = new GLGlyph(' ', 559.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[5] = new GLGlyph(' ', 690.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[6] = new GLGlyph(' ', 50.000000f, 265.000000f, 45.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[7] = new GLGlyph(' ', 153.000000f, 265.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[8] = new GLGlyph(' ', 285.000000f, 265.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[9] = new GLGlyph(' ', 427.000000f, 265.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
           m_Glyphs[10] = new GLGlyph(' ', 557.000000f, 265.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
        } };
    static public  EngSimpleFont GetInstance() { return font; };
}