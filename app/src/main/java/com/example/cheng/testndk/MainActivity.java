package com.example.cheng.testndk;

import android.net.Uri;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.MotionEvent;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private SurfaceView mSurfaceView;
    private long _native;
    private int statusBar;

    private ArrayList<Integer> points = new ArrayList<Integer>();
    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;

    private void addPoint(int pointId) {
        points.add(pointId);
    }

    private int removePoint(Integer pointId) {
        points.remove(pointId);

        return points.size();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {

//        switch (event.getActionMasked()) {
//            case MotionEvent.ACTION_DOWN:
//
//                int pointId = event.getPointerId(event.getActionIndex());
//                float x = event.getX(pointId);
//                float y = event.getY(pointId);
//
//                addPoint(pointId);
//
//                setPoint(pointId, x, y, _native);
//                onPointerDown(pointId, _native);
//
//                break;
//
//            case MotionEvent.ACTION_POINTER_DOWN:
//
//                int pointId2 = event.getPointerId(event.getActionIndex());
//                float x2 = event.getX(pointId2);
//                float y2 = event.getY(pointId2);
//
//                addPoint(pointId2);
//
//                setPoint(pointId2, x2, y2, _native);
//                onPointerDown(pointId2, _native);
//
//                break;
//
//            case MotionEvent.ACTION_UP:
//
//                int pointId3 = event.getPointerId(event.getActionIndex());
//
//                removePoint(pointId3);
//                onPointerUp(pointId3, _native);
//
//                break;
//
//            case MotionEvent.ACTION_POINTER_UP:
//
//                int pointId4 = event.getPointerId(event.getActionIndex());
//
//                removePoint(pointId4);
//                onPointerUp(pointId4, _native);
//
//                break;
//
//            case MotionEvent.ACTION_MOVE:
//
//
//                for (int i = 0; i < points.size(); i++) {
//
//                    int actionIndex = event.findPointerIndex(points.get(i));
//                    float x_now = event.getX(actionIndex);
//                    float y_now = event.getY(actionIndex);
//
//                    setPoint(points.get(i), x_now, y_now, _native);
//
//                }
//                onPointerMoved(_native);
//
//                break;
//        }
//
        return super.onTouchEvent(event);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

//        // Example of a call to a native method
//        TextView tv = (TextView) findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());

        _native = createNativeContext();


        mSurfaceView = new SurfaceView(this);

        mSurfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {

                changeSurface(width, height, holder.getSurface(), _native);
            }

            @Override
            public void surfaceCreated(SurfaceHolder holder) {

            }

            @Override
            public void surfaceDestroyed(SurfaceHolder holder) {

                destorySurface(_native);

            }
        });

        new Thread() {
            @Override
            public void run() {
                super.run();

                renderThread(_native);
            }
        }.start();


        mSurfaceView.setOnTouchListener(new View.OnTouchListener() {

            @Override
            public boolean onTouch(View v, MotionEvent event) {

                switch (event.getActionMasked()) {
                    case MotionEvent.ACTION_DOWN:

                        int pointId = event.getPointerId(event.getActionIndex());
                        float x = event.getX(pointId);
                        float y = event.getY(pointId);

                        addPoint(pointId);

                        setPoint(pointId, x, y, _native);
                        onPointerDown(pointId, _native);

                        break;

                    case MotionEvent.ACTION_POINTER_DOWN:

                        int pointId2 = event.getPointerId(event.getActionIndex());
                        float x2 = event.getX(pointId2);
                        float y2 = event.getY(pointId2);

                        addPoint(pointId2);

                        setPoint(pointId2, x2, y2, _native);
                        onPointerDown(pointId2, _native);

                        break;

                    case MotionEvent.ACTION_UP:

                        int pointId3 = event.getPointerId(event.getActionIndex());

                        removePoint(pointId3);
                        onPointerUp(pointId3, _native);

                        break;

                    case MotionEvent.ACTION_POINTER_UP:

                        int pointId4 = event.getPointerId(event.getActionIndex());

                        removePoint(pointId4);
                        onPointerUp(pointId4, _native);

                        break;

                    case MotionEvent.ACTION_MOVE:


                        for (int i = 0; i < points.size(); i++) {

                            int actionIndex = event.findPointerIndex(points.get(i));
                            float x_now = event.getX(actionIndex);
                            float y_now = event.getY(actionIndex);

                            setPoint(points.get(i), x_now, y_now, _native);

                        }
                        onPointerMoved(_native);

                        break;
                }

                return true;
            }
        });
        setContentView(mSurfaceView);


        int resourceId = getResources().getIdentifier("status_bar_height", "dimen", "android");
        if (resourceId > 0) {
            statusBar = getResources().getDimensionPixelSize(resourceId);
        }
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
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

    public native void changeSurface(int i_width, int i_height, Surface i_surface, long _native);

    public native void renderThread(long _native);

    public native void onResumeActivity(long _native);

    public native void onPauseActivity(long _native);

    public native void destorySurface(long _native);

    public native void onPointerDown(int i_point_id, long _native);

    public native void onPointerUp(int i_point_id, long _native);

    public native void onPointerMoved(long _native);

    public native void setPoint(int i_point_id, float i_x, float i_y, long _native);

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    public Action getIndexApiAction() {
        Thing object = new Thing.Builder()
                .setName("Main Page") // TODO: Define a title for the content shown.
                // TODO: Make sure this auto-generated URL is correct.
                .setUrl(Uri.parse("http://[ENTER-YOUR-URL-HERE]"))
                .build();
        return new Action.Builder(Action.TYPE_VIEW)
                .setObject(object)
                .setActionStatus(Action.STATUS_TYPE_COMPLETED)
                .build();
    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        AppIndex.AppIndexApi.start(client, getIndexApiAction());
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        AppIndex.AppIndexApi.end(client, getIndexApiAction());
        client.disconnect();
    }
}
