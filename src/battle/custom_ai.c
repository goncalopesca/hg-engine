#include "../../include/custom_ai.h"
#include "../../include/debug.h"
#include "../../include/constants/moves.h"

/*
 * For now, this is intentionally simple.
 *
 * Later, this function should probably inspect the trainer ID.
 * hg-engine exposes BattleWork_GetTrainerIndex in rom.ld, so that is likely
 * useful later, but we are not touching it in the first base commit.
 */
enum CustomAIStyle CustomAI_GetStyle(
    struct BattleSystem *bw UNUSED,
    struct BattleStruct *sp UNUSED,
    u32 battler UNUSED
) {
    return CUSTOM_AI_STYLE_STANDARD;
}

/*
 * Placeholder candidate builder.
 *
 * This is deliberately not used yet. We are creating the architecture first,
 * then we will fill in legality, targeting, scoring, and final write-back.
 */
static int CustomAI_BuildMoveCandidates(
    struct BattleSystem *bw UNUSED,
    struct BattleStruct *sp UNUSED,
    u32 battler UNUSED,
    struct CustomAIMoveCandidate *out UNUSED,
    int max UNUSED
) {
    return 0;
}

/*
 * Placeholder scorer.
 *
 * Later this becomes the heart of the AI.
 */
static s16 CustomAI_ScoreMoveCandidate(
    struct BattleSystem *bw UNUSED,
    struct BattleStruct *sp UNUSED,
    u32 battler UNUSED,
    struct CustomAIMoveCandidate *candidate UNUSED,
    enum CustomAIStyle style UNUSED
) {
    return 0;
}

/*
 * Main custom AI function.
 *
 * Important: right now this does not override anything.
 * Returning CUSTOM_AI_CHOICE_NONE means "let vanilla / existing hg-engine AI continue."
 */
struct CustomAIResult CustomAI_ChooseAction(
    struct BattleSystem *bw,
    struct BattleStruct *sp,
    u32 battler
) {
    struct CustomAIResult result;
    enum CustomAIStyle style;

    result.kind = CUSTOM_AI_CHOICE_NONE;
    result.moveSlot = 0;
    result.move = MOVE_NONE;
    result.target = 0;
    result.score = 0;

    style = CustomAI_GetStyle(bw, sp, battler);

#ifdef DEBUG_CUSTOM_AI
    debug_printf(
        "[CustomAI] battler=%d style=%d\n",
        battler,
        style
    );
#endif

    return result;
}

void CustomAI_DebugObserve(
    struct BattleSystem *bw UNUSED,
    struct BattleStruct *sp,
    u32 battler
) {
#ifdef DEBUG_CUSTOM_AI
    debug_printf(
        "[CustomAI Observe] battler=%d move=%d slot=%d\n",
        battler,
        sp->waza_no_select[battler],
        sp->waza_no_pos[battler]
    );
#endif
}

void CustomAI_ForceFightCommand(
    struct BattleSystem *bw UNUSED,
    struct BattleStruct *sp,
    u32 battler,
    u8 moveSlot,
    u8 target
) {
    sp->playerActions[battler][0] = CONTROLLER_COMMAND_FIGHT_INPUT;
    sp->playerActions[battler][1] = target;
    sp->playerActions[battler][2] = moveSlot + 1;
    sp->playerActions[battler][3] = SELECT_FIGHT_COMMAND;

    sp->waza_no_pos[battler] = moveSlot;
    sp->waza_no_select[battler] = sp->battlemon[battler].move[moveSlot];

#ifdef DEBUG_CUSTOM_AI
    debug_printf(
        "[CustomAI] forced battler=%d moveSlot=%d move=%d target=%d\n",
        battler,
        moveSlot,
        sp->waza_no_select[battler],
        target
    );
#endif
}