### Web UI for ArtNet node

This website is basically the embedded UI for the ESP8226 node. You can use the files in this directory as a starting point in designing your UI and then execute the website generator included in the `bin` directory to generate an Arduino `.h` file that you can include in your embedded project.

##### Usage

The following commands will generate a file named `webui.h` containing all the static C++ data structures necessary to ebed the website in the ESP8226 binary.

```
$ cd webui
$ ../bin/packweb > webui.h
```
