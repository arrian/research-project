

all:
	$(MAKE) clean
	$(MAKE) app-lib

poly-lib:
	cd ~/Dev/polycode-dev && make && cp ./Core/Contents/libPolycore.a ~/Dev/research-project/study-3/app-lib/deps/

app-lib:
	g++ PolySceneMultiLabel.cpp PolyExtemporeCore.cpp PolycodeApp.cpp -o bin/app.dylib -dynamiclib -I /Users/arrian/Dev/polycode-dev/Core/Contents/Include -I /Users/arrian/Dev/polycode-dev/Modules/Contents/2DPhysics/Include -I /Users/arrian/Dev/polycode-dev/Modules/Contents/UI/Include -I /Users/arrian/Dev/polycode-dev/Dependencies/freetype/src/freetype/include -I . -lglfw3 -L . deps/libPolycore.a deps/libPolycode2DPhysics.a deps/libpng15.a deps/libfreetype.a deps/liblibogg.a deps/liblibvorbis.a deps/liblibvorbisfile.a deps/liblua5.1.a deps/libphysfs.a deps/libz.a deps/libPolycode2DPhysics.a deps/libPolycode3DPhysics.a deps/libPolycodeTUIO.a deps/libPolycodeUI.a deps/libBox2D.a -framework opengl -framework openal -framework Cocoa -framework IOKit

test:
	g++ PolySceneMultiLabel.cpp PolyExtemporeCore.cpp PolycodeApp.cpp -o bin/app-test -I /Users/arrian/Dev/polycode-dev/Core/Contents/Include -I /Users/arrian/Dev/polycode-dev/Modules/Contents/2DPhysics/Include -I /Users/arrian/Dev/polycode-dev/Modules/Contents/UI/Include -I /Users/arrian/Dev/polycode-dev/Dependencies/freetype/src/freetype/include -I . -lglfw3 -L . deps/libPolycore.a deps/libPolycode2DPhysics.a deps/libpng15.a deps/libfreetype.a deps/liblibogg.a deps/liblibvorbis.a deps/liblibvorbisfile.a deps/liblua5.1.a deps/libphysfs.a deps/libz.a deps/libPolycode2DPhysics.a deps/libPolycode3DPhysics.a deps/libPolycodeTUIO.a deps/libPolycodeUI.a deps/libBox2D.a -framework opengl -framework openal -framework Cocoa -framework IOKit

clean:
	rm -f bin/app.dylib


	