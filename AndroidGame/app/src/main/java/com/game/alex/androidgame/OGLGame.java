package com.game.alex.androidgame;

import android.content.Context;
import android.opengl.GLSurfaceView;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

import java.util.ArrayList;

//import android.view.Display;

//import javax.microedition.khronos.egl.EGLConfig;
//import javax.microedition.khronos.opengles.GL10;
import android.opengl.Matrix;
import android.provider.ContactsContract;
import android.util.DisplayMetrics;
import android.view.WindowManager;

import static android.opengl.GLES20.GL_COLOR_BUFFER_BIT;
import static android.opengl.GLES20.GL_DEPTH_BUFFER_BIT;
import static android.opengl.GLES20.GL_FLOAT;
import static android.opengl.GLES20.GL_FRAGMENT_SHADER;
import static android.opengl.GLES20.GL_TEXTURE_2D;
import static android.opengl.GLES20.GL_TRIANGLES;
import static android.opengl.GLES20.GL_LINES;
import static android.opengl.GLES20.GL_VERTEX_SHADER;
import static android.opengl.GLES20.GL_BLEND;										// Enable Blending
import static android.opengl.GLES20.GL_SRC_ALPHA;
import static android.opengl.GLES20.GL_ONE_MINUS_SRC_ALPHA;
import static android.opengl.GLES20.GL_FALSE;
import static android.opengl.GLES20.glBlendFunc;
import static android.opengl.GLES20.glClearColor;
import static android.opengl.GLES20.glDrawArrays;
import static android.opengl.GLES20.glEnable;
import static android.opengl.GLES20.glDisable;
import static android.opengl.GLES20.glGenTextures;
import static android.opengl.GLES20.glLineWidth;
import static android.opengl.GLES20.glClear;
import static android.opengl.GLES20.glEnableVertexAttribArray;
import static android.opengl.GLES20.glGetAttribLocation;
import static android.opengl.GLES20.glGetUniformLocation;
import static android.opengl.GLES20.glUniform4f;
import static android.opengl.GLES20.glVertexAttribPointer;
import static android.opengl.GLES20.glViewport;
import static android.opengl.GLES20.glUseProgram;
import static android.opengl.GLES20.glDeleteProgram;
import static android.opengl.GLES20.glGetError;
import static android.opengl.GLES20.glDisableVertexAttribArray;

import com.game.alex.androidgame.GameObjects.GridLine;
import com.game.alex.androidgame.GameObjects.GameStructs.OGLColor;
import com.game.alex.androidgame.GameObjects.NumberDrum;
import com.game.alex.androidgame.GameObjects.GamePlayer;
import com.game.alex.androidgame.OGLUtils.OGLBuffer;
import com.game.alex.androidgame.OGLUtils.ShaderUtils;
import com.game.alex.androidgame.OGLUtils.TextureManager;
import com.game.alex.androidgame.OGLUtils.TextureUtils;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import com.game.alex.androidgame.GameObjects.GameStructs;
import com.game.alex.androidgame.GameObjects.GameLive;
import com.game.alex.androidgame.OGLUtils.OGLDrawBuffer;
import com.game.alex.androidgame.OGLUtils.FontImage;
import com.game.alex.androidgame.OGLUtils.EngSimpleFont;
import com.game.alex.androidgame.OGLUtils.DigitFont;

import java.nio. FloatBuffer;
import java.util.Date;


public class OGLGame  implements GLSurfaceView.Renderer {
   //
    enum  EPrize {
        HourGlass
    };

    enum  EPlayState {
        Normal,
        Die,
        Gameover,
        LevelComplete
    }
    protected EPlayState		m_ePlayState = EPlayState.Normal;
/*     void		PlayStateTreatment();
    //CGLTexture		m_Font;  // используется только в DrawScene()
    //GLuint			base;
    //
    std::string		m_AppPath;
    bool			m_keys[256];					// Array Used For The Keyboard Routine
    //
    std::vector<GLfloat>	m_LightAmbient = { 0.9f, 0.9f, 0.9f, 1.0f };
    std::vector<GLfloat>	m_LightDiffuse = { 1.0f, 1.0f,  1.0f, 1.0f };
    std::vector<GLfloat>	m_LightSpecular = { 1.0f, 1.0f,  1.0f, 1.0f };
    std::vector<GLfloat>	m_LightPosition = { 500.0f, 500.0f, 2000.0f, 1.0f };
    //
    CGLTexture		m_Background;*/
    protected Context       m_Context;
    protected int			m_iCountBack = 28;
    //std::vector< char >	m_vUsedBack;
    //size_t		GetNewBackgroundNum();
   // std::vector< std::string >		m_BackNames;
   // using steady_time = std::chrono::time_point<std::chrono::steady_clock>;
    //steady_time		m_CurrentTime;
    //
    protected TextureUtils m_Line;
    //
    public int			      	m_iHorSize = 11;
    public int			      	m_iVertSize = 11;
    public float   		    	m_SellHeight = 45.0f;
    public float   		    	m_SellWidth = 75.0f;
    public float	    		m_fGameFieldX = 50.0f;
    public float	    		m_fGameFieldY = 220.0f;
    protected ArrayList< ArrayList< GridLine > >	m_vVertLine;// [11][10];		// Keeps Track Of Verticle Lines
    protected ArrayList< ArrayList< GridLine > >	m_vHorLine;// [10][11];			// Keeps Track Of Horizontal Lines

