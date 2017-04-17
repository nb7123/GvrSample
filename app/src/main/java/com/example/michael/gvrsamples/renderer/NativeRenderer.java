package com.example.michael.gvrsamples.renderer;

import android.content.res.AssetManager;

/**
 * Created by michael on 17-4-17.
 */

public class NativeRenderer {
    // activity life cycle method
    public native void nativeOnCreate(long gvr_ctx, AssetManager amg);
    public native void nativeOnResume();
    public native void nativeOnPause();
    public native void nativeShutdown();
    //

    // surface draw lifecycle method
    public native void nativeOnSurfaceCreated();
    public native void nativeOnDraw();
    //
}
