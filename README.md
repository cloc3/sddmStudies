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
https://raw.githubusercontent.com/cloc3/sddm/autoCompletion/sddmAuc.mp4
