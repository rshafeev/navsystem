### Description ###
The client-server system was used for vehicle movement monitoring in real time 
See demonstration video: https://www.youtube.com/channel/UCQoB2TN1E5OpkYd_JGMihaA

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