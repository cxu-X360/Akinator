#include "Tree.h"

#define MAX_FILENAME_SIZE 64

const char* error_names[-COUNT_ERRORS+1] = {"HUYNA_PTR_TREE",
	"HUYNA_PTR_ROOT",
	"WRONG_VALUES",
};

#define NIL_COLOR "\"#36343b\""
#define NODE_COLOR "blue"

#define ERROR_INFO(error) ((!error) ? "no error" : (error_names[-(error)-1]))
#define ERROR_COLOR(error) ((!error) ? "darkgreen" : "\"#590c0e\"")
#define BORDER_COLOR (!left_nd_ctx.node || !right_nd_ctx.node) ? "\"#ee1414\"" : "white"

typedef struct
{
    FILE* file;
    Tree_t* tree;
} TrContext;

typedef struct
{
    Node_t* node;
    size_t num_node;
} NdContext;


static void _create_head_info(TrContext* ctx)
{
    fprintf(ctx->file, 
		"digraph Tree\n{\n"
		"\tgraph [rankdir=TB, bgcolor=\"#FAE3FF\", pad=0.25, nodesep=0.25, ranksep=0.35];\n"
		"\tnode [shape=record, fixedsize=false, style=\"rounded,filled\", width=0.6, fontname=\"monospace\", fontsize=10, fontcolor=white, color=white fillcolor="NODE_COLOR", penwidth=1.3, margin=\"0.13,0.06\"];\n"
		"\tedge [color=black, arrowsize=0.75, fontname=\"monospace\", fontsize=9, penwidth=1.3];\n"
	);
}

static void _create_error_info(TrContext* ctx)
{
	fprintf(ctx->file, "\"Info\" [label = \"ERRORNO = %d(%s)} \", shape=record, fixedsize=false, fillcolor = %s]\n",
			ctx->tree->error, ERROR_INFO(ctx->tree->error), ERROR_COLOR(ctx->tree->error));
}

static void _create_node(TrContext* tr_ctx, NdContext* nd_ctx)
{
    if (!nd_ctx->node) 
    {
        return;
    }

    NdContext left_nd_ctx = {.node = nd_ctx->node->left, .num_node = (nd_ctx->num_node<<1)+1};
    NdContext right_nd_ctx = {.node = nd_ctx->node->right, .num_node = (nd_ctx->num_node<<1)+2};

 
    fprintf(tr_ctx->file, "\tnode_%lb [label=\" {ptr = %p | %s | {left = %p | right = %p}}\", color=%s];\n", 
			nd_ctx->num_node, nd_ctx->node, nd_ctx->node->val, nd_ctx->node->left, nd_ctx->node->right, BORDER_COLOR);
    

    if (left_nd_ctx.node) fprintf(tr_ctx->file, "\t\"node_%lb\" -> \"node_%lb\" [penwidth=1.6, label=\"YES\"];\n",
			nd_ctx->num_node, left_nd_ctx.num_node);
    _create_node(tr_ctx, &left_nd_ctx);

    if (right_nd_ctx.node) fprintf(tr_ctx->file, "\t\"node_%lb\" -> \"node_%lb\" [penwidth=1.6, label=\"NO\"];\n",
			nd_ctx->num_node, right_nd_ctx.num_node);
    _create_node(tr_ctx, &right_nd_ctx);
    
    return;
}

void graph_script_geenrate(Tree_t* tree, int* dump_file_num)
{
    char file_name[MAX_FILENAME_SIZE] = {};
	snprintf(file_name, MAX_FILENAME_SIZE - 1, "./logs/dot-files/graph_code_%d.dot", *dump_file_num);

	FILE* graph_script = fopen(file_name, "w");	
	printf("%s opened\n", file_name);
	TrContext tr_ctx = {.tree = tree, .file = graph_script};
    NdContext nd_ctx = {.node = tree->root, .num_node = 0};

    _create_head_info(&tr_ctx);
	_create_error_info(&tr_ctx);
    
    _create_node(&tr_ctx, &nd_ctx);


    fprintf(tr_ctx.file, "}\n");
	fclose(tr_ctx.file);
	(*dump_file_num)++;
}



