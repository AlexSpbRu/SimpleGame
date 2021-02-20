package com.game.alex.androidgame.Observers;

import java.util.Observable;
import android.view.MotionEvent;

/**
 * Created by kag on 13.03.2018.
 */

public class TouchObservable extends Observable {
    public class CTouchData {
        float       m_fX;
        float       m_fY;
        int         m_iactionMask = MotionEvent.ACTION_POINTER_UP;
    }
    //
    protected CTouchData[]      m_TouchData = new CTouchData[10];
    protected int       lastPI = 0;
    protected boolean   inTouch = false;
    //
    private volatile static TouchObservable  m_Instance;
    private TouchObservable() {};
    public static TouchObservable   getInstance() {
        if( m_Instance == null ) {
            synchronized (TouchObservable.class) {
                if( m_Instance == null )
                    m_Instance = new TouchObservable();
            }
        }
        return m_Instance;
    }
    public void onTouch( MotionEvent Event ) {
        // событие
        int actionMask = Event.getActionMasked();
        // индекс касания
        lastPI = Event.getActionIndex();
        // число касаний
        int pointerCount = Event.getPointerCount();
        int pInd;

        switch (actionMask) {
            case MotionEvent.ACTION_DOWN: // первое касание
                inTouch = true;
            case MotionEvent.ACTION_POINTER_DOWN: // последующие касания
                break;

            case MotionEvent.ACTION_UP: // прерывание последнего касания
                inTouch = false;
            case MotionEvent.ACTION_POINTER_UP: // прерывания касаний
              break;

            case MotionEvent.ACTION_MOVE: // движение
                for (int i = 0; i < pointerCount; i++) {
                    pInd = Event.getPointerId(i);
                    m_TouchData[pInd].m_fX = Event.getX(i);
                    m_TouchData[pInd].m_fY = Event.getY(i);
                    m_TouchData[pInd].m_iactionMask = actionMask;
                }
                break;
        }
        //
        m_TouchData[lastPI].m_iactionMask = actionMask;
        m_TouchData[lastPI].m_fX = Event.getX(lastPI);
        m_TouchData[lastPI].m_fY = Event.getY(lastPI);
        //
        if (inTouch) {
        }
    }
}
