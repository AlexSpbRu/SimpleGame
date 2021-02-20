package com.game.alex.androidgame.OGLUtils;


import android.content.Context;
import android.opengl.GLES20;

import com.game.alex.androidgame.R;

public class TextureManager {
    public enum  EType  {
        None,
        Player,
        Enemy,
        Hourglass,
        Bomb,
        Live,
        Line,
        Arrow,
        Font,
        Pause,
        Start,
        Speed,
        Digits,
        Background,
        PanelBtn,
        Panel,
        EnglishSimpleFont,
        DigitFont,
        textureCount
    }
    public static TextureManager  m_Instance = null;
    private TextureUtils m_arrTextures[]  = new TextureUtils[EType.textureCount.ordinal()];
    public  TextureManager(Context context) { Init(context); }
    private  boolean Init(Context context){
        int res = R.drawable.player1;
        int pos = EType.Player.ordinal();
        TextureUtils text1 = new TextureUtils();
        int ret = text1.loadTexture( context, R.drawable.player1,GLES20.GL_LINEAR, GLES20.GL_LINEAR);
        m_arrTextures[EType.Player.ordinal()] = text1;
        TextureUtils text2 = new TextureUtils();
        ret |= text2.loadTexture( context, R.drawable.bomb1,GLES20.GL_LINEAR, GLES20.GL_LINEAR);
        m_arrTextures[EType.Bomb.ordinal()] =  text2;
        TextureUtils text3 = new TextureUtils();
        ret |= text3.loadTexture( context, R.drawable.enemy1,GLES20.GL_LINEAR, GLES20.GL_LINEAR);
        m_arrTextures[EType.Enemy.ordinal()] =  text3;
        TextureUtils text4 = new TextureUtils();
        ret |=  text4.loadTexture( context, R.drawable.hourglass2, GLES20.GL_LINEAR, GLES20.GL_LINEAR);
        m_arrTextures[EType.Hourglass.ordinal()] =  text4;
        TextureUtils text5 = new TextureUtils();
        ret |=  text5.loadTexture( context, R.drawable.line, GLES20.GL_LINEAR, GLES20.GL_LINEAR);
        m_arrTextures[EType.Line.ordinal()] =  text5;
        TextureUtils text6 = new TextureUtils();
        ret |= text6.loadTexture( context, R.drawable.star, GLES20.GL_LINEAR, GLES20.GL_LINEAR);
        m_arrTextures[EType.Live.ordinal()] =  text6;
        TextureUtils text7 = new TextureUtils();
        ret |= text7.loadTexture( context, R.drawable.back/*ground*/,GLES20.GL_LINEAR, GLES20.GL_LINEAR);
        m_arrTextures[EType.Background.ordinal()] =  text7;

        TextureUtils text8 = new TextureUtils();
        ret |= text8.loadTexture( context, R.drawable.alphabet_free_png, GLES20.GL_LINEAR, GLES20.GL_LINEAR);
        m_arrTextures[EType.EnglishSimpleFont.ordinal()] =  text8;
        TextureUtils text9 = new TextureUtils();
        ret |= text9.loadTexture( context, R.drawable.digits1, GLES20.GL_LINEAR, GLES20.GL_LINEAR);
        m_arrTextures[EType.Digits.ordinal()] =  text9;

        TextureUtils text10 = new TextureUtils();
        ret |= text10.loadTexture( context, R.drawable.digits_font, GLES20.GL_LINEAR, GLES20.GL_LINEAR);
        m_arrTextures[EType.DigitFont.ordinal()] =  text10;
        return ret != 0;
    }
    public static TextureManager GetInstance(){
        if( m_Instance != null)
            return m_Instance;
        else
            return null;
    }

    public TextureUtils	GetTexture( EType  Texture){
        return m_arrTextures[Texture.ordinal()];
    }
}
