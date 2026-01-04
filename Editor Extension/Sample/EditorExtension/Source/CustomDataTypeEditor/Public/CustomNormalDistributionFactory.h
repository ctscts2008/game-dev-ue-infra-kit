#pragma once
#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "CustomNormalDistributionFactory.generated.h"

UCLASS()
class UCustomNormalDistributionFactory : public UFactory
{
	GENERATED_BODY()
	public:
	UCustomNormalDistributionFactory();

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};