#include "AVYUVAdaptor.h"


int AVYUVAdaptor::AVPixelFormatToYUVChroma(PixelFormat pix_fmt) throw (AVException*)
{
    switch (pix_fmt)
	{
		case PIX_FMT_YUV420P: return Y4M_CHROMA_420MPEG2; break;
		case PIX_FMT_YUVJ422P:
		case PIX_FMT_YUV422P: return Y4M_CHROMA_422; break;
		case PIX_FMT_YUVJ444P:
		case PIX_FMT_YUV444P: return Y4M_CHROMA_444; break;
		case PIX_FMT_YUV411P: return Y4M_CHROMA_411; break;
		case PIX_FMT_YUVJ420P: return Y4M_CHROMA_420JPEG; break;
		default:
            
            
			throw new AVException("unsupported y4m chroma: " + std::string(av_get_pix_fmt_name(pix_fmt)),UNSUPPORTED_CHROMA);
			break;
	}
}

// don't like the look of this allocation
const y4m_ratio_t AVYUVAdaptor::AVRationalToY4mRatio (AVRational rational)
{
	y4m_ratio_t fr;
	
	fr.d = rational.den;
	fr.n = rational.num;
    
    return fr; // explodes when stack is freed?
}

void AVYUVAdaptor::copyAVFrameToY4MFrame (uint8_t ** frameData, AVFrame *pFrame)
{
    
    int y;
    int cw;
    int w;
    int h;
    int ch;
    
    // get these values
    
    for (y=0; y<h; y++) {
		
		memcpy(frameData[0]+y*w,(pFrame->data[0])+y*pFrame->linesize[0],w);
		if (y<ch) {
			memcpy(frameData[1]+y*cw,(pFrame->data[1])+y*pFrame->linesize[1],cw);
			memcpy(frameData[2]+y*cw,(pFrame->data[2])+y*pFrame->linesize[2],cw);
		}
	}
}

int AVYUVAdaptor::interlaceAndOrderToY4M (int i, int tff)
{
	
	if (i) {
		if (tff) {
			return(Y4M_ILACE_TOP_FIRST);
		} else {
			return(Y4M_ILACE_BOTTOM_FIRST);
		}
	} else {
		return(Y4M_ILACE_NONE);
	}
}
