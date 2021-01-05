training<br /><br />

<b>ShowTime</b><br />
This is a slide show program written in C++ and Qt. This is my first time exposure to Qt and Qt-Creator, and a refresher in C++. It displays photos contained in a selected directory. This project was started around the 22nd of December 2020 and completely in usable form on the 5th of January 2021.

<br /><br />

<b>Uses the following elements</b><br /><br />

<b>Qt UI</b><br />
Loading images. Getting the images size themsleves based upon the current size of the window while maintaining the aspect ratio (resizeEvent). Setting up the menu. Setting up Slots for the menu selections. Setting up the tool bar and associated Slots. There is small splashes of CSS to provide adjustments to the appearence of the UI.<br /><br />

<b>QFileDialog and QDir</b><br />
Use of getExistingDirectory to select a directory. Once a directory is selected a <b>QStringList</b> of files is created filtered for .jpg, .png, and .bmp using <b>setNameFilters</b>.<br /><br />

<b>QThread</b><br />
The heart of the program is the QThread. A QStringList is passed into the class that in embedded in the thread along with other variables. The thread combined with a Signal and Slot iterates through the QStringList extracting one filename at a time, making use of the QThread::msleep() function to provide a delay, and <b>emitting</b> it back to the main thread where the assigned slot inserts it into the QLabel. <br /><br />

Obvious C++ syntax for the program logic.<br /><br /><br />

<b>Todo List</b><br />
<ul>
<li>A way of setting the time delay. Its currently hard coded for 5 seconds.</li>
<li>Dialog to change the order of the images.</li>
<li>A way to step forwards and backwards. </li>
<li>?? Fullscreen mode ??</li>
</ul>
