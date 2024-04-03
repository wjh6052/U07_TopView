#include "CSliceMesh.h"
#include "Global.h"
#include "Components/StaticMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"


ACSliceMesh::ACSliceMesh()
{
	CHelpers::CreateSceneComponent(this, &Root, "Root");
	CHelpers::CreateSceneComponent(this, &OriginMesh, "OriginMesh", Root);
	CHelpers::CreateSceneComponent(this, &CopiedMesh, "CopiedMesh", Root);


	UStaticMesh* meshAsset;
	CHelpers::GetAsset<UStaticMesh>(&meshAsset, "StaticMesh'/Game/Robot/SM_Robot.SM_Robot'");
	OriginMesh->SetStaticMesh(meshAsset);

	OriginMesh->SetVisibility(false);
	OriginMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	CopiedMesh->SetSimulatePhysics(true);
	CopiedMesh->bUseComplexAsSimpleCollision = false;

}


void ACSliceMesh::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UKismetProceduralMeshLibrary::CopyProceduralMeshFromStaticMeshComponent
	(
		OriginMesh,
		2,
		CopiedMesh,
		true
	);




}


void ACSliceMesh::BeginPlay()
{
	Super::BeginPlay();
	
}