    //
    NumberDrum		            m_Score = new NumberDrum(this, 6);//<6, SlidingTexture>
    NumberDrum		            m_Stage = new NumberDrum(this, 2);//<2, SlidingTexture>
    NumberDrum		            m_Level = new NumberDrum(this, 2);//<2, SlidingTexture>
    //NumberDrum		            m_fps   = new NumberDrum(this, 2);

    protected int			    m_iLives = 5;										// Player Lives
    protected int		    	m_iLevel = 1;										// Internal Game Level
    protected int				m_iLevel2 = 1;										// Displayed Game Level
    protected int	            m_iStage = 1;										// Game Stage
    protected int				m_iDelay = 0;										// Enemy Delay
    protected boolean	    	m_bFilled = true;									// Done Filling In The Grid?
    protected boolean		    m_bPause = false;
    //
    protected float	    		m_fMaxWidth = 1280.0f;		//  размер окна
    public float	    		m_fMaxHeight = 1024.0f;
    //
    public byte[]               m_iSteps = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    public int		            m_iSpeed = 7;
    //
    protected boolean		    m_bAp = true;
    protected boolean		    m_bAnti = false;
    //
    protected int		        m_iScore = 0;
    //
    protected boolean		    m_bMouseDown = false;
    //
    protected boolean		    m_btextureEnable;
    protected boolean		    m_balphaEnable;
    //
    OGLDrawBuffer           	m_DrawBuffer;
    protected int		        m_iVertexSize	= 3;  //   размерности
    protected int		        m_iColorSize	= 4;
    protected int		        m_iTextureSize	= 3;
    //
    float[]				        m_ProjMatrix = new float[16];
    float[]				        m_ModelMatrix = new float[16];
    float[]				        m_ResultMatrix = new float[16];
    //   для рисования фона
    float[]                     m_fTex = { 0.0f, 1.0f, 1.0f, 1.0f,  0.0f, 0.0f, 1.0f, 0.0f };
    short[]                     m_iIndex = { 2, 1, 0, 2, 3, 1 };
//    COGLESFont			m_FontES;
    ShaderUtils					m_Shader;
    protected int				m_iVertShAttr = 0;		//  координата вершины
    protected int				m_iTexCoordLoc = 0;		//	текстурная координгата
    protected int				m_iColorLoc = 0;		//	цвет вершины
    protected int				m_iSamplerLoc = 0;		//	номер текстуры
    //   для управления движением Игроком при помощи мыши
    protected int				m_iXprev = -1;
    protected int				m_iYprev = -1;
    //
    protected int               m_iProgramId = 0;
    protected int               m_iVertShaderID;
    protected int               m_iFragShaderID;
    protected int               m_iuColorLocation = 0;
    protected int               m_iaPositionLocation = 0;
    public int  GetProgrammId() { return m_iProgramId; };
    //
    FontImage                   m_Font = null;//EngSimpleFont.GetInstance();
    DigitFont                   m_DigFont = null;
    GamePlayer	                m_player = new GamePlayer(this);
    protected ArrayList	        m_vEnemy;			// Enemy Information
    protected ArrayList		    m_Prizes;
    protected GameLive[]	    m_vLives = null;
    //  для расчета FPS
    int                         m_iCadrCount= 0;
    long                        m_StartTime = 0;
    long                        m_StopTime = 0;
    float                       m_dFPS = 0.0f;
    //
    SensorManager               m_sensorManager;
    Sensor                      m_sensorGravity;
    //Sensor                      m_sensorGyroscope;
    //Sensor                      m_sensorUncalibratedGyroscope;
   // Sensor                      m_sensorRotationVector;
    float[]                     m_fGravityData = new float[3];

    SensorEventListener listener = new SensorEventListener() {

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
        }

        @Override
        public void onSensorChanged(SensorEvent event) {
            switch (event.sensor.getType()) {
                case Sensor.TYPE_ROTATION_VECTOR:
                    for (int i = 0; i < 3; i++) {
                        float val = event.values[i];
                        //valuesAccelGravity[i] = (float) (0.1 * event.values[i] + 0.9 * valuesAccelGravity[i]);
                        //valuesAccelMotion[i] = event.values[i]
                        //        - valuesAccelGravity[i];
                    }
                    break;
                case Sensor.TYPE_GYROSCOPE:
                    for (int i = 0; i < 3; i++) {
                        float val = event.values[i];
                    }
                    break;
                case Sensor.TYPE_GRAVITY:
                    for (int i = 0; i < 3; i++) {
                        m_fGravityData[i] = event.values[i];
                    }
                    m_player.m_bKeyRIGHT = m_fGravityData[0] < -0.5f;
                    m_player.m_bKeyLEFT = m_fGravityData[0] > 0.5f;
                    m_player.m_bKeyUP = m_fGravityData[1] < -0.5f;
                    m_player.m_bKeyDOWN = m_fGravityData[1] > 0.5f;
                    break;
            }

        }
    };
    //   элементы управления
 //   COGLControl				m_ControlArrowLeft;
