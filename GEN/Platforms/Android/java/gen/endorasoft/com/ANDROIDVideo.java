
package gen.endorasoft.com;

import gen.endorasoft.com.GENNativeActivity;

import android.media.MediaCodec;
import android.media.MediaFormat;
import android.util.Log;

import java.io.IOException;
import java.util.ArrayDeque;
import java.util.Queue;
import java.nio.ByteBuffer;




public class ANDROIDVideo
{
	public class CodecCallback extends MediaCodec.Callback
	{
		public CodecCallback()
		{
			
		}

		@Override
		public void onError(MediaCodec codec, MediaCodec.CodecException e)
		{
			Log.e("MediaCodec Callback", "Error");
		}

		@Override
		public void onInputBufferAvailable(MediaCodec codec, int index)
		{
			Log.e("MediaCodec Callback", "Input buffer available");
		}

		@Override
		public void onOutputBufferAvailable(MediaCodec codec, int index, MediaCodec.BufferInfo info)
		{
			Log.e("MediaCodec Callback", "Output buffer available");
		}

		@Override
		public void onOutputFormatChanged(MediaCodec codec, MediaFormat format)
		{
			Log.w("MediaCodec Callback", "Setting video scaling mode");
			//codec.setVideoScalingMode(MediaCodec.VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING);
		}

	}

	public ANDROIDVideo()
	{
		

	}

	// need to read and interpret SPS and PPS on C++ side
	public boolean IniCodec(byte[] SPS, byte[] PPS, String mime, int width, int height, int fps)// throws IOException
	{
		
		try 
		{
			codec = MediaCodec.createDecoderByType(mime);
		}
		catch(IOException e)
		{
			return false;
		}

		format = MediaFormat.createVideoFormat(mime, width, height);

		format.setInteger(MediaFormat.KEY_FRAME_RATE, fps);
		format.setInteger(MediaFormat.KEY_PUSH_BLANK_BUFFERS_ON_STOP, 1);

		ByteBuffer SPS_BB = ByteBuffer.wrap(SPS);
		ByteBuffer PPS_BB = ByteBuffer.wrap(PPS);

		format.setByteBuffer("csd-0", SPS_BB);
		format.setByteBuffer("csd-1", PPS_BB);

		int sizeSPS = SPS_BB.remaining();
		int sizePPS = PPS_BB.remaining();

		String sps_str = String.format("SPS size: %d", sizeSPS);
		String pps_str = String.format("PPS size: %d", sizePPS);

		Log.d("MediaCodec IniCodec", sps_str);
		Log.d("MediaCodec IniCodec", pps_str);

		LogBuffer(SPS);
		LogBuffer(PPS);

		// Log size of the buffers

		//cb = new ANDROIDVideo.CodecCallback();

		//codec.setCallback(cb);
		codec.configure(format, GENNativeActivity.mainsurface.getHolder().getSurface(), null, 0);
		
		codec.start();

		// get all the input and output buffers required for decoding
		mInputBuffers           = codec.getInputBuffers(); // if callback is set this fails because codec is configured in asyncronous mode
        mOutputBuffers          = codec.getOutputBuffers();
        mOutputBufferInfo       = new MediaCodec.BufferInfo[mOutputBuffers.length];
        mAvailableInputBuffers  = new ArrayDeque<Integer>(mOutputBuffers.length);
        mAvailableOutputBuffers = new ArrayDeque<Integer>(mInputBuffers.length);

		codec.setVideoScalingMode(MediaCodec.VIDEO_SCALING_MODE_SCALE_TO_FIT_WITH_CROPPING);

		Log.d("MediaCodec Decode", "Decode Called");

		return true;
	}

