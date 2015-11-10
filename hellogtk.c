#include <gtk/gtk.h>
/*callback function */
static void destroy (GtkWidget *widget, gpointer data)
{
	gtk_main_quit();
}

int main(int argc, char *argv[])
{
	GtkWidget *window, *button;
	gtk_init (&argc, &argv);

	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 10);

	button = gtk_button_new_with_label ("hello world");

	g_signal_connect (GTK_OBJECT (button), "Clicked", GTK_SIGNAL_FUNC
	(destroy), NULL);

	gtk_container_add (GTK_CONTAINER (window), button);

	gtk_widget_show (button);
	gtk_widget_show (window);

	gtk_main();
	return (0);
}
