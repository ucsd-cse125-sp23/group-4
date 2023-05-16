#include "core/game/mode/GameModeBases.h"

#include "core/util/global.h"

#include <algorithm>
#include <random>

CountGameMode::CountGameMode() { }
void CountGameMode::registerTrackers(Level* level)
{
    GameMode::registerTrackers(level);

    level->statisticManager->registerStat("it_count", 0);
    level->eventManager->registerTaggingEventHandler([](TaggingEvent event) {
        if (Player* p = dynamic_cast<Player*>(event.tagee))
            p->level->statisticManager->setValue(
                p->pid, "it_count",
                std::get<int>(p->level->statisticManager->getValue(p->pid, "it_count")) +
                1);
        });
}
int CountGameMode::queryScore(uint32_t pid)
{
    return std::get<int>(level->statisticManager->getValue(pid, "it_count"));
}

TimeGameMode::TimeGameMode()
{
}
void TimeGameMode::registerTrackers(Level* level)
{
    GameMode::registerTrackers(level);

    level->statisticManager->registerStat("ticks_it", 0);
    level->eventManager->registerTaggingEventHandler([](TaggingEvent event) {
        if (Player* p = dynamic_cast<Player*>(event.tagger))
            p->level->statisticManager->setValue(
                p->pid, "ticks_it",
                std::get<int>(p->level->statisticManager->getValue(p->pid, "ticks_it")) +
                event.ticksIt);
        });
}
int TimeGameMode::queryScore(uint32_t pid)
{
    int ticksIt = std::get<int>(level->statisticManager->getValue(pid, "ticks_it"));
    Player* p = level->players[pid];
    TaggedStatusModifierData* data =
        static_cast<TaggedStatusModifierData*>(p->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get());
    if (data->isIt)
        ticksIt += level->getAge() - data->taggedTime;
    return ticksIt;
}

NTaggerGameMode::NTaggerGameMode(int n) : n(n) {}
void NTaggerGameMode::initPlayers(std::map<uint32_t, Player*> players)
{
    std::vector<Player*> pPerm;
    for (auto pair : players)
        pPerm.push_back(pair.second);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(pPerm.begin(), pPerm.end(), g);
    
    for (int i = 0; i < n; i++)
    {
        TaggedStatusModifierData* data =
            static_cast<TaggedStatusModifierData*>(pPerm[i]->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get());
        data->isIt = true;
        data->taggedTime = 0;
    }
}

TeamedTaggerGameMode::TeamedTaggerGameMode() {}
void TeamedTaggerGameMode::initPlayers(std::map<uint32_t, Player*> players)
{
    std::vector<Player*> pPerm;
    for (auto pair : players)
        pPerm.push_back(pair.second);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(pPerm.begin(), pPerm.end(), g);

    for (int i = 0; i < players.size() / 2; i++)
    {
        TaggedStatusModifierData* data =
            static_cast<TaggedStatusModifierData*>(pPerm[i]->getModifiers(TAGGED_STATUS_MODIFIER)[0]->get());
        data->isIt = true;
        data->taggedTime = 0;
    }
}