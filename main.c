#include "src/Akinator.h"

int main (int argc, char **argv)
{
    Tree_t* main_tree = fread_tree("data/Stands.txt",  PREORDER);
    if (!main_tree) return HUYNA_PTR_TREE;

    UserData user_data = {.tree = main_tree};
    GtkApplication* app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect (app, "activate", G_CALLBACK(activate), &user_data);
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    tree_dump(main_tree);
    tree_destroy(main_tree);
    return status;
}
