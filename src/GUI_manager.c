#include "Akinator.h"

void create_text(UserData* user_data, SIDE side)
{
    size_t message_max_size = strlen(user_data->node->val) + 20;
    char* message_text = calloc(message_max_size, sizeof(char));

    snprintf(message_text, message_max_size, "Is it %s?", 
      (side == LEFT) ? user_data->node->left->val : 
      ((side == RIGHT) ? user_data->node->right->val : 
      (((side == CURRENT ? user_data->node->val : 
        "__NONE__")))));

    gtk_label_set_text(GTK_LABEL(user_data->question_label), message_text);
    free(message_text);
}

GtkWindow* create_main_window(GtkApplication* app, gpointer user_data)
{
    GtkWidget* window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Akinator");
    gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);

    return GTK_WINDOW(window);
}

GtkWidget* create_stack_of_menues(GtkWindow* window)
{
    GtkWidget* stack = gtk_stack_new();
    gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
    gtk_window_set_child(window, stack);

    return stack;
}

void launch_game(UserData* user_data)
{
    GtkWidget* start_screen = create_start_screen(user_data);
    GtkWidget* game_guess_screen = create_guess_game_screen(user_data);
    GtkWidget* win_screen = create_win_screen(user_data);
    GtkWidget* loose_screen = create_loose_screen(user_data); 

        
    gtk_stack_add_named(user_data->stack, game_guess_screen, "Guess_game_screen");
    gtk_stack_add_named(user_data->stack, start_screen, "Start_screen");  
    gtk_stack_add_named(user_data->stack, win_screen, "Win_screen");
    gtk_stack_add_named(user_data->stack, loose_screen, "Loose_screen");  

    gtk_stack_set_visible_child_name(user_data->stack, "Start_screen");
}

void activate (GtkApplication *app, gpointer user_data)
{
    GtkWindow* window = create_main_window(app, user_data);
    GtkWidget* stack = create_stack_of_menues(window);
    ((UserData*) user_data)->stack = GTK_STACK(stack);
    
    launch_game((UserData*) user_data);
    
    gtk_window_present(window);
}