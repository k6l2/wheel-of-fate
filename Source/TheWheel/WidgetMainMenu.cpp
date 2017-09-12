// Fill out your copyright notice in the Description page of Project Settings.
#include "TheWheel.h"
#include "WidgetMainMenu.h"
#include "Runtime/UMG/Public/UMG.h"
#include "WidgetOption.h"
//void UWidgetMainMenu::onHudConfirm()
//{
//    onAddOptionClicked();
//}
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
    editTextOptionName->SetKeyboardFocus();
    scrollBoxOptions->AddChild(widgetNewOption);
    // calculate how much % widgetNewOption is going to have based on # of children
    const int32 numChildren = scrollBoxOptions->GetChildrenCount();
    const float newPercent = 1.f / numChildren;
    widgetNewOption->percentage(newPercent);
    // subtract that % from each existing child
    //  % = %old - newPercent*%old
    for (int32 c = 0; c < numChildren - 1; c++)
    {
        auto child = scrollBoxOptions->GetChildAt(c);
        if (!child) continue;
        auto childOption = Cast<UWidgetOption>(child);
        if (!childOption) continue;
        childOption->percentage(childOption->percentage() - newPercent*childOption->percentage());
    }
}
void UWidgetMainMenu::onSpinTheWheelClicked()
{
    auto world = GetWorld();
    check(world);
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
    //FInputModeGameOnly inputMode;
    //world->GetFirstPlayerController()->SetInputMode(inputMode);
}
FEventReply UWidgetMainMenu::handleOnKeyDown(const FKeyEvent & InKeyEvent)
{
    auto world = GetWorld();
    check(world);
    auto pController = world->GetFirstPlayerController();
    check(pController);
    auto pInput = pController->PlayerInput;
    check(pInput);
    auto confirmKeys = pInput->GetKeysForAction("confirm");
    for (auto keyMapping : confirmKeys)
    {
        if (InKeyEvent.GetKey() == keyMapping.Key)
        {
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
                    TEXT("confirm key handled!"));
            }
            //onAddOptionClicked();
            return FEventReply(true);
        }
    }
    return FEventReply();// default FEventReply is "unhandled"
}
void UWidgetMainMenu::editTextOptionName_OnTextCommitted(
    const FText & Text,
    ETextCommit::Type CommitMethod)
{
    UWidget* widgetEditTextOptionName = GetWidgetFromName("editTextOptionName");
    check(widgetEditTextOptionName);
    UEditableTextBox* editTextOptionName = Cast<UEditableTextBox>(widgetEditTextOptionName);
    check(editTextOptionName);
    switch (CommitMethod)
    {
    case ETextCommit::Type::OnCleared:
        editTextOptionName->SetKeyboardFocus();
        break;
    case ETextCommit::Type::Default:
        break;
    case ETextCommit::Type::OnEnter:
        onAddOptionClicked();
        break;
    }
}