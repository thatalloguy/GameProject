//
// Created by allos on 8/9/2024.
//

#ifndef DUCKWATCHERS_DEBTCOLLECTOR_H
#define DUCKWATCHERS_DEBTCOLLECTOR_H

#include "Objects/Entity.h"
#include "../Core/FishingManager.h"
#include "../Core/TimeManager.h"
#include "imgui.h"
#include "DialogSystem.h"

namespace Characters {


    enum class CharacterState : unsigned int {
        Opening = 0,
        Wensday = 1,
        Disabled = 2,
        Talking = 3
    };

    class DebtCollector {
    public:

        DebtCollector() = default;
        ~DebtCollector();

        void initialize(Quack::Entity& baseEntity, TimeManager& timeManager);

        void update(Player& player);

        void drawUI(ImVec2 windowSize, Player& player);

    private:

        //void opening();

        Quack::Entity* _entity;
        TimeManager* _timeManager;
        CharacterState _currentState{CharacterState::Disabled}; // MUST BE OPENING!!
        bool hasYapped = false;

        //dialog;
        DialogBox opening {
            .author = "The Debt Collector",
            .dialogs = {
                    "Rise and Shine..",
                    "You inherited quite the place.",
                    "Same goes for your father's debt.",
                    "Now... He owed me 500 bucks.",
                    "Officially you have to pay right now.",
                    "But I see you don't have the money.",
                    "That's why I give you a week.",
                    "Now...",
                    "I have to go very important business."
            }
        };

        DialogBox reminder_1 {
            .author = "The Debt Collector",
            .dialogs = {
                    "We meet again.",
                    "I see the wallet is looking skinny",
                    "Skinny rabbits don't survive the winter...",
                    "But I am sure you will.",
                    "Good Luck I suppose"
            }
        };
        DialogBox reminder_2 {
                .author = "The Debt Collector",
                .dialogs = {
                        "That's a happy face.",
                        "I suppose the money is flowing.",
                        "Nice to hear that for once",
                        "Good luck...",
                        "But I don't suppose you need it!"
                }
        };
        DialogBox reminder_3 {
                .author = "The Debt Collector",
                .dialogs = {
                        "Ah I see that you've been working hard",
                        "That face, you need more sleep!",
                        "You almost look like the reaper himself!",
                        "Good luck... you need it!"
                }
        };

        DialogBox ending_1 {
            .author = "The Debt Collector",
            .dialogs = {
                    "Well, the debt is all paid.",
                    "Which I have to admit is a rare sight",
                    "Most of my clients don't pay the debt.",
                    "But you are one of the lucky ones.",
                    "Your father would be proud."
            }
        };

        DialogBox ending_2 {
            .author = "THE DEBT COLLECTOR?",
            .dialogs = {
                    "Well, you did not quite pay it...",
                    "Sad to see...  ",
                    "Look...",
                    "This isn't personal. ",
                    "Just business.."
            }
        };

        DialogBox ending_3 {
                .author = "??? ???? ??????????",
                .dialogs = {
                        "Well, you did not quite pay it...",
                        "Sad to see...  ",
                        "????? ?? ?? ?? ?",
                        "Your",
                        "Time",
                        "Has",
                        "Arrived"
                }
        };
    };

}
#endif //DUCKWATCHERS_DEBTCOLLECTOR_H
