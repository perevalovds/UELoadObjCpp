// Copyright Epic Games, Inc. All Rights Reserved.

// *********************************************************
// To compile, required to add to ..Build.cs the following: 
// "MeshDescription", "StaticMeshDescription"
// *********************************************************

#include "UESurfCppGameModeBase.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"

FMeshDescription build_grid_mesh_description(int x_count, int y_count, float x_scale, float y_scale)
{
	FMeshDescription mesh_desc;

	FStaticMeshAttributes attributes(mesh_desc);
	attributes.Register();

	int vertex_count = x_count * y_count;
	int quad_count = (x_count - 1) * (y_count - 1);
	int triangle_count = quad_count * 2;

	mesh_desc.ReserveNewVertices(vertex_count);
	mesh_desc.ReserveNewVertexInstances(vertex_count);

	mesh_desc.CreatePolygonGroup();
	mesh_desc.ReserveNewPolygons(triangle_count);
	mesh_desc.ReserveNewTriangles(triangle_count);
	mesh_desc.ReserveNewEdges(triangle_count * 3);

	for (int v = 0; v < vertex_count; ++v)
	{
		mesh_desc.CreateVertex();
		mesh_desc.CreateVertexInstance(v);
	}

	for (int x = 0; x < x_count - 1; ++x)
	{
		for (int y = 0; y < y_count - 1; ++y)
		{
			FVertexInstanceID v0 = y + x * x_count;
			FVertexInstanceID v1 = v0 + 1;
			FVertexInstanceID v3 = y + (x + 1) * x_count;
			FVertexInstanceID v2 = v3 + 1;

			mesh_desc.CreateEdge(v0, v1);
			mesh_desc.CreateEdge(v1, v3);
			mesh_desc.CreateEdge(v3, v0);

			mesh_desc.CreateEdge(v1, v2);
			mesh_desc.CreateEdge(v2, v3);
			mesh_desc.CreateEdge(v3, v1);
		}
	}

	mesh_desc.BuildVertexIndexers();

	for (int x = 0; x < x_count - 1; ++x)
	{
		for (int y = 0; y < y_count - 1; ++y)
		{
			FVertexInstanceID v0 = y + x * x_count;
			FVertexInstanceID v1 = v0 + 1;
			FVertexInstanceID v3 = y + (x + 1) * x_count;
			FVertexInstanceID v2 = v3 + 1;

			mesh_desc.CreateTriangle(0, { v0, v1, v3 });
			mesh_desc.CreateTriangle(0, { v1, v2, v3 });
		}
	}

	auto positions = mesh_desc.GetVertexPositions().GetRawArray();;
	auto uvs = mesh_desc.VertexInstanceAttributes().GetAttributesRef<FVector2f>(MeshAttribute::VertexInstance::TextureCoordinate).GetRawArray();

	for (int x = 0; x < x_count; ++x)
	{
		for (int y = 0; y < y_count; ++y)
		{
			int i = y + x * x_count;

			float u = (float)y / (y_count - 1);
			float v = (float)x / (x_count - 1);

			uvs[i] = FVector2f(u, v);
			positions[i] = FVector3f((v - 0.5f) * x_scale, (u - 0.5f) * y_scale, 0);
		}
	}

	return mesh_desc;
}

bool AUESurfCppGameModeBase::LoadMesh(AActor* Actor)
{
	//AActor* actor = nullptr; //need an actor to be the outer object

	FMeshDescription mesh_desc = build_grid_mesh_description(20, 20, 200, 200);
	
	/*
	FStaticMeshAttributes attributes(mesh_desc);
	
	attributes.Register();

	TVertexAttributesRef<FVector3f> positions = mesh_desc.GetVertexPositions();

	mesh_desc.ReserveNewVertices(4);
	FVertexID v0 = mesh_desc.CreateVertex();
	FVertexID v1 = mesh_desc.CreateVertex();
	FVertexID v2 = mesh_desc.CreateVertex();
	FVertexID v3 = mesh_desc.CreateVertex();

	mesh_desc.ReserveNewVertexInstances(4);
	FVertexInstanceID vi0 = mesh_desc.CreateVertexInstance(v0);
	FVertexInstanceID vi1 = mesh_desc.CreateVertexInstance(v1);
	FVertexInstanceID vi2 = mesh_desc.CreateVertexInstance(v2);
	FVertexInstanceID vi3 = mesh_desc.CreateVertexInstance(v3);

	mesh_desc.ReserveNewUVs(4);
	FUVID uv0 = mesh_desc.CreateUV();
	FUVID uv1 = mesh_desc.CreateUV();
	FUVID uv2 = mesh_desc.CreateUV();
	FUVID uv3 = mesh_desc.CreateUV();

	FPolygonGroupID polygon_group = mesh_desc.CreatePolygonGroup();

	mesh_desc.ReserveNewPolygons(1);
	mesh_desc.CreatePolygon(polygon_group, { vi0, vi1, vi2, vi3 });

	positions = attributes.GetVertexPositions();

	positions[0] = FVector3f(-100, -100, 0);
	positions[1] = FVector3f(-100, 100, 0);
	positions[2] = FVector3f(100, 100, 0);
	positions[3] = FVector3f(100, -100, 0);

	TVertexInstanceAttributesRef<FVector3f> normals = attributes.GetVertexInstanceNormals();

	normals[0] = FVector3f(0, 0, -1);
	normals[1] = FVector3f(0, 0, -1);
	normals[2] = FVector3f(0, 0, -1);
	normals[3] = FVector3f(0, 0, -1);

	TVertexInstanceAttributesRef<FVector2f> uvs = attributes.GetVertexInstanceUVs();

	uvs[0] = FVector2f(0, 0);
	uvs[1] = FVector2f(0, 1);
	uvs[2] = FVector2f(1, 1);
	uvs[3] = FVector2f(1, 0);
	*/

	// At least one material must be added
	UStaticMesh* mesh = NewObject<UStaticMesh>(Actor, FName(TEXT("ProceduralStaticMesh")));
	mesh->GetStaticMaterials().Add(FStaticMaterial());

	UStaticMesh::FBuildMeshDescriptionsParams mdParams;
	mdParams.bBuildSimpleCollision = true;

	mesh->NaniteSettings.bEnabled = true;

	// Build static mesh
	mesh->BuildFromMeshDescriptions({ &mesh_desc }, mdParams);
	return true;
}