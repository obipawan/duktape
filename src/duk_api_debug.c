/*
 *  Debugging related API calls
 */

#include "duk_internal.h"

void duk_dump_context_raw(duk_context *ctx) {
	int idx;
	int top;

	/* We don't duk_require_stack() here now, but rely on the caller having
	 * enough space.
	 */

	top = duk_get_top(ctx);
	duk_push_array(ctx);
	for (idx = 0; idx < top; idx++) {
		duk_dup(ctx, idx);
		duk_put_prop_index(ctx, -2, idx);
	}

	/* FIXME: conversion errors should not propagate outwards.
	 * Perhaps values need to be coerced individually?
	 */
	duk_bi_json_stringify_helper(ctx,
	                             duk_get_top_index(ctx),  /*idx_value*/
	                             DUK_INVALID_INDEX,  /*idx_replacer*/
	                             DUK_INVALID_INDEX,  /*idx_space*/
	                             DUK_JSON_FLAG_EXT_CUSTOM |
	                             DUK_JSON_FLAG_ASCII_ONLY |
	                             DUK_JSON_FLAG_AVOID_KEY_QUOTES /*flags*/);

	duk_push_sprintf(ctx, "ctx %p: top=%d, stack=%s", (void *) ctx, (int) top, duk_safe_to_string(ctx, -1));
	duk_replace(ctx, -3);  /* [ ... arr jsonx(arr) res ] -> [ ... res jsonx(arr) ] */
	duk_pop(ctx);
	DUK_ASSERT(duk_is_string(ctx, -1));
}
