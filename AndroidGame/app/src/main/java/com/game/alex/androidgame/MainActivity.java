package com.game.alex.androidgame;

import android.support.v7.app.AppCompatActivity;
import android.app.ActivityManager;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.os.Bundle;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Display;
import android.widget.Toast;
import android.graphics.Point;

import android.view.Window;
import android.view.WindowManager;

import java.util.List;

import com.game.alex.androidgame.OGLUtils.TextureManager;
import com.game.alex.androidgame.Observers.TouchObservable;

public class MainActivity extends AppCompatActivity  implements OnTouchListener {

    private GLSurfaceView glSurfaceView;
    SensorManager sensorManager;
    List<Sensor> sensors;

   // int upPI = 0;
    //int downPI = 0;
    //boolean inTouch = false;
    @Override
    public boolean onTouch(View v, MotionEvent event) {
        TouchObservable.getInstance().onTouch(event);
 /*       // событие
        int actionMask = event.getActionMasked();
        // индекс касания
        int pointerIndex = event.getActionIndex();
        // число касаний
        int pointerCount = event.getPointerCount();

        switch (actionMask) {
            case MotionEvent.ACTION_DOWN: // первое касание
                inTouch = true;
            case MotionEvent.ACTION_POINTER_DOWN: // последующие касания
                downPI = pointerIndex;
                break;

            case MotionEvent.ACTION_UP: // прерывание последнего касания
                inTouch = false;
            case MotionEvent.ACTION_POINTER_UP: // прерывания касаний
                upPI = pointerIndex;
                break;

            case MotionEvent.ACTION_MOVE: // движение
                for (int i = 0; i < 10; i++) {

                    if (i < pointerCount) {
                    } else {
                    }

                }
                break;
        }

        if (inTouch) {
        }*/
        return true;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().hide();
        // remove title
        //requestWindowFeature(Window.FEATURE_NO_TITLE);
        //getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
        //        WindowManager.LayoutParams.FLAG_FULLSCREEN);
        //
        if (!supportES2()) {
            Toast.makeText(this, "OpenGl ES 2.0 is not supported", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
        glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(2);
        glSurfaceView.setRenderer(new OGLGame(this));
        setContentView(glSurfaceView);
        //TextureManager.m_Instance = new TextureManager(this);
        sensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        sensors = sensorManager.getSensorList(Sensor.TYPE_ALL);
    }
    @Override
    protected void onPause() {
        super.onPause();
        glSurfaceView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        glSurfaceView.onResume();
    }

    private boolean supportES2() {
        ActivityManager activityManager =
                (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();
        return (configurationInfo.reqGlEsVersion >= 0x20000);
    }
}
