

all:
	$(MAKE) clean
	$(MAKE) app-lib

poly-lib:
	cd ~/Dev/polycode && make && cp ./Release/Darwin/Framework/Core/lib/libPolycore.a ~/Dev/research-project/study-3/app-lib/deps/

app-lib:
	g++ cocoa-bind.mm PolycodeExampleAppDelegate.mm PolycodeView.mm PolycodeApp.mm -o bin/app.dylib -dynamiclib -I /Users/arrian/Dev/polycode/Core/Contents/Include -I /Users/arrian/Dev/polycode/Dependencies/freetype/src/freetype/include -I . -L . deps/libPolycore.a deps/libpng15.a deps/libfreetype.a deps/liblibogg.a deps/liblibvorbis.a deps/liblibvorbisfile.a deps/liblua5.1.a deps/libphysfs.a deps/libz.a deps/libPolycode2DPhysics.a deps/libPolycode3DPhysics.a deps/libPolycodeTUIO.a deps/libPolycodeUI.a -framework Cocoa -framework opengl -framework openal -framework IOKit

test:
	g++ cocoa-bind.mm PolycodeExampleAppDelegate.mm PolycodeView.mm PolycodeApp.mm -o bin/app-test -I /Users/arrian/Dev/polycode/Core/Contents/Include -I /Users/arrian/Dev/polycode/Dependencies/freetype/src/freetype/include -I . -L . deps/libPolycore.a deps/libpng15.a deps/libfreetype.a deps/liblibogg.a deps/liblibvorbis.a deps/liblibvorbisfile.a deps/liblua5.1.a deps/libphysfs.a deps/libz.a deps/libPolycode2DPhysics.a deps/libPolycode3DPhysics.a deps/libPolycodeTUIO.a deps/libPolycodeUI.a -framework Cocoa -framework opengl -framework openal -framework IOKit

clean:
	rm -f bin/app.dylib