// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MessageSystemSubsystem.h"
#include "MessageStruct.h"
#include "MessageSystemBPLibrary.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MessengerComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReceiveMessageDelegate, UMessengerComponent*, DeliveringComponent, FMessageStruct, Message);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MESSAGESYSTEM_API UMessengerComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	// Broadcast these messages when event is triggered.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message System")
	TArray<FMessageStruct> MessageEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message System")
	FMessageStruct TRY_MessageEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message System")
	AActor* TRY_Actor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message System")
	TWeakObjectPtr<AActor> TRY_SoftActor;
	
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UMessengerComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "MessageSystem")
	void ReceiveMessage(FMessageStruct Message);

	UFUNCTION(BlueprintCallable, Category = "MessageSystem")
	void BroadcastEvent(FName TriggerEventName, FMessageParametersStruct TriggerParameters);

	UFUNCTION(BlueprintCallable, Category = "MessageSystem")
	void SendMessage(AActor* TargetActor, FName SendEvent, FMessageParametersStruct TriggerParameters);

	UPROPERTY(BlueprintAssignable, Category = "MessageSystem")
	FReceiveMessageDelegate OnReceiveMessage;













// TODO, implement some registering with the sub system.
// Can't happen during construct, causes crash on editor startup
// try:
// - OnRegister
// - InitializeComponent
// - PostInitProperties
// - PostLoad
// - Modify
// - PostEditChangeProperty
// - PostEditChangeChainProperty

protected:
	void OnRegister() override;

public:
	void InitializeComponent() override;
	void PostInitProperties() override; //Called after the C++ constructor and after the properties have been initialized, including those loaded from config. // This is called first When added or begin play happens. 
	void PostLoad() override;

#if WITH_EDITOR
	bool Modify(bool bAlwaysMarkDirty = true) override; //That the object will be modified. If we are currently recording into the transaction buffer (undo/redo), save a copy of this object into the buffer and marks the package as needing to be saved.// This is called second When added or begin play happens. 
	void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

};
