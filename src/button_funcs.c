#include "Akinator.h"

void yes_branch(GtkButton* button, gpointer data)
{
    UserData* user_data = (UserData*) data;

    if (!user_data->node->left) 
    {  
        gtk_stack_set_visible_child_name(user_data->stack, "Win_screen");

        return;
    }

    create_text(user_data, LEFT);
    user_data->node = user_data->node->left;
}

void no_branch(GtkButton* butoon, gpointer data)
{
    UserData* user_data = (UserData*) data;

    if (!user_data->node->right) 
    {
        gtk_widget_set_visible(user_data->restart_button, false);
        gtk_stack_set_visible_child_name(user_data->stack, "Loose_screen");

        return;
    }

    create_text(user_data, RIGHT);  
    user_data->node = user_data->node->right;

}

void go_to_main_menu(GtkButton* button, gpointer data)
{
    UserData* user_data = (UserData*) data;
    user_data->node = user_data->tree->root;
    create_text(user_data, CURRENT);
    gtk_stack_set_visible_child_name(user_data->stack, "Start_screen");
}

void start_game(GtkButton* button,  gpointer data)
{
    UserData* user_data = (UserData*) data;
    gtk_stack_set_visible_child_name(user_data->stack, "Guess_game_screen");
}

void export_tree(GtkButton* button, gpointer data)
{
    UserData* user_data = (UserData*) data;

    FILE* data_file = fopen("data/Stands.txt", "w");
    if (!data_file) {fprintf(stderr, RED"NO data file\n"DEFUALT); return;}

    FILE* log_file = fopen("logs/text_logs/log.txt", "a");
    if (!log_file) {fprintf(stderr, RED"No log file\n"DEFUALT); return;}

    fprint_tree(data_file, user_data->tree, PREORDER);
    fprint_tree(log_file, user_data->tree, CLASSIC);

    int _ = 0;
    graph_script_geenrate(user_data->tree, &_);
    system("./create_png.sh");

    fclose(data_file);
    fclose(log_file);
}

void quit_game(GtkButton* button, gpointer data)
{
    gtk_window_destroy(GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(button))));
}