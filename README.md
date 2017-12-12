This repository is a study for adding autocompletion feature in sddm (https://github.com/sddm/sddm).

DON'T USE THIS CODE IF YOU DON'T KNOW WHAT YOU ARE DOING.

In this moment the patch is working, but it is well far to be ready.

A new C++ class is added to provide the new autoCompletion feature. 
A new UNameBox.qml component is added to interface autocompletion mechanism between qml and C++ code.
A new theme "autoComp", derived from the basic "elarun", is available fo testing.

The autocompletion engine mechanisme is based on a trie data structure (https://en.wikipedia.org/wiki/Trie), so it shoud be light enough to carry a lot of users.

Some other enhancements are possible, developing the actual implementation:

 - a graphical userlist may be added;
 - a filter username selection may be coupled to graphical userlist.
   (for example, if the user types the letter a, all entries in the userlist not starting with a disappear.
 - the autocompletion matching rules may be enhanced, to something better than a simple serial mechanism.
   (for example, if the user types the letters a and the letter b, all entries containing the letter a followed -even not immediatly- by b, are selected.

look here for a video:
https://raw.githubusercontent.com/cloc3/sddm/sddmAuc.mp4

Here  a list of files interested by the patch:

diff --git a/README.md b/README.md
diff --git a/components/2.0/UNameBox.qml b/components/2.0/UNameBox.qml
diff --git a/components/CMakeLists.txt b/components/CMakeLists.txt
diff --git a/components/common/qmldir b/components/common/qmldir
diff --git a/data/themes/CMakeLists.txt b/data/themes/CMakeLists.txt
diff --git a/data/themes/autoComp/Main.qml b/data/themes/autoComp/Main.qml
diff --git a/data/themes/autoComp/README b/data/themes/autoComp/README
diff --git a/data/themes/autoComp/angle-down.png b/data/themes/autoComp/angle-down.png
diff --git a/data/themes/autoComp/elarun.jpg b/data/themes/autoComp/elarun.jpg
diff --git a/data/themes/autoComp/images/background.png b/data/themes/autoComp/images/background.png
diff --git a/data/themes/autoComp/images/lock.png b/data/themes/autoComp/images/lock.png
diff --git a/data/themes/autoComp/images/login_active.png b/data/themes/autoComp/images/login_active.png
diff --git a/data/themes/autoComp/images/login_normal.png b/data/themes/autoComp/images/login_normal.png
diff --git a/data/themes/autoComp/images/rectangle.png b/data/themes/autoComp/images/rectangle.png
diff --git a/data/themes/autoComp/images/rectangle_overlay.png b/data/themes/autoComp/images/rectangle_overlay.png
diff --git a/data/themes/autoComp/images/session_normal.png b/data/themes/autoComp/images/session_normal.png
diff --git a/data/themes/autoComp/images/system_hibernate.png b/data/themes/autoComp/images/system_hibernate.png
diff --git a/data/themes/autoComp/images/system_reboot.png b/data/themes/autoComp/images/system_reboot.png
diff --git a/data/themes/autoComp/images/system_shutdown.png b/data/themes/autoComp/images/system_shutdown.png
diff --git a/data/themes/autoComp/images/system_suspend.png b/data/themes/autoComp/images/system_suspend.png
diff --git a/data/themes/autoComp/images/system_switch_user.png b/data/themes/autoComp/images/system_switch_user.png
diff --git a/data/themes/autoComp/images/user_icon.png b/data/themes/autoComp/images/user_icon.png
diff --git a/data/themes/autoComp/metadata.desktop b/data/themes/autoComp/metadata.desktop
diff --git a/data/themes/autoComp/theme.conf b/data/themes/autoComp/theme.conf
diff --git a/sddmAuc.mp4 b/sddmAuc.mp4
diff --git a/sddmAuc.patch b/sddmAuc.patch
diff --git a/src/greeter/AutoCompletion.cpp b/src/greeter/AutoCompletion.cpp
diff --git a/src/greeter/AutoCompletion.h b/src/greeter/AutoCompletion.h
diff --git a/src/greeter/CMakeLists.txt b/src/greeter/CMakeLists.txt
diff --git a/src/greeter/GreeterApp.cpp b/src/greeter/GreeterApp.cpp
diff --git a/src/greeter/GreeterApp.h b/src/greeter/GreeterApp.h
