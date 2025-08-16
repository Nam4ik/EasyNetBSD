#include <gtk/gtk.h>
#include <stdio.h>
#include <stdint.h> 

#include "DiskTools.h"

static GtkWidget* create_language_page() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *label = gtk_label_new("Select installation language:");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 10);
    
    
    GtkWidget *combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo), "en", "English");
    gtk_combo_box_text_append(GTK_COMBO_BOX_TEXT(combo), "ru", "Русский");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);
    gtk_box_pack_start(GTK_BOX(box), combo, FALSE, FALSE, 5);
    
    return box;
}


static GtkWidget* create_disk_page() {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *label = gtk_label_new("Disk partitioning setup:");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 10);
    
  
    GtkWidget *entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter disk device (e.g. sd0)");
    gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 5);
    
    return box;
}


static void on_next_clicked(GtkButton *button, GtkStack *stack) {
    const gchar *name = gtk_stack_get_visible_child_name(stack);
    if (g_strcmp0(name, "language") == 0) {
        gtk_stack_set_visible_child_name(stack, "disk");
    }
    
}
static void on_back_clicked(GtkButton *button, GtkStack *stack) {
    const gchar *name = gtk_stack_get_visible_child_name(stack);
    if (g_strcmp0(name, "disk") == 0) {
        gtk_stack_set_visible_child_name(stack, "language");
    }
    
}


static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *main_box;
    GtkWidget *content_box;
    GtkWidget *nav_box;
    GtkWidget *next_button;
    GtkWidget *back_button;
    GtkStack *stack;
    
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "NetBSD Graphical Installer");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    
    
    main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 15);
    gtk_container_add(GTK_CONTAINER(window), main_box);
    
    
    stack = GTK_STACK(gtk_stack_new());
    gtk_stack_set_transition_type(stack, GTK_STACK_TRANSITION_TYPE_SLIDE_LEFT_RIGHT);
    
    
    gtk_stack_add_named(stack, create_language_page(), "language");
    gtk_stack_add_named(stack, create_disk_page(), "disk");
   
    
    
    content_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(content_box), GTK_WIDGET(stack), TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(main_box), content_box, TRUE, TRUE, 0);
    
    
    nav_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_set_homogeneous(GTK_BOX(nav_box), TRUE);
    gtk_widget_set_valign(nav_box, GTK_ALIGN_END);
    
    back_button = gtk_button_new_with_label("Back");
    g_signal_connect(back_button, "clicked", G_CALLBACK(on_back_clicked), stack);
    gtk_box_pack_start(GTK_BOX(nav_box), back_button, TRUE, TRUE, 0);
    
    next_button = gtk_button_new_with_label("Next");
    g_signal_connect(next_button, "clicked", G_CALLBACK(on_next_clicked), stack);
    gtk_box_pack_start(GTK_BOX(nav_box), next_button, TRUE, TRUE, 0);
    
    gtk_box_pack_start(GTK_BOX(main_box), nav_box, FALSE, FALSE, 0);
    
    gtk_widget_show_all(window);
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;
    
    app = gtk_application_new("org.netbsd.graphical-installer", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    
    return status;
}
