#include "PawnPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "PawnPlayerController.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "PawnGameState.h"
#include "Shuriken.h"
#include "Katana.h"


APawnPlayer::APawnPlayer()
{
    UE_LOG(LogTemp, Warning, TEXT("APawnPlayer"));
    PrimaryActorTick.bCanEverTick = true;
 
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
    CapsuleComponent->InitCapsuleSize(42.f, 96.f);
    RootComponent = CapsuleComponent;
    CapsuleComponent->SetSimulatePhysics(false);

    
    Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(CapsuleComponent);
    Mesh->SetRelativeLocation(FVector(0.f, 0.f, -90.f));

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(CapsuleComponent);
    SpringArm->TargetArmLength = 300.0f;
    SpringArm->bUsePawnControlRotation = true;

    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
    if (Mesh)
    {
        OverheadWidget->SetupAttachment(Mesh);
    }
    OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    MovementInput = FVector2D::ZeroVector;
    LookInput = FVector2D::ZeroVector;
    Velocity = FVector::ZeroVector;

    MaxHealth = 100.f;
    Health = MaxHealth;

    CurrentWeapon = nullptr;
    WeaponSocketName = "WeaponSocket";
    WeaponSocketName2 = "WeaponSocket2";

}

void APawnPlayer::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
    if (DefaultWeaponClass)
    {
        AShuriken* NewWeapon = GetWorld()->SpawnActor<AShuriken>(DefaultWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (NewWeapon)
        {
            EquipShuriken(NewWeapon);
        }
    }

    if (SecondaryWeaponClass)
    {
        AKatana* NewWeapon = GetWorld()->SpawnActor<AKatana>(SecondaryWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (NewWeapon)
        {
            EquipKatana(NewWeapon);
        }
        if (!GetWorld()) return;
    }




}

void APawnPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UE_LOG(LogTemp, Warning, TEXT("Tick"));
    const FRotator CameraRotation = Camera->GetComponentRotation();
    const FRotator YawOnlyRotation(0.f, CameraRotation.Yaw, 0.f);

    const FVector CameraForward = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X);
    const FVector CameraRight = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::Y);

    const FVector InputDirection = (CameraForward * MovementInput.X) + (CameraRight * MovementInput.Y);
    const FVector DesiredVelocity = InputDirection.GetSafeNormal() * MaxWalkSpeed;
    Velocity = FMath::VInterpTo(Velocity, DesiredVelocity, DeltaTime, VelocityInterpSpeed);

    if (!Velocity.IsNearlyZero())
    {
        FRotator NewRotation = Velocity.Rotation();
        NewRotation.Pitch = 0.f;
        NewRotation.Roll = 0.f;
        SetActorRotation(NewRotation);
    }

    
    AddActorWorldOffset(Velocity * DeltaTime, true);

    if (!FMath::IsNearlyZero(LookInput.X))
    {
        const float YawRotation = LookInput.X * TurnRate * DeltaTime;
        AddControllerYawInput(YawRotation);  // 컨트롤러가 Yaw 회전 처리
    }

    if (!FMath::IsNearlyZero(LookInput.Y))
    {
        FRotator CurrentArmRotation = SpringArm->GetRelativeRotation();
        float NewPitch = CurrentArmRotation.Pitch - (LookInput.Y * LookUpRate * DeltaTime);
        NewPitch = FMath::Clamp(NewPitch, MinPitch, MaxPitch);  // MinPitch, MaxPitch로 제한
        CurrentArmRotation.Pitch = NewPitch;
        SpringArm->SetRelativeRotation(CurrentArmRotation);
    }

    if (!FMath::IsNearlyZero(Velocity.SizeSquared()))
    {
        // 이동하는 방향으로 캐릭터 회전 (Yaw만 적용)
        FRotator NewRotation = Velocity.Rotation();
        NewRotation.Pitch = 0.f;  // Pitch(상하) 회전 방지
        NewRotation.Roll = 0.f;   // Roll(기울기) 회전 방지
        SetActorRotation(NewRotation);
    }

    SpringArm->bUsePawnControlRotation = true;  // 카메라가 컨트롤러의 회전을 따라가도록 설정
    bUseControllerRotationYaw = false;  // 플레이어 본체는 회전하지 않도록 설정

    UE_LOG(LogTemp, Warning, TEXT("in"));
    if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()))
    {
        UE_LOG(LogTemp, Warning, TEXT("out"));
        if (PlayerController->PlayerCameraManager)
        {
           // if (bIsAiming)
            {
                float TargetFOV = bIsAiming ? 60.f : 90.f;
                float NewFOV = FMath::FInterpTo(PlayerController->PlayerCameraManager->GetFOVAngle(), TargetFOV, DeltaTime, 10.f);
                PlayerController->PlayerCameraManager->SetFOV(NewFOV);
            }
        }
    }
}

void APawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        if (APawnPlayerController* MyController = Cast<APawnPlayerController>(GetController()))
        {
            if (MyController->MoveAction)
            {
                EnhancedInput->BindAction(MyController->MoveAction, ETriggerEvent::Triggered, this, &APawnPlayer::Move);
                EnhancedInput->BindAction(MyController->MoveAction, ETriggerEvent::Completed, this, &APawnPlayer::StopMove);
            }

            if (MyController->LookAction)
            {
                EnhancedInput->BindAction(MyController->LookAction, ETriggerEvent::Triggered, this, &APawnPlayer::Look);
                EnhancedInput->BindAction(MyController->LookAction, ETriggerEvent::Completed, this, &APawnPlayer::StopLook);
            }

            if (MyController->FireAction)
            {
                EnhancedInput->BindAction(MyController->FireAction, ETriggerEvent::Triggered, this, &APawnPlayer::FireWeapon);
            
            }
            if (MyController->AimAction)
            {
                EnhancedInput->BindAction(MyController->AimAction, ETriggerEvent::Triggered, this, &APawnPlayer::StartAiming);
                EnhancedInput->BindAction(MyController->AimAction, ETriggerEvent::Completed, this, &APawnPlayer::StopAiming);
            }
            UE_LOG(LogTemp, Warning, TEXT("call"));
            
        }
    }
}

void APawnPlayer::Move(const FInputActionValue& Value)
{
    MovementInput = Value.Get<FVector2D>();
    if (!MovementInput.IsNearlyZero())
    {
        FRotator CameraRotation = SpringArm->GetComponentRotation();
        CameraRotation.Pitch = 0.f;  // 위아래 회전은 반영하지 않음
        CameraRotation.Roll = 0.f;

        SetActorRotation(CameraRotation); // 플레이어를 카메라 방향으로 회전
    }
}

void APawnPlayer::StopMove(const FInputActionValue& Value)
{
    MovementInput = FVector2D::ZeroVector;
}

void APawnPlayer::Look(const FInputActionValue& Value)
{
    LookInput = Value.Get<FVector2D>();
}

void APawnPlayer::StopLook(const FInputActionValue& Value)
{
    LookInput = FVector2D::ZeroVector;
}

void APawnPlayer::Jump(const FInputActionValue& Value)
{
    if (Value.Get<bool>())
    {
        if (!Velocity.Z)  // 공중에 있지 않으면 점프
        {
            FVector JumpImpulse(0.f, 0.f, JumpHeight);  // JumpHeight는 점프 높이 값
            CapsuleComponent->AddImpulse(JumpImpulse, NAME_None, true);
        }
    }
}


void APawnPlayer::StartAiming()
{
    bIsAiming = true;
}

void APawnPlayer::StopAiming()
{
    bIsAiming = false;
}


float APawnPlayer::GetHealth() const
{
    return Health;
}

void APawnPlayer::AddHealth(float Amount)
{
    Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
    UpdateOverheadHP();
}

float APawnPlayer::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
    UpdateOverheadHP();

    if (Health <= 0.f)
    {
        OnDeath();
    }

    return DamageAmount;
}

void APawnPlayer::UpdateOverheadHP()
{
    if (!OverheadWidget) return;

    UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
    if (!OverheadWidgetInstance) return;

    if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
    {
        HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
    }
}

void APawnPlayer::OnDeath()
{
    APawnGameState* PawnGameState = GetWorld() ? GetWorld()->GetGameState < APawnGameState>() : nullptr;
    if (PawnGameState)
    {
        PawnGameState->OnGameOver();
    }

    Destroy();
}


void APawnPlayer::EquipShuriken(AShuriken* WeaponToEquip)
{
    if (!WeaponToEquip) return;

    if (EquippedShuriken)
    {
        EquippedShuriken->Destroy();
    }

    EquippedShuriken = WeaponToEquip;
    EquippedShuriken->SetOwner(this);

    if (Mesh)
    {
        EquippedShuriken->AttachToComponent(Mesh,
            FAttachmentTransformRules::SnapToTargetNotIncludingScale,
            WeaponSocketName
        );
    }

    UE_LOG(LogTemp, Warning, TEXT("Shuriken Equipped: %s"), *EquippedShuriken->GetName());
}

void APawnPlayer::FireWeapon()
{
	if (EquippedShuriken)
	{
		EquippedShuriken->Fire();
	}
}

void APawnPlayer::EquipKatana(AKatana* WeaponToEquip)
{
    if (!WeaponToEquip) return;

    if (EquippedKatana)
    {
        EquippedKatana->Destroy();
    }

    EquippedKatana = WeaponToEquip;
    EquippedKatana->SetOwner(this);

    if (Mesh)
    {
        EquippedKatana->AttachToComponent(Mesh,
            FAttachmentTransformRules::SnapToTargetNotIncludingScale,
            WeaponSocketName2
        );
    }

    UE_LOG(LogTemp, Warning, TEXT("Katana Equipped: %s"), *EquippedKatana->GetName());
}

void APawnPlayer::SlashWeapon()
{
	if (EquippedKatana)
	{
		EquippedKatana->Slash();
	}
}