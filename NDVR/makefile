CXX = g++
RCINT = rootcint
RC = root-config

BDIR = bin

TARGET = ndvr.exe
TARGETSRC := $(TARGET:.exe=.cxx)

TOBJS =
TOBJH := $(TOBJS:.cxx=.hxx)

ROOTCFLAGS := #`$(RC) --cflags`
ROOTLDFLAGS := #`$(RC) --libs --glibs`

GLFWINC = $(shell PKG_CONFIG_PATH=$(PKGCONFIGPATH) pkg-config --cflags glfw3)
GLFWLIB = $(shell PKG_CONFIG_PATH=$(PKGCONFIGPATH) pkg-config --libs glfw3)
GLFWLIB64 = $(shell PKG_CONFIG_PATH=$(PKGCONFIGPATH) pkg-config --libs glfw3)

OGLF=-framework OpenGL

PWD := $(shell pwd)

PKGCONFIGPATH := $(PWD)/glfw_install/lib64/pkgconfig

CXXFLAGS := -fPIC $(ROOTCFLAGS) $(GLFWINC) -g -std=c++11 -Wall
LDFLAGS := $(ROOTLDFLAGS) $(GLFWLIB64) -Wl,-rpath,.


.PHONY: all clean

$(TARGET): $(TARGETSRC) glfw_install/include/GLFW/glfw3.h glfw_install/lib64/libglfw.3.dylib
	$(CXX) $(TARGETSRC) -o $@ $(CXXFLAGS) $(LDFLAGS) $(OGLF)

glfw/.git:
	git clone git@github.com:glfw/glfw.git;


glfw: glfw/.git
	mkdir -p glfw_install
	cd glfw_install; cmake ../glfw -DBUILD_SHARED_LIBS=YES -DLIB_SUFFIX=64 -DCMAKE_INSTALL_PREFIX=`pwd` -DGLFW_USE_RETINA=YES -DGLFW_USE_MENUBAR=YES;
	cd glfw_install; $(MAKE) -j4 && $(MAKE) install

glfw_install/include/GLFW/glfw3.h: glfw
glfw_install/lib64/libglfw.3.dylib: glfw

clean:
	rm -rf glfw glfw_install $(TARGET)
