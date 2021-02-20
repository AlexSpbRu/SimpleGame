package com.game.alex.androidgame.OGLUtils;

import com.game.alex.androidgame.R;
//import FontImage;

/**
 * Created by kag on 02.02.2018.
 */

public class DigitFont extends FontImage {
    static DigitFont    font = new DigitFont()
        { {
            m_iResID = R.drawable.digits_font;
            m_Image = TextureManager.GetInstance().GetTexture(TextureManager.EType.DigitFont);
            m_sName = "digits_font";
            m_iWidth = 800;
            m_iHeight = 402;
            m_iXoff = 0;
            m_iYoff = 0;
            m_iGlyphHeight = 121;
            m_Glyphs = new GLGlyph[13];
            m_Glyphs[0] = new GLGlyph('1', 15.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[1] = new GLGlyph('2', 145.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[2] = new GLGlyph('3', 285.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[3] = new GLGlyph('4', 421.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[4] = new GLGlyph('5', 559.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[5] = new GLGlyph('6', 690.000000f, 36.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[6] = new GLGlyph('.', 50.000000f, 265.000000f, 45.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[7] = new GLGlyph('7', 153.000000f, 265.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[8] = new GLGlyph('8', 285.000000f, 265.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[9] = new GLGlyph('9', 427.000000f, 265.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[10] = new GLGlyph('0', 557.000000f, 265.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[11] = new GLGlyph(',', 50.000000f, 265.000000f, 45.000000f, 121.000000f, 0.000000f, 0.000000f);
            m_Glyphs[12] = new GLGlyph(' ', 690.000000f, 265.000000f, 94.000000f, 121.000000f, 0.000000f, 0.000000f);
        } };
    static public  DigitFont GetInstance() { return font; };
}

