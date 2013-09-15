#include "Player.h"
#include <iostream>

Player::Player(PixMap *pixMap, int playerNum)
{
    this->pixMap = pixMap;
    this->playerNum = playerNum;

    gravity = -0.1f;
    drag = 0.99;

    health = 100; /** Make init attributes method **/
    damageWell = 0;
    energy = 2000;
    energyWell = 0;
    fuel = 100;
    power = 10;
    currency = 100;
    deathDamage = 100;
    deathRadius = 50;

    pos = Vect(0, 0);
    joint = pos;
    cannon = pos;
    velo = Vect (0, 0);

    fireCounter = 0;
    stepCounter = 0;
    accumDamage = 0;
    shakeAngle = rand() % 361;
    iconBob = 0;
    iconBobInc = 0.2;

    cannonAngle = 0;
    shellAngle = 0;

    availableProj.push_back({NORMAL, 3}); /** Temporary **/
    availableProj.push_back({SPLITTER, 1}); /** Temporary **/
    availableProj.push_back({NEBULA, 2}); /** Temporary **/
    availableProj.push_back({BURROWER, 2}); /** Temporary **/
    availableProj.push_back({TORRENT, 3}); /** Temporary **/
    availableProj.push_back({UNDERMINE, 3}); /** Temporary **/
    availableProj.push_back({SHARPSHOT, 3}); /** Temporary **/
    availableProj.push_back({EMP, 3}); /** Temporary **/
    availableProj.push_back({PYROCLASM, 2}); /** Temporary **/
    availableProj.push_back({CLUSTERBOMB, 3}); /** Temporary **/
    availableProj.push_back({RICOCHETRIFLE, 3}); /** Temporary **/
    availableProj.push_back({BLACKSHEEPWALL, 3}); /** Temporary **/
    availableProj.push_back({FIREWORKS, 3}); /** Temporary **/
    availableProj.push_back({SKIPPER, 3}); /** Temporary **/
    availableProj.push_back({EPICENTER, 3}); /** Temporary **/
    currentProjIndex = 10; /** Temporary **/

    isDead = false;
    dieCounter = 0;
    UIInit();
} // end constructor

Player::~Player()
{
    delete UI;
} // end destructor

Vect Player::rayCollide()
{
    Vect rayPos = pos;
    if (velo != Vect(0, 0)) {
        float angle = velo.getAngle() - 180;
        Vect rayVelo = createVect(angle, 1);
        while(pixMap->checkPixCollide(rayPos) && !pixMap->checkInBounds(rayPos))
        {
            rayPos.x += rayVelo.x;
            rayPos.y -= rayVelo.y;
        } // end while
    }
    return rayPos;
} // end rayCollide method

void Player::drawReticule()
{
} // end drawReticule method

void Player::stepEnergy()
{
    if (energyWell > 0) {
        energy -= 1;
        energyWell -= 1;
    }
    if (energy <= 0) {
        energyWell = 0;
        energy = 0;
    }
} // end damageStep method

void Player::stepDamage()
{
    if (damageWell > 0) {
        health -= 1;
        damageWell -= 1;
    }
    if (health <= 0) {
        damageWell = 0;
        health = 0;
        kill();
    }
} // end damageStep method

bool Player::dieStep()
{
    dieCounter += 1;

    float angle = (float)rand() / (float)RAND_MAX * 361;
    float newVelo = (float)rand() / (float)RAND_MAX * 5;
    Particle *newParticle = new ElectricParticle(pixMap, collisionBox.center, {cos(degreetoRad(angle)) * newVelo, sin(degreetoRad(angle)) * newVelo}, 60, false);
    newParticle->setColor({0.6f, 0.2f, 0.7f, 1.0f});
    pEng.pushParticle(newParticle);

    if (dieCounter < 60) {
        color.red += 0.01;
        color.green += 0.01;
        color.blue += 0.01;
    }
    if (dieCounter < 25) {
        mesh.applyForce(pos, -5, 0, 360);
        for (int i = 0; i < 15; i += 1) {
            float newAngle = (float)rand() / (float)RAND_MAX * 361;
            float distance = (float)rand() / (float)RAND_MAX * 70;
            Particle *newParticle = new AcceleratorLine(pixMap, {collisionBox.center.x + cos(degreetoRad(newAngle)) * distance, collisionBox.center.y + sin(degreetoRad(newAngle)) * distance},
                                                        {0, 0}, &pos, distance * 2, false);
            newParticle->setColor(color);
            pEng.pushParticle(newParticle);
        } // end for (i)
    } else if (dieCounter == 85){
        mesh.applyForce(pos, deathRadius, 0, 360);
        ParticleEmitter *newEmitter = new ParticleEmitter(pixMap, 1);
        newEmitter->setPos(collisionBox.center);
        newEmitter->setVelo({0.0f, 0.0f});
        newEmitter->setAngle(0, 360);
        newEmitter->setMaxParticles(20000);
        newEmitter->setMaxParticleDuration(100);
        pEng.pushEmitter(newEmitter);
        newEmitter = new ElectricEmitter(pixMap, 10);
        newEmitter->setPos(collisionBox.center);
        newEmitter->setVelo({0.0f, 0.0f});
        newEmitter->setAngle(0, 360);
        newEmitter->setMaxParticles(100);
        newEmitter->setMaxParticleDuration(60);
        pEng.pushEmitter(newEmitter);
        pixMap->pushRemove(collisionBox.center, deathRadius);
    } else if (dieCounter == 90) {
        pixMap->checkPlayerDamages(collisionBox.center, deathRadius, deathDamage); /** should outright kill but doesn't **/
        return true;
    }
    return false;
} // end dieStep method

void Player::UIInit()
{
    // WINDOW
        // AUDIO MENU
        Container *audioMenu = new Container(Vect(30, 50), WINWIDTH - 60, WINHEIGHT - 120, 1.0f, true, FADEOUT, true);
        audioMenu->setDefaultTexture("Media/audioMenu.bmp");

        // MAIN MENU
        Container *mainMenu = new Container(Vect(30, 50), WINWIDTH - 60, WINHEIGHT - 120, 1.0f, true, FADEOUT, true);
        mainMenu->setDefaultTexture("Media/mainMenu.bmp");

        Container *header = createHeader(mainMenu, audioMenu);
        Container *shop = createShop();
        Container *footer = createFooter(shop);

    UI = new Container(Vect(0, 0), WINWIDTH, WINHEIGHT, 1.0f, false, NONE, true);
    UI->addEntity(shop);
    UI->addEntity(header);
    UI->addEntity(footer);
    UI->addEntity(audioMenu);
    UI->addEntity(mainMenu);
} // end setUI method

Container *Player::createHeader(UIEntity *mainMenu, UIEntity *audioMenu)
{
        // EXIT BUTTON
        Button *exitButton = new Button(Vect(WINWIDTH - 20, 12), 13, 13, 1.0f, false, NONE);
        exitButton->setDefaultTexture("Media/exitButton.bmp");

        // AUDIO MENU BUTTON
        Button *audioMenuButton = new Button(Vect(25, 11), 15, 15, 1.0f, false, NONE);
        audioMenuButton->setDefaultTexture("Media/audioButton.bmp");
        audioMenuButton->addEntity(audioMenu, TOGGLEFADE, 20);
        audioMenuButton->addEntity(mainMenu, FADEOUT, 20);

        // MAIN MENU BUTTON
        Button *mainMenuButton = new Button(Vect(5, 11), 15, 15, 1.0f, false, NONE);
        mainMenuButton->setDefaultTexture("Media/menuButton.bmp");
        mainMenuButton->addEntity(mainMenu, TOGGLEFADE, 20);
        mainMenuButton->addEntity(audioMenu, FADEOUT, 20);

        // TURN TIMER
        turnTimer = new Timer(Vect(WINWIDTH / 2 - 40, 12), 80, 14, 1.0f, false, NONE);
        turnTimer->setDefaultTexture("Media/timer.bmp");

    Container *header = new Container(Vect(0, 0), WINWIDTH, 30, 1.0f, false, NONE, true);
    header->setDefaultTexture("Media/header.bmp");
    header->addEntity(mainMenuButton);
    header->addEntity(audioMenuButton);
    header->addEntity(exitButton);
    header->addEntity(turnTimer);
    return header;
} // end createHeader method

