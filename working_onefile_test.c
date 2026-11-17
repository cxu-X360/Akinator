#include <gtk/gtk.h>
#include <stdio.h>
#include "glib.h"
#include "includes/Tree/Node.h"
#include "includes/Tree/Tree.h"

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

static GtkWidget* create_start_screen(UserData* user_data);
static GtkWidget* create_guess_game_screen(UserData* user_data);
static GtkWidget* create_win_screen(UserData* user_data);
static GtkWidget* create_loose_screen(UserData* user_data);
static void launch_game(UserData* user_data);


//SERVICE 

static void create_text(UserData* user_data, SIDE side)
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



//BUTTON FUCTIONS

static void yes_branch(GtkButton* button, gpointer data)
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

static void no_branch(GtkButton* butoon, gpointer data)
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

static void go_to_main_menu(GtkButton* button, gpointer data)
{
  UserData* user_data = (UserData*) data;
  user_data->node = user_data->tree->root;
  create_text(user_data, CURRENT);
  gtk_stack_set_visible_child_name(user_data->stack, "Start_screen");
}

static void start_game(GtkButton* button,  gpointer data)
{
  UserData* user_data = (UserData*) data;
  gtk_stack_set_visible_child_name(user_data->stack, "Guess_game_screen");
}


static void export_tree(GtkButton* button, gpointer data)
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

static void quit_game(GtkButton* button, gpointer data)
{
  gtk_window_destroy(GTK_WINDOW(gtk_widget_get_root(GTK_WIDGET(button))));
}



//ENTRY FUNCIONS

static void entry_name_activate(GtkEntry* entry, gpointer data)
{
  UserData* user_data = (UserData*) data;

  char* new_name_text = strdup(gtk_editable_get_text(GTK_EDITABLE(entry)));
  g_print("You`ve entered: %s\n", new_name_text);

  Node_t* old_name_node = _node_create();
  Node_t* new_name_node = _node_create();
  _node_init(old_name_node, user_data->node->val);
  _node_init(new_name_node, new_name_text);

  if (_node_link(user_data->node, old_name_node, RIGHT)) return;
  if (_node_link(user_data->node, new_name_node, LEFT)) return;


  gtk_editable_set_text(GTK_EDITABLE(entry),"Enter name...");
}

static void entry_quality_activate(GtkEntry* entry, gpointer data)
{
  UserData* user_data = (UserData*) data;

  if (!user_data->node->left) 
  {
    gtk_editable_set_text(GTK_EDITABLE(entry),"You CAN`T enter a quality before name!! Enter name first, PLEASE");
    return;
  }

  char* new_quality_text = strdup(gtk_editable_get_text(GTK_EDITABLE(entry)));
  g_print("You`ve entered: %s\n", new_quality_text);

  user_data->node->val = new_quality_text;

  gtk_editable_set_text(GTK_EDITABLE(entry),"Enter quality...");

  gtk_widget_set_visible(user_data->restart_button, true);
}



//CREATING SCREENS

static GtkWindow* create_main_window(GtkApplication* app, gpointer user_data)
{
  GtkWidget* window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Akinator");
  gtk_window_set_default_size (GTK_WINDOW (window), 500, 450);

  return GTK_WINDOW(window);
}

static GtkWidget* create_stack_of_menues(GtkWindow* window)
{
  GtkWidget* stack = gtk_stack_new();
  gtk_stack_set_transition_type(GTK_STACK(stack), GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT);
  gtk_window_set_child(window, stack);

  return stack;
}

static GtkWidget* create_win_screen(UserData* user_data)
{
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 100);
  gtk_widget_set_margin_top(box, 50);

  gtk_box_append(GTK_BOX(box), gtk_label_new("HEHE.... I`ve won!!")); 

  GtkWidget* restart_button = gtk_button_new_with_label("Quit to the main menu");
  g_signal_connect(restart_button, "clicked", G_CALLBACK(go_to_main_menu), user_data);
  gtk_box_append(GTK_BOX(box), restart_button);

  return box;
}

