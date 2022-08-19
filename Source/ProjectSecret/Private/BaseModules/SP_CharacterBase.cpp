// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseModules/SP_CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASP_CharacterBase::ASP_CharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup Spring Arm Component
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocation(FVector {20.f, 0.f, 70.f});
	SpringArmComp->ProbeSize = 5.f;
	SpringArmComp->CameraLagSpeed = 20.f;
	SpringArmComp->CameraRotationLagSpeed = 0.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->bUsePawnControlRotation = true;


	// Setup Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->SetRelativeScale3D_Direct(FVector{0.25f, 0.25f, 0.25f});
	CameraComp->bUsePawnControlRotation = false;


	// Set Orient Rotation to Movement enabled
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Disable Controller Rotation Yaw
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASP_CharacterBase::BeginPlay()
{
	Super::BeginPlay();

	// Camera Zoom Setup
	if (CameraZoomCurve)
	{
		// Zoom in Timeline Float 
		FOnTimelineFloat CameraZoomInTimelineFloat;
		CameraZoomInTimelineFloat.BindUFunction(this, FName("CameraZoomInTimelineF"));
		CameraZoomInTimeline.AddInterpFloat(CameraZoomCurve, CameraZoomInTimelineFloat);
		CameraZoomInTimeline.SetLooping(false);
		
		// Zoom out Timeline Float 
		FOnTimelineFloat CameraZoomOutTimelineFloat;
		CameraZoomOutTimelineFloat.BindUFunction(this, FName("CameraZoomOutTimelineF"));
		CameraZoomOutTimeline.AddInterpFloat(CameraZoomCurve, CameraZoomOutTimelineFloat);
		CameraZoomOutTimeline.SetLooping(false);
	}
}

// Camera Zoom in Timeline Function
void ASP_CharacterBase::CameraZoomInTimelineF()
{
	CameraZoomTimelineValue = CameraZoomInTimeline.GetPlaybackPosition();
	// The float value adjusts the camera zoom force
	CameraZoomCurveFloatValue = 5.f * CameraZoomCurve->GetFloatValue(CameraZoomTimelineValue);
	// Updates the previous location of the camera
	const FVector NewCameraLocation = FVector{CameraComp->GetRelativeLocation().X + CameraZoomCurveFloatValue, CameraComp->GetRelativeLocation().Y, CameraComp->GetRelativeLocation().Z};
	
	CameraComp->SetRelativeLocation(NewCameraLocation);
}

// Zooms the camera in and checks if the location is not out of bounds
void ASP_CharacterBase::CameraZoomIn()
{
	// Max Zoom in
	if (CameraComp->GetRelativeLocation().X >= 150.f) return;
	
	CameraZoomInTimeline.PlayFromStart();
}

// Camera Zoom out Timeline Function
void ASP_CharacterBase::CameraZoomOutTimelineF()
{
	CameraZoomTimelineValue = CameraZoomOutTimeline.GetPlaybackPosition();
	// The float value adjusts the camera zoom force
	CameraZoomCurveFloatValue = -5.f * CameraZoomCurve->GetFloatValue(CameraZoomTimelineValue);
	// Updates the previous location of the camera
	const FVector NewCameraLocation = FVector{CameraComp->GetRelativeLocation().X + CameraZoomCurveFloatValue, CameraComp->GetRelativeLocation().Y, CameraComp->GetRelativeLocation().Z};
	
	CameraComp->SetRelativeLocation(NewCameraLocation);
}

// Zooms the camera out and checks if the location is not out of bounds
void ASP_CharacterBase::CameraZoomOut()
{
	// Max Zoom out
	if (CameraComp->GetRelativeLocation().X <= -150.f) return;
	
	CameraZoomOutTimeline.PlayFromStart();
}

void ASP_CharacterBase::CameraZoomReset()
{
	CameraComp->SetRelativeLocation(FVector{0.f, CameraComp->GetRelativeLocation().Y, CameraComp->GetRelativeLocation().Z});
}

// Move forwards and backwards
void ASP_CharacterBase::MoveForward(float Value)
{
	// Get control rotation, and zeros out pitch and roll in order to avoid funky stuff
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(), Value, false);
}

// Move right and left
void ASP_CharacterBase::MoveRight(float Value)
{
	// Get control rotation, and zeros out pitch and roll in order to avoid funky stuff
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	// Gets the Y Axis from Control rotation
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value, false);
}

// Called every frame
void ASP_CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Camera Zoom Timeline Tick
	CameraZoomInTimeline.TickTimeline(DeltaTime);
	CameraZoomOutTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ASP_CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASP_CharacterBase::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASP_CharacterBase::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Lookup", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	
	PlayerInputComponent->BindAction("CameraZoomIn", IE_Pressed, this, &ASP_CharacterBase::CameraZoomIn);
	PlayerInputComponent->BindAction("CameraZoomOut", IE_Pressed, this, &ASP_CharacterBase::CameraZoomOut);
	PlayerInputComponent->BindAction("CameraZoomReset", IE_Pressed, this, &ASP_CharacterBase::CameraZoomReset);
}