//    COGLControl				m_ControlArrowRight;
 //   COGLControl				m_ControlArrowUp;
//    COGLControl				m_ControlArrowDown;
//    COGLButton				m_btnPause;
    //
/*    void		CalcNewPosition();

    void		CheckPrize(int X, int Y);*/
    protected void	CalcCellsSize(){
//        int wdt = m_Background.Width();
//        int hgt = m_Background.Height();
        //float h_w = 1.0f;//(float)hgt / (float)wdt;
        float maxWdt = m_fMaxWidth - 2*m_fGameFieldX;
        float maxHgt = m_fMaxHeight - 30 - m_fGameFieldY;
        //if (maxWdt*h_w > maxHgt)
        //    maxWdt = (int)(maxHgt / h_w);
        //else
        //    maxHgt = (int)(maxWdt*h_w);
        m_SellWidth = (int)(maxWdt / (m_iHorSize - 1));
        m_SellHeight = (int)(maxHgt / (m_iVertSize - 1));
        //
        /*auto left = m_fGameFieldX + ((m_iHorSize - 1) / 2 - 1)*m_SellWidth - 50.0f,
                top = m_fGameFieldY + ((m_iVertSize - 1) / 2)*m_SellHeight - 50.0f;
        m_ControlArrowLeft.SetPosition(Vec3{ left, top, -3.0f }, Vec3{ left + 100.0f, top + 100.0f, -3.0f }, ERotation::R0);

        left = m_fGameFieldX + ((m_iHorSize - 1) / 2 + 1)*m_SellWidth - 50.0f;
        top = m_fGameFieldY + ((m_iVertSize - 1) / 2)*m_SellHeight - 50.0f;
        m_ControlArrowRight.SetPosition(Vec3{ left, top, -3.0f }, Vec3{ left + 100.0f, top + 100.0f, -3.0f }, ERotation::R180);

        left = m_fGameFieldX + ((m_iHorSize - 1) / 2)*m_SellWidth - 50.0f;
        top = m_fGameFieldY + ((m_iVertSize - 1) / 2 - 1)*m_SellHeight - 50.0f;
        m_ControlArrowUp.SetPosition(Vec3{ left, top, -3.0f }, Vec3{ left + 100.0f, top + 100.0f, -3.0f }, ERotation::R90);

        left = m_fGameFieldX + ((m_iHorSize - 1) / 2)*m_SellWidth - 50.0f;
        top = m_fGameFieldY + ((m_iVertSize - 1) / 2 + 1)*m_SellHeight - 50.0f;
        m_ControlArrowDown.SetPosition(Vec3{ left, top, -3.0f }, Vec3{ left + 100.0f, top + 100.0f, -3.0f }, ERotation::R270);*/

    }
 /*
#ifndef OPENGL_SHADER
    void		InitGL() noexcept;
#else
    void		InitGLES() noexcept;
    void		DrawBuffer(CGLTexture* Texture, CTextVertexColorBuffer& Buffer ) noexcept;
#endif

    */
    void	ResetLives()  {
        for ( int i = 0; i != m_iLives; ++i ) {
            m_vLives[i].SetPosition(250 + i * 40, 90);
            m_vLives[i].ResetObject(true);
        }
    }


    protected void		ResetObjects( boolean NewLvel ) {

       /* m_player.ResetObject(NewLvel);

        auto numEnemy =  m_iStage + m_iLevel;
//	numEnemy = 1;
        m_vEnemy.resize( numEnemy, CGameEnemy(this) );
        for (auto& enemy : m_vEnemy )				// Loop Through All The Enemies
        {
            enemy.ResetObject(NewLvel);
        }*/
        if (NewLvel) {
            //  персчитываем несработавшие призы и добаваляем их к новым
        /*    auto nHour = 0, nBomb = 0;
            for (auto& pr : m_Prizes)
            if (pr->m_eActive != CGamePrize::EState::Stop) {
                if (pr->m_eType == CGamePrize::EType::Bomb) nBomb++;
                else if (pr->m_eType == CGamePrize::EType::Hourglass) nHour++;
            }
            //
            for (auto& pr : m_Prizes)
            delete pr;
            m_Prizes.clear();
            auto numPrizes = numEnemy * 2;
            m_Prizes.resize( numPrizes, nullptr );
            float fnumHour = rand();
            float fnumBomb = rand();
            fnumHour = fnumHour / (fnumHour + fnumBomb)*numPrizes;
            int numHour = fnumHour + 0.5f,
                    numBomb = min( numPrizes - numHour, numEnemy - 1 );
            if (m_iLevel == 1)   //  первый уровень - должен быть простым
                numBomb = numEnemy - 1;
            numHour = numPrizes - numBomb;
            //  добавляем призы, сохранившиеся с предыдущего уровня
            numBomb += nBomb;
            numHour += nHour;
            //  вычисляем случайным образом число разных призов
            // задаем случайные номера для ячеек, где спрятаны часы
            int num = 0;
            for (auto& hg : m_Prizes) {
                if(num < numHour)
                    hg = new CGameHourglass(this);
                else if(num < numHour + numBomb )
                    hg = new CGameBomb(this);
                num++;
                bool uniqueCell = true;
                int xcell = 0;
                int ycell = 0;
                do {
                    uniqueCell = true;
                    xcell = rand() % (m_iHorSize - 1);
                    ycell = rand() % (m_iHorSize - 1);
                    for (auto& hg1 : m_Prizes)
                    if ( hg1 != nullptr && hg1->m_iX == xcell && hg1->m_iY == ycell)
                        uniqueCell = false;
                } while (!uniqueCell);
                hg->m_iX = xcell;
                hg->m_iY = ycell;
                hg->ResetObject( NewLvel );
                IO_TRACE("prize in SELL ( %d, %d )\n", hg->m_iX, hg->m_iY);
            }
            m_HourglassData.clear();
            */
            //		инициализация линий
            int i = 0;
            for ( int row = 0 ; row < m_iVertSize ; ++row )
                for( int col = 0 ; col < m_iHorSize; col++ )   {
                    if (row < m_iVertSize - 1) {
                        m_vVertLine.get(col).get(row).Init(GameStructs.EDirection.Bottom, -3.0f, m_fGameFieldX + col*m_SellWidth, m_fGameFieldY + row*m_SellHeight, m_SellHeight);
                        i++;
                    }
                    if (col < m_iHorSize - 1) {
                        m_vHorLine.get(col).get(row).Init(GameStructs.EDirection.Left, -3.0f, m_fGameFieldX + col*m_SellWidth, m_fGameFieldY + row*m_SellHeight, m_SellWidth);
                        i++;
                    }
                }

        }
    }

    public OGLGame(Context context ){
        m_Context = context;
        //
        m_player.ResetObject(true);
        //
        m_vVertLine = new ArrayList< ArrayList<GridLine>>(m_iHorSize);
        m_vVertLine.trimToSize();
        m_vVertLine.ensureCapacity(m_iHorSize);
        for( int i = 0 ; i != m_iHorSize ; ++i ) {
            ArrayList<GridLine> line = new ArrayList<GridLine>( m_iVertSize-1 );
            for( int j = 0 ; j != m_iVertSize-1 ; ++j )
                line.add( new GridLine(this) );
            line.trimToSize();
            m_vVertLine.add( line );
        }
        //m_vVertLine( m_iHorSize, std::vector<SWayPart>(m_iVertSize-1, SWayPart(this)) ),
        m_vHorLine = new ArrayList< ArrayList<GridLine>>( m_iHorSize-1 );
        m_vHorLine.trimToSize();
        for( int i = 0 ; i != m_iHorSize-1 ; ++i ) {
            ArrayList<GridLine> row = new ArrayList<GridLine>( m_iVertSize );
            for( int j = 0 ; j != m_iVertSize ; ++j )
                row.add( new GridLine(this) );
            row.trimToSize();
            m_vHorLine.add( row );;
        }
        //
        m_vLives = new GameLive[5];
        for( int i = 0 ; i < 5 ; i++ )
            m_vLives[i] = new GameLive(this);
        ResetLives();
        //m_vHorLine(m_iVertSize-1, std::vector<SWayPart>(m_iHorSize, SWayPart(this)) )
        m_DrawBuffer = new OGLDrawBuffer( 3, 4, 10000 );
        //
        Matrix.setIdentityM( m_ModelMatrix, 0 );
        Matrix.setIdentityM( m_ResultMatrix, 0 );
        //
        m_Score.SetPos(570.0f, 50, 0.0f, 24.0f, 32.0f);
        m_Score.SetNumber(0);

        m_Stage.SetPos(150.0f, 70, 0.0f, 24.0f, 32.0f);
        m_Stage.SetNumber(m_iStage);

        m_Level.SetPos(150.0f, 35, 0.0f, 24.0f, 32.0f);
        m_Level.SetNumber(m_iLevel2);

        //m_fps.SetPos(150.0f, 105, 0.0f, 24.0f, 32.0f);
        //m_fps.SetNumber(0);
        //m_fps.SetAnimation(false);
        //
        m_sensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);

        m_sensorGravity = m_sensorManager.getDefaultSensor(Sensor.TYPE_GRAVITY);
        //m_sensorGyroscope = m_sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
        //m_sensorUncalibratedGyroscope = m_sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE_UNCALIBRATED);
        //m_sensorRotationVector = m_sensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
        //   переднести в обработку паузы
        m_sensorManager.registerListener(listener, m_sensorGravity,
                SensorManager.SENSOR_DELAY_NORMAL);
        //m_sensorManager.registerListener(listener, m_sensorGyroscope,
        //        SensorManager.SENSOR_DELAY_NORMAL);
        //m_sensorManager.registerListener(listener, m_sensorRotationVector,
        //        SensorManager.SENSOR_DELAY_NORMAL);
        DisplayMetrics displaymetrics = new DisplayMetrics();
        WindowManager windowManager = (WindowManager) context.getSystemService(Context.WINDOW_SERVICE);
        windowManager.getDefaultDisplay().getMetrics(displaymetrics);
        int screenWidth = displaymetrics.widthPixels;
        int screenHeight = displaymetrics.heightPixels;
    }

    public boolean		Init( int Width, int Height ) {
        if (m_iCountBack == 0)
            return false;
     //   void InitAPI();
     //   InitAPI();

     //   size_t num = GetNewBackgroundNum();
     //   if (!m_Background.CreateTexture(/*"d:\\Projects\\GameGL\\Bin\\Data\\Game\\Girls\\error26.jpg"*/m_BackNames[num], GL_LINEAR, GL_LINEAR, 1))
     //       return false;
        SetMaxSize(Width, Height);
        CalcCellsSize();
        //

        //m_Line = TextureManager.GetInstance(m_Context).GetTexture(TextureManager.EType.Line);
        //strPath = m_AppPath + "Data\\Game\\line.png";
        //if (!m_Line.CreateTexture(strPath, GL_LINEAR, GL_LINEAR, 1))
       //if( !m_Line.IsTexture() )
        //    return false;
        //
       // InitGLES();
        //
        ResetObjects( true );
        //m_player.Init();
        glEnable(GL_BLEND);										// Enable Blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        return true;
    }
