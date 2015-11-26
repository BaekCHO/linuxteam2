#include <gtk/gtk.h>

GtkWidget* createConsoleBox()
{
    GtkWidget* textArea = gtk_text_view_new();
    GtkWidget* scrollbar= gtk_vscrollbar_new(gtk_text_view_get_vadjustment(GTK_TEXT_VIEW(textArea)));
    GtkWidget* textEntry = gtk_entry_new();

    GtkWidget* console = gtk_table_new(3, 2, FALSE);

    gtk_table_attach_defaults(GTK_TABLE(console), textArea, 0, 1, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(console), scrollbar, 1, 2, 0, 1);
    gtk_table_attach_defaults(GTK_TABLE(console), textEntry, 0, 2, 1, 2);
    //This code sets the preferred size for the widget, so it does not ask for extra space
    gtk_widget_set_size_request(textArea, 320, 240);

    return console;
}


int main(int argc,char* argv[]){
    GtkWidget *window;
    gtk_init(&argc, &argv);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Simple Sample");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);// does not  matter this size
    gtk_container_add(GTK_CONTAINER(window), createConsoleBox());
    gtk_widget_show_all(window);
    gtk_window_set_resizable(GTK_WINDOW(window),FALSE);//because of this
    gtk_main();
    return 0;
}
