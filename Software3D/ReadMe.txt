-------------------------------------------------------
---	             Software 3D Renderer				---
-------------------------------------------------------

Welcome to the main project, a software 3D renderer.

This Readme will serve as a changelog, as well as document the individual sections of the application.

---------------------
- CHANGELOG			-
---------------------

v0.2 (09-11-2009):
Project now contains a second window for display and change of rendering parameters.
	- Window currently contains one listbox: requires expansion and linking into functions in RenderLibrary.

v0.1 (20-10-2009):
Project started. Currently contains a standard Visual Studio Win32 project, with:
	- Code for GDI+ startup, shutdown and handling
	- Pre-compiled header (stdafx.h / .cpp) which contains the Gdiplus.h header
	- CRT debug flags for dumping memory leaks on close