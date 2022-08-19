// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/TimelineComponent.h"
#include "SP_CharacterBase.generated.h"

// Preload components in order to not have any future issues
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTSECRET_API ASP_CharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASP_CharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/////////////////////////////////////
	/*             CAMERA              */
	/////////////////////////////////////

	// Spring arm positioning the camera behind the Character
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	// Camera is attached to the spring arm that follows the Character
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	// Curve float for Camera Zoom
	UPROPERTY(EditAnywhere, Category = "Camera")
	UCurveFloat* CameraZoomCurve;

	// Current Timeline value in order to have a smooth transition
	float CameraZoomTimelineValue;

	// Current Float value to multiply with the camera force in order to have a smooth transition
	float CameraZoomCurveFloatValue;
	
	// Timeline for Camera Zoom In
	UPROPERTY()
	FTimeline CameraZoomInTimeline;

	// Camera Zoom in Timeline function
	UFUNCTION()
	void CameraZoomInTimelineF();

	// Camera Zoom in input and bound check
	UFUNCTION()
	void CameraZoomIn();

	// Timeline for Camera Zoom Out
	UPROPERTY()
	FTimeline CameraZoomOutTimeline;

	// Camera Zoom Out Timeline function
	UFUNCTION()
	void CameraZoomOutTimelineF();

	// Camera Zoom Out input and bound check
	UFUNCTION()
	void CameraZoomOut();
	
	// Camera Zoom Reset input
	UFUNCTION()
	void CameraZoomReset();
	
	/***********************************/

	/////////////////////////////////////
	/*             MOVEMENT            */
	/////////////////////////////////////
	
	// Move Forwards and Backwards     
	void MoveForward(float Value);  

	// Move Right and Left             
	void MoveRight(float Value);

	/***********************************/

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
