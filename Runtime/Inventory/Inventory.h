//
// Created by allos on 6/14/2024.
//

#ifndef MAX_INVENTORY_SIZE
#define MAX_INVENTORY_SIZE 8
#endif



#ifndef GAME_INVENTORY_H
#define GAME_INVENTORY_H



class Page {

public:

    Page() {
        init();
    }
    ~Page() {
        destroy();
    }

    virtual void init() {};
    virtual void destroy() {};

    virtual void renderLeftPage() {};
    virtual void renderRightPage() {  };

    virtual const char* getName() { return "UNKNOWN"; };
};


enum class ItemRarity : unsigned int {
    Common = 0,
    Uncommon = 1,
    Rare = 2
};

class Item {
public:
    virtual void init() {};
    virtual void update() {};
    virtual void destroy() {};

    ItemRarity rarity = ItemRarity::Common;
    const char* description = "UNKNOWN";
    const char* name = "UNKNOWN";
    unsigned int ItemID = 0;
};

/* ItemID map
 * 0 | UNKNOWN;
 * 1 | FishingRod
 */





#endif //GAME_INVENTORY_H