Container *Player::createFooter(UIEntity *shop)
{
        // HEALTH BAR
        healthBar = new PercentageBar(Vect(5, 2), 128, 16, 1.0f, false, &health, health, NONE);
        healthBar->setDefaultTexture("Media/HealthBar.bmp");

        // ENERGY BAR
        energyBar = new PercentageBar(Vect(5, 23), 128, 4, 1.0f, false, &energy, energy, NONE);
        energyBar->setDefaultTexture("Media/EnergyBar.bmp");

        // FUEL BAR
        fuelBar = new PercentageBar(Vect(5, 32), 128, 4, 1.0f, false, &fuel, fuel, NONE);
        fuelBar->setDefaultTexture("Media/FuelBar.bmp");

        // WEAPON SELECTOR
        weapons = new Container(Vect(200, 10), 100, 10, 1.0f, false, NONE, true);
        weapons->setDefaultTexture("Media/toolTip.bmp");

        // SHOP BUTTON CONTAINER
            // SHOP BUTTON
            Button *shopButton = new Button(Vect(5, 5), 50, 20, 1.0f, false, NONE);
            shopButton->setDefaultTexture("Media/ShopButton.bmp");
            shopButton->addEntity(shop, TOGGLEDOWNSLIDE, 40);

        Container *shopButtonContainer = new Container(Vect(WINWIDTH - 220, 5), 150, 30, 1.0f, false, NONE, true);
        shopButtonContainer->setDefaultTexture("Media/ShopButtonContainer.bmp");
        shopButtonContainer->addEntity(shopButton);

    Container *footer = new Container(Vect(0, WINHEIGHT - 41), WINWIDTH, 41, 1.0f, false, NONE, true);
    footer->setDefaultTexture("Media/footer.bmp");
    footer->addEntity(healthBar);
    footer->addEntity(energyBar);
    footer->addEntity(fuelBar);
    footer->addEntity(shopButtonContainer);
    footer->addEntity(weapons);
    return footer;
} // end createFooter method