	public boolean SendFrame(byte[] frame, long timestamp, int flags, boolean render)
	{
		ByteBuffer frame_BB = ByteBuffer.wrap(frame);
		int size = frame_BB.remaining();

		String str = String.format("Frame Size: %d", size);
		Log.e("MediaCodec SendFrame", str);
		Log.e("MediaCodec SendFrame", "before checks");

		if (size > 0 &&  !mAvailableInputBuffers.isEmpty())
		{
            int index = mAvailableInputBuffers.remove();
            ByteBuffer buffer = mInputBuffers[index];
 
            // we can't write our sample to a lesser capacity input buffer.
            if (size > buffer.capacity())
			{
				return false;
            }
 
			Log.d("MediaCodec SendFrame", "queuing input buffer");

            buffer.clear();
            buffer.put(frame_BB);

			int mflags = 0;

			codec.queueInputBuffer(index, 0, size, timestamp, mflags);
		}

		// Render the decoded frame
		update();
		if (!mAvailableOutputBuffers.isEmpty())
		{
            int index = mAvailableOutputBuffers.remove();
 
			Log.d("MediaCodec SendFrame", "queuing output buffer?");

            // releases the buffer back to the codec
            codec.releaseOutputBuffer(index, render);
        }

		return true;
	}

	public void LogBuffer(byte[] buffer)
	{
		String str = "";

		for(byte b : buffer)
		{
			str = String.format("%s %02X", str, b);
		}

		str = String.format("NALU: %s", str);

		Log.w("MediaCodec Buffer", str);
	}

	public void update()
	{
		int index;
 
        // Get valid input buffers from the codec to fill later in the same order they were
        // made available by the codec.
        while ((index = codec.dequeueInputBuffer(0)) != MediaCodec.INFO_TRY_AGAIN_LATER)
		{
            mAvailableInputBuffers.add(index);
        }

		// need to do the same with the output buffers
		MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
        while ((index = codec.dequeueOutputBuffer(info, 0)) !=  MediaCodec.INFO_TRY_AGAIN_LATER)
		{
            switch (index)
			{
                case MediaCodec.INFO_OUTPUT_BUFFERS_CHANGED:
                    mOutputBuffers = codec.getOutputBuffers();
                    mOutputBufferInfo = new MediaCodec.BufferInfo[mOutputBuffers.length];
                    mAvailableOutputBuffers.clear();
                    break;
                //case MediaCodec.INFO_OUTPUT_FORMAT_CHANGED:
                //    if (mOutputFormatChangedListener != null)
				//	{
                //        mHandler.post(new Runnable()
				//		{
                //            @Override
                //            public void run()
				//			{
                //                mOutputFormatChangedListener
                //                        .outputFormatChanged(MediaCodecWrapper.this,
                //                                mDecoder.getOutputFormat());
 				//
                //            }
                //        });
                //    }
                //    break;
                default:
                    // Making sure the index is valid before adding to output buffers. We've already
                    // handled INFO_TRY_AGAIN_LATER, INFO_OUTPUT_FORMAT_CHANGED &
                    // INFO_OUTPUT_BUFFERS_CHANGED i.e all the other possible return codes but
                    // asserting index value anyways for future-proofing the code.
                    if(index >= 0)
					{
                        mOutputBufferInfo[index] = info;
                        mAvailableOutputBuffers.add(index);
                    } else
					{
                        throw new IllegalStateException("Unknown status from dequeueOutputBuffer");
                    }
                    break;
            }
 
        }
	}

	// internal handles to the codec and format for mediacodec
	private MediaCodec codec;
	private MediaFormat format;

	private ANDROIDVideo.CodecCallback cb;

	// References to the internal buffers managed by the codec. The codec
    // refers to these buffers by index, never by reference so it's up to us
    // to keep track of which buffer is which.
    private ByteBuffer[] mInputBuffers;
    private ByteBuffer[] mOutputBuffers;

	// Indices of the input buffers that are currently available for writing. We'll
    // consume these in the order they were dequeued from the codec.
    private Queue<Integer> mAvailableInputBuffers;
 
    // Indices of the output buffers that currently hold valid data, in the order
    // they were produced by the codec.
    private Queue<Integer> mAvailableOutputBuffers;
 
    // Information about each output buffer, by index. Each entry in this array
    // is valid if and only if its index is currently contained in mAvailableOutputBuffers.
    private MediaCodec.BufferInfo[] mOutputBufferInfo;
};

