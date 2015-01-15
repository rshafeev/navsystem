### Description ###
The client-server system was used for vehicle movement monitoring in real time 
See demonstration video: [https://www.youtube.com/channel/UCQoB2TN1E5OpkYd_JGMihaA](Monitor Navigation (Client-Server)System, C++)

<a href="http://www.youtube.com/watch?feature=player_embedded&v=YOUTUBE_VIDEO_ID_HERE
" target="_blank"><img src="http://img.youtube.com/vi/YOUTUBE_VIDEO_ID_HERE/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

### Used technologies and tools ###
* Basic: C++, WinAPI/MFC, Visual Studio 2013
* Network: TCP/IP sockets
* Render map: raster maps, Google Maps(static blocks)

### Requires ###
* MFC framework
* opengl: "glut, glaux, glew" libs(for 3d maps)

### Some problems ###
- if you work in Visual Studio 2013, you can get the next error: "error MSB8031: Building an MFC project for a non-Unicode character...."
  For solving this problem we propose to setup package http://www.microsoft.com/en-us/download/details.aspx?id=40770