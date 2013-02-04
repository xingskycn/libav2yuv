#include "DissolveTransition.h"

DissolveTransition::DissolveTransition(AVObject *f, AVObject *t, unsigned int fr)
{
	this->setFrames(fr);
	this->setAVFrom(f);
	this->setAVTo(t);
	
	frameCounter = 0;
	
	this->allocFrame();
}

DissolveTransition::~DissolveTransition()
{

	delete fromAV;
	delete toAV;
	
}


void DissolveTransition::setAVFrom (AVObject *s)
{
	fromAV = s;
	
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

void DissolveTransition::setAVTo (AVObject *s)
{
	toAV = s;
}	

void DissolveTransition::setFrames(unsigned int f)
{
	frames = f;
}


int DissolveTransition::decodeNextFrame(void)
{
	
	AVFrame *pFrameFrom;
	AVFrame *pFrameTo;
	int ret;
	double percent;
	
	int h = this->getHeight();
	int w = this->getWidth();
	int ch = this->getChromaHeight();
	int cw = this->getChromaWidth();
	
	
	ret = fromAV->decodeNextFrame();
	if (ret<0)
		return ret;
	ret = toAV->decodeNextFrame();
	if (ret<0)
		return ret;
	
	pFrameFrom = fromAV->getAVFrame();
	pFrameTo = toAV->getAVFrame();

	percent = 1.0 * frameCounter / frames;
		
	// be a good idea to optimise this.
	for (int y=0; y<h; y++) {

		for (int x=0; x<w; x++) {
		
			*(pFrame->data[0]+x+y*pFrame->linesize[0]) = percent * *(pFrameTo->data[0]+x+y*pFrameTo->linesize[0]) + (1.0-percent) * *(pFrameFrom->data[0]+x+y*pFrameFrom->linesize[0]);
			
			if (y<ch && x<cw) {
				*(pFrame->data[1]+x+y*pFrame->linesize[1]) = percent * *(pFrameTo->data[1]+x+y*pFrameTo->linesize[1]) + (1.0-percent) * *(pFrameFrom->data[1]+x+y*pFrameFrom->linesize[1]);
				*(pFrame->data[2]+x+y*pFrame->linesize[2]) = percent * *(pFrameTo->data[2]+x+y*pFrameTo->linesize[2]) + (1.0-percent) * *(pFrameFrom->data[2]+x+y*pFrameFrom->linesize[2]);
			}
		}
	}
	
	frameCounter++;
	
	return this->getHeight()*pFrame->linesize[0] +
	this->getHeight()*pFrame->linesize[1] +
	this->getHeight()*pFrame->linesize[2];
	
}



void DissolveTransition::dumpFormat(void)
{
	std::cerr << "Cross Dissolve Transition\nFROM:\n";
	fromAV->dumpFormat();
	std::cerr << "TO: \n" ;
	toAV->dumpFormat();
}
