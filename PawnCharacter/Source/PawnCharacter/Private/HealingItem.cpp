#include "HealingItem.h"
#include "PawnPlayer.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (APawnPlayer* PlayerCharacter = Cast<APawnPlayer>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}

		Destroy();
	}
}