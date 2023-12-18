// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Components/DecalComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Iteraction/EnemyInterface.h"
#include "UI/Widget/DamageTextComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if(AbilitySystemComponent == nullptr)
	{
		AbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AbilitySystemComponent;
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);

	// bind actions in InputConfig with functions
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorResult.ImpactPoint);
	}
}

void AAuraPlayerController::AutoRun()
{
	if(!bAutoRunning) return;
	if(APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpine = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpine - CachedDestination).Length();
		if(DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
 		}
 	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,
	bool bBlockedHit, bool bCriticalHit)
{
	if(IsValid(TargetCharacter) && DamageTexComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter, DamageTexComponentClass);
		/** Register this component, creating any rendering/physics state. Will also add itself to the outer Actor's Components array, if not already present. */
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();

	const FRotator Rotator = GetControlRotation();

	const FRotator YawRotation{0.f, Rotator.Yaw, 0.f};

	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if(APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardVector, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightVector, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor) LastActor->UnHighLightActor();
		if (ThisActor) ThisActor->UnHighLightActor();
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	GetHitResultUnderCursor(ECC_Visibility, false, CursorResult);
	if (!CursorResult.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorResult.GetActor());

	if(LastActor != ThisActor)
	{
		if(LastActor)
		{
			LastActor->UnHighLightActor();
		}
		if(ThisActor)
		{
			ThisActor->HighLightActor();
		}
	}
	
}

void AAuraPlayerController::AbilityInputTagPressed(const FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor != nullptr ? true : false;
		// bAutoRunning is false when you just press LMB button
		bAutoRunning = false;
	}
	else
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagPressed(InputTag);
		}
		return;
	}

	if(bTargeting || bShiftKeyDown)
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagPressed(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if(CursorResult.bBlockingHit)
		{
			CachedDestination = CursorResult.ImpactPoint;
		}

		if(APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation());
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
	// if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
}

void AAuraPlayerController::AbilityInputTagReleased(const FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	// decide whether auto run 
	if(!bTargeting && !bShiftKeyDown)
	{
		APawn* ControlledPawn = GetPawn();
		if(FollowTime <= ShortPressThreshold)
		{
			UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination);
			if(Path)
			{
				Spline->ClearSplinePoints();
				for(const FVector& PointLoc : Path->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				if(Path->PathPoints.Num() > 0)
				{
					CachedDestination = Path->PathPoints[Path->PathPoints.Num() - 1];
				}
				bAutoRunning = true;
			}
			if (GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
			bTargeting = false;
		}
		FollowTime = 0.f;
	}
	else
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}
	
}



