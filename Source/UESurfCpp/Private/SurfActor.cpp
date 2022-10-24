// Fill out your copyright notice in the Description page of Project Settings.


#include "SurfActor.h"
#include "MeshDescription.h"
#include "StaticMeshAttributes.h"
#include "ProceduralMeshComponent.h"
#include "DeUtils.h"

/*
Add to ...Build.cs to PublicDependencyModuleNames:
"MeshDescription", "StaticMeshDescription", "ProceduralMeshComponent"
*/


// Sets default values
ASurfActor::ASurfActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratedMesh"));
	RootComponent = ObjectMesh;

}

// Called when the game starts or when spawned
void ASurfActor::BeginPlay()
{
	Super::BeginPlay();
	LoadMesh();
	//MakeTestTriangle();
}

void ASurfActor::UpdateMaterial()
{
	UMaterialInterface* Mat = MaterialSlot;
	if (ObjectMesh != nullptr && Mat != nullptr) {
		ObjectMesh->SetMaterial(0, Mat);
	}
}

// Called every frame
void ASurfActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Set material
void ASurfActor::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName l_propName(PropertyChangedEvent.Property != nullptr ? PropertyChangedEvent.Property->GetFName() : NAME_None);
	//UE_LOG(LogTemp, Warning, TEXT("Changed property %s"), *l_propName.ToString());

	if (l_propName.ToString() == "MaterialSlot") UpdateMaterial();

}

void ASurfActor::LoadMesh()
{
	FString BasePath = FPaths::ProjectDir();
	FString FileName = BasePath + MeshPath;
	std::string fileName = TCHAR_TO_ANSI(*FileName); //"d:/perevalovds.com/_2022/2022-EEG_Food/UEFood/NiagaraGPU 5.0/Meshes/swirl1.obj";
	std::vector<float> XYZ;
	std::vector<int> Tri;
	bool load_success = DeUtils::LoadObjMesh(fileName, XYZ, Tri);
	if (!load_success) {
		UE_LOG(LogTemp, Error, TEXT("ASurfActor: Error loading mesh %s"), *FileName);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("ASurfActor: Loaded mesh %s"), *FileName);


	int nV = XYZ.size() / 3;
	int nT = Tri.size() / 3;

	FMeshDescription mesh_desc;
	FStaticMeshAttributes attributes(mesh_desc);
	attributes.Register();
	TVertexAttributesRef<FVector3f> positions = mesh_desc.GetVertexPositions();

	mesh_desc.ReserveNewVertices(nV);
	std::vector<FVertexID> vid(nV);
	for (int i = 0; i < nV; i++) {
		vid[i] = mesh_desc.CreateVertex();
	}

	positions = attributes.GetVertexPositions();
	for (int i = 0; i < nV; i++) {
		int k = i * 3;
		positions[i] = FVector3f(XYZ[k], XYZ[k+1], XYZ[k+2]);
	}

	mesh_desc.ReserveNewVertexInstances(nV);
	std::vector<FVertexID> vinstid(nV);
	for (int i = 0; i < nV; i++) {
		vinstid[i] = mesh_desc.CreateVertexInstance(vid[i]);
	}
	FPolygonGroupID polygon_group = mesh_desc.CreatePolygonGroup();

	mesh_desc.ReserveNewPolygons(nT);
	for (int i = 0; i < nT; i++) {
		int k = i * 3;
		mesh_desc.CreatePolygon(polygon_group, { vinstid[Tri[k]], vinstid[Tri[k+1]], vinstid[Tri[k+2]] });
	}

	// At least one material must be added
	UStaticMesh* mesh = NewObject<UStaticMesh>(this, FName(TEXT("ProceduralStaticMesh")));
	mesh->GetStaticMaterials().Add(FStaticMaterial());

	UStaticMesh::FBuildMeshDescriptionsParams mdParams;
	mdParams.bBuildSimpleCollision = false;
	mdParams.bAllowCpuAccess = true;
	//mesh->NaniteSettings.bEnabled = true;

	// Build static mesh
	mesh->BuildFromMeshDescriptions({ &mesh_desc }, mdParams);
	if (MaterialSlot)
	{
		mesh->SetMaterial(0, MaterialSlot);
	}

	ObjectMesh->SetStaticMesh(mesh);
    Mesh = ObjectMesh->GetStaticMesh();
}


void ASurfActor::MakeTestTriangle()
{
	FMeshDescription mesh_desc;// = build_grid_mesh_description(20, 20, 200, 200);
	FStaticMeshAttributes attributes(mesh_desc);
	attributes.Register();
	TVertexAttributesRef<FVector3f> positions = mesh_desc.GetVertexPositions();

	mesh_desc.ReserveNewVertices(3);
	FVertexID v0 = mesh_desc.CreateVertex();
	FVertexID v1 = mesh_desc.CreateVertex();
	FVertexID v2 = mesh_desc.CreateVertex();

	positions = attributes.GetVertexPositions();

	positions[0] = FVector3f(-100, 0, 0);
	positions[1] = FVector3f(100, 0, 0);
	positions[2] = FVector3f(0, 100, 100);

	mesh_desc.ReserveNewVertexInstances(3);
	FVertexInstanceID vi0 = mesh_desc.CreateVertexInstance(v0);
	FVertexInstanceID vi1 = mesh_desc.CreateVertexInstance(v1);
	FVertexInstanceID vi2 = mesh_desc.CreateVertexInstance(v2);

	FPolygonGroupID polygon_group = mesh_desc.CreatePolygonGroup();

	mesh_desc.ReserveNewPolygons(1);
	mesh_desc.CreatePolygon(polygon_group, { vi0, vi1, vi2 });


	/*TVertexInstanceAttributesRef<FVector3f> normals = attributes.GetVertexInstanceNormals();

	normals[0] = FVector3f(0, 0, -1);
	normals[1] = FVector3f(0, 0, -1);
	normals[2] = FVector3f(1, 0, 0);*/

	/*mesh_desc.ReserveNewUVs(3);
	FUVID uv0 = mesh_desc.CreateUV();
	FUVID uv1 = mesh_desc.CreateUV();
	FUVID uv2 = mesh_desc.CreateUV();

	TVertexInstanceAttributesRef<FVector2f> uvs = attributes.GetVertexInstanceUVs();

	uvs[0] = FVector2f(0, 0);
	uvs[1] = FVector2f(0, 1);
	uvs[2] = FVector2f(1, 1);
	*/

	// At least one material must be added
	Mesh = NewObject<UStaticMesh>(this, FName(TEXT("ProceduralStaticMesh")));
	Mesh->GetStaticMaterials().Add(FStaticMaterial());

	UStaticMesh::FBuildMeshDescriptionsParams mdParams;
	mdParams.bBuildSimpleCollision = false;
	mdParams.bAllowCpuAccess = true;
	//mesh->NaniteSettings.bEnabled = true;

	// Build static mesh
	Mesh->BuildFromMeshDescriptions({ &mesh_desc }, mdParams);
	if (MaterialSlot)
	{
		Mesh->SetMaterial(0, MaterialSlot);
	}

	ObjectMesh->SetStaticMesh(Mesh);

	//UpdateMaterial();
}


