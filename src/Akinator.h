#ifndef AKTOR_H
#define AKTOR_H

#include "../includes/Tree/Tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "glib.h"
#include "../includes/Tree/Node.h"
#include "../includes/Tree/Tree.h"

typedef struct 
{
    Tree_t* tree;
    Node_t* node;
    GtkWidget* question_label;
    GtkStack* stack;
    GtkWidget* restart_button;
} UserData;

typedef enum
{
    QUALITY = 1,
    THING = 2,
} MESSAGE_TYPE;


#define GTK_USERDATA(obj) ((UserData*) obj)

//Service
void create_text(UserData* user_data, SIDE side);

//Creating base of app
void activate (GtkApplication *app, gpointer user_data);
GtkWindow* create_main_window(GtkApplication* app, gpointer user_data);
GtkWidget* create_stack_of_menues(GtkWindow* window);
void launch_game(UserData* user_data);

//Creating screens
GtkWidget* create_start_screen(UserData* user_data);
GtkWidget* create_guess_game_screen(UserData* user_data);
GtkWidget* create_win_screen(UserData* user_data);
GtkWidget* create_loose_screen(UserData* user_data);

//Functions, when button clicked
void yes_branch(GtkButton* button, gpointer data);
void no_branch(GtkButton* butoon, gpointer data);
void go_to_main_menu(GtkButton* button, gpointer data);
void start_game(GtkButton* button,  gpointer data);
void export_tree(GtkButton* button, gpointer data);
void quit_game(GtkButton* button, gpointer data);

//Functions, when entry activated
void entry_name_activate(GtkEntry* entry, gpointer data);
void entry_quality_activate(GtkEntry* entry, gpointer data);

#endif