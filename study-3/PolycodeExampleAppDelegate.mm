/*
 Copyright (C) 2011 by Ivan Safrin
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

#import "PolycodeExampleAppDelegate.h"

#include <iostream>

@implementation PolycodeExampleAppDelegate

@synthesize window;
@synthesize mainView;

- (id)init {
    if (self = [super init]) {
    	frame = NSMakeRect(0, 0, 640, 480);
        window  = [[[NSWindow alloc] initWithContentRect:frame
		                styleMask:NSBorderlessWindowMask
		                backing:NSBackingStoreBuffered
		                defer:NO] autorelease];
        [window center];
		[window setBackgroundColor:[NSColor redColor]];
		[window makeKeyAndOrderFront:NSApp];

		std::cout << "created window" << std::endl;

		NSOpenGLPixelFormatAttribute attrs[] =
		{
		    NSOpenGLPFADoubleBuffer,
		    NSOpenGLPFADepthSize, 32,
		    0
		};
		 
		NSOpenGLPixelFormat* pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];

		std::cout << "created pixel format" << std::endl;

		mainView = [[PolycodeView alloc] init];
		[mainView initWithFrame:frame pixelFormat:pixFmt];

		std::cout << "initialised polycode" << std::endl;

		[[window contentView] addSubview: mainView];

		app = new HelloPolycodeApp(mainView);
    }
    return self;
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	std::cout << "application finished launching" << std::endl;
		
	timer = [NSTimer timerWithTimeInterval:(1.0f/60.0f)
		target:self
		selector:@selector(animationTimer:)
		userInfo:nil
		repeats:YES];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSDefaultRunLoopMode];
	[[NSRunLoop currentRunLoop] addTimer:timer forMode:NSEventTrackingRunLoopMode];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (void)animationTimer:(NSTimer *)timer
{
	if(!app->Update()) {
		[[NSApplication sharedApplication] stop:self];
	}
}

- (void)update 
{
	app->Update();
}

@end