Container *Player::createShop()
{
        // SHOP PREVIEW WINDOW
            // WEAPON PREVIEW WINDOW
            Container *weaponPreviewWindow = new Container(Vect(0, 20), 180, 280, 1.0f, false, NONE, true);
            weaponPreviewWindow->setDefaultTexture("Media/PreviewWindow.bmp");

            // SHIP PREVIEW WINDOW
            Container *shipPreviewWindow = new Container(Vect(0, 20), 180, 280, 1.0f, true, FADEOUT, true);
            shipPreviewWindow->setDefaultTexture("Media/PreviewWindow.bmp");

        Container *shopPreviewWindow = new Container(Vect(0, 20), 180, 280, 1.0f, false, NONE, true);
        shopPreviewWindow->addEntity(weaponPreviewWindow);
        shopPreviewWindow->addEntity(shipPreviewWindow);

        // SHOP MAIN WINDOW
            // WEAPON BUY LIST
                // WEAPON LISTING AREA
                    // TERRAFORMING SORT LIST
                        // TERRAFORMING SORT TEXT
                        TextArea *sortText = new TextArea(Vect (0, 0), 190, 20, 1.0f, false, NONE);
                        sortText->setDefaultTexture("Media/footer.bmp");
                        sortText->setText("TERRAFORMING");

                    Container *terraformingSortList = new Container(Vect(0, 10), 190, 260, 1.0f, false, NONE, true);
                    terraformingSortList->addEntity(sortText);

                    // DESTRUCTIVE SORT LIST
                        // DESTRUCTIVE SORT TEXT
                        sortText = new TextArea(Vect (0, 0), 190, 20, 1.0f, false, NONE);
                        sortText->setDefaultTexture("Media/footer.bmp");
                        sortText->setText("DESTRUCTIVE");

                    Container *destructiveSortList = new Container(Vect(0, 10), 190, 260, 1.0f, true, FADEOUT, true);
                    destructiveSortList->addEntity(sortText);

                    // UTILITY SORT LIST
                        // UTILITY SORT TEXT
                        sortText = new TextArea(Vect (0, 0), 190, 20, 1.0f, false, NONE);
                        sortText->setDefaultTexture("Media/footer.bmp");
                        sortText->setText("UTILITY");

                    Container *weaponUtilitySortList = new Container(Vect(0, 10), 190, 260, 1.0f, true, FADEOUT, true);
                    weaponUtilitySortList->addEntity(sortText);

                Container *weaponListingArea = new Container(Vect(5, 20), 190, 260, 1.0f, false, NONE, true);
                weaponListingArea->addEntity(terraformingSortList);
                weaponListingArea->addEntity(destructiveSortList);
                weaponListingArea->addEntity(weaponUtilitySortList);

                // TERRAFORMING SORT
                Button *terraformingSort = new Button (Vect(65, 6), 20, 20, 1.0f, false, NONE);
                terraformingSort->setDefaultTexture("Media/footer.bmp");
                terraformingSort->addEntity(terraformingSortList, FADEIN);
                terraformingSort->addEntity(destructiveSortList, FADEOUT);
                terraformingSort->addEntity(weaponUtilitySortList, FADEOUT);

                // DESTRUCTIVE SORT
                Button *destructiveSort = new Button (Vect (90, 6), 20, 20, 1.0f, false, NONE);
                destructiveSort->setDefaultTexture("Media/footer.bmp");
                destructiveSort->addEntity(terraformingSortList, FADEOUT);
                destructiveSort->addEntity(destructiveSortList, FADEIN);
                destructiveSort->addEntity(weaponUtilitySortList, FADEOUT);

                // UTILITY SORT
                Button *weaponUtilitySort = new Button(Vect(115, 6), 20, 20, 1.0f, false, NONE);
                weaponUtilitySort->setDefaultTexture("Media/footer.bmp");
                weaponUtilitySort->addEntity(terraformingSortList, FADEOUT);
                weaponUtilitySort->addEntity(destructiveSortList, FADEOUT);
                weaponUtilitySort->addEntity(weaponUtilitySortList, FADEIN);

            Container *weaponBuyList = new Container(Vect (0, 15), 200, 310, 1.0f, false, NONE, true);
            weaponBuyList->setDefaultTexture("Media/subtleTexture1.bmp");
            weaponBuyList->addEntity(terraformingSort);
            weaponBuyList->addEntity(destructiveSort);
            weaponBuyList->addEntity(weaponUtilitySort);
            weaponBuyList->addEntity(weaponListingArea);

            // SHIP BUY LIST
                // SHIP LISTING AREA
                    // MOVEMENT SORT LIST
                        // MOVEMENT SORT TEXT
                        sortText = new TextArea(Vect (0, 0), 190, 20, 1.0f, false, NONE);
                        sortText->setDefaultTexture("Media/footer.bmp");
                        sortText->setText("MOVEMENT");

                    Container *movementSortList = new Container(Vect(0, 10), 190, 260, 1.0f, false, NONE, true);
                    movementSortList->addEntity(sortText);

                    // DEFENSIVE SORT LIST
                        // DEFENSIVE SORT TEXT
                        sortText = new TextArea(Vect (0, 0), 190, 20, 1.0f, false, NONE);
                        sortText->setDefaultTexture("Media/footer.bmp");
                        sortText->setText("DEFENSIVE");

                    Container *defensiveSortList = new Container(Vect(0, 10), 190, 260, 1.0f, true, FADEOUT, true);
                    defensiveSortList->addEntity(sortText);

                    // OFFENSIVE SORT LIST
                        // OFFENSIVE SORT TEXT
                        sortText = new TextArea(Vect (0, 0), 190, 20, 1.0f, false, NONE);
                        sortText->setDefaultTexture("Media/footer.bmp");
                        sortText->setText("OFFENSIVE");

                    Container *offensiveSortList = new Container(Vect(0, 10), 190, 260, 1.0f, true, FADEOUT, true);
                    offensiveSortList->addEntity(sortText);

                    // UTILITY SORT LIST
                        // UTILITY SORT TEXT
                        sortText = new TextArea(Vect (0, 0), 190, 20, 1.0f, false, NONE);
                        sortText->setDefaultTexture("Media/footer.bmp");
                        sortText->setText("UTILITY");

                    Container *shipUtilitySortList = new Container(Vect(0, 10), 190, 260, 1.0f, true, FADEOUT, true);
                    shipUtilitySortList->addEntity(sortText);

                Container *shipListingArea = new Container(Vect(5, 20), 190, 260, 1.0f, false, NONE, true);
                shipListingArea->addEntity(movementSortList);
                shipListingArea->addEntity(defensiveSortList);
                shipListingArea->addEntity(offensiveSortList);
                shipListingArea->addEntity(shipUtilitySortList);

                // MOVEMENT SORT
                Button *movementSort = new Button (Vect(52.5, 6), 20, 20, 1.0f, false, NONE);
                movementSort->setDefaultTexture("Media/toolTip.bmp");
                movementSort->addEntity(movementSortList, FADEIN);
                movementSort->addEntity(defensiveSortList, FADEOUT);
                movementSort->addEntity(offensiveSortList, FADEOUT);
                movementSort->addEntity(shipUtilitySortList, FADEOUT);

                // DEFENCE SORT
                Button *defenceSort = new Button (Vect(77.5, 6), 20, 20, 1.0f, false, NONE);
                defenceSort->setDefaultTexture("Media/toolTip.bmp");
                defenceSort->addEntity(movementSortList, FADEOUT);
                defenceSort->addEntity(defensiveSortList, FADEIN);
                defenceSort->addEntity(offensiveSortList, FADEOUT);
                defenceSort->addEntity(shipUtilitySortList, FADEOUT);

                // OFFENCE SORT
                Button *offenceSort = new Button (Vect(102.5, 6), 20, 20, 1.0f, false, NONE);
                offenceSort->setDefaultTexture("Media/toolTip.bmp");
                offenceSort->addEntity(movementSortList, FADEOUT);
                offenceSort->addEntity(defensiveSortList, FADEOUT);
                offenceSort->addEntity(offensiveSortList, FADEIN);
                offenceSort->addEntity(shipUtilitySortList, FADEOUT);

                // UTILITY SORT
                Button *shipUtilitySort = new Button (Vect(127.5, 6), 20, 20, 1.0f, false, NONE);
                shipUtilitySort->setDefaultTexture("Media/toolTip.bmp");
                shipUtilitySort->addEntity(movementSortList, FADEOUT);
                shipUtilitySort->addEntity(defensiveSortList, FADEOUT);
                shipUtilitySort->addEntity(offensiveSortList, FADEOUT);
                shipUtilitySort->addEntity(shipUtilitySortList, FADEIN);

            Container *shipBuyList = new Container(Vect(0, 15), 200, 310, 1.0f, true, FADEOUT, true);
            shipBuyList->setDefaultTexture("Media/subtleTexture1.bmp");
            shipBuyList->addEntity(movementSort);
            shipBuyList->addEntity(defenceSort);
            shipBuyList->addEntity(offenceSort);
            shipBuyList->addEntity(shipUtilitySort);
            shipBuyList->addEntity(shipListingArea);

            // WEAPON BUY LIST BUTTON
            Button *weaponBuyListButton = new Button (Vect (0, 0), 100, 15, 1.0f, false, NONE);
            weaponBuyListButton->setDefaultTexture("Media/toolTip.bmp");
            weaponBuyListButton->addEntity(weaponBuyList, FADEIN, 20);
            weaponBuyListButton->addEntity(shipBuyList, FADEOUT, 20);
            weaponBuyListButton->addEntity(weaponPreviewWindow, FADEIN);
            weaponBuyListButton->addEntity(shipPreviewWindow, FADEOUT);

            // SHIP BUY LIST BUTTON
            Button *shipBuyListButton = new Button (Vect (100, 0), 100, 15, 1.0f, false, NONE);
            shipBuyListButton->setDefaultTexture("Media/footer.bmp");
            shipBuyListButton->addEntity(weaponBuyList, FADEOUT, 20);
            shipBuyListButton->addEntity(shipBuyList, FADEIN, 20);
            shipBuyListButton->addEntity(weaponPreviewWindow, FADEOUT);
            shipBuyListButton->addEntity(shipPreviewWindow, FADEIN);

        Container *shopMainWindow = new Container(Vect(180, 0), 200, 320, 1.0f, false, NONE, true);
        shopMainWindow->addEntity(weaponBuyList);
        shopMainWindow->addEntity(weaponBuyListButton);
        shopMainWindow->addEntity(shipBuyList);
        shopMainWindow->addEntity(shipBuyListButton);

    // ITEM LIST MODE
    Button *listMode = new Button(Vect (380, 0), 20, 20, 1.0f, false, NONE);
    listMode->setDefaultTexture("Media/ListMode.bmp");

    // ITEM ICON MODE
    Button *iconMode = new Button(Vect (380, 20), 20, 20, 1.0f, false, NONE);
    iconMode->setDefaultTexture("Media/IconMode.bmp");

    Container *shop = new Container(Vect(WINWIDTH - 400, 50), 400, 320, 1.0f, true, SLIDEDOWNHIDE, true);
    shop->addEntity(shopPreviewWindow);
    shop->addEntity(shopMainWindow);
    shop->addEntity(listMode);
    shop->addEntity(iconMode);

    setWeaponPurchaseUI(weaponPreviewWindow, terraformingSortList, destructiveSortList, weaponUtilitySortList, listMode, iconMode);
    setShipPurchaseUI(shipPreviewWindow, movementSortList, defensiveSortList, offensiveSortList, shipUtilitySortList, listMode, iconMode);

    return shop;
} // end createShop method

