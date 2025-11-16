/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "ScriptMgr.h"
#include "Item.h"
#include "Config.h"

class Player;

// Local constant to identify quest items (ItemClass = 12)
static constexpr uint32 ITEM_CLASS_QUEST_ID = 12u;
// Local constant to identify keys (ItemClass = 13)
static constexpr uint32 ITEM_CLASS_KEY_ID = 13u;

// Helper function to check if module is enabled
bool IsModuleEnabled()
{
    return sConfigMgr->GetOption<bool>("ChangeSellPrices.Enabled", true);
}

class ChangeSellPricesItem : public ItemScript
{
public:
    ChangeSellPricesItem() : ItemScript("ChangeSellPricesItem") { }

    bool OnItemSellPrice(Player* /*player*/, Item* item, uint32& sellPrice) override
    {
        if (!IsModuleEnabled())
            return true;

        // Exclude quest items
        const ItemTemplate* proto = item ? item->GetTemplate() : nullptr;
        if (!proto)
            return true;
        if (proto->Class == ITEM_CLASS_QUEST_ID || proto->Class == ITEM_CLASS_KEY_ID)
            return true;

        // Exclude quest-bonded items (bonding 4 or 5)
        if (proto->Bonding == 4u || proto->Bonding == 5u)
            return true;

        // Only modify items that currently have zero sell price
        if (sellPrice == 0)
        {
            uint32 newSellPrice = sConfigMgr->GetOption<uint32>("ChangeSellPrices.ChangeTo", 1000);
            sellPrice = newSellPrice; // make it sellable by assigning configured value
        }
        
        return true;
    }
};

void Addmod_change_sell_pricesScripts()
{
    new ChangeSellPricesItem();
}
