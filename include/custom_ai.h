#ifndef CUSTOM_AI_H
#define CUSTOM_AI_H

#include "types.h"
#include "battle.h"

#define CUSTOM_AI_MAX_MOVE_CANDIDATES 16

enum CustomAIStyle {
    CUSTOM_AI_STYLE_NONE = 0,
    CUSTOM_AI_STYLE_BASIC,
    CUSTOM_AI_STYLE_STANDARD,
    CUSTOM_AI_STYLE_BOSS,
};

enum CustomAIChoiceKind {
    CUSTOM_AI_CHOICE_NONE = 0,
    CUSTOM_AI_CHOICE_MOVE,
    CUSTOM_AI_CHOICE_SWITCH,
    CUSTOM_AI_CHOICE_ITEM,
};

struct CustomAIMoveCandidate {
    u8 moveSlot;
    u16 move;
    u8 target;
    s16 score;
    u32 flags;
};

struct CustomAIResult {
    enum CustomAIChoiceKind kind;
    u8 moveSlot;
    u16 move;
    u8 target;
    s16 score;
};

/*
 * Main entry point we will eventually call from a hook.
 * For now, this only returns CUSTOM_AI_CHOICE_NONE.
 */
struct CustomAIResult CustomAI_ChooseAction(
    struct BattleSystem *bw,
    struct BattleStruct *sp,
    u32 battler
);

/*
 * Trainer style system.
 * At first this is hardcoded. Later this can become a per-trainer table.
 */
enum CustomAIStyle CustomAI_GetStyle(
    struct BattleSystem *bw,
    struct BattleStruct *sp,
    u32 battler
);

void CustomAI_DebugObserve(
    struct BattleSystem *bw,
    struct BattleStruct *sp,
    u32 battler
);

void CustomAI_ForceFightCommand(
    struct BattleSystem *bw,
    struct BattleStruct *sp,
    u32 battler,
    u8 moveSlot,
    u8 target
);

#endif