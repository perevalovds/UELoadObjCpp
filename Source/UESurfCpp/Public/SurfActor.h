// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "SurfActor.generated.h"

UCLASS()
class UESURFCPP_API ASurfActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASurfActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Set material
	//virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void UpdateMaterial();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Access to StaticMeshComponent
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* ObjectMesh;

	// Path to mesh relative to project path
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FString MeshPath;

	// Access to StaticMesh
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* Mesh;

	// Set this material to mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Material)
	UMaterialInterface* MaterialSlot = nullptr;

	// Signal to reload mesh
	UFUNCTION(BlueprintCallable, Category = "Mesh")
	void LoadMesh();


protected:
	void MakeTestTriangle();

};
