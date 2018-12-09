Fcitx的笔画输入法

输入方法:

键值   | 输入的内容
-------:|:----------
i     | 一（横）
o     | 丨（竖）
p     | 丿（撇）
k     | 丶（点）
l     | ㄥ（折）
m     | *（通配）

最大候选词数：5

内含汉字数：21005

内置词汇量：95102

安装前准备:

sudo apt-get install fcitx-libs-dev

安装:

cd FcitxBiHuaInputMethod

mkdir build

cd build

cmake ..

make && sudo make install
