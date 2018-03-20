package gen.endorasoft.com;

import android.app.Activity;
import android.app.NativeActivity;
import android.os.Bundle;
import android.view.View;
import android.util.DisplayMetrics;

import android.graphics.SurfaceTexture;
import android.view.Surface;
import android.view.SurfaceView;
import android.view.SurfaceHolder;
import android.view.ViewGroup.LayoutParams;

import android.graphics.Point;

public class GENNativeActivity extends NativeActivity
{
	// it's probably a good idea to load OpenAL in here
	// as that allows us to avoid having a custom version
	static {
	// try to load and capture exception to avoid loading error
	  try
	  {
	    System.loadLibrary("MGD");
	  }
	  catch (UnsatisfiedLinkError e)
	  {
	  }
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

		//getWindow().takeSurface(null);
		//mainsurface = new SurfaceView(this);
		//mainsurface.getHolder().addCallback(this);
		//setContentView(mainsurface);

		GetDPI();
    }

	@Override
	public void onResume()
	{
		super.onResume();

		View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN
									  | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
									  | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
									  | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
									  | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
									  | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);

		// NOT DO, FORCES APPLICATION RESTART ON ORIENTATION CHANGE
		//Point size = new Point();
		//getWindowManager().getDefaultDisplay().getSize(size);
		//LayoutParams params = new LayoutParams(size.x, (int)(size.y/2.0f));
		//mainsurface.setLayoutParams(params);
		//mainsurface.getHolder().setFixedSize(size.x, (int)(size.y/2.0f));
	}

	//@Override
	//public void surfaceCreated (SurfaceHolder holder)
	//{
	//	super.surfaceCreated(holder);
	//	mainsurface = holder.getSurface();
	//}

	public void GetDPI() {
		DisplayMetrics metrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(metrics);
		dpi = metrics.densityDpi;
	}

	// For Video Test
	public static void CreateSurfaceTexture(int gltexture) {
		surfacetexture = new SurfaceTexture(gltexture);
		surface = new Surface(surfacetexture);
	}

	public static void UpdateSurfaceTexture() {
		surfacetexture.updateTexImage();
	}

	public static Surface GetSurfaceTextureSurface() {
		return surface;
	}

	public static int dpi;
	public static SurfaceTexture surfacetexture;
	public static Surface surface;
	public static SurfaceView mainsurface;
}