void Player::setWeaponPurchaseUI(Container *weaponPreviewWindow, Container *terraformingList, Container *destructiveList, Container *utilityList, Button *listMode,Button *iconMode)
{
    Vect padding(5, 5);
    Container *terraformingListList = new Container(Vect(0, 15), 190, 245, 1.0f, false, NONE, false);
    terraformingListList->setDefaultTexture("Media/ListingArea.bmp");
    terraformingList->addEntity(terraformingListList);
    Container *terraformingIconList = new Container(Vect(0, 15), 190, 245, 1.0f, true, FADEOUT, false);
    terraformingIconList->setDefaultTexture("Media/ListingArea.bmp");
    terraformingIconList->setPadding(padding);
    terraformingList->addEntity(terraformingIconList);

    Container *destructiveListList = new Container(Vect(0, 15), 190, 245, 1.0f, false, NONE,false);
    destructiveListList->setDefaultTexture("Media/ListingArea.bmp");
    destructiveList->addEntity(destructiveListList);
    Container *destructiveIconList = new Container(Vect(0, 15), 190, 245, 1.0f, true, FADEOUT, false);
    destructiveIconList->setDefaultTexture("Media/ListingArea.bmp");
    destructiveIconList->setPadding(padding);
    destructiveList->addEntity(destructiveIconList);

    Container *utilityListList = new Container(Vect(0, 15), 190, 245, 1.0f, false, NONE, false);
    utilityListList->setDefaultTexture("Media/ListingArea.bmp");
    utilityList->addEntity(utilityListList);
    Container *utilityIconList = new Container(Vect(0, 15), 190, 245, 1.0f, true, FADEOUT, false);
    utilityIconList->setDefaultTexture("Media/ListingArea.bmp");
    utilityIconList->setPadding(padding);
    utilityList->addEntity(utilityIconList);

    listMode->addEntity(terraformingListList, FADEIN);
    listMode->addEntity(destructiveListList, FADEIN);
    listMode->addEntity(utilityListList, FADEIN);
    listMode->addEntity(terraformingIconList, FADEOUT);
    listMode->addEntity(destructiveIconList, FADEOUT);
    listMode->addEntity(utilityIconList, FADEOUT);

    iconMode->addEntity(terraformingListList, FADEOUT);
    iconMode->addEntity(destructiveListList, FADEOUT);
    iconMode->addEntity(utilityListList, FADEOUT);
    iconMode->addEntity(terraformingIconList, FADEIN);
    iconMode->addEntity(destructiveIconList, FADEIN);
    iconMode->addEntity(utilityIconList, FADEIN);

    Container *weaponTestPreview;
    Container *testListWeapon;
    Container *testListWeaponIcon;
    Container *testIconWeapon;

    std::vector <char *> iconList = {"Media/barrage.bmp", "Media/blacksheepWall.bmp", "Media/blackhole.bmp", "Media/fireworks.bmp", "Media/Liquifier.bmp", "Media/buildMisc.bmp", "Media/nano.bmp", "Media/sunburst.bmp", "Media/warp.bmp"}; /** temp **/
    std::vector <char *> typeList = {"destructive", "terraforming", "destructive", "destructive", "terraforming", "terraforming", "terraforming", "destructive", "utility"}; /** Temp **/

    for (int i = 0; i < 9; i += 1) {
        /** parse weapon data **/
        weaponTestPreview = new Container(Vect(10, 10), 160, 270, 1.0f, false, NONE, true);
        weaponTestPreview->setDefaultTexture("Media/toolTip.bmp");
        weaponPreviewWindow->addEntity(weaponTestPreview);

        testListWeaponIcon = new Container(Vect(1, 1), 40, 23, 1.0f, false, NONE, true);
        testListWeaponIcon->setDefaultTexture((char*)iconList[i]); /** make safer cast **/

        testListWeapon = new Container (Vect (0, 0), 190, 25, 1.0f, false, NONE, true);
        testListWeapon->setDefaultTexture("Media/toolTip.bmp");
        testListWeapon->addEntity(testListWeaponIcon);

        testIconWeapon = new Container (Vect (0, 0), 40, 23, 1.0f, false, NONE, true);
        testIconWeapon->setDefaultTexture((char*)iconList[i]); /** make safer cast **/

        if (typeList[i] == "terraforming") {
            terraformingListList->addEntity(testListWeapon);
            terraformingIconList->addEntity(testIconWeapon);
        } else if (typeList[i] == "destructive") {
            destructiveListList->addEntity(testListWeapon);
            destructiveIconList->addEntity(testIconWeapon);
        } else {
            utilityListList->addEntity(testListWeapon);
            utilityIconList->addEntity(testIconWeapon);
        }
    } // end for (i)
} // end setWeaponPurchaseButtons method

void Player::setShipPurchaseUI(Container *shipPreviewWindow, Container *movementList, Container *defenceList, Container *offenceList, Container *utilityList, Button *listMode, Button *iconMode)
{
    Vect padding(5, 5);
    Container *movementListList = new Container(Vect(0, 15), 190, 245, 1.0f, false, NONE, false);
    movementListList->setDefaultTexture("Media/ListingArea.bmp");
    movementList->addEntity(movementListList);
    Container *movementIconList = new Container(Vect(0, 15), 190, 245, 1.0f, true, FADEOUT, false);
    movementIconList->setDefaultTexture("Media/ListingArea.bmp");
    movementIconList->setPadding(padding);
    movementList->addEntity(movementIconList);

    Container *defenceListList = new Container(Vect(0, 15), 190, 245, 1.0f, false, NONE,false);
    defenceListList->setDefaultTexture("Media/ListingArea.bmp");
    defenceList->addEntity(defenceListList);
    Container *defenceIconList = new Container(Vect(0, 15), 190, 245, 1.0f, true, FADEOUT, false);
    defenceIconList->setDefaultTexture("Media/ListingArea.bmp");
    defenceIconList->setPadding(padding);
    defenceList->addEntity(defenceIconList);

    Container *offenceListList = new Container(Vect(0, 15), 190, 245, 1.0f, false, NONE, false);
    offenceListList->setDefaultTexture("Media/ListingArea.bmp");
    offenceList->addEntity(offenceListList);
    Container *offenceIconList = new Container(Vect(0, 15), 190, 245, 1.0f, true, FADEOUT, false);
    offenceIconList->setDefaultTexture("Media/ListingArea.bmp");
    offenceIconList->setPadding(padding);
    offenceList->addEntity(offenceIconList);

    Container *utilityListList = new Container(Vect(0, 15), 190, 245, 1.0f, false, NONE, false);
    utilityListList->setDefaultTexture("Media/ListingArea.bmp");
    utilityList->addEntity(utilityListList);
    Container *utilityIconList = new Container(Vect(0, 15), 190, 245, 1.0f, true, FADEOUT, false);
    utilityIconList->setDefaultTexture("Media/ListingArea.bmp");
    utilityIconList->setPadding(padding);
    utilityList->addEntity(utilityIconList);

    listMode->addEntity(movementListList, FADEIN);
    listMode->addEntity(defenceListList, FADEIN);
    listMode->addEntity(offenceListList, FADEIN);
    listMode->addEntity(utilityListList, FADEIN);
    listMode->addEntity(movementIconList, FADEOUT);
    listMode->addEntity(defenceIconList, FADEOUT);
    listMode->addEntity(offenceIconList, FADEOUT);
    listMode->addEntity(utilityIconList, FADEOUT);

    iconMode->addEntity(movementListList, FADEOUT);
    iconMode->addEntity(defenceListList, FADEOUT);
    iconMode->addEntity(offenceListList, FADEOUT);
    iconMode->addEntity(utilityListList, FADEOUT);
    iconMode->addEntity(movementIconList, FADEIN);
    iconMode->addEntity(defenceIconList, FADEIN);
    iconMode->addEntity(offenceIconList, FADEIN);
    iconMode->addEntity(utilityIconList, FADEIN);
} // end setShipPurchaseButtons method

void Player::addMovements()
{
    Vect force;
    unsigned int numForces = moveForces.size();
     for (std::vector <Vect>::iterator i = moveForces.begin(); i < moveForces.end(); i += 1) {
        force.x += (*i).x; /** Make a vect function for adding Vects **/
        force.y += (*i).y; /** Make a vect function for adding Vects **/
        moveForces.erase(i);
    } // end for (i)
    if ((pixMap->checkPixCollide(pos) && !pixMap->checkPixLiquid(pos)) && force.y == 0) {
        float slopeAngle = degreetoRad(pixMap->getSlopeAngle(pos));
        if (force.x > 0 && (radtoDegree(slopeAngle) < 65)) {
            pos.x += cos(slopeAngle) / 2;
            pos.y -= sin(slopeAngle) / 2;
        } else if (force.x < 0 && (radtoDegree(slopeAngle) > -65)) {
            pos.x -= cos(slopeAngle) / 2;
            pos.y += sin(slopeAngle) / 2;
        }
    } else {
        if (numForces > 0) {
            createJetParticles(velo);
            addForce(force);
        }
        if (force.x < 0 && shellAngle < 55) {
            shellAngle += 2;
        } else if (force.x > 0 && shellAngle > -55) {
            shellAngle -= 2;
        } else if (force.x == 0) {
            shellAngle -= shellAngle / 35;
        }
    }
} // end addMovements method

