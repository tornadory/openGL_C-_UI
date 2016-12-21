package com.example.cheng.testndk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private SurfaceView mSurfaceView;
    private long _native;

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
}
