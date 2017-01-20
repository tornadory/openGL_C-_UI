package com.example.cheng.testndk;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private SurfaceView mSurfaceView;
    private long _native;
    private int statusBar;

    private ArrayList<Integer> points=new ArrayList<Integer>();

    private void addPoint(int pointId)
    {
        points.add(pointId);
    }

    private int removePoint(Integer pointId)
    {
        points.remove( pointId);

        return points.size();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {

        switch (event.getActionMasked())
        {
            case MotionEvent.ACTION_DOWN:

                int pointId=event.getPointerId(event.getActionIndex());
                float x=event.getX(pointId);
                float y=event.getY(pointId);

                addPoint(pointId);
                onPointerDown(pointId,x,y-statusBar,_native);

                break;

            case MotionEvent.ACTION_POINTER_DOWN:

                int pointId2=event.getPointerId(event.getActionIndex());
                float x2=event.getX(pointId2);
                float y2=event.getY(pointId2);

                addPoint(pointId2);
                onPointerDown(pointId2,x2,y2-statusBar,_native);

                break;

            case MotionEvent.ACTION_UP:

                int pointId3=event.getPointerId(event.getActionIndex());

                removePoint(pointId3);
                onPointerUp(pointId3, event.getX(event.getActionIndex()), event.getY(event.getActionIndex())-statusBar, _native);

                break;

            case MotionEvent.ACTION_POINTER_UP:

                int pointId4=event.getPointerId(event.getActionIndex());

                removePoint(pointId4);
                onPointerUp(pointId4, event.getX(event.getActionIndex()), event.getY(event.getActionIndex())-statusBar, _native);

                break;

            case MotionEvent.ACTION_MOVE:


               for(int i=0;i<points.size();i++)
               {

                    int actionIndex=event.findPointerIndex(points.get(i));
                    float x_now=event.getX(actionIndex);
                    float y_now=event.getY(actionIndex);

                    onPointerMoved(points.get(i),x_now,y_now-statusBar,_native);
                }

                break;
        }

        return super.onTouchEvent(event);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//        // Example of a call to a native method
//        TextView tv = (TextView) findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());

        _native=createNativeContext();


        mSurfaceView=new SurfaceView(this);

        mSurfaceView.getHolder().addCallback(new SurfaceHolder.Callback()
        {
            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

                changeSurface(width,height,holder.getSurface(),_native);
            }

            @Override
            public void surfaceCreated(SurfaceHolder holder) {

            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {

                destorySurface(_native);

            }
        });

        new Thread()
        {
            @Override
            public void run() {
                super.run();

                renderThread(_native);
            }
        }.start();
        setContentView(mSurfaceView);



        int resourceId=getResources().getIdentifier("status_bar_height","dimen","android");
        if(resourceId>0)
        {
            statusBar=getResources().getDimensionPixelSize(resourceId);
        }
    }


    @Override
    protected void onResume() {
        super.onResume();

        onResumeActivity(_native);
    }

    @Override
    protected void onPause() {
        super.onPause();

        onPauseActivity(_native);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */

    public native String stringFromJNI();
    public native long createNativeContext();
    public native void changeSurface(int i_width, int i_height, Surface i_surface,long _native);
    public native void renderThread(long _native);
    public native void onResumeActivity(long _native);
    public native void onPauseActivity(long _native);
    public native void destorySurface(long _native);

    public native void onPointerDown(int i_point_id,float i_x, float i_y,long _native);
    public native void onPointerUp(int i_point_id, float i_x, float i_y, long _native);
    public native void onPointerMoved(int i_point_id, float i_x, float i_y, long _native);
}