void Player::setAngle(float cannonAngle)
{
    this->cannonAngle = cannonAngle;
} // end setAngle method

void Player::setPower(float power)
{
    this->power = power;
} // end setPower method

void Player::setPosition(Vect pos)
{
    this->pos = pos;
} // end setPosition method

void Player::setColor(Color color)
{
    this->color = color;
} // end setColor method

float Player::getAngle()
{
    return cannonAngle;
} // end getAngle method

Vect Player::getPosition()
{
    return pos;
} // end getPosition method

bool Player::checkCollision(Vect pos)
{
    return collisionBox.doesCollide(pos);
} // end checkCollision method

bool Player::checkRadialCollision(Vect pos, float radius)
{
    return collisionBox.doesRadialCollide(pos, radius);
} // end checkRadialCollision

void Player::nextProjectile()
{
    if (currentProjIndex == (int)availableProj.size() - 1) {
        currentProjIndex = 0;
    } else {
        currentProjIndex += 1;
    }
} // end nextProjectile method

void Player::previousProjectile()
{
    if (currentProjIndex == 0) {
        currentProjIndex = (int)availableProj.size() - 1;
    } else {
        currentProjIndex -= 1;
    }
} // end nextProjectile method

void Player::fireProjectile()
{
    fireCounter = 1;
    mesh.applyForce(cannon, 10, 0, 360);
    ParticleEmitter *newEmitter = new FireEmitter(pixMap, 10); /** Temporary duration **/
    newEmitter->setPos(cannon);
    newEmitter->setVelo({0.0f, 0.0f}); /** Temporary **/
    newEmitter->setAngle(cannonAngle - 15, cannonAngle + 25); /** Temporary Angles **/
    newEmitter->setMaxParticles(100);  /** Temporary Max Particles **/
    newEmitter->setMaxParticleDuration(150); /** Temporary maxDuration **/
    pEng.pushEmitter(newEmitter);
    Projectile *newProjectile;
    Vect newVelo({cos(degreetoRad(cannonAngle)) * power, -sin(degreetoRad(cannonAngle)) * power});

    int currentProj = availableProj[currentProjIndex].first;
    int level = availableProj[currentProjIndex].second;
    if (currentProj == NORMAL) {
        newProjectile = new Projectile (pixMap, cannon, newVelo, level);
    } else if (currentProj == NEBULA) {
        newProjectile = new Nebula (pixMap, cannon, newVelo, level);
    } else if (currentProj == TORRENT) {
        newProjectile = new Torrent(pixMap, cannon, newVelo, level);
    } else if (currentProj == SPLITTER) {
        newProjectile = new Splitter(pixMap, cannon, newVelo, level);
    } else if (currentProj == BURROWER) {
        newProjectile = new Burrower(pixMap, cannon, newVelo, power * 5, level);
    } else if (currentProj == UNDERMINE) {
        newProjectile = new Undermine(pixMap, cannon, newVelo, level);
    } else if (currentProj == SHARPSHOT) {
        newProjectile = new SharpShot(pixMap, cannon, newVelo, level);
    } else if (currentProj == EMP) {
        newProjectile = new Emp(pixMap, cannon, newVelo, level);
    } else if (currentProj == PYROCLASM) {
        newProjectile = new Pyroclasm(pixMap, cannon, newVelo, level);
    } else if (currentProj == CLUSTERBOMB) {
        newProjectile = new ClusterBomb(pixMap, cannon, newVelo, level);
    } else if (currentProj == RICOCHETRIFLE) {
        newProjectile = new RicochetRifle(pixMap, cannon, newVelo, level);
    } else if (currentProj == BLACKSHEEPWALL) {
        newProjectile = new BlackSheepWall(pixMap, cannon, newVelo, level);
    } else if (currentProj == FIREWORKS) {
        newProjectile = new Fireworks(pixMap, cannon, newVelo, level);
    } else if (currentProj == SKIPPER) {
        newProjectile = new Skipper(pixMap, cannon, newVelo, level);
    } else if (currentProj == EPICENTER) {
        newProjectile = new Epicenter(pixMap, cannon, newVelo, level);
    }
    int energyCost = newProjectile->getEnergyCost();
    if (energy - energyWell >= energyCost) {
        energyWell += energyCost;
        pixMap->pushProjectile(newProjectile);
    }
} // end fireProjectile method

void Player::damage(float damage)
{
    if (!isDead) {
        accumDamage += damage;
    }
} // end damage method

void Player::stepIcon()
{
    iconBob += iconBobInc;
    if (iconBob > 5 || iconBob < -5)
        iconBobInc *= -1;
} // end stepIcon method

void Player::drawIcon()
{
    glColor({1.0f, 0.2f, 0.3f, 1.0f});
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(pos.x, pos.y - 25 + iconBob);
        glVertex2f(pos.x - 3, pos.y - 35 + iconBob);
        glVertex2f(pos.x - 3, pos.y - 30 + iconBob);
        glVertex2f(pos.x - 7, pos.y - 30 + iconBob);
        glVertex2f(pos.x, pos.y - 23 + iconBob);
        glVertex2f(pos.x + 7, pos.y - 30 + iconBob);
        glVertex2f(pos.x + 3, pos.y - 30 + iconBob);
        glVertex2f(pos.x + 3, pos.y - 35 + iconBob);
        glVertex2f(pos.x - 3, pos.y - 35 + iconBob);
    glEnd();
} // end drawIcon method

void Player::click(Vect mPos)
{
    UI->onClick(mPos);
} // end click method

void Player::release(Vect mPos)
{
    UI->onRelease(mPos);
} // end release method

void Player::stepUI(Vect mPos)
{
    UI->step(mPos);
} // end stepUI method

void Player::drawUI()
{
    UI->draw(UI->getPos(), UI->getAlpha());
} // end drawAttributeBars method

bool Player::checkTurnTimer()
{
    return turnTimer->checkTimer();
} // end checkTurnTimer method

void Player::resetTurnTimer()
{
    turnTimer->resetTimer();
} // end resetTurnTimer method

void Player::draw()
{
    drawCannon();
    drawShell();
    drawReticule();
    /** Light Effect, Remove Later possibly **/
    glColor4f(color.red, color.green, color.blue, 0.02f + (float)dieCounter / 4000);
    for (int i = 0; i < 40; i += 2) {
        glBegin(GL_TRIANGLE_FAN);
            glVertexVect(collisionBox.center);
            for (int j = 0; j <= 360; j += 1)
                glVertex2f(joint.x + cos(degreetoRad(j)) * i, joint.y + 5 + sin(degreetoRad(j)) * i);
        glEnd();
    } // end for (i)
} // end draw method

