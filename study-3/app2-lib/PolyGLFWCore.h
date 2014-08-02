#pragma once

#include <GLFW/glfw3.h>
#include "PolyString.h"
#include "PolyGlobals.h"
#include "PolyLogger.h"
#include "PolyCore.h"
#include "PolyString.h"
#include "PolyRectangle.h"
#include <vector>
#include <iostream>
#include "PolyGLRenderer.h"
#include <mach/mach_time.h>
#include <unistd.h>
#include <pthread.h>
#include "PolyInputEvent.h"
#include "PolyGLSLShaderModule.h"

using namespace Polycode;

using std::vector;

class _PolyExport PosixMutex : public CoreMutex {
public:
	pthread_mutex_t pMutex;
};

class VideoModeChangeInfo {
	public:
	int xRes;
	int yRes;
	bool fullScreen;
	bool vSync;
	int aaLevel;
	int anisotropyLevel;
	bool needResolutionChange;
};

class _PolyExport GLFWCore : public Core {		
public:
	
	GLFWCore(String title, int xRes=640, int yRes=480, bool fullScreen=false, bool vSync=false, int aaLevel=0, int anisotropyLevel=0, int frameRate=40, int monitorIndex=-1, bool retinaSupport=false);
	virtual ~GLFWCore();
	
	void enableMouse(bool newval);
	unsigned int getTicks();		
	bool Update();
	
	void Render();
							
	void setVideoMode(int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel);		
	void resizeTo(int xRes, int yRes);
	void createThread(Threaded *target);		
	
	void createFolder(const String& folderPath);
	void copyDiskItem(const String& itemPath, const String& destItemPath);
	void moveDiskItem(const String& itemPath, const String& destItemPath);
	void removeDiskItem(const String& itemPath);
	String openFolderPicker();
	vector<String> openFilePicker(vector<CoreFileExtension> extensions, bool allowMultiple);
	
	String executeExternalCommand(String command, String args, String inDirectory="");
	
	void launchApplicationWithFile(String application, String file);
	void openFileWithApplication(String file, String application);
	
	void setCursor(int cursorType);
	void warpCursor(int x, int y);
	
	void openURL(String url);
	
	void copyStringToClipboard(const String& str);
	String getClipboardString();
	
	void makeApplicationMain();
	
	void lockMutex(CoreMutex *mutex);
	void unlockMutex(CoreMutex *mutex);
	CoreMutex *createMutex();
	
	void checkEvents();
	
	int lastMouseY;
	int lastMouseX;
	
	bool checkSpecialKeyEvents(PolyKEY key);

    Number getBackingXRes();
    Number getBackingYRes();
    				
protected:	

	GLFWwindow* window;
	
	void _setVideoMode(int xRes, int yRes, bool fullScreen, bool vSync, int aaLevel, int anisotropyLevel);

	uint64_t initTime;
    bool retinaSupport;
	
	VideoModeChangeInfo modeChangeInfo;
};













///////////////////

// GLFWwindowposfun 	glfwSetWindowPosCallback (GLFWwindow *window, GLFWwindowposfun cbfun)
//  	Sets the position callback for the specified window. More...
 
// GLFWwindowsizefun 	glfwSetWindowSizeCallback (GLFWwindow *window, GLFWwindowsizefun cbfun)
//  	Sets the size callback for the specified window. More...
 
// GLFWwindowclosefun 	glfwSetWindowCloseCallback (GLFWwindow *window, GLFWwindowclosefun cbfun)
//  	Sets the close callback for the specified window. More...
 
// GLFWwindowrefreshfun 	glfwSetWindowRefreshCallback (GLFWwindow *window, GLFWwindowrefreshfun cbfun)
//  	Sets the refresh callback for the specified window. More...
 
// GLFWwindowfocusfun 	glfwSetWindowFocusCallback (GLFWwindow *window, GLFWwindowfocusfun cbfun)
//  	Sets the focus callback for the specified window. More...
 
// GLFWwindowiconifyfun 	glfwSetWindowIconifyCallback (GLFWwindow *window, GLFWwindowiconifyfun cbfun)
//  	Sets the iconify callback for the specified window. More...
 
// GLFWframebuffersizefun 	glfwSetFramebufferSizeCallback (GLFWwindow *window, GLFWframebuffersizefun cbfun)
//  	Sets the framebuffer resize callback for the specified window. More...
