#include "glib.h"
#include <stdio.h>
#define DEBUG
#include "Tree.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define READ_ERROR(message) do {fprintf(stderr, RED message DEFUALT);\
                                return NULL;} while(0)

#define SHIFT_PTR(shift) for(int i = 0; i < shift; i++)(*source)++;



char* skip_spaces(char* string)
{
    char* ptr = string;

    while (*ptr != '\0')
    {
        if (!isspace(*ptr))
        {
            return ptr;
        }
        ptr++;
    }
    return ptr;
}

static int _node_read_val(Node_t* node, char** source)
{
    START_FUNC

    int offset = 0;
    char* val = strdup(*source);

    sscanf(*source, "\"%[^\"]%n", val,  &offset);
    offset++;

    $(printf("source = <%s>, offset = <%d> \n", *source, offset);)
    _node_init(node, val);
    
    SHIFT_PTR(offset);
    $(printf("node_val = <%s>, source = <%s>\n", node->val, *source);)

    END_FUNC
    return 0;
}

static int is_nil(char** source)
{
    char temp = 0;
    int offset = 0;
    if (sscanf(*source, "nil}%n%c", &offset, &temp) != 1)
    {
        return 0;
    }

    SHIFT_PTR(offset);
    return 1;
}

static Node_t* _node_fread_preorder(char** source $(, int level))
{
    START_FUNC

    $(printf("cur level = <%d>\n", level));
    if (**source == '{')
    {
        SHIFT_PTR(1);

        if (is_nil(source)) return NULL;
        Node_t* new_node = _node_create();

        if (_node_read_val(new_node, source)) {_node_destroy(new_node); READ_ERROR("Wrong val in node\n");}

        Node_t* left_node = _node_fread_preorder(source $(, level+1));
        if (_node_link(new_node, left_node, LEFT)) READ_ERROR("Reading error\n");            

        Node_t* right_node = _node_fread_preorder(source $(, level+1));
        if (_node_link(new_node, right_node, RIGHT)) READ_ERROR("Readint erro\n");
    
        SHIFT_PTR(1);

        END_FUNC
        return new_node;
    }

    READ_ERROR("Can`t continue recursion\n");
}

static Node_t* _node_fread_inorder(char** source $(, int level))
{
    START_FUNC

    $(printf("cur level = <%d>\n", level));
    if (**source == '{')
    {
        SHIFT_PTR(1);

        if (is_nil(source)) return NULL;
        Node_t* new_node = _node_create();

        Node_t* left_node = _node_fread_preorder(source $(, level+1));
        _node_link(new_node, left_node, LEFT);

        if (_node_read_val(new_node, source)) READ_ERROR("Wrong val in node\n");

        Node_t* right_node = _node_fread_preorder(source $(, level+1));
        _node_link(new_node, right_node, RIGHT);
    
        SHIFT_PTR(1);

        END_FUNC
        return new_node;
    }

    READ_ERROR("Can`t continue recursion\n");
}

static Node_t* _node_fread_postorder(char** source $(, int level))
{
     START_FUNC

    $(printf("cur level = <%d>\n", level));
    if (**source == '{')
    {
        SHIFT_PTR(1);

        if (is_nil(source)) return NULL;
        Node_t* new_node = _node_create();

        Node_t* left_node = _node_fread_preorder(source $(, level+1));
        _node_link(new_node, left_node, LEFT);

        Node_t* right_node = _node_fread_preorder(source $(, level+1));
        _node_link(new_node, right_node, RIGHT);

        if (_node_read_val(new_node, source)) READ_ERROR("Wrong val in node\n");

        SHIFT_PTR(1);

        END_FUNC
        return new_node;
    }

    READ_ERROR("Can`t continue recursion\n");
}


static Node_t* (*_node_fread_funcs[])(char** source $(, int level)) = 
{
    _node_fread_preorder, 
    _node_fread_inorder, 
    _node_fread_postorder
};

static size_t get_file_size(const char* filename)
{
    struct stat file_info = {};

    if (stat(filename, &file_info)) {fprintf(stderr, RED"cant get stat of file\n"DEFUALT); return 0;}

    return (size_t) file_info.st_size;
}

Tree_t* fread_tree(const char* filename, TREE_IO_MODE mode)
{
    START_FUNC

    if (mode < 0 || mode > COUNT_MODES - 1)
    {
        fprintf(stderr, RED"WRONG READ mode <%d>, try again later one of this variants:\n"
                                          "\t0 - PREORDER;\n"
                                          "\t1 - INORDER;\n"
                                          "\t2 - POSTOORDER;\n"DEFUALT, mode);
        return NULL;
    }

    FILE* file = fopen(filename, "r");
    size_t file_size = get_file_size(filename);
    if (!file_size) 
    {
        fprintf(stderr, RED"ERROR can`t get filesize\n"DEFUALT);
        return HUYNA;
    }
    char* buffer = calloc(file_size + 3, sizeof(char));

    fread(buffer, sizeof(char),file_size , file); 
    fclose(file);
    char* source = skip_spaces(buffer);

    Tree_t* tree = tree_create();
    $(printf("tree = [%p]", tree);)
    if (!(tree->root = _node_fread_funcs[mode](&source $(, 0))))
    {
        fprintf(stderr, RED"ERROR while reading a tree\n"DEFUALT);
        return HUYNA;
    }

    CHECK(tree);

    $(printf("in tree_reader - root = <%s>\n", tree->root->val);)


    free(buffer);
    END_FUNC
    return tree;
}