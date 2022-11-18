Open a *Terminal*.

1.  *Clone the github tree to get the source code*

```
git clone --recursive https://github.com/triangles/triangles.git
cd triangles
```

2.  *Download and install Brew for Mac* (https://brew.sh)

```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

3.  *Install dependencies with Brew*

```
brew install autoconf
brew install automake
brew install pkg-config
brew install protobuf
brew install boost@1.57 --cc=clang 
brew link boost@1.57 --force
brew install berkeley-db@4 
brew link berkeley-db@4 --force
brew install openssl 
brew install miniupnpc
brew install libevent
brew install qt5  [only use if building the Wallet UI]
brew link qt5 --force
brew install qrencode [only use if building the Wallet UI]
```
3.  *Build Triangles*

```
cd triangles
./autogen.sh
./configure --with-gui=qt5 --disable-tests [or use --without-gui if only building the daemon]
make
```

4. *Outputs*

The daemon will be located under *src/Trianglesd*, the Wallet UI under *src/qt/Triangles-qt*.

You can now start them directly (either the daemon or the wallet but never both of them at the same time!), or you can first copy them somewhere else and then start them by typing in their respective names. The apps will automatically find your default Triangles directory (if you already have one).

5. *Creating a DMG*

After a successful compilation you have the option to create a DMG for easier macOS deployment. 

For this type: `sudo make deploy` in the Terminal and enter the **administrator password** when asked. 

Your Mac will now start to create several files and also open a Window with Triangles icons in it. **Do not interfere with this process!** Let it finish its work. After a successful DMG creation you'll get the **Triangles-Qt.dmg** file in `triangles` directory. Use this file by double clicking on it and installing the Wallet in the Applications path.
