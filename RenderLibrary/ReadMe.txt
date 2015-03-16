-------------------------------------------------------
---					Render Library					---
-------------------------------------------------------

Welcome to the RenderLibrary project, a collection of classes designed to render 3D models loaded from MD2 files.

This Readme will serve as a changelog, as well as document the individual sections of the application.

---------------------
- CHANGELOG			-
---------------------

v0.2 (10-11-2009):
Library is now capable of rendering multiple objects with multiple ambient and directional lights.

v0.1 (09-11-2009):
Library is currently capable of rendering wireframe models (with backface culling) and models with ambient lighting (with backface culling and depth sorting).
Capable of loading multiple models into ObjectManager. Capable of using multiple light sources via LightManager.
	
	- Known bug: problem with backface culling in certain orientations of model and camera.
	- Known bug: combined translation and rotation matrix in camera. Currently commented out.
	- Todo:
		- Finish directional lighting.
		- Consolidate local object creation.
		- Create a proper public / private interface inside the library.
		- Make namespace calls consistent.
		- Create an interface for objects in the Intro3D project.
		- Speed optimisations.