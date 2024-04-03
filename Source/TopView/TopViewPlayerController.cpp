#include "TopViewPlayerController.h"
#include "Global.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "TopViewCharacter.h"
#include "Engine/World.h"
#include "KismetProceduralMeshLibrary.h"


ATopViewPlayerController::ATopViewPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ATopViewPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void ATopViewPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &ATopViewPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &ATopViewPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ATopViewPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ATopViewPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &ATopViewPlayerController::OnResetVR);

	InputComponent->BindAction("RButton", IE_Pressed, this, &ATopViewPlayerController::OnRButton);
}

void ATopViewPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ATopViewPlayerController::MoveToMouseCursor()
{

	// Trace to see what is under the mouse cursor
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);

	if (Hit.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(Hit.ImpactPoint);
	}
	
}

void ATopViewPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void ATopViewPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ATopViewPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void ATopViewPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

void ATopViewPlayerController::OnRButton()
{
	//LineTrace

	FVector start = GetPawn()->GetActorLocation();

	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	FVector end = FVector(Hit.ImpactPoint.X, Hit.ImpactPoint.Y, start.Z);

	TArray<AActor*> ignores;
	ignores.Add(GetPawn());

	FHitResult hitResult;

	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		start,
		end,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ignores,
		EDrawDebugTrace::ForDuration,
		hitResult,
		true,
		FLinearColor::Green,
		FLinearColor::Red,
		1.0f
	);

	CheckFalse(hitResult.IsValidBlockingHit());

	UProceduralMeshComponent* otherComp = Cast<UProceduralMeshComponent>(hitResult.Component);
	CheckNull(otherComp);

	FVector lineDirection = (end - start).GetSafeNormal();
	FVector planeNormal = lineDirection ^ GetPawn()->GetActorUpVector();

	UProceduralMeshComponent* newotherComp = nullptr;

	UMaterial* materialAsset;
	CHelpers::GetAssetDynamic(&materialAsset, "Material'/Game/Materials/Surface/MAT_HalfProcMesh.MAT_HalfProcMesh'");

	UKismetProceduralMeshLibrary::SliceProceduralMesh
	(
		otherComp,
		hitResult.Location,
		planeNormal,
		true,
		newotherComp,
		EProcMeshSliceCapOption::CreateNewSectionForCap,
		materialAsset
	);

	newotherComp->SetSimulatePhysics(true);
	newotherComp->AddImpulse(lineDirection * 800.0f, NAME_None, true);

}