bool Player::step()/** Split into different functions **/
{
    if (health <= 60 && rand() % (int)((health + 10) / 10) == 0) { /** Alter possibly **/
        float angle = (float)rand() / (float)RAND_MAX * 40 + 70;
        float newVelo = (float)rand() / (float)RAND_MAX / 4;
        Particle *newParticle = new SmokeParticle(pixMap, collisionBox.center, {cos(degreetoRad(angle)) * newVelo, sin(degreetoRad(angle)) * newVelo}, 100, true, false);
        newParticle->setColor({0.2f, 0.2f, 0.2f, 0.6f});
        pEng.pushParticle(newParticle);
    }
    if (accumDamage > 0 && stepCounter % 10 == 0) {
        std::ostringstream oss;
        oss << accumDamage;
        Particle *newParticle = new DamageParticle(pixMap, collisionBox.center, Vect((float)rand() / (float)RAND_MAX * 0.2f - 0.1f, 1.2f), oss.str(), 280, true, false);
        newParticle->setColor({1.0f, 0.1f, 0.1f, 1.0f});
        pEng.pushParticle(newParticle);
        damageWell = accumDamage;
        accumDamage = 0;
    }
    if (pos.x - 13 + velo.x < 0) { /** Make based on collision box **/
        pos.x = 13;
        velo.x = 0;
    } else if (pos.x  + 13 + velo.x > mapWidth - 1) { /** Make based on collision box **/
        pos.x = mapWidth - 1 - 13;
        velo.x = 0;
    } else {
        pos.x += velo.x;
    }
    if (pos.y + velo.y > mapHeight - 1) { /** Make based on collision box **/
        pos.y = mapHeight - 1;
        velo.y = 0;
    } else {
        pos.y -= velo.y;
    }

    setRelationPoints();

    if ((pixMap->checkPixCollide(pos) && !pixMap->checkPixLiquid(pos)) || pos.y == mapWidth - 1) {
        shellAngle = pixMap->getSlopeAngle(pos);
        velo.x = 0;
        velo.y = 0;
    } else {
        velo.x *= drag;
        velo.y *= drag;

        velo.y += gravity;
    }

    stepDamage();
    stepEnergy();
    addMovements();

    stepCounter += 1;
    if (isDead) {
        return dieStep();
    }
    return false;
} // end step method

void Player::move(Vect force)
{
    moveForces.push_back(force);
} // end move method

void Player::addForce(Vect force)
{
    velo.x += force.x;
    velo.y += force.y;
} // end applyForce method

void Player::kill()
{
    isDead = true;
} // end kill method

Tank::Tank(PixMap *pixMap, int playerNum) : Player(pixMap, playerNum)
{
    cannonLength = 15;

    joint = {pos.x + cos(degreetoRad(shellAngle + 90)) * 9, pos.y - sin(degreetoRad(shellAngle + 90)) * 9};
    cannon = {joint.x + cos(degreetoRad(cannonAngle)) * cannonLength, joint.y + sin(degreetoRad(cannonAngle)) * cannonLength};

    Vect jetPos1(pos.x + cos(degreetoRad(shellAngle + 158.2)) * 5.39, pos.y - sin(degreetoRad(shellAngle + 158.2)) * 5.39);
    jets.push_back(jetPos1);
    Vect jetPos2(pos.x + cos(degreetoRad(shellAngle + 21.8)) * 5.39, pos.y - sin(degreetoRad(shellAngle + 21.8)) * 5.39);
    jets.push_back(jetPos2);

    collisionBox.topLeft = {pos.x + cos(degreetoRad(shellAngle + 135)) * 18.38, pos.y - sin(degreetoRad(shellAngle + 135)) * 18.38};
    collisionBox.topRight = {pos.x + cos(degreetoRad(shellAngle + 45)) * 18.38, pos.y - sin(degreetoRad(shellAngle + 45)) * 18.38};
    collisionBox.bottomRight = {pos.x + cos(degreetoRad(shellAngle)) * 13, pos.y - sin(degreetoRad(shellAngle)) * 13};
    collisionBox.bottomLeft = {pos.x + cos(degreetoRad(shellAngle + 180)) * 13, pos.y - sin(degreetoRad(shellAngle + 180)) * 13};
    collisionBox.center = {pos.x + cos(degreetoRad(shellAngle + 90)) * 13 / 2, pos.y - sin(degreetoRad(shellAngle + 90)) * 13 / 2};
} // end constructor

void Tank::drawCannon()
{
    glColor4f(color.red, color.green, color.blue, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertexVect(joint);
        glVertexVect(cannon);
        glVertex2f(cannon.x + cos(degreetoRad(cannonAngle)) * 5 - cos(degreetoRad(cannonAngle + 90)) * 2, cannon.y + sin(degreetoRad(cannonAngle)) * 5 - sin(degreetoRad(cannonAngle + 90)) * 2);
        glVertex2f(joint.x - cos(degreetoRad(cannonAngle + 90)) * 2, joint.y - sin(degreetoRad(cannonAngle + 90)) * 2);
    glEnd();
} // end drarCannon method

void Tank::drawShell()
{
    glColor4f(color.red, color.green, color.blue, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertexVect(pos);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 163)) * 13.6, pos.y - sin(degreetoRad(shellAngle + 163)) * 13.6);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 126)) * 8.6, pos.y - sin(degreetoRad(shellAngle + 126)) * 8.6);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 90)) * 5, pos.y - sin(degreetoRad(shellAngle + 90)) * 5);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 54)) * 8.6, pos.y - sin(degreetoRad(shellAngle + 54)) * 8.6);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 17)) * 13.6, pos.y - sin(degreetoRad(shellAngle + 17)) * 13.6);
    glEnd();
    glBegin(GL_LINE_LOOP);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 90)) * 5, pos.y - sin(degreetoRad(shellAngle + 90)) * 5);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 129.66)) * 12.8, pos.y - sin(degreetoRad(shellAngle + 129.66)) * 12.8);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 90)) * 13, pos.y - sin(degreetoRad(shellAngle + 90)) * 13);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 51.34)) * 12.8, pos.y - sin(degreetoRad(shellAngle + 51.34)) * 12.8);
    glEnd();
} // end drawShell method

void Tank::setRelationPoints()
{
    joint = {pos.x + cos(degreetoRad(shellAngle + 90)) * 9, pos.y - sin(degreetoRad(shellAngle + 90)) * 9};
    cannon = {joint.x + cos(degreetoRad(cannonAngle)) * cannonLength, joint.y + sin(degreetoRad(cannonAngle)) * cannonLength};
    jets[0] = {pos.x + cos(degreetoRad(shellAngle + 158.2)) * 5.39, pos.y - sin(degreetoRad(shellAngle + 158.2)) * 5.39};
    jets[1] = {pos.x + cos(degreetoRad(shellAngle + 21.8)) * 5.39, pos.y - sin(degreetoRad(shellAngle + 21.8)) * 5.39};
    collisionBox.topLeft = {pos.x + cos(degreetoRad(shellAngle + 135)) * 18.38, pos.y - sin(degreetoRad(shellAngle + 135)) * 18.38};
    collisionBox.topRight = {pos.x + cos(degreetoRad(shellAngle + 45)) * 18.38, pos.y - sin(degreetoRad(shellAngle + 45)) * 18.38};
    collisionBox.bottomRight = {pos.x + cos(degreetoRad(shellAngle)) * 13, pos.y - sin(degreetoRad(shellAngle)) * 13};
    collisionBox.bottomLeft = {pos.x + cos(degreetoRad(shellAngle + 180)) * 13, pos.y - sin(degreetoRad(shellAngle + 180)) * 13};
    collisionBox.center = {pos.x + cos(degreetoRad(shellAngle + 90)) * 13 / 2, pos.y - sin(degreetoRad(shellAngle + 90)) * 13 / 2};
} // end setRelationPoint

void Tank::createJetParticles(Vect force)
{
    for (std::vector<Vect>::iterator i = jets.begin(); i < jets.end(); i += 1)
    {
        if (force.x > 1.0) {
            force.x = 1.0;
        } else if (force.x < -1.0) {
            force.x = -1.0;
        }
        if (fabs(force.y) > 1.0)
            force.y = 1.0;
        Particle *newParticle = new LineParticle(pixMap, (*i), {-force.x, -fabs(force.y)}, 60, true, false);
        newParticle->setColor({0.8f, 0.1f, 0.1f, 1.0f});
        pEng.pushParticle(newParticle);
    } // end for (i)
} // end createJetParticles method

