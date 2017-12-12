This repository is a study for adding autocompletion feature in sddm (https://github.com/sddm/sddm).

DON'T USE THIS CODE IF YOU DON'T KNOW WHAT YOU ARE DOING.

In this moment the patch is working, but it is well far to be ready.

All C++ code is included inside UserModel.cpp and UserModel.h files.
A new UNameBox.qml component is added to interface autocompletion mechanism between qml and C++ code.
A new theme "autoComp", derived from the basic "elarun", is available fo testing.

The autocompletion engine mechanisme is based on a trie data structure (https://en.wikipedia.org/wiki/Trie), so it shoud be light enough to carry a lot of users.

Some other enhancements are possible, developing the actual implementation:

 - C++ code may be separated from UserModel files;
 - a graphical userlist may be added;
 - a filter username selection may be coupled to graphical userlist.
   (for example, if the user types the letter a, all entries in the userlist not starting with a disappear.
 - the autocompletion matching rules may be enhanced, to something better than a simple serial mechanism.
   (for example, if the user types the letters a and the letter b, all entries containing the letter a followed -even not immediatly- by b, are selected.

look here for a video:
https://raw.githubusercontent.com/cloc3/sddm/sddmAuc.mp4

Here  a list of files interested by the patch:

diff -ruN sddm/CMakeLists.txt.user sddmAuc/CMakeLists.txt.user
diff -ruN sddm/components/2.0/UNameBox.qml sddmAuc/components/2.0/UNameBox.qml
diff -ruN sddm/components/CMakeLists.txt sddmAuc/components/CMakeLists.txt
diff -ruN sddm/components/common/qmldir sddmAuc/components/common/qmldir
diff -ruN sddm/data/themes/autoComp/Main.qml sddmAuc/data/themes/autoComp/Main.qml
diff -ruN sddm/data/themes/autoComp/metadata.desktop sddmAuc/data/themes/autoComp/metadata.desktop
diff -ruN sddm/data/themes/autoComp/README sddmAuc/data/themes/autoComp/README
diff -ruN sddm/data/themes/autoComp/theme.conf sddmAuc/data/themes/autoComp/theme.conf
diff -ruN sddm/data/themes/CMakeLists.txt sddmAuc/data/themes/CMakeLists.txt
diff -ruN sddm/README.md sddmAuc/README.md
diff -ruN sddm/src/greeter/AutoCompletion.cpp sddmAuc/src/greeter/AutoCompletion.cpp
diff -ruN sddm/src/greeter/AutoCompletion.h sddmAuc/src/greeter/AutoCompletion.h
diff -ruN sddm/src/greeter/UserModel.cpp sddmAuc/src/greeter/UserModel.cpp
diff -ruN sddm/src/greeter/UserModel.h sddmAuc/src/greeter/UserModel.h
