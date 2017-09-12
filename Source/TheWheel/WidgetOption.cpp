// Fill out your copyright notice in the Description page of Project Settings.
#include "TheWheel.h"
#include "WidgetOption.h"
#include "Runtime/UMG/Public/UMG.h"
UWidgetOption::UWidgetOption(const FObjectInitializer & ObjectInitializer)
    :Super(ObjectInitializer)
    ,oldPercentage(1.f)
{
}
float UWidgetOption::percentage() const
{
    UWidget* widgetSliderProbability = GetWidgetFromName("sliderProbability");
    check(widgetSliderProbability);
    USlider* sliderProbability = Cast<USlider>(widgetSliderProbability);
    check(sliderProbability);
    return sliderProbability->GetValue();
}
void UWidgetOption::percentage(float newPercent)
{
    UWidget* widgetSliderProbability = GetWidgetFromName("sliderProbability");
    check(widgetSliderProbability);
    USlider* sliderProbability = Cast<USlider>(widgetSliderProbability);
    check(sliderProbability);
    sliderProbability->SetValue(newPercent);
    oldPercentage = newPercent;
}
void UWidgetOption::setOptionText(const FText& inText)
{
    UWidget* widgetTxtOption = GetWidgetFromName("txtOption");
    check(widgetTxtOption);
    UTextBlock* txtOption = Cast<UTextBlock>(widgetTxtOption);
    check(txtOption);
    txtOption->SetText(inText);
}
void UWidgetOption::onRemoveClicked()
{
    UPanelWidget* parent = GetParent();
    parent->RemoveChild(this);
    ///TODO: re-normalize the remaining childrens' percentages, 
    /// similar to how onProbabilityChanged does it...
}
void UWidgetOption::onProbabilityChanged(float newValue)
{
    UPanelWidget* parent = GetParent();
    UScrollBox* scrollBoxOptions = Cast<UScrollBox>(parent);
    check(scrollBoxOptions);
    UWidget* widgetSliderProbability = GetWidgetFromName("sliderProbability");
    check(widgetSliderProbability);
    USlider* sliderProbability = Cast<USlider>(widgetSliderProbability);
    check(sliderProbability);
    //if (GEngine)
    //{
    //    GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
    //        //TEXT("percentage=") + FString::SanitizeFloat(sliderProbability->GetValue()) +
    //        TEXT("oldPercentage=") + FString::SanitizeFloat(oldPercentage) +
    //        TEXT(" newValue=") + FString::SanitizeFloat(newValue));
    //}
    const int32 numChildren = scrollBoxOptions->GetChildrenCount();
    const int32 thisIndex = scrollBoxOptions->GetChildIndex(this);
    // first, get the total values of all other children
    float totalValues = 0.f;
    for (int32 c = 0; c < numChildren; c++)
    {
        if (c == thisIndex) continue;
        auto child = scrollBoxOptions->GetChildAt(c);
        if (!child) continue;
        auto childOption = Cast<UWidgetOption>(child);
        if (!childOption) continue;
        totalValues += childOption->percentage();
    }
    // then, figure out how much % we can distribute to all other children (1.f - newValue)
    const float remainder = 1.f - newValue;
    // next, for each child, figure how much of the remainder they get
    //  based on (value/totalValues)*remainder
    for (int32 c = 0; c < numChildren; c++)
    {
        if (c == thisIndex) continue;
        auto child = scrollBoxOptions->GetChildAt(c);
        if (!child) continue;
        auto childOption = Cast<UWidgetOption>(child);
        if (!childOption) continue;
        const float newPercentage = totalValues == 0 ? 0.f :// prevent NaN values
            (childOption->percentage() / totalValues)*remainder;
        childOption->percentage(newPercentage);
    }
    oldPercentage = newValue;
}