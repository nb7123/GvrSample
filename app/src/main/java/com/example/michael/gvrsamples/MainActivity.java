package com.example.michael.gvrsamples;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.example.michael.gvrsamples.renderer.NativeRenderer;
import com.google.vr.ndk.base.AndroidCompat;
import com.google.vr.ndk.base.GvrLayout;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainActivity extends Activity {
    private static final String TAG = MainActivity.class.getSimpleName();

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("sample-jni");
    }

    private GvrLayout gvrLayout;
    private GLSurfaceView surfaceView;
    private NativeRenderer renderer;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Ensure fullscreen immersion.
        setImmersiveSticky();
        getWindow()
                .getDecorView()
                .setOnSystemUiVisibilityChangeListener(
                        new View.OnSystemUiVisibilityChangeListener() {
                            @Override
                            public void onSystemUiVisibilityChange(int visibility) {
                                if ((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0) {
                                    setImmersiveSticky();
                                }
                            }
                        });

        // create GvrLayout
        gvrLayout = new GvrLayout(this);
        // create surface view
        surfaceView = new GLSurfaceView(this);
        // init surface view
        initSurfaceView(surfaceView);
        // set surface view as gvr layout present view
        gvrLayout.setPresentationView(surfaceView);
        // Enable async reprojection for low-latency rendering on supporting
        // devices. Note that this must be set prior to calling initialize_gl()
        // on the native gvr_context.
        gvrLayout.setAsyncReprojectionEnabled(true);

        // Install the GvrLayout into the View hierarchy.
        setContentView(gvrLayout);

        // Set up the necessary VR settings.
        AndroidCompat.setVrModeEnabled(this, true);

        // native api
        renderer = new NativeRenderer();
        renderer.nativeOnCreate(gvrLayout.getGvrApi().getNativeGvrContext(), getAssets());
    }

    @Override
    protected void onResume() {
        super.onResume();
        gvrLayout.onResume();
        surfaceView.onResume();
        renderer.nativeOnResume();
    }

    @Override
    protected void onPause() {
        renderer.nativeOnPause();
        surfaceView.onPause();
        gvrLayout.onPause();

        super.onPause();
    }

    @Override
    protected void onDestroy() {
        renderer.nativeShutdown();
        gvrLayout.shutdown();
        super.onDestroy();
    }

    private void setImmersiveSticky() {
        getWindow()
                .getDecorView()
                .setSystemUiVisibility(
                        View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                                | View.SYSTEM_UI_FLAG_FULLSCREEN
                                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY);
    }

    private void initSurfaceView(GLSurfaceView surfaceView) {
        surfaceView.setEGLContextClientVersion(2);
        surfaceView.setEGLConfigChooser(8, 8, 8, 0, 16, 0);

        surfaceView.setRenderer(new GLSurfaceView.Renderer() {
            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                Log.d(TAG, "On surface created");
                renderer.nativeOnSurfaceCreated();
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int width, int height) {
                Log.d(TAG, "On surface changed");
            }

            @Override
            public void onDrawFrame(GL10 gl) {
                renderer.nativeOnDraw();
            }
        });
    }
}
