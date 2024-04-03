#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSliceMesh.generated.h"

UCLASS()
class TOPVIEW_API ACSliceMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSliceMesh();

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleDefaultsOnly)
		class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* OriginMesh;

	UPROPERTY(VisibleDefaultsOnly)
		class UProceduralMeshComponent* CopiedMesh;
};