SpaceInvader::SpaceInvader(PixMap *pixMap, int playerNum) : Player(pixMap, playerNum)
{
    color = {1.0f, 1.0f, 1.0f, 1.0f};
    size = 2;
    jets.push_back(pos);

    collisionBox.topLeft = {pos.x - 6 * size, pos.y - 8 * size};
    collisionBox.topRight = {pos.x + 6 * size, pos.y - 8 * size};
    collisionBox.bottomRight = {pos.x + 6 * size, pos.y};
    collisionBox.bottomLeft = {pos.x - 6 * size, pos.y};
    collisionBox.center = {pos.x, pos.y - 4 * size};
} // end constructor

void SpaceInvader::drawCannon()
{
} // end drarCannon method

void SpaceInvader::drawShell()
{
    /** Make better :P **/
    glColor4f(color.red, color.green, color.blue, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(pos.x - 1 * size, pos.y);
        glVertex2f(pos.x - 3 * size, pos.y);
        glVertex2f(pos.x - 3 * size, pos.y - 1 * size);
        glVertex2f(pos.x - 1 * size, pos.y - 1 * size);

        glVertex2f(pos.x + 1 * size, pos.y);
        glVertex2f(pos.x + 3 * size, pos.y);
        glVertex2f(pos.x + 3 * size, pos.y - 1 * size);
        glVertex2f(pos.x + 1 * size, pos.y - 1 * size);

        glVertex2f(pos.x - 3 * size, pos.y - 1 * size);
        glVertex2f(pos.x - 4 * size, pos.y - 1 * size);
        glVertex2f(pos.x - 4 * size, pos.y - 6 * size);
        glVertex2f(pos.x - 3 * size, pos.y - 6 * size);

        glVertex2f(pos.x + 3 * size, pos.y - 1 * size);
        glVertex2f(pos.x + 4 * size, pos.y - 1 * size);
        glVertex2f(pos.x + 4 * size, pos.y - 6 * size);
        glVertex2f(pos.x + 3 * size, pos.y - 6 * size);

        glVertex2f(pos.x - 4 * size, pos.y - 3 * size);
        glVertex2f(pos.x - 5 * size, pos.y - 3 * size);
        glVertex2f(pos.x - 5 * size, pos.y - 5 * size);
        glVertex2f(pos.x - 4 * size, pos.y - 5 * size);

        glVertex2f(pos.x + 4 * size, pos.y - 3 * size);
        glVertex2f(pos.x + 5 * size, pos.y - 3 * size);
        glVertex2f(pos.x + 5 * size, pos.y - 5 * size);
        glVertex2f(pos.x + 4 * size, pos.y - 5 * size);

        glVertex2f(pos.x + 5 * size, pos.y - 1 * size);
        glVertex2f(pos.x + 6 * size, pos.y - 1 * size);
        glVertex2f(pos.x + 6 * size, pos.y - 4 * size);
        glVertex2f(pos.x + 5 * size, pos.y - 4 * size);

        glVertex2f(pos.x - 5 * size, pos.y - 1 * size);
        glVertex2f(pos.x - 6 * size, pos.y - 1 * size);
        glVertex2f(pos.x - 6 * size, pos.y - 4 * size);
        glVertex2f(pos.x - 5 * size, pos.y - 4 * size);

        glVertex2f(pos.x - 2 * size, pos.y - 2 * size);
        glVertex2f(pos.x - 3 * size, pos.y - 2 * size);
        glVertex2f(pos.x - 3 * size, pos.y - 4 * size);
        glVertex2f(pos.x - 2 * size, pos.y - 4 * size);

        glVertex2f(pos.x + 2 * size, pos.y - 2 * size);
        glVertex2f(pos.x + 3 * size, pos.y - 2 * size);
        glVertex2f(pos.x + 3 * size, pos.y - 4 * size);
        glVertex2f(pos.x + 2 * size, pos.y - 4 * size);

        glVertex2f(pos.x - 2 * size, pos.y - 5 * size);
        glVertex2f(pos.x - 3 * size, pos.y - 5 * size);
        glVertex2f(pos.x - 3 * size, pos.y - 7 * size);
        glVertex2f(pos.x - 2 * size, pos.y - 7 * size);

        glVertex2f(pos.x + 2 * size, pos.y - 5 * size);
        glVertex2f(pos.x + 3 * size, pos.y - 5 * size);
        glVertex2f(pos.x + 3 * size, pos.y - 7 * size);
        glVertex2f(pos.x + 2 * size, pos.y - 7 * size);

        glVertex2f(pos.x - 3 * size, pos.y - 7 * size);
        glVertex2f(pos.x - 4 * size, pos.y - 7 * size);
        glVertex2f(pos.x - 4 * size, pos.y - 8 * size);
        glVertex2f(pos.x - 3 * size, pos.y - 8 * size);

        glVertex2f(pos.x + 3 * size, pos.y - 7 * size);
        glVertex2f(pos.x + 4 * size, pos.y - 7 * size);
        glVertex2f(pos.x + 4 * size, pos.y - 8 * size);
        glVertex2f(pos.x + 3 * size, pos.y - 8 * size);

        glVertex2f(pos.x - 1 * size, pos.y - 2 * size);
        glVertex2f(pos.x - 2 * size, pos.y - 2 * size);
        glVertex2f(pos.x - 2 * size, pos.y - 6 * size);
        glVertex2f(pos.x - 1 * size, pos.y - 6 * size);

        glVertex2f(pos.x + 1 * size, pos.y - 2 * size);
        glVertex2f(pos.x + 2 * size, pos.y - 2 * size);
        glVertex2f(pos.x + 2 * size, pos.y - 6 * size);
        glVertex2f(pos.x + 1 * size, pos.y - 6 * size);

        glVertex2f(pos.x + 1 * size, pos.y - 2 * size);
        glVertex2f(pos.x - 1 * size, pos.y - 2 * size);
        glVertex2f(pos.x - 1 * size, pos.y - 6 * size);
        glVertex2f(pos.x + 1 * size, pos.y - 6 * size);
    glEnd();
} // end drawShell method

void SpaceInvader::setRelationPoints()
{
    cannon.x = pos.x;
    cannon.y = pos.y - 2;
    joint.x = pos.x;
    joint.y = pos.y - 8;
    jets[0] = pos;
    collisionBox.topLeft = {pos.x - 6 * size, pos.y - 8 * size};
    collisionBox.topRight = {pos.x + 6 * size, pos.y - 8 * size};
    collisionBox.bottomRight = {pos.x + 6 * size, pos.y};
    collisionBox.bottomLeft = {pos.x - 6 * size, pos.y};
    collisionBox.center = {pos.x, pos.y - 4 * size};
} // end setRelationPoint

void SpaceInvader::createJetParticles(Vect force)
{
    for (std::vector<Vect>::iterator i = jets.begin(); i < jets.end(); i += 1)
    {
    } // end for (i)
} // end createJetParticles method

GeometryShip::GeometryShip(PixMap *pixMap, int playerNum) : Player(pixMap, playerNum)
{
    color = {0.4f, 0.2f, 0.9f, 1.0f};
    Vect jetPos(pos);
    jets.push_back(jetPos);

    collisionBox.topLeft = {pos.x + cos(degreetoRad(shellAngle + 125.54)) * 17.2, pos.y - sin(degreetoRad(shellAngle + 125.54)) * 17.2};
    collisionBox.topRight = {pos.x + cos(degreetoRad(shellAngle + 54.46)) * 17.2, pos.y - sin(degreetoRad(shellAngle + 54.46)) * 17.2};
    collisionBox.bottomRight = {pos.x + cos(degreetoRad(shellAngle)) * 10, pos.y - sin(degreetoRad(shellAngle)) * 10};
    collisionBox.bottomLeft = {pos.x + cos(degreetoRad(shellAngle + 180)) * 10, pos.y - sin(degreetoRad(shellAngle)) * 10};
    collisionBox.center = {pos.x + cos(degreetoRad(shellAngle + 90)) * 13.99 / 2, pos.y - sin(degreetoRad(shellAngle + 90)) * 13.99 / 2};
} // end constructor

void GeometryShip::drawCannon()
{
} // end drarCannon method

void GeometryShip::drawShell()
{
    glColor4f(color.red, color.green, color.blue, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertexVect(pos);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 149.04)) * 11.66, pos.y - sin(degreetoRad(shellAngle + 149.04)) * 11.66);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 110)) * 14.86, pos.y - sin(degreetoRad(shellAngle + 110)) * 14.86);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 138.82)) * 10.63, pos.y - sin(degreetoRad(shellAngle + 138.82)) * 10.63);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 90)) * 4, pos.y - sin(degreetoRad(shellAngle + 90)) * 4);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 41.18)) * 10.63, pos.y - sin(degreetoRad(shellAngle + 41.18)) * 10.63);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 70)) * 14.86, pos.y - sin(degreetoRad(shellAngle + 70)) * 14.86);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 30.96)) * 11.66, pos.y - sin(degreetoRad(shellAngle + 30.96)) * 11.66);
    glEnd();
} // end drawShell method

