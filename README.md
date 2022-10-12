# UELoadObjCpp
Example Unreal Engine 5 project which loads StaticMesh from OBJ file runtime

## Details

Project provides SurfActor C++ class which loads mesh at BeginPlay.

It loads only vertices and polygons (3 or 4 vertices per each).

Texture coordinates and normals not supported.


## Usage

Open project in Unreal Endine Editor and run it.
You will see wireframe mesh. It's OBj file loaded from Meshes folder at runtime.
You can specify from which file load it, and access to loaded StaticMesh as "Mesh" of that Actor.
