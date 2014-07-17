//#pragma once

//#import <Cocoa/Cocoa.h>

#ifdef __cplusplus

class CocoaApp
{
public:
	CocoaApp();
	~CocoaApp();

	void run();
	void update();

};





extern "C" {

#endif

	struct app;

	struct app* app_create();
	void app_destroy(struct app* a);
	void app_update(struct app* a);

#ifdef __cplusplus
}

#endif


// int main(int argc, char* argv[])
// {
// 	app* a = reinterpret_cast<app*>(new CocoaApp());

// 	//delete reinterpret_cast<CocoaApp*>(a);


// 	// CocoaApp();
// 	return 0;
// }


