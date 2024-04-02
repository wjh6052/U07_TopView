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

protected:
	virtual void BeginPlay() override;

private:
	//Root
	//OriginMesh(SM)
	//CopiedMesh(ProcMesh)
	//-> Slice

};
