// CustomNormalDistribution.cpp



#include "CustomNormalDistribution.h"

UCustomNormalDistribution::UCustomNormalDistribution()
	: Mean(0.5f)
	, StandardDeviation(0.2f)
{
	std::random_device rd; // 获取硬件随机源（若可用）
	RandomNumberGenerator = std::mt19937(rd()); 
}

float UCustomNormalDistribution::DrawSample()
{
	return std::normal_distribution<>(Mean, StandardDeviation)(RandomNumberGenerator);
}

void UCustomNormalDistribution::LogSample()
{
	UE_LOG(LogTemp, Log, TEXT("%f"), DrawSample())
}