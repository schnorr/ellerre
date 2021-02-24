uname -a
#export LIBRARY_PATH="$BUILD_DIR/.apt/usr/lib/x86_64-linux-gnu:$BUILD_DIRR/lib/R/lib:$BUILD_DIR/tcltk/lib:$LIBRARY_PATH"
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/app/.apt/usr/lib/x86_64-linux-gnu
echo $HOME
echo $LD_LIBRARY_PATH
rm -rf ellerre/
git clone git://github.com/schnorr/ellerre.git
mkdir -p ellerre/build
cd ellerre/build
cmake -DBUILD_SHARED_LIBS=OFF -DCMAKE_INSTALL_PREFIX:PATH=/app/.apt/usr ..
make

