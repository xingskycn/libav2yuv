#include "ChromaFilter.h"

ChromaFilter::ChromaFilter(AVObject *s) 
{
	imgConvertCtx = NULL;
	this->setAVSource (s);
}


ChromaFilter::ChromaFilter(AVObject *s, int convertMode) throw (AVException*)
{
	this->setAVSource (s);
	this->setY4MChroma(convertMode);
}

ChromaFilter::~ChromaFilter()
{
	if (imgConvertCtx)
		sws_freeContext(imgConvertCtx);
	
	delete source;
	
	// delete pictureBuffer;
	
	// does this call the superclass's destructor?
	// yes 
}


void ChromaFilter::setAVSource (AVObject *s)
{
	source = s;
	
	this->setFrameRate(s->getFrameRate());
	this->setSampleAspect(s->getSampleAspect());
	this->setHeight(s->getHeight());
	this->setWidth(s->getWidth());
	
	// we don't get interlace information until the first frame is decoded.
	
	/*
	this->setInterlaced(s->getIsInterlaced());
	this->setInterlaceTopFieldFirst(s->getInterlaceTopFieldFirst());
	*/
}	

void ChromaFilter::setAVChroma(PixelFormat convertMode) throw (AVException*)
{

   // std::cerr << "setAVChroma\n";

	this->setChromaSampling(convertMode);
	this->allocFrame();

	imgConvertCtx = sws_getContext(this->getWidth(), this->getHeight(), source->getChromaSampling(), 
								   this->getWidth(), this->getHeight(), convertMode, SWS_BICUBIC, NULL, NULL, NULL); 

}

void ChromaFilter::setY4MChroma(int convertMode) throw (AVException*)
{
    
   // std::cerr << "setY4MChroma\n";
	this->setChromaSamplingFromY4M(convertMode);
	this->setAVChroma(this->getChromaSampling());	
}

int ChromaFilter::decodeNextFrame(void)
{

	AVFrame *pFrameSource;
	
	int ret = source->decodeNextFrame();
	if (ret<0)
		return ret;
	pFrameSource = source->getAVFrame();
	
	return sws_scale(imgConvertCtx,  (const uint8_t * const *)((AVPicture *)pFrameSource)->data,
					 pFrameSource->linesize, 0, this->getHeight(),pFrame->data, pFrame->linesize);
	
}

void ChromaFilter::dumpFormat(void)
{
	std::cerr << "chroma conversion filter: " << this->getChromaSamplingName() << "\n";
    source->dumpFormat();

}
