#include "PolyGLFWCore.h"

void glfwErrorCallback(int code, const char* message)
{
	std::cout << "------glfw error " << code << ": " << message << std::endl;
}

void glErrorCallback(GLenum source,
					 GLenum type,
					 GLuint id,
					 GLenum severity,
					 GLsizei length,
					 const GLchar* message,
					 void* userParam)
{
	std::cout << "------gl error " << id << ": " << message << std::endl;
}

GLFWCore::GLFWCore(String title, int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel, int frameRate, int monitorIndex, bool retinaSupport)
	: Core(xRes, yRes, fullScreen, vSync, aaLevel, anisotropyLevel, frameRate, monitorIndex)
{
	glfwSetErrorCallback(glfwErrorCallback);
    if (!glfwInit()) throw std::runtime_error("failed to init glfw");
    window = glfwCreateWindow(xRes, yRes, title.getSTLString().c_str(), NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("failed to create glfw window");
    }
    glfwMakeContextCurrent(window);

    initTime = mach_absolute_time();

   	renderer = new OpenGLRenderer();
	services->setRenderer(renderer);
	
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	renderer->Resize(width, height);

	renderer->Init();
    //glfwSetKeyCallback(window, key_callback);
    //glDebugMessageCallback(glErrorCallback, NULL);

    CoreServices::getInstance()->installModule(new GLSLShaderModule());	
}

GLFWCore::~GLFWCore()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void GLFWCore::enableMouse(bool newval)
{

}

unsigned int GLFWCore::getTicks()
{
	uint64_t time = mach_absolute_time();	
	double conversion = 0.0;
	
	mach_timebase_info_data_t info;
	mach_timebase_info( &info );
	conversion = 1e-9 * (double) info.numer / (double) info.denom;	
	
	return (((double)(time - initTime)) * conversion) * 1000.0f;
}

bool GLFWCore::Update()
{
	if(!running) return false;
	doSleep();
	
	if(modeChangeInfo.needResolutionChange) {
		_setVideoMode(modeChangeInfo.xRes, modeChangeInfo.yRes, modeChangeInfo.fullScreen, modeChangeInfo.vSync, modeChangeInfo.aaLevel, modeChangeInfo.anisotropyLevel);
		modeChangeInfo.needResolutionChange = false;
	}
							
	updateCore();
	checkEvents();
	return running;
}

void GLFWCore::Render()
{
	lockMutex(CoreServices::getRenderMutex());
	
	if(!paused) {	
		renderer->BeginRender();
	}

	services->Render();

	if(!paused) {		
		renderer->EndRender();
		glfwSwapBuffers(window);
	}
	
	unlockMutex(CoreServices::getRenderMutex());
}

void GLFWCore::setVideoMode(int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel)
{
	std::cout << "set video mode called" << std::endl;
}

void GLFWCore::resizeTo(int xRes, int yRes)
{
	std::cout << "resize called" << std::endl;
}

void *ManagedThreadFuncGLFW(void *data) {
	Threaded *target = static_cast<Threaded*>(data);
	target->runThread();
	target->scheduledForRemoval = true;
	return NULL;
}

void GLFWCore::createThread(Threaded *target)
{
	Core::createThread(target);
	pthread_t thread;
	pthread_create( &thread, NULL, ManagedThreadFuncGLFW, (void*)target);
}		

void GLFWCore::createFolder(const String& folderPath)
{
	std::cout << "called createFolder with" << folderPath.getSTLString() << std::endl;
}

void GLFWCore::copyDiskItem(const String& itemPath, const String& destItemPath)
{
	std::cout << "called copyDiskItem with" << itemPath.getSTLString() << " and " << destItemPath.getSTLString() << std::endl;
}

void GLFWCore::moveDiskItem(const String& itemPath, const String& destItemPath)
{
	std::cout << "called moveDiskItem with" << itemPath.getSTLString() << " and " << destItemPath.getSTLString() << std::endl;
}

void GLFWCore::removeDiskItem(const String& itemPath)
{
	std::cout << "called removeDiskItem with" << itemPath.getSTLString() << std::endl;
}

String GLFWCore::openFolderPicker()
{
	std::cout << "called openFolderPicker" << std::endl;
	return "";
}

vector<String> GLFWCore::openFilePicker(vector<CoreFileExtension> extensions, bool allowMultiple)
{
	std::cout << "called openFilePicker" << std::endl;
	return vector<String>();
}

String GLFWCore::executeExternalCommand(String command, String args, String inDirectory)
{
	std::cout << "called executeExternalCommand" << command.getSTLString() << std::endl;
	return "";
}

void GLFWCore::launchApplicationWithFile(String application, String file)
{
	std::cout << "launch app with file called" << std::endl;
}

void GLFWCore::openFileWithApplication(String file, String application)
{
	std::cout << "open file with app called" << std::endl;
}

void GLFWCore::setCursor(int cursorType)
{
	std::cout << "set cursor called" << std::endl;
}

void GLFWCore::warpCursor(int x, int y)
{
	std::cout << "warp cursor called" << std::endl;
}

void GLFWCore::openURL(String url)
{
	std::cout << "open url called" << std::endl;
}

void GLFWCore::copyStringToClipboard(const String& str)
{
	std::cout << "copy string to clipboard called" << std::endl;
}

String GLFWCore::getClipboardString()
{
	std::cout << "get clipboard string called" << std::endl;
	return "";
}

void GLFWCore::makeApplicationMain()
{
	std::cout << "make application main called" << std::endl;
}

void GLFWCore::lockMutex(CoreMutex *mutex)
{
	// std::cout << "----------locking mutex" << std::endl;
	PosixMutex *m = (PosixMutex*) mutex;
	pthread_mutex_lock(&m->pMutex);
}

void GLFWCore::unlockMutex(CoreMutex *mutex)
{
	// std::cout << "----------unlock mutex" << std::endl;
	PosixMutex *m = (PosixMutex*) mutex;
	pthread_mutex_unlock(&m->pMutex);
}

CoreMutex* GLFWCore::createMutex()
{
	std::cout << "----------created mutex" << std::endl;
	PosixMutex *mutex = new PosixMutex();	
	pthread_mutex_init(&mutex->pMutex, NULL);
	return mutex;
}		

void GLFWCore::checkEvents()
{
	glfwPollEvents();
	// std::cout << "check events called" << std::endl;
}		

bool GLFWCore::checkSpecialKeyEvents(PolyKEY key)
{
	return false;
}		

Number GLFWCore::getBackingXRes()
{
	return getXRes();
}

Number GLFWCore::getBackingYRes()
{
	return getYRes();
}

void GLFWCore::_setVideoMode(int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel)
{
	std::cout << "called _setVideoMode" << std::endl;
}


