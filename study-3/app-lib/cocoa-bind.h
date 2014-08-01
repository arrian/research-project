//#pragma once

//#import <Cocoa/Cocoa.h>


#ifdef __cplusplus

class PolycodeApp;

class CocoaApp
{
public:
	CocoaApp();
	~CocoaApp();

	void run();
	void update();

	PolycodeApp* getPolycode();
};

extern "C" {

#endif

	struct app;
	struct polycode;

	struct app* app_create();
	void app_destroy(struct app* a);
	void app_update(struct app* a);
	struct polycode* app_get_polycode(struct app* a);


#ifdef __cplusplus
}

#endif


