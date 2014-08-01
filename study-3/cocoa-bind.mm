#include "cocoa-bind.h"

#include <iostream>
#import <Cocoa/Cocoa.h>

#include "PolycodeExampleAppDelegate.h"

NSAutoreleasePool * pool = nullptr;
PolycodeExampleAppDelegate *appDelegate = nullptr;

CocoaApp::CocoaApp()
{

	pool = [[NSAutoreleasePool alloc] init];
	[NSApplication sharedApplication];

	std::cout << "created pool and nsapplication" << std::endl;


	//----------------
	// id menubar = [[NSMenu new] autorelease];
 //    id appMenuItem = [[NSMenuItem new] autorelease];
 //    [menubar addItem:appMenuItem];
 //    [NSApp setMainMenu:menubar];
 //    id appMenu = [[NSMenu new] autorelease];
 //    id appName = [[NSProcessInfo processInfo] processName];
 //    id quitTitle = [@"Quit " stringByAppendingString:appName];
 //    id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
 //        action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
 //    [appMenu addItem:quitMenuItem];
 //    [appMenuItem setSubmenu:appMenu];
	//--------------

	appDelegate = [[PolycodeExampleAppDelegate alloc] init];
	std::cout << "created delegate" << std::endl;

	[NSApp setDelegate:appDelegate];
	std::cout << "set the delegate" << std::endl;

	[NSApp finishLaunching];
	std::cout << "finished launching in cocoa-bind" << std::endl;
	

}

CocoaApp::~CocoaApp()
{
	[pool release];	
}

void CocoaApp::update()
{
	std::cout << "started update in cocoa-bind" << std::endl;
	NSEvent *event =
            [NSApp
                nextEventMatchingMask:NSAnyEventMask
                untilDate:[NSDate distantFuture]
                inMode:NSDefaultRunLoopMode
                dequeue:YES];

    std::cout << "sending event in cocoa-bind" << std::endl;
 
    [NSApp sendEvent:event];

    std::cout << "updating windows in cocoa-bind" << std::endl;
    [NSApp updateWindows];

    std::cout << "updating app delegate in cocoa-bind" << std::endl;
	[appDelegate update];
}

void CocoaApp::run()
{
	std::cout << "running nsapp in cocoa-bind" << std::endl;
	[NSApp run];
}

PolycodeApp* CocoaApp::getPolycode()
{
	if(appDelegate) return [appDelegate getPolycode];
	std::cout << "got null polycode in cocoa-bind" << std::endl;
	return nullptr;
}

app* app_create()
{
	std::cout << "creating app" << std::endl;
	return reinterpret_cast<app*>(new CocoaApp());
}

void app_destroy(app* a)
{
	delete reinterpret_cast<CocoaApp*>(a);
}

void app_update(app* a)
{
	reinterpret_cast<CocoaApp*>(a)->update();
}

polycode* app_get_polycode(app* a)
{
	return reinterpret_cast<polycode*>(reinterpret_cast<CocoaApp*>(a)->getPolycode());
}