/*    void		GetWindowSize(int& Width, int& Height)  noexcept;*/
    protected void Move() {
        m_player.Move();
        //  звездочки живут своей жизнью все время
        for (GameLive lv : m_vLives)
            lv.Move();
        //
        m_Score.Move();
        m_Stage.Move();
        m_Level.Move();
        //m_fps.Move();
        //

        /*if (m_ePlayState != EPlayState::Normal || m_bPause)
            return;
        for (size_t numEnemy = 0; numEnemy < m_vEnemy.size(); numEnemy++)	{
            m_vEnemy[numEnemy].Check();
            m_vEnemy[numEnemy].Move();
            //
            if ( abs( m_vEnemy[numEnemy].m_fFx - m_player.m_fFx ) < 10 && abs( m_vEnemy[numEnemy].m_fFy - m_player.m_fFy ) < 10 )	{
                m_iLives--;								// If So, Player Loses A Life
                m_vLives[m_iLives].m_State = EGameLiveState::Fall;  //  должна упасть
                m_ePlayState = EPlayState::Die;
                if (m_iLives == 0)
                    m_ePlayState = EPlayState::Gameover;
                ViewPanel(true, m_ePlayState);
                //CPlaySound::GetInstance().playSound("Data/Die.wav", CPlaySound::EPlayType::Once);
                PlaySound("Data/Die.wav", NULL, SND_ASYNC);	// Play The Death Sound
                //
                memset(m_keys, 0, sizeof(m_keys));
                //
                m_bPause = true;
                m_btnPause.SetPressed(true);
                break;
            }
        }
        //
        m_player.Move();
        //
        if (m_bFilled &&  m_ePlayState == EPlayState::Normal) {
            m_ePlayState = EPlayState::LevelComplete;
            ViewPanel( true, m_ePlayState);
            //CPlaySound::GetInstance().playSound("Data/Complete.wav", CPlaySound::EPlayType::Once);
            PlaySound("Data/Complete.wav", NULL, SND_ASYNC);	// If So, Play The Level Complete Sound
            //
            m_iStage++;								// Increase The Stage
            if (m_iStage > 3) {						// Is The Stage Higher Than 3?
                m_iStage = 1;						// If So, Set The Stage To One
                m_iLevel++;							// Increase The Level
                m_iLevel2++;
                m_iLives++;
                if (m_iLives > 5)					// Does The Player Have More Than 5 Lives?
                    m_iLives = 5;					// If So, Set Lives To Five
                ResetLives();
                if (m_iLevel > 3) 					// Is The Level Greater Than 3?
                    m_iLevel = 3;					// If So, Set The Level To 3
            }
            //
            m_bPause = true;
            m_btnPause.SetPressed(true);*/

           // m_Score.SetNumber(m_iScore);
            //m_Stage.SetNumber(m_iStage);
            //m_Level.SetNumber(m_iLevel2);
           // m_fps.SetNumber((int)m_dFPS);
        /*}
        // If The Player Hits The Hourglass While It's Being Displayed On The Screen
        for( auto& hg : m_Prizes)
        hg->Move();*/

    }

    protected void	SetMaxSize( int Width, int Height )  {
        m_fMaxWidth = (float)Width;
        m_fMaxHeight = (float)Height;
    }
