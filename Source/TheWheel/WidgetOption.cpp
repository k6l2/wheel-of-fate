// Fill out your copyright notice in the Description page of Project Settings.
#include "TheWheel.h"
#include "WidgetOption.h"
#include "Runtime/UMG/Public/UMG.h"
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
}