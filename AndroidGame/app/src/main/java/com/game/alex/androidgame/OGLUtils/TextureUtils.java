package com.game.alex.androidgame.OGLUtils;


import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;

import static android.opengl.GLES20.GL_TEXTURE0;
import static android.opengl.GLES20.GL_TEXTURE_2D;
import static android.opengl.GLES20.glActiveTexture;
import static android.opengl.GLES20.glBindTexture;
import static android.opengl.GLES20.glDeleteTextures;
import static android.opengl.GLES20.glGenTextures;
import static android.opengl.GLES20.glGetError;


public class TextureUtils {
    private int	        m_iTexture = 0;
    private boolean	    m_bCreated = false;
    //
    private int			m_iWidth = 0;
    private int			m_iHeight = 0;
    private int			m_iImageSize = 0;
     //bool	LoadBitmap(const std::string& FileName);
   // public :
    //CGLTexture() {};
    //bool glLoadImage(const std::string& FileName);
    //boolean CreateTexture( GLint MAGfilter, GLint MINfilter );
   // boolean CreateTexture( const std::string& FileName, GLint MAGfilter, GLint MINfilter, int VerticallyFlip );
     public void DeleteTexture() {
        final int[] textureIds = new int[1];
        textureIds[0] = m_iTexture;
        glDeleteTextures(1, textureIds,0);
        m_iTexture = 0;
        m_bCreated = false;
    }
    public boolean IsTexture() { return m_bCreated; }
    public boolean Bind() {
        //glBindTexture(GL_TEXTURE_2D, m_iTexture);
        if (m_bCreated) {
            glBindTexture(GL_TEXTURE_2D, m_iTexture);
            int err = 0;//glGetError();
            return err == 0;
        }
        else return false;
    }
    public int Width() { return m_iWidth; }
    public int Height() { return m_iHeight; }
    public void SetWidth( int Width ) {  m_iWidth = Width; }
    public void SetHeight( int Height ) {  m_iHeight = Height; }
    public int loadTexture(Context context, int resourceId, int MAGfilter, int MINfilter ) {
        int ret = 0;
        // создание объекта текстуры
        int[] textureIds = new int[1];
        glGenTextures(1, textureIds, 0);
        if (textureIds[0] == 0)
           return ret;

        // получение Bitmap
        final BitmapFactory.Options options = new BitmapFactory.Options();
        options.inScaled = false;

        final Bitmap bitmap = BitmapFactory.decodeResource(
                context.getResources(), resourceId, options);

        if (bitmap == null) {
            glDeleteTextures(1, textureIds, 0);
            return 0;
        }

        m_iHeight = bitmap.getHeight();
        m_iWidth = bitmap.getWidth();
        // настройка объекта текстуры
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureIds[0]);

        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MIN_FILTER, MINfilter/*GLES20.GL_LINEAR*/);
        GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D, GLES20.GL_TEXTURE_MAG_FILTER, MAGfilter/*GLES20.GL_LINEAR*/);

        GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);

        bitmap.recycle();

        m_iTexture = textureIds[0];
        m_bCreated = true;
        return textureIds[0];
    }
}
