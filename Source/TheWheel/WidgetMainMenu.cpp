// Fill out your copyright notice in the Description page of Project Settings.
#include "TheWheel.h"
#include "WidgetMainMenu.h"
#include "Runtime/UMG/Public/UMG.h"
#include "WidgetOption.h"
void UWidgetMainMenu::onAddOptionClicked()
{
    auto world = GetWorld();
    check(world);
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
            TEXT("btnAddOption clicked!"));
    }
    UWidget* widgetScrollBoxOptions = GetWidgetFromName("scrollBoxOptions");
    check(widgetScrollBoxOptions);
    UWidget* widgetEditTextOptionName = GetWidgetFromName("editTextOptionName");
    check(widgetEditTextOptionName);
    UEditableTextBox* editTextOptionName = Cast<UEditableTextBox>(widgetEditTextOptionName);
    check(editTextOptionName);
    if (editTextOptionName->GetText().IsEmpty())
    {
        return;
    }
    UScrollBox* scrollBoxOptions = Cast<UScrollBox>(widgetScrollBoxOptions);
    check(scrollBoxOptions);
    UWidgetOption* widgetNewOption = CreateWidget<UWidgetOption>(world, widgetTemplateOptions);
    check(widgetNewOption);
    widgetNewOption->setOptionText(editTextOptionName->GetText());
    editTextOptionName->SetText(FText::FromString(""));
    scrollBoxOptions->AddChild(widgetNewOption);
}
void UWidgetMainMenu::onSpinTheWheelClicked()
{
    UWidget* widgetScrollBoxOptions = GetWidgetFromName("scrollBoxOptions");
    check(widgetScrollBoxOptions);
    UScrollBox* scrollBoxOptions = Cast<UScrollBox>(widgetScrollBoxOptions);
    check(scrollBoxOptions);
    if (scrollBoxOptions->GetChildrenCount() < 1)
    {
        // we require >= 1 option for the wheel to actually work! //
        return;
    }
    /// TODO: store the list of options inside scrollBoxOptions somewhere globally probably
    /// TODO: generate the wheel's dynamic texture for our list of options
    RemoveFromViewport();
}