#include "consts.h"
#include "generals.h"

// These tables are initialized into RAM.

const char *table_prompts[] = {
	s_table_prompt_start,
	s_table_prompt_end,
	s_table_prompt_step
};

const char *ratio_optn[] = {
	s_ratio_optn_c,
	s_ratio_optn_d
};

const char *verif_result[] = {
	s_verif_result_false,
	s_verif_result_true
};

const char *ineq_result[] = {
	s_ineq_allreal,
	s_ineq_nosolution
};

// Seems to be unused. RAM variables are accessed directly via their addresses without using this pointer.
const char **ram_main_start = &d_080DC;
