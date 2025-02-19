// Copyright (c) 2019 Chris Ohk, Youngjoong Kim, SeungHyun Jeon

// We are making my contributions/submissions to this project solely in our
// personal capacity and are not conveying any rights to any intellectual
// property of any third parties.

#include "doctest_proxy.hpp"

#include <Rosetta/Actions/Generic.hpp>
#include <Rosetta/Cards/Cards.hpp>
#include <Rosetta/Enums/CardEnums.hpp>
#include <Rosetta/Games/Game.hpp>
#include <Rosetta/Zones/DeckZone.hpp>
#include <Rosetta/Zones/FieldZone.hpp>
#include <Rosetta/Zones/GraveyardZone.hpp>
#include <Rosetta/Zones/HandZone.hpp>
#include <Rosetta/Zones/SecretZone.hpp>
#include <Rosetta/Zones/SetasideZone.hpp>

using namespace RosettaStone;

TEST_CASE("[Generic] - ShuffleIntoDeck")
{
    GameConfig config;
    config.player1Class = CardClass::ROGUE;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_START);

    Player* curPlayer = game.GetCurrentPlayer();
    Playable* coinCard =
        Entity::GetFromCard(curPlayer, Cards::FindCardByID("GAME_005"),
                            std::nullopt, curPlayer->GetHandZone());

    Generic::ShuffleIntoDeck(curPlayer, coinCard);
    CHECK_EQ(curPlayer->GetDeckZone()->GetCount(), 1);
    CHECK_EQ((*curPlayer->GetDeckZone())[0]->card->id, "GAME_005");
}

TEST_CASE("[Generic] - ShuffleIntoDeck_Full")
{
    GameConfig config;
    config.player1Class = CardClass::ROGUE;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = false;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_START);

    Player* curPlayer = game.GetCurrentPlayer();
    Playable* coinCard =
        Entity::GetFromCard(curPlayer, Cards::FindCardByID("GAME_005"),
                            std::nullopt, curPlayer->GetHandZone());

    for (std::size_t i = 0; i < MAX_DECK_SIZE; ++i)
    {
        Playable* tempCard =
            Entity::GetFromCard(curPlayer, Cards::FindCardByID("GAME_005"),
                                std::nullopt, curPlayer->GetHandZone());
        curPlayer->GetDeckZone()->Add(tempCard);
    }

    Generic::ShuffleIntoDeck(curPlayer, coinCard);
    CHECK_EQ(curPlayer->GetDeckZone()->GetCount(), MAX_DECK_SIZE);
}

TEST_CASE("[Generic] - GetZone")
{
    GameConfig config;
    config.player1Class = CardClass::ROGUE;
    config.player2Class = CardClass::PALADIN;
    config.startPlayer = PlayerType::PLAYER1;
    config.doFillDecks = true;
    config.autoRun = false;

    Game game(config);
    game.Start();
    game.ProcessUntil(Step::MAIN_START);

    Player* curPlayer = game.GetCurrentPlayer();

    CHECK_EQ(curPlayer->GetDeckZone(),
              Generic::GetZone(curPlayer, ZoneType::DECK));
    CHECK_EQ(curPlayer->GetFieldZone(),
              Generic::GetZone(curPlayer, ZoneType::PLAY));
    CHECK_EQ(curPlayer->GetGraveyardZone(),
              Generic::GetZone(curPlayer, ZoneType::GRAVEYARD));
    CHECK_EQ(curPlayer->GetHandZone(),
              Generic::GetZone(curPlayer, ZoneType::HAND));
    CHECK_EQ(curPlayer->GetSecretZone(),
              Generic::GetZone(curPlayer, ZoneType::SECRET));
    CHECK_EQ(curPlayer->GetSetasideZone(),
              Generic::GetZone(curPlayer, ZoneType::SETASIDE));
    CHECK_EQ(nullptr, Generic::GetZone(curPlayer, ZoneType::INVALID));
    CHECK_EQ(nullptr, Generic::GetZone(curPlayer, ZoneType::REMOVEDFROMGAME));
}