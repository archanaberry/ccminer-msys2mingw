# To change the cuda arch, edit Makefile.am and run ./build.sh

extracflags="-march=native "

./configure CXXFLAGS="$extracflags"

