
/*------------------------------------------------------------------------------------------
//	VIDEOCODECH264.CPP
//	
//	p`latform independent part of H264 codec
//   
//	Author						: Imanol Celaya Ruiz de Alegria
//	Date Of Creation	: 11/04/2016 12:44:24
//	Last Modification	:	
//	
//	GEN  Copyright (C).  All right reserved.
//----------------------------------------------------------------------------------------*/
	
	
/*---- INCLUDES --------------------------------------------------------------------------*/
	
#include "VIDEOCodecH264.h"
#include "MEDIAH264FrameReader.h"
//#include "VIDEOH264FrameReaderLibAV.h"
#include "XBufferBits.h"

#include "XDebug.h"

/*---- GENERAL VARIABLE ------------------------------------------------------------------*/
	
	
/*---- CLASS MEMBERS ---------------------------------------------------------------------*/





/*-------------------------------------------------------------------
//	VIDEOCODECH264::VIDEOCODECH264
*/	
/**	
//	
//	Class Constructor VIDEOCODECH264
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 13:48:56
//	
*/
/*-----------------------------------------------------------------*/
VIDEOCODECH264::VIDEOCODECH264(VIDEOPLAYER* videoplayer) : VIDEOCODEC(videoplayer)
{
}





/*-------------------------------------------------------------------
//	VIDEOCODECH264::~VIDEOCODECH264
*/	
/**	
//	
//	 Class Destructor VIDEOCODECH264
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 13:48:50
//	
*/
/*-----------------------------------------------------------------*/
VIDEOCODECH264::~VIDEOCODECH264()
{
}





/*-------------------------------------------------------------------
//	VIDEOCODECH264::SetFile
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 13:48:44
//	
//	@return 			bool : 
//
//  @param				filename : 
*/
/*-----------------------------------------------------------------*/
bool VIDEOCODECH264::SetFile(XCHAR* filename)
{
	//framereader = new VIDEOH264FRAMEREADERLIBAV();
	if(!framereader) return false;

	framereader->SetVideoEnabled(true);

	if(!framereader->OpenFile(filename))
		{
			XDEBUG_PRINTCOLOR(4, __L("Can't Open File %s"), filename);
			delete framereader;
			framereader = NULL;
			return false;
		}

	////XDEBUG_PRINTCOLOR(4, __L("Reading SPS"));
	//framereader->ReadFrame(SPS);
	//////XDEBUG_PRINTDATABLOCK(4, SPS.GetPointer(0), SPS.GetSize());
	////XDEBUG_PRINTCOLOR(4, __L("Reading PPS"), filename);
	//framereader->ReadFrame(PPS);
	//////XDEBUG_PRINTDATABLOCK(4, PPS.GetPointer(0), PPS.GetSize());
	//
	//XVECTOR<XBYTE> RBSP_SPS;
	//RBSP_SPS.Resize(SPS.GetSize());
	//
	//XVECTOR<XBYTE> NAL;
	//NAL.Resize(SPS.GetSize() - 4);
	//
	//memcpy(NAL.GetPointer(0), SPS.GetPointer(4), NAL.GetSize()*sizeof(XBYTE));
	//

	//XVECTOR<XBYTE> NAL;
	//NAL.Resize(framereader->GetSPS()->GetSize() - 4);
	//
	//memcpy(NAL.GetPointer(0), framereader->GetSPS()->GetPointer(4), NAL.GetSize()*sizeof(XBYTE));
	//
	//XVECTOR<XBYTE> RBSP_SPS;
	//RBSP_SPS.Resize(NAL.GetSize());
	//framereader->NALToRBSP(NAL, RBSP_SPS);
	////
	//////FillInfo(SPS);
	//
	//FillInfo(RBSP_SPS);
	//
	//width = framereader->width;
	//height = framereader->height;

	return true;
}



MEDIAFRAMEREADER* VIDEOCODECH264::GetFrameReader()
{
	return framereader;
}



