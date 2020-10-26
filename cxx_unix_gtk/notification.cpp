#include <gtk/gtk.h>

static void activate(GApplication* app, gpointer user_data)
{
    GtkWidget* widget;

    auto* notification = g_notification_new("Lunch is ready");
    g_notification_set_body(notification, "Today we have pancakes and salad, and fruit and cake for dessert");

    g_application_send_notification(G_APPLICATION(app), "lunch-is-ready", notification);
    g_object_unref(notification);

    widget = gtk_application_window_new(GTK_APPLICATION(app));
    gtk_widget_show(widget);
}

int main(int argc, char** argv)
{
    auto* app = gtk_application_new("com.tobias-hienzsch.examples-gtk", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    auto const status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}