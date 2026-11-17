#include "Akinator.h"

GtkWidget* create_win_screen(UserData* user_data)
{
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 100);
    gtk_widget_set_margin_top(box, 50);

    gtk_box_append(GTK_BOX(box), gtk_label_new("HEHE.... I`ve won!!")); 

    GtkWidget* restart_button = gtk_button_new_with_label("Quit to the main menu");
    g_signal_connect(restart_button, "clicked", G_CALLBACK(go_to_main_menu), user_data);
    gtk_box_append(GTK_BOX(box), restart_button);

    return box;
}

GtkWidget* create_loose_screen(UserData* user_data)
{
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 100);
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

GtkWidget* create_start_screen(UserData* user_data)
{
    user_data->node = user_data->tree->root;
    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 100);
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

GtkWidget* create_guess_game_screen(UserData* user_data)
{
    GtkWidget* game_guess_screen = gtk_box_new(GTK_ORIENTATION_VERTICAL, 100);
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