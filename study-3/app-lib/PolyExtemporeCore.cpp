#include "PolyExtemporeCore.h"

ExtemporeCore::ExtemporeCore(String title, int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel, int frameRate, int monitorIndex, bool retinaSupport)
	: Core(xRes, yRes, fullScreen, vSync, aaLevel, anisotropyLevel, frameRate, monitorIndex)
{
    initTime = mach_absolute_time();

   	renderer = new OpenGLRenderer();
	services->setRenderer(renderer);
	
	renderer->Resize(xRes, yRes);
	renderer->Init();

    CoreServices::getInstance()->installModule(new GLSLShaderModule());	
}

ExtemporeCore::~ExtemporeCore()
{
}

void ExtemporeCore::enableMouse(bool newval)
{

}

unsigned int ExtemporeCore::getTicks()
{
	uint64_t time = mach_absolute_time();	
	double conversion = 0.0;
	
	mach_timebase_info_data_t info;
	mach_timebase_info( &info );
	conversion = 1e-9 * (double) info.numer / (double) info.denom;	
	
	return (((double)(time - initTime)) * conversion) * 1000.0f;
}

bool ExtemporeCore::Update()
{
	if(!running) return false;
	doSleep();
	
	if(modeChangeInfo.needResolutionChange) {
		setVideoMode(modeChangeInfo.xRes, modeChangeInfo.yRes, modeChangeInfo.fullScreen, modeChangeInfo.vSync, modeChangeInfo.aaLevel, modeChangeInfo.anisotropyLevel);
		modeChangeInfo.needResolutionChange = false;
	}
	updateCore();
	checkEvents();
	return running;
}

void ExtemporeCore::Render()
{
	lockMutex(CoreServices::getRenderMutex());
	if(!paused) {	
		renderer->BeginRender();
	}
	services->Render();
	if(!paused) {		
		renderer->EndRender();
	}
	
	unlockMutex(CoreServices::getRenderMutex());
}

void ExtemporeCore::setVideoMode(int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel, bool retinaSupport)
{
	std::cout << "set video mode called" << std::endl;
}

void ExtemporeCore::resizeTo(int xRes, int yRes)
{
	std::cout << "resize called" << std::endl;
}

void *ManagedThreadFuncExtempore(void *data) {
	Threaded *target = static_cast<Threaded*>(data);
	target->runThread();
	target->scheduledForRemoval = true;
	return NULL;
}

void ExtemporeCore::createThread(Threaded *target)
{
	Core::createThread(target);
	pthread_t thread;
	pthread_create( &thread, NULL, ManagedThreadFuncExtempore, (void*)target);
}		

void ExtemporeCore::createFolder(const String& folderPath)
{
	std::cout << "called createFolder with" << folderPath.getSTLString() << std::endl;
}

void ExtemporeCore::copyDiskItem(const String& itemPath, const String& destItemPath)
{
	std::cout << "called copyDiskItem with" << itemPath.getSTLString() << " and " << destItemPath.getSTLString() << std::endl;
}

void ExtemporeCore::moveDiskItem(const String& itemPath, const String& destItemPath)
{
	std::cout << "called moveDiskItem with" << itemPath.getSTLString() << " and " << destItemPath.getSTLString() << std::endl;
}

void ExtemporeCore::removeDiskItem(const String& itemPath)
{
	std::cout << "called removeDiskItem with" << itemPath.getSTLString() << std::endl;
}

String ExtemporeCore::openFolderPicker()
{
	std::cout << "called openFolderPicker" << std::endl;
	return "";
}

vector<String> ExtemporeCore::openFilePicker(vector<CoreFileExtension> extensions, bool allowMultiple)
{
	std::cout << "called openFilePicker" << std::endl;
	return vector<String>();
}

String ExtemporeCore::executeExternalCommand(String command, String args, String inDirectory)
{
	std::cout << "called executeExternalCommand" << command.getSTLString() << std::endl;
	return "";
}

void ExtemporeCore::launchApplicationWithFile(String application, String file)
{
	std::cout << "launch app with file called" << std::endl;
}

void ExtemporeCore::openFileWithApplication(String file, String application)
{
	std::cout << "open file with app called" << std::endl;
}

void ExtemporeCore::setCursor(int cursorType)
{
	std::cout << "set cursor called" << std::endl;
}

void ExtemporeCore::warpCursor(int x, int y)
{
	std::cout << "warp cursor called" << std::endl;
}

void ExtemporeCore::openURL(String url)
{
	std::cout << "open url called" << std::endl;
}

void ExtemporeCore::copyStringToClipboard(const String& str)
{
	std::cout << "copy string to clipboard called" << std::endl;
}

String ExtemporeCore::getClipboardString()
{
	std::cout << "get clipboard string called" << std::endl;
	return "";
}

void ExtemporeCore::makeApplicationMain()
{
	std::cout << "make application main called" << std::endl;
}

void ExtemporeCore::lockMutex(CoreMutex *mutex)
{
	// std::cout << "----------locking mutex" << std::endl;
	PosixMutex *m = (PosixMutex*) mutex;
	pthread_mutex_lock(&m->pMutex);
}

void ExtemporeCore::unlockMutex(CoreMutex *mutex)
{
	// std::cout << "----------unlock mutex" << std::endl;
	PosixMutex *m = (PosixMutex*) mutex;
	pthread_mutex_unlock(&m->pMutex);
}

CoreMutex* ExtemporeCore::createMutex()
{
	std::cout << "----------created mutex" << std::endl;
	PosixMutex *mutex = new PosixMutex();	
	pthread_mutex_init(&mutex->pMutex, NULL);
	return mutex;
}		

void ExtemporeCore::checkEvents()
{
}		

bool ExtemporeCore::checkSpecialKeyEvents(PolyKEY key)
{
	return false;
}		

Number ExtemporeCore::getBackingXRes()
{
	return getXRes();
}

Number ExtemporeCore::getBackingYRes()
{
	return getYRes();
}

bool ExtemporeCore::systemUpdate()
{
	if(!running) return false;
	doSleep();
	
	// if(modeChangeInfo.needResolutionChange) {
	// 	_setVideoMode(modeChangeInfo.xRes, modeChangeInfo.yRes, modeChangeInfo.fullScreen, modeChangeInfo.vSync, modeChangeInfo.aaLevel, modeChangeInfo.anisotropyLevel);
	// 	modeChangeInfo.needResolutionChange = false;
	// }
							
	updateCore();
	checkEvents();
	return running;
}

// void ExtemporeCore::setVideoMode(int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel, bool retinaSupport)
// {
// 	std::cout << "called _setVideoMode" << std::endl;
// }