static GtkWidget* create_loose_screen(UserData* user_data)
{
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 30);
  gtk_widget_set_margin_top(box, 50);

  gtk_box_append(GTK_BOX(box), gtk_label_new("OH OH..... I`ve lost(\nPlease, tell me, what you guessed")); 

  GtkWidget* name_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    gtk_box_append(GTK_BOX(name_box), gtk_label_new("It is:"));
    
    GtkWidget* entry_name  = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(entry_name), "Enter name...");
    g_signal_connect(entry_name, "activate", G_CALLBACK(entry_name_activate), user_data);
    gtk_box_append(GTK_BOX(name_box), entry_name);
  
  gtk_box_append(GTK_BOX(box), name_box);



  gtk_box_append(GTK_BOX(box), gtk_label_new("What`s a different?")); 

  GtkWidget* quality_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    gtk_box_append(GTK_BOX(quality_box), gtk_label_new("It is"));

    GtkWidget* entry_quality  = gtk_entry_new();
    gtk_editable_set_text(GTK_EDITABLE(entry_quality), "Enter quality...");
    g_signal_connect(entry_quality, "activate", G_CALLBACK(entry_quality_activate), user_data);
    gtk_box_append(GTK_BOX(quality_box), entry_quality);
  
  gtk_box_append(GTK_BOX(box), quality_box);

  GtkWidget* restart_button = gtk_button_new_with_label("Quit to the main menu");
  user_data->restart_button = restart_button;
  g_signal_connect(restart_button, "clicked", G_CALLBACK(go_to_main_menu), user_data);
  gtk_box_append(GTK_BOX(box), restart_button);

  return box;
}

static GtkWidget* create_start_screen(UserData* user_data)
{
  user_data->node = user_data->tree->root;
  GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 40);
  gtk_widget_set_margin_top(box, 50);

  gtk_box_append(GTK_BOX(box), gtk_label_new("STAND\nAKINATOR")); 

  GtkWidget* start_button = gtk_button_new_with_label("Start game");
  g_signal_connect(start_button, "clicked", G_CALLBACK(start_game), user_data);
  gtk_box_append(GTK_BOX(box), start_button);

  GtkWidget* export_button = gtk_button_new_with_label("Export tree");
  g_signal_connect(export_button, "clicked", G_CALLBACK(export_tree), user_data);
  gtk_box_append(GTK_BOX(box), export_button);

  GtkWidget* quit_button = gtk_button_new_with_label("Quit");
  g_signal_connect(quit_button, "clicked", G_CALLBACK(quit_game), NULL);
  gtk_box_append(GTK_BOX(box), quit_button);

  return box;
}

static GtkWidget* create_guess_game_screen(UserData* user_data)
{
  GtkWidget* game_guess_screen = gtk_box_new(GTK_ORIENTATION_VERTICAL, 70);
  gtk_widget_set_margin_top(game_guess_screen, 180);

  GtkWidget* label = gtk_label_new(NULL);
  user_data->question_label = label;
  create_text(user_data, CURRENT);
  
    PangoAttrList* attrs = pango_attr_list_new();
    PangoAttribute* size_attr = pango_attr_size_new(24 * PANGO_SCALE);
    pango_attr_list_insert(attrs, size_attr);

    gtk_label_set_attributes(GTK_LABEL(label), attrs);

    pango_attr_list_unref(attrs);

  gtk_box_append(GTK_BOX(game_guess_screen), label);


  GtkWidget* grid= gtk_grid_new();
  gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);
  gtk_box_append(GTK_BOX(game_guess_screen), grid);


  GtkWidget* yes_button = gtk_button_new_with_label("YES");
  gtk_widget_set_size_request(yes_button, 60, 60);
  g_signal_connect(yes_button, "clicked", G_CALLBACK(yes_branch), user_data);

  GtkWidget* no_button = gtk_button_new_with_label("NO");
  gtk_widget_set_size_request(no_button, 60, 60);
  g_signal_connect(no_button, "clicked", G_CALLBACK(no_branch), user_data);
  

  gtk_grid_attach(GTK_GRID(grid), yes_button, 0, 0, 1, 1);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 100);
  gtk_grid_attach(GTK_GRID(grid), no_button, 2, 0, 1, 1);

  return game_guess_screen;
}

//ACTIVATING AND SETUPING

static void launch_game(UserData* user_data)
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

static void activate (GtkApplication *app, gpointer user_data)
{
  GtkWindow* window = create_main_window(app, user_data);
  GtkWidget* stack = create_stack_of_menues(window);
  ((UserData*) user_data)->stack = GTK_STACK(stack);
  
  launch_game((UserData*) user_data);
  
  gtk_window_present(window);
}

int main (int argc, char **argv)
{
  //if (argc != 2) return -fprintf(stderr, RED"Wrong enter data. PLease run like *main_file_name.c* *data_file_path.txt* "DEFUALT);

  Tree_t* main_tree = fread_tree("data/Stands.txt",  PREORDER);
  if (!main_tree) 
  {
      fprintf(stderr, RED"Can`t create tree\n"DEFUALT);
      return HUYNA_PTR_TREE;
  }
  UserData user_data = {.tree = main_tree};
  GtkApplication* app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK(activate), &user_data);
  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  tree_dump(main_tree);
  tree_destroy(main_tree);
  return status;
}