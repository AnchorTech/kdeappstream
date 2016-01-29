# kdeappstream
HTML5 backend for QT applications

Requirements
------------
* libfontconfig1-dev
* kdelibs5-dev
* libqt4-dev
* cmake


Building kdeappstream on Linux\X11
----------------------------------

>$ cd ~/kdeappstream

>$ mkdir ~/kdeappstream/build

>$ cmake ..

>$ make

On Ubuntu/Debian
>$ sudo make install

On Fedora/Centos/Ojuba
>$ su -c make install

For add application to kappstream-server
>$ kappstream-server a applicationName

For remove application from kappstream-server
>$ kappstream-server r applicationName

For Try
>$ kappstream-server

Open Location
> http://127.0.0.1:8080