/*#ifndef OPENGL_SHADER
#else
    void		LoadIdentity()  noexcept { glhLoadIdentityf2(&m_Matrix); };
    void		AffineTransformation2D(const Vec3& Offset, GLfloat Angle, const Vec3& RotAxis, const Vec3& Scale) noexcept;
    void		AffineTransformation2D(const Vec3&& Offset, GLfloat Angle, const Vec3&& RotAxis, const Vec3&& Scale) noexcept;
    void		AddVertices(const GLfloat* Vertises, int VertSize, const GLfloat* Texture, int TexSize, const SOGLColor& Color) noexcept;*/

    @Override
    public void onSurfaceChanged(GL10 arg0, int width, int height) {
        m_fMaxWidth = width;		//  размер окна
        m_fMaxHeight = height;
        glViewport(0, 0, width, height);
        //
        Matrix.setIdentityM(m_ProjMatrix, 0);
        Matrix.orthoM( m_ProjMatrix, 0, 0.0f, width, 0.0f, height, -5.0f, 5.0f );
        m_DrawBuffer.SetProjectMatrix( m_ProjMatrix );
        //
        Init(width, height);
     }

    @Override
    public void onSurfaceCreated(GL10 arg0, EGLConfig arg1) {
        //
        int[] max = new int[1];
        arg0.glGetIntegerv(GL10.GL_MAX_TEXTURE_SIZE, max, 0);
        //
        glClearColor(0f, 0f, 0f, 1f);
        m_Shader = new ShaderUtils();
        m_iVertShaderID = m_Shader.createShader( m_Context, GL_VERTEX_SHADER , R.raw.vertex_shader );
        m_iFragShaderID = m_Shader.createShader( m_Context, GL_FRAGMENT_SHADER , R.raw.fragment_shader );
        m_iProgramId = m_Shader.createProgram( m_iVertShaderID, m_iFragShaderID );

        //glUseProgram(m_iProgramId);
        m_DrawBuffer.SetShaderProgrammId(m_iProgramId);
       // bindData();
        TextureManager.m_Instance = new TextureManager(m_Context);
        m_Font = EngSimpleFont.GetInstance();
        m_DigFont = DigitFont.GetInstance();
    }

    /*private void bindData() {
        m_iuColorLocation = glGetUniformLocation( m_iProgramId, "u_Color");
        glUniform4f(m_iuColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
        m_iaPositionLocation = glGetAttribLocation( m_iProgramId, "a_Position");
        vertexData.position(0);
        glVertexAttribPointer(m_iaPositionLocation, 2, GL_FLOAT, false, 0, vertexData);
        glEnableVertexAttribArray(m_iaPositionLocation);
    }*/

    /*void DrawBuffer( OGLDrawBuffer Buffer )
    {
        if (Buffer.VertexSize() == 0)
            return;
        glUseProgram(m_iProgramId);
        int err = glGetError();
        // Load the vertex position
        glVertexAttribPointer(m_iVertShAttr, Buffer.VertexDimension, GL_FLOAT, false, 0, FloatBuffer.wrap( Buffer.GetVertexData(), 0, Buffer.VertexSize()));
        // Load the texture coordinate
        glVertexAttribPointer(m_iTexCoordLoc, Buffer.TextureDimension, GL_FLOAT, false, 0, FloatBuffer.wrap( Buffer.GetTextureData(), 0, Buffer.TextureSize() ));
        // Load the texture coordinate
        glVertexAttribPointer(m_iColorLoc, Buffer.ColorDimension, GL_FLOAT, false, 0, FloatBuffer.wrap( Buffer.GetColorData(), 0, Buffer.ColorSize() ));

        glEnableVertexAttribArray(m_iVertShAttr);
        glEnableVertexAttribArray(m_iTexCoordLoc);
        glEnableVertexAttribArray(m_iColorLoc);

        // Bind the texture
        //glActiveTexture(GL_TEXTURE0);
        //Texture->Bind();

        // Set the texture sampler to texture unit to 0
       // glUniform1i(m_iSamplerLoc, 0);

        // Draw quad with nearest sampling
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
        glDrawArrays(GL_TRIANGLES, 0, Buffer.Count());


        glDisableVertexAttribArray(m_iVertShAttr);
        glDisableVertexAttribArray(m_iTexCoordLoc);
        glDisableVertexAttribArray(m_iColorLoc);

        glDeleteProgram(m_iProgramId);

        Buffer.Clear();
    }
    */


    @Override
    public void onDrawFrame(GL10 arg0)   {
        if( m_iCadrCount == 0 )
            m_StartTime = System.currentTimeMillis();
        m_iCadrCount++;
        if( m_iCadrCount == 10 ) {
            m_StopTime = System.currentTimeMillis();
            m_iCadrCount = 0;
            m_dFPS = 10000.0f/(m_StopTime - m_StartTime);
            m_iScore += 1;
       }
        //
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        //
        float[] vertex =
                { 10.0f, 10.0f, -4.0f,		            m_fMaxWidth - 10.0f, 10.0f, -4.0f,
                  10.0f, m_fMaxHeight - 10.0f, -4.0f,   m_fMaxWidth - 10.0f, m_fMaxHeight - 10.0f, -4.0f };
        try {

            //m_DrawBuffer.DrawElements(GL_TRIANGLES);
            //glEnable(GL_TEXTURE_2D);
            m_DrawBuffer.AddVertex( vertex, vertex.length );
            m_DrawBuffer.AddTexture( m_fTex, m_fTex.length);
            m_DrawBuffer.AddIndex( m_iIndex, m_iIndex.length );
            m_DrawBuffer.SetColor( new OGLColor( 1.0f, 1.0f, 1.0f, 1.0f  ));
            m_DrawBuffer.DrawBuffer(TextureManager.GetInstance().GetTexture(TextureManager.EType.Background), GL_TRIANGLES);
            //
            m_Font.SetSize(30, 45);
            m_Font.SetColor( 1.0f, 0.5f, 1.0f, 1.0f );
            m_Font.TextDraw("GRID   CRAZY", 20, m_fMaxHeight - 54, 0.0f, m_DrawBuffer);

            m_Font.SetColor( 1.0f, 1.0f, 0.0f, 1.0f );
            m_Font.SetSize(20, 30);

            m_Font.TextDraw("Level: ", 20, m_fMaxHeight - 90, 0.0f, m_DrawBuffer);
            m_Font.TextDraw("Stage: ", 20, m_fMaxHeight - 125, 0.0f, m_DrawBuffer);
            m_Font.TextDraw("Score: ", 450, m_fMaxHeight - 103, 0.0f, m_DrawBuffer);
            String sFPS = "FPS : ";
            m_Font.TextDraw(sFPS, 20, m_fMaxHeight - 170, 0.0f, m_DrawBuffer);

            /*if (m_ePlayState == EPlayState::Gameover) {
                m_Font.SetColor({ (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, (rand() % 255) / 255.0f, 0.80f });
                m_Font.TextDraw(L"GAME OVER", 480, m_fMaxHeight - 40, 0.0f, m_DrawBuffer);
                m_Font.TextDraw(L"PRESS SPACE", 465, m_fMaxHeight - 60, 0.0f, m_DrawBuffer);
            }*/
            m_DrawBuffer.DrawBuffer( m_Font.GetTexture(), GL_TRIANGLES);
            //
            m_DigFont.SetSize(20, 30);
            m_DigFont.TextDraw(Integer.toString((int)m_dFPS), 80, m_fMaxHeight - 170, 0.0f, m_DrawBuffer);

            m_DigFont.TextDraw(String.format("%7.4f  %7.4f  %7.4f",m_fGravityData[0],  m_fGravityData[1], m_fGravityData[2]), 20, m_fMaxHeight - 200, 0.0f, m_DrawBuffer);
            String gravOut = Float.toString(m_fGravityData[0]) + "  " + Float.toString(m_fGravityData[1]) + "  " +
                    Float.toString(m_fGravityData[2]);

            m_DrawBuffer.DrawBuffer( m_DigFont.GetTexture(), GL_TRIANGLES);
            //
            //
            for ( GameLive lv : m_vLives)
                lv.Draw(m_DrawBuffer);
            m_DrawBuffer.DrawBuffer( TextureManager.GetInstance().GetTexture(m_vLives[0].GetTexture() ), GL_TRIANGLES );
            //glLineWidth(20);
            //m_DrawBuffer.DrawBuffer(TextureManager.GetInstance().GetTexture(TextureManager.EType.None), GL_LINES);
            //glDisable(GL_TEXTURE_2D);
            //
            GridLine grLine;
            for (int ix = 0; ix < m_iHorSize; ix++) {            // Loop From Left To Right
                for (int iy = 0; iy < m_iVertSize; iy++) {      // Loop From Top To Bottom
                    if (ix < m_vHorLine.size() && iy < m_vHorLine.get(ix).size()) {  // Dont Draw To Far Right
                        grLine = m_vHorLine.get(ix).get(iy);
                        if ( grLine.GetCheck()) {                 // Has The Horizontal Line Been Traced
                            grLine.SetColor(1.0f, 1.0f, 1.0f, 0.25f);
                        }
					    else {            // If A Horizontal Line Isn't Filled
                            m_bFilled = false;
                            grLine.SetColor(0.0f, 0.5f, 1.0f, 0.25f);
                        }
                        //
                        grLine.Draw(m_DrawBuffer);
                    }

                    if (ix < m_vVertLine.size() && iy < m_vVertLine.get(ix).size()) {        // Dont Draw To Far Down

                        grLine = m_vVertLine.get(ix).get(iy);
                        if ( grLine.GetCheck()) {                 // Has The Horizontal Line Been Traced
                            grLine.SetColor(1.0f, 1.0f, 1.0f, 0.25f);
                        }
                        else {            // If A Horizontal Line Isn't Filled
                            m_bFilled = false;
                            grLine.SetColor(0.0f, 0.5f, 1.0f, 0.25f);
                        }

                        grLine.Draw(m_DrawBuffer);
                    }
                }
            }
            //
            glLineWidth(8);
            m_DrawBuffer.DrawBuffer(TextureManager.GetInstance().GetTexture(TextureManager.EType.None), GL_LINES);
            //
            m_Score.SetNumber(m_iScore);
            m_Score.Draw(m_DrawBuffer);
            m_Stage.SetNumber(m_iStage);
            m_Level.SetNumber(m_iLevel2);
            m_Stage.Draw(m_DrawBuffer);
            m_Level.Draw(m_DrawBuffer);

            //m_fps.SetNumber((int)m_dFPS);
            //m_fps.Draw(m_DrawBuffer);

            m_DrawBuffer.LoadIdentity();
            m_DrawBuffer.DrawBuffer(TextureManager.GetInstance().GetTexture(m_Stage.GetTexture()), GL_TRIANGLES);

            m_player.Draw(m_DrawBuffer);
            m_DrawBuffer.DrawBuffer(TextureManager.GetInstance().GetTexture(m_player.GetTexture()), GL_TRIANGLES);

        } catch(  OGLBuffer.BufferExeption E ) {}

        Move();
    }

    public void ActivatePrize(int X, int Y, boolean Horizontal)   // помечаем линию ( горизонтальную если Horizontal = true ) с коорд. X,Y как пройденную
    {
        //	часы лежат на линиях ( а не в прямоугольниках )
        //CheckPrize(X, Y);
        if (Horizontal) {
            if (m_vHorLine.get(X).get(Y).GetCheck() == false)
                m_iScore += 1;

            boolean horCondBefor1 = Y < m_iVertSize - 1 && m_vHorLine.get(X).get(Y).GetCheck() && m_vHorLine.get(X).get(Y+1).GetCheck() &&
                    m_vVertLine.get(X).get(Y).GetCheck() && m_vVertLine.get(X+1).get(Y).GetCheck();
            boolean horCondBefor2 = Y > 0 && m_vHorLine.get(X).get(Y).GetCheck() && m_vHorLine.get(X).get(Y-1).GetCheck() &&
                    m_vVertLine.get(X).get(Y-1).GetCheck() && m_vVertLine.get(X+1).get(Y-1).GetCheck();

            m_vHorLine.get(X).get(Y).SetCheck( true );

            if (Y < m_iVertSize - 1 && m_vHorLine.get(X).get(Y).GetCheck() && m_vHorLine.get(X).get(Y+1).GetCheck() &&
                    m_vVertLine.get(X).get(Y).GetCheck() && m_vVertLine.get(X+1).get(Y).GetCheck())
                if(!horCondBefor1)
                    m_iScore += 10;
            if (Y > 0 && m_vHorLine.get(X).get(Y).GetCheck() && m_vHorLine.get(X).get(Y-1).GetCheck() &&
                    m_vVertLine.get(X).get(Y-1).GetCheck() && m_vVertLine.get(X+1).get(Y-1).GetCheck() )
                if (!horCondBefor2)
                    m_iScore += 10;
        }
        if (!Horizontal) {
            if (m_vVertLine.get(X).get(Y).GetCheck() == false)
                m_iScore += 1;

            boolean vertCondBefor1 = X < m_iHorSize - 1 && m_vHorLine.get(X).get(Y).GetCheck() && m_vHorLine.get(X).get(Y+1).GetCheck() &&
                    m_vVertLine.get(X).get(Y).GetCheck() && m_vVertLine.get(X+1).get(Y).GetCheck();
            boolean vertCondBefor2 = X > 0 && m_vHorLine.get(X-1).get(Y).GetCheck() && m_vHorLine.get(X-1).get(Y+1).GetCheck() &&
                    m_vVertLine.get(X).get(Y).GetCheck() && m_vVertLine.get(X-1).get(Y).GetCheck();

            m_vVertLine.get(X).get(Y).SetCheck(true);
            if (X < m_iHorSize - 1 && m_vHorLine.get(X).get(Y).GetCheck() && m_vHorLine.get(X).get(Y+1).GetCheck() &&
                    m_vVertLine.get(X).get(Y).GetCheck() && m_vVertLine.get(X+1).get(Y).GetCheck())
                if(!vertCondBefor1)
                    m_iScore += 10;
            if (X > 0 && m_vHorLine.get(X-1).get(Y).GetCheck() && m_vHorLine.get(X-1).get(Y+1).GetCheck() &&
                    m_vVertLine.get(X).get(Y).GetCheck() && m_vVertLine.get(X-1).get(Y).GetCheck())
                if (!vertCondBefor2)
                    m_iScore += 10;
        }
        return;

    }
/*#endif
    GLvoid		ResizeScene(GLsizei width, GLsizei height) noexcept ;
    void		ActivatePrize( int X, int Y, bool Horizontal ) noexcept;  // помечаем линию ( горизонтальную если Horizontal = true ) с коорд. X,Y как пройденную
    // и проверяем не нада ли открыть прямоугольник и нет ли там приза
    //	 IMouseEvent
    void	onMouseMove(int X, int Y)  noexcept;
    void	onLMouseDown(int X, int Y)  noexcept;
    void	onLMouseUp(int X, int Y)  noexcept;
    //   IButtonEvent
    void	onButtonDown(const COGLButton* Control)  noexcept;
    void	onButtonUp(const COGLButton* Control)  noexcept;
    //   IControlEvent
    void	onLButtonDown(const COGLControl* Control)  noexcept;
    void	onLButtonUp(const COGLControl* Control)  noexcept;
    void	onMouseMove(const COGLControl* Control)  noexcept;
    void	onMouseLeave(const COGLControl* Control)  noexcept;*/

}