void GeometryShip::setRelationPoints()
{
    joint = {pos.x + cos(degreetoRad(shellAngle + 90)) * 9, pos.y - sin(degreetoRad(shellAngle + 90)) * 9};
    cannon = joint;
    jets[0] = pos;
    collisionBox.topLeft = {pos.x + cos(degreetoRad(shellAngle + 125.54)) * 17.2, pos.y - sin(degreetoRad(shellAngle + 125.54)) * 17.2};
    collisionBox.topRight = {pos.x + cos(degreetoRad(shellAngle + 54.46)) * 17.2, pos.y - sin(degreetoRad(shellAngle + 54.46)) * 17.2};
    collisionBox.bottomRight = {pos.x + cos(degreetoRad(shellAngle)) * 10, pos.y - sin(degreetoRad(shellAngle)) * 10};
    collisionBox.bottomLeft = {pos.x + cos(degreetoRad(shellAngle + 180)) * 10, pos.y - sin(degreetoRad(shellAngle)) * 10};
    collisionBox.center = {pos.x + cos(degreetoRad(shellAngle + 90)) * 13.99 / 2, pos.y - sin(degreetoRad(shellAngle + 90)) * 13.99 / 2};
} // end setRelationPoint

void GeometryShip::createJetParticles(Vect force)
{
    for (std::vector<Vect>::iterator i = jets.begin(); i < jets.end(); i += 1)
    {
        float angle = radtoDegree(atan2(-force.y, force.x));
        ParticleEmitter *newEmitter = new ParticleEmitter(pixMap, 1); /** Temporary duration **/
        newEmitter->setPos(pos);
        newEmitter->setVelo({0.0f, 0.0f}); /** Temporary **/
        newEmitter->setAngle(angle + 170, angle + 190); /** Temporary Angles **/
        newEmitter->setMaxParticles(5);  /** Temporary Max Particles **/
        newEmitter->setMaxParticleDuration(50); /** Temporary maxDuration **/
        pEng.pushEmitter(newEmitter);
    } // end for (i)
} // end createJetParticles method

Motherload::Motherload(PixMap *pixMap, int playerNum) : Player(pixMap, playerNum)
{
    color = {0.4f, 0.2f, 0.9f, 1.0f};
    Vect jetPos(pos);
    jets.push_back(jetPos);

    collisionBox.topLeft = {pos.x + cos(degreetoRad(shellAngle + 125.54)) * 17.2, pos.y - sin(degreetoRad(shellAngle + 125.54)) * 17.2};
    collisionBox.topRight = {pos.x + cos(degreetoRad(shellAngle + 54.46)) * 17.2, pos.y - sin(degreetoRad(shellAngle + 54.46)) * 17.2};
    collisionBox.bottomRight = {pos.x + cos(degreetoRad(shellAngle)) * 10, pos.y - sin(degreetoRad(shellAngle)) * 10};
    collisionBox.bottomLeft = {pos.x + cos(degreetoRad(shellAngle + 180)) * 10, pos.y - sin(degreetoRad(shellAngle)) * 10};
} // end constructor

void Motherload::drawCannon()
{
} // end drarCannon method

void Motherload::drawShell()
{
    glColor4f(color.red, color.green, color.blue, 1.0f);
    glBegin(GL_TRIANGLE_FAN);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 90)) * 8, pos.y - sin(degreetoRad(shellAngle + 90)) * 8);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle)) * 8, pos.y - sin(degreetoRad(shellAngle)) * 8);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 45)) * 11.31, pos.y - sin(degreetoRad(shellAngle + 45)) * 11.31);
        for (int angle = shellAngle; angle <= shellAngle + 180; angle += 5) {
            glVertex2f(pos.x + cos(degreetoRad(shellAngle + 90)) * 8 + cos(degreetoRad(angle)) * 8, pos.y - sin(degreetoRad(shellAngle + 90)) * 8 - sin(degreetoRad(angle)) * 8);
        } // end for (angle)
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 135)) * 11.31, pos.y - sin(degreetoRad(shellAngle + 135)) * 11.31);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle + 180)) * 8, pos.y - sin(degreetoRad(shellAngle + 180)) * 8);
        glVertex2f(pos.x + cos(degreetoRad(shellAngle)) * 8, pos.y - sin(degreetoRad(shellAngle)) * 8);
    glEnd();
} // end drawShell method

void Motherload::setRelationPoints()
{
    joint = {pos.x + cos(degreetoRad(shellAngle + 90)) * 9, pos.y - sin(degreetoRad(shellAngle + 90)) * 9};
    cannon = joint;
    jets[0] = Vect(pos.x + cos(degreetoRad(shellAngle + 90)) * 8 + cos(degreetoRad(135)) * 8, pos.y - sin(degreetoRad(shellAngle + 90)) * 8 - sin(degreetoRad(135)) * 8);
    collisionBox.topLeft = {pos.x + cos(degreetoRad(shellAngle + 116.57)) * 17.88, pos.y - sin(degreetoRad(shellAngle + 116.57)) * 17.88};
    collisionBox.topRight = {pos.x + cos(degreetoRad(shellAngle + 63.43)) * 17.88, pos.y - sin(degreetoRad(shellAngle + 63.43)) * 17.88};
    collisionBox.bottomRight = {pos.x + cos(degreetoRad(shellAngle)) * 8, pos.y - sin(degreetoRad(shellAngle)) * 8};
    collisionBox.bottomLeft = {pos.x + cos(degreetoRad(shellAngle + 180)) * 8, pos.y - sin(degreetoRad(shellAngle)) * 8};
    collisionBox.center = {pos.x + cos(degreetoRad(shellAngle + 90)) * 8, pos.y - sin(degreetoRad(shellAngle + 90)) * 8};
} // end setRelationPoint

void Motherload::createJetParticles(Vect force)
{
    for (std::vector<Vect>::iterator i = jets.begin(); i < jets.end(); i += 1)
    {
        float angle = (float)rand() / (float)RAND_MAX * 40 + 70;
        float newVelo = (float)rand() / (float)RAND_MAX / 4;
        Particle *newParticle = new SmokeParticle(pixMap, collisionBox.center, {cos(degreetoRad(angle)) * newVelo, sin(degreetoRad(angle)) * newVelo}, 20, true, false);
        newParticle->setColor({0.2f, 0.2f, 0.2f, 0.6f});
        pEng.pushParticle(newParticle);
    } // end for (i)
} // end createJetParticles method