/*-------------------------------------------------------------------
//	VIDEOCODECH264::FillInfo
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 13:48:36
//	
//	@return 			bool : 
//
//  @param				frame : 
*/
/*-----------------------------------------------------------------*/
bool VIDEOCODECH264::FillInfo(XVECTOR<XBYTE> & frame)
{
	// hardcoded to HD for now
	width		= 1920;
	height	= 1080;

	// copy over the NAL unit without the NAL header
	XBUFFER xbuffer;
	xbuffer.Resize(frame.GetSize());
	memcpy(xbuffer.Get(), frame.GetPointer(0), (frame.GetSize())*sizeof(XBYTE));
	XBUFFERBITS bits(&xbuffer);

	XDWORD profile = bits.ReadU(8);
	// ignoring constraints sets
	//bits.ReadU(1);
	//bits.ReadU(1);
	//bits.ReadU(1);
	//bits.ReadU(1);
	//bits.ReadU(1);
	//bits.ReadU(1);
	//// this needs to be zero
	//XDWORD reserved = bits.ReadU(2);

	bits.ReadU(8);

	// this crazyness comes from section 7.3.2.1 in the H264 standard
	XDWORD level = bits.ReadU(8);
	XDWORD seq_parameter_set_id = bits.ReadUE();

	if((profile == 100) || (profile == 110) || (profile == 122) || (profile == 244) || (profile ==	44) || (profile ==	83)
	|| (profile ==	86) || (profile == 118) || (profile == 128) || (profile == 138) || (profile == 139) || (profile == 134))
		{
			XDWORD chroma_format_idc = bits.ReadUE();
			if(chroma_format_idc == 3)
				{
					XDWORD separate_colour_plane_flag = bits.ReadU(1);
				}

			XDWORD bit_depth_luma_minus8		= bits.ReadUE();
			XDWORD bit_depth_chroma_minus8	= bits.ReadUE();

			XDWORD qpprime_y_zero_transform_bypass_flag = bits.ReadU(1);
			XDWORD seq_scaling_matrix_present_flag			= bits.ReadU(1);

			if(seq_scaling_matrix_present_flag)
				{
					for(int i = 0; i <((chroma_format_idc != 3) ? 8 : 12); i++)
						{
							bits.ReadU(1);
							if(i < 6)
								{
								}
							else
								{
								}
						}
				}
		}

	XDWORD log2_max_frame_num_minus4	= bits.ReadUE();
	XDWORD pic_order_cnt_type					= bits.ReadUE();
	if(pic_order_cnt_type == 0)
		{
			XDWORD log2_max_pic_order_cut_lsb_minus4 = bits.ReadUE();
		}
	else if(pic_order_cnt_type == 1)
		{
			XDWORD delta_pic_order_always_zero_flag				= bits.ReadU(1);
			XDWORD offset_for_non_ref_pic									= bits.ReadSE();
			XDWORD offset_for_top_to_bottom_field					= bits.ReadSE();
			XDWORD num_ref_frames_in_pic_order_cnt_cycle	= bits.ReadUE();
			for(XDWORD i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++)
				{
					bits.ReadSE();
				}
		}

	XDWORD max_num_ref_frames											= bits.ReadUE();
	XDWORD gaps_in_frame_num_value_allowed_flag		= bits.ReadU(1);
	XDWORD pic_width_in_mbs_minus1								= bits.ReadUE();
	XDWORD pic_height_in_map_units_minus1					= bits.ReadUE();
	XDWORD frame_mbs_only_flag										= bits.ReadU(1);
	if(frame_mbs_only_flag)
		{
			XDWORD adaptive_frame_field_flag = bits.ReadU(1);
		}

	XDWORD direct_8x8_inference_flag	= bits.ReadU(1);
	XDWORD frame_cropping_flag				= bits.ReadU(1);

	XDWORD frame_crop_left_offset		= 0;
	XDWORD frame_crop_right_offset	= 0;
	XDWORD frame_crop_top_offset		= 0;
	XDWORD frame_crop_bottom_offset	= 0;

	if(frame_cropping_flag)
		{
			frame_crop_left_offset		= bits.ReadUE();
			frame_crop_right_offset		= bits.ReadUE();
			frame_crop_top_offset			= bits.ReadUE();
			frame_crop_bottom_offset	= bits.ReadUE();
		}

	// compute picture width and height
	//width		= ((pic_width_in_mbs_minus1 +1)*16) - frame_crop_right_offset*2 - frame_crop_left_offset*2;
	width = 1920;
	height	= ((2 - frame_mbs_only_flag)* (pic_height_in_map_units_minus1 +1) * 16) - (frame_crop_top_offset * 2) - (frame_crop_bottom_offset * 2);

	XDWORD vui_parameters_present_flag = bits.ReadU(1);

	fps = 25;

	if(vui_parameters_present_flag)
		{
			ReadVUI(bits);
		}

	return true;
}



/*-------------------------------------------------------------------
//	VIDEOCODECH264::ReadVUI
*/	
/**	
//	
//	
//	
//	@author				Imanol Celaya Ruiz de Alegria
//	@version			11/04/2016 13:48:26
//	
//  @param				bits : 
*/
/*-----------------------------------------------------------------*/
void VIDEOCODECH264::ReadVUI(XBUFFERBITS& bits)
{
	// From Annex E E.1.1 in the H264 Standard
	XDWORD aspect_ratio_info_present_flag = bits.ReadU(1);
	if(aspect_ratio_info_present_flag)
		{
			XDWORD aspect_ratio_idc = bits.ReadU(8);
			// need to check what Extended_SAR is in spec
			//if(aspect_ratio_idc == Extended_SAR)
			//{
			//	XDWORD sar_width = bits.ReadU(16);
			//	XDWORD sar_height = bits.ReadU(16);
			//}
		}

	XDWORD overscan_info_present_flag = bits.ReadU(1);
	if(overscan_info_present_flag)
		{
			XDWORD overscan_appropiate_flag = bits.ReadU(1);
		}

	XDWORD video_signal_type_present_flag = bits.ReadU(1);
	if(video_signal_type_present_flag)
		{
			XDWORD video_format											= bits.ReadU(3);
			XDWORD video_full_range_flag						= bits.ReadU(1);
			XDWORD colour_description_present_flag	= bits.ReadU(1);
			if(colour_description_present_flag)
				{
					XDWORD colour_primaries					= bits.ReadU(8);
					XDWORD transfer_characteristics = bits.ReadU(8);
					XDWORD matrix_coefficients			= bits.ReadU(8);
				}
		}

	XDWORD chroma_loc_info_present_flag = bits.ReadU(1);
	if(chroma_loc_info_present_flag)
		{
			XDWORD chroma_sample_loc_type_top_field			= bits.ReadUE();
			XDWORD chroma_sample_loc_type_bottom_field	= bits.ReadUE();
		}

	XDWORD timing_info_present_flag = bits.ReadU(1);
	if(timing_info_present_flag)
		{
			XDWORD num_units_in_tick			= bits.ReadU(32);
			XDWORD time_scale							= bits.ReadU(32);
			XDWORD fixed_frame_rate_flag	= bits.ReadU(1);
			fps = ((float)num_units_in_tick)/((float)time_scale);
			fps /= 2.0f;
			XDWORD fps_ = (XDWORD)fps;
			XDEBUG_PRINTCOLOR(4, __L("FPS: %d"), fps_);
		}
}

