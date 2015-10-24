CXX = g++
PWD := $(CURDIR)
UNAME := $(shell uname)
CMAKEEXE = $(PWD)/cmake_install/bin/cmake
RCINT = rootcint
RC = root-config

BDIR = bin

TARGET = ndvr
TARGETSRC := ndvr.cxx

OBJS = ReadGeom.cc WindowLoop.cc
OBJH := $(OBJS:.cc=.hxx)
OBJO := $(OBJS:.cc=.o)

ROOTCFLAGS := `$(RC) --cflags`
ROOTLDFLAGS := `$(RC) --evelibs`

PKGCONFIGPATH := $(PWD)/glfw_install/lib64/pkgconfig

GLFWINC = $$(PKG_CONFIG_PATH=$(PKGCONFIGPATH) pkg-config --cflags glfw3)
GLFWLIB = $$(PKG_CONFIG_PATH=$(PKGCONFIGPATH) pkg-config --libs glfw3)
GLFWLIB64 = $$(PKG_CONFIG_PATH=$(PKGCONFIGPATH) pkg-config --libs glfw3)

CXXFLAGS := -fPIC $(ROOTCFLAGS) $(GLFWINC) -g -std=c++11 -Wall
EXECXXFLAGS := $(ROOTCFLAGS) $(GLFWINC) -Iutils/ -g -std=c++11 -Wall
LDFLAGS := -Lutils/lib -lCLITools $(ROOTLDFLAGS) $(GLFWLIB64)

ifeq ($(UNAME), Linux)
  EXECXXFLAGS +=-lGL
endif
ifeq ($(UNAME), Darwin)
  EXECXXFLAGS +=-framework OpenGL
endif

ifeq ($(UNAME), Linux)
	GLFWBUILDLIB=glfw_install/lib64/libglfw.so
endif
ifeq ($(UNAME), Darwin)
	GLFWBUILDLIB=
endif

.PHONY: clean

$(BDIR)/$(TARGET): $(TARGETSRC) $(GLFWBUILDLIB) $(OBJO)
	$(CXX) $(TARGETSRC) -o $(TARGET) $(OBJO) $(EXECXXFLAGS) $(LDFLAGS) $(OGLF)
	mkdir -p $(BDIR); mv ndvr $(BDIR)/
	rm ./*.o

cmake/.git:
	git submodule init; git submodule update

cmake_install/bin/cmake: cmake/.git
	cd cmake; git checkout v3.3.2
	mkdir cmake_install
	cd cmake_install && ../cmake/bootstrap --prefix=$(PWD)/cmake_install && $(MAKE) -j4 && $(MAKE) install

glfw/.git:
	git submodule init; git submodule update

$(GLFWBUILDLIB): cmake_install/bin/cmake glfw/.git
	cd glfw; git checkout 3.1.2
	mkdir -p glfw_install
	cd glfw_install; $(CMAKEEXE) ../glfw -DBUILD_SHARED_LIBS=YES -DLIB_SUFFIX=64 -DCMAKE_INSTALL_PREFIX=$(PWD)/glfw_install;
	cd glfw_install; $(MAKE) -j4 && $(MAKE) install

utils/.git:
	git submodule init; git submodule update

utils/lib/libCLITools.a: utils/.git
	cd utils; $(MAKE) libCLITools.a

clean:
	rm -rf glfw_install cmake_install $(BDIR) .*o
	cd utils; $(MAKE) clean
