### Description ###
The client-server system was used for vehicle movement monitoring in real time 

#See demonstration video:#
 [![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/jWkSbgRwfmw/0.jpg)](http://www.youtube.com/watch?v=jWkSbgRwfmw)

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

- If ADO Connection CreateInstance Fails, read: http://support.microsoft.com/kb/2517589. (For win7 download and setup: http://www.microsoft.com/en-us/download/confirmation.aspx?id=28477)