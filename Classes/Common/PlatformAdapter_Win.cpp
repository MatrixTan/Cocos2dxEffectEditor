#include "Common/PlatformAdapter.h"

NS_EE_BEGIN


bool PlatformAdapter::isOtherAudioPlaying(void) 
{ 
	return false; 
}
void PlatformAdapter::stopOtherAudio(void)
{
}
void PlatformAdapter::resumeOtherAudio(void)
{
}
std::string PlatformAdapter::getFilePath(const std::string& types)
{ 
	return ""; 
}
std::string PlatformAdapter::getSaveFilePath(const std::string& type)
{
	return ""; 
}

NS_EE_END