#include "../../include/custom_ai.h"
#include "../../include/debug.h"
#include "../../include/constants/moves.h"

enum CustomAIStyle CustomAI_GetStyle(
    struct BattleSystem *bsys UNUSED,
    struct BattleStruct *ctx UNUSED,
    u32 battler UNUSED
) {
    return CUSTOM_AI_STYLE_STANDARD;
}

static int CustomAI_BuildMoveCandidates(
    struct BattleSystem *bsys UNUSED,
    struct BattleStruct *ctx UNUSED,
    u32 battler UNUSED,
    struct CustomAIMoveCandidate *out UNUSED,
    int max UNUSED
) {
    return 0;
}

static s16 CustomAI_ScoreMoveCandidate(
    struct BattleSystem *bsys UNUSED,
    struct BattleStruct *ctx UNUSED,
    u32 battler UNUSED,
    struct CustomAIMoveCandidate *candidate UNUSED,
    enum CustomAIStyle style UNUSED
) {
    return 0;
}

struct CustomAIResult CustomAI_ChooseAction(
    struct BattleSystem *bsys,
    struct BattleStruct *ctx,
    u32 battler
) {
    struct CustomAIResult result;
    enum CustomAIStyle style;

    result.kind = CUSTOM_AI_CHOICE_NONE;
    result.moveSlot = 0;
    result.move = MOVE_NONE;
    result.target = 0;
    result.score = 0;

    style = CustomAI_GetStyle(bsys, ctx, battler);

#ifdef DEBUG_CUSTOM_AI
    debug_printf(
        "[CustomAI] battler=%d style=%d\n",
        battler,
        style
    );
#endif

    return result;
}

u8 LONG_CALL CustomAI_SelectMove(struct BattleSystem *bsys, int battler)
{
    struct BattleStruct *ctx = bsys->sp;

    u8 moveSlot = 0;
    u8 target = 0;

    /*
     * Temporary baseline:
     * choose the first usable damaging move.
     *
     * This confirms our custom AI hook is active while avoiding
     * endless Growl/Leer-style status spam.
     */
    for (u8 i = 0; i < 4; i++) {
        u16 move = ctx->battlemon[battler].move[i];

        if (move == MOVE_NONE)
            continue;

        if (ctx->battlemon[battler].pp[i] == 0)
            continue;

        if (ctx->moveTbl[move].power == 0)
            continue;

        moveSlot = i;
        break;
    }

    ctx->waza_no_pos[battler] = moveSlot;
    ctx->waza_no_select[battler] = ctx->battlemon[battler].move[moveSlot];
    ctx->aiWorkTable.ai_dir_select_client[battler] = target;

#ifdef DEBUG_CUSTOM_AI
    debug_printf(
        "[CustomAI_SelectMove] battler=%d moveSlot=%d move=%d target=%d\n",
        battler,
        moveSlot,
        ctx->waza_no_select[battler],
        target
    );
#endif

    return moveSlot;
}