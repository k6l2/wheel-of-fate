// Fill out your copyright notice in the Description page of Project Settings.
#include "TheWheel.h"
#include "WidgetMainMenu.h"
#include "Runtime/UMG/Public/UMG.h"
#include "WidgetOption.h"
#include "HudMainMenu.h"
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
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
            TEXT("UWidgetMainMenu::onSpinTheWheelClicked()"));
    }
    auto world = GetWorld();
    check(world);
    UWidget* widgetScrollBoxOptions = GetWidgetFromName("scrollBoxOptions");
    check(widgetScrollBoxOptions);
    UScrollBox* scrollBoxOptions = Cast<UScrollBox>(widgetScrollBoxOptions);
    check(scrollBoxOptions);
    const int32 numOptions = scrollBoxOptions->GetChildrenCount();
    if (numOptions < 1)
    {
        // we require >= 1 option for the wheel to actually work! //
        return;
    }
    // store the list of options inside scrollBoxOptions somewhere globally (in HUD for now) //
    auto hud = Cast<AHudMainMenu>(world->GetFirstPlayerController()->GetHUD());
    check(hud);
    // You need >= 3 choice colors to have distinct edges for each pie slice...
    static const TArray<FColor> CHOICE_COLORS = {
        {90,0,213},
        {0,255,0},
        {0,0,255}
    };
    float startPercent = 0;
    for (int32 c = 0; c < numOptions; c++)
    {
        auto child = scrollBoxOptions->GetChildAt(c);
        if (!child) continue;
        auto childOption = Cast<UWidgetOption>(child);
        if (!childOption) continue;
        FWheelChoice choice;
        choice.name = childOption->optionText().ToString();
        choice.percent = childOption->percentage();
        choice.percentStart = startPercent;
        startPercent += choice.percent;
        int32 colorIndex = c % CHOICE_COLORS.Num();
        // if the last color is the same as the first color...
        if (c == numOptions - 1 && colorIndex == 0)
        {
            // set the color to be the next color to prevent
            //  the case where two pie colors are touching
            colorIndex = (c + 1) % CHOICE_COLORS.Num();
        }
        choice.color = CHOICE_COLORS[colorIndex];
        hud->wheelChoices.Add(choice);
        UE_LOG(LogTemp, Warning, TEXT("choice name=%s percentStart=%f percent=%f"),
            *choice.name, choice.percentStart, choice.percent);
    }
    // generate the wheel's dynamic texture for our list of options //
    // https://wiki.unrealengine.com/Dynamic_Textures
    static const int32 CHOICES_TEXTURE_W = 512;
    static const int32 CHOICES_TEXTURE_H = 512;
    //static const int32 TOTAL_CHOICES_TEX_PIXELS = CHOICES_TEXTURE_W*CHOICES_TEXTURE_H;
    wheelChoicesTexture = UTexture2D::CreateTransient(CHOICES_TEXTURE_W, CHOICES_TEXTURE_H);
    //wheelChoicesTexture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
    //wheelChoicesTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
    //wheelChoicesTexture->SRGB = 0;
    //wheelChoicesTexture->UpdateResource();
    const int32 numMips = wheelChoicesTexture->PlatformData->Mips.Num();
    for (int32 mipLevel = 0; mipLevel < numMips; mipLevel++)
    {
        FTexture2DMipMap& mip = wheelChoicesTexture->PlatformData->Mips[mipLevel];
        const int32 mipTotalPixels = mip.SizeX*mip.SizeY;
        void* bulkData = mip.BulkData.Lock(LOCK_READ_WRITE);
        FColor* colorData = static_cast<FColor*>(bulkData);
        // write choices pixels to data //
        int32 wheelChoiceIndex = 0;
        for (int32 i = 0; i < mipTotalPixels; i++)
        {
            float pixelPercent = float(i) / mipTotalPixels;
            auto currChoice = hud->wheelChoices[wheelChoiceIndex];
            if (pixelPercent > currChoice.percentStart + currChoice.percent)
            {
                currChoice = hud->wheelChoices[++wheelChoiceIndex];
            }
            colorData[i] = currChoice.color;
        }
        mip.BulkData.Unlock();
        wheelChoicesTexture->UpdateResource();
    }
    // Create a dynamic texture for the wheel actor,
    //  and set the "texChoices" texture parameter to our new dynamic texture! //
    for (TActorIterator<AStaticMeshActor> ActorItr(world); ActorItr; ++ActorItr)
    {
        AStaticMeshActor* Mesh = *ActorItr;
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
                TEXT("\tMesh->GetName()=") + Mesh->GetName());
        }
        if (Mesh->GetName().Contains("wheel"))
        {
            UStaticMeshComponent* wheelStaticMeshComponent = Mesh->GetStaticMeshComponent();
            check(wheelStaticMeshComponent);
            wheelDynamicFrontMaterial = 
                wheelStaticMeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(
                    0, wheelStaticMeshComponent->GetMaterial(0));
            UTexture* prevTexChoices;
			FMaterialParameterInfo matParamInfo;
			matParamInfo.Name = "texChoices";
            wheelDynamicFrontMaterial->GetTextureParameterValue(matParamInfo, prevTexChoices);
            if (GEngine)
            {
                GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Yellow,
                    TEXT("prevTexChoices name=") + prevTexChoices->GetName());
            }
            wheelDynamicFrontMaterial->SetTextureParameterValue("texChoices", wheelChoicesTexture);
            break;
        }
    }
    // The user is done with the HUD, we can now remove it from the screen //
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
