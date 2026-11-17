#include "Akinator.h"

void entry_name_activate(GtkEntry* entry, gpointer data)
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

void entry_quality_activate(GtkEntry* entry, gpointer data)
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
