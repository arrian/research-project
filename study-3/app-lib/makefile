

all:
	clean
	app-lib

app-lib:
	g++ cocoa-bind.mm PolycodeExampleAppDelegate.mm PolycodeView.mm PolycodeApp.mm -o app.dylib -dynamiclib -I /Users/arrian/Dev/polycode/Core/Contents/Include -I /Users/arrian/Dev/polycode/Dependencies/freetype/src/freetype/include -I . -L . libPolycore.a libpng15.a libfreetype.a liblibogg.a liblibvorbis.a liblibvorbisfile.a liblua5.1.a libphysfs.a libz.a libPolycode2DPhysics.a libPolycode3DPhysics.a libPolycodeTUIO.a libPolycodeUI.a -framework Cocoa -framework opengl -framework openal -framework IOKit

app-bin:
	g++ cocoa-bind.mm PolycodeExampleAppDelegate.mm PolycodeView.mm PolycodeApp.mm -o MinimalistCocoaApp -I /Users/arrian/Dev/polycode/Core/Contents/Include -I /Users/arrian/Dev/polycode/Dependencies/freetype/src/freetype/include -I . -L . libPolycore.a libpng15.a libfreetype.a liblibogg.a liblibvorbis.a liblibvorbisfile.a liblua5.1.a libphysfs.a libz.a libPolycode2DPhysics.a libPolycode3DPhysics.a libPolycodeTUIO.a libPolycodeUI.a -framework Cocoa -framework opengl -framework openal -framework IOKit

clean:
	rm bin/app.dylib