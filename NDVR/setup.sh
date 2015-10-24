#!/bin/sh

export LD_LIBRARY_PATH="$(readlink -f .)/glfw_install/lib64:${LD_LIBRARY_PATH}"
export PATH="$(readlink -f .)/bin:${PATH}"
