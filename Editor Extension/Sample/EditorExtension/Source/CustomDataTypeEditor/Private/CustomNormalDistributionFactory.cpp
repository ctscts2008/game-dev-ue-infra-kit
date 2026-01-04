#include "CustomNormalDistributionFactory.h"
#include "CustomNormalDistribution.h"

UCustomNormalDistributionFactory::UCustomNormalDistributionFactory()
{
	SupportedClass = UCustomNormalDistribution::StaticClass();
	bCreateNew = true;
}

UObject* UCustomNormalDistributionFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UCustomNormalDistribution>(InParent, InClass, InName, Flags, Context);
}

