/* include standard i/o files & gtk */
#include <stdio.h>
#include <gtk/gtk.h>
#include <gtksourceview/gtksource.h>

/* function declarations */
void program_end();
void file_new();
void file_open();
void file_save();
void file_save_as();
void toggle_line_numbers();
void toggle_line_highlight();
void set_style();

/* global gtk variables */
GtkWidget *win;
GtkWidget *code;
GtkSourceBuffer *buffer;
GtkSourceFile *source_file;
GtkSourceStyleSchemeManager *style_scheme_manager;
GtkSourceStyleScheme *style_scheme;

/* display window */
int main(int argc, char *argv[])
{
	/* initialize gtk */
	gtk_init(&argc, &argv);
	gtk_source_init();

	/* window */
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(GTK_WINDOW(win), 600, 400);
	gtk_window_set_title(GTK_WINDOW(win), "Cody");

	/* accelerator */
	GtkAccelGroup *accel_group = gtk_accel_group_new ();
	gtk_window_add_accel_group (GTK_WINDOW(win), accel_group);
	
	/* menu bar */
	GtkWidget *menu_bar = gtk_menu_bar_new();

	/* file menu */
	GtkWidget *file = gtk_menu_item_new_with_label("File");
	GtkWidget *file_menu = gtk_menu_new();
	GtkWidget *file_menu_new = gtk_menu_item_new_with_label("New File");
	GtkWidget *file_menu_open = gtk_menu_item_new_with_label("Open File");
	GtkWidget *file_menu_save = gtk_menu_item_new_with_label("Save");
	GtkWidget *file_menu_save_as = gtk_menu_item_new_with_label("Save As");
	GtkWidget *file_menu_quit = gtk_menu_item_new_with_label("Quit");

	/* add file menu to menu bar */
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), file);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), file_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_menu_new);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_menu_open);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_menu_save);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_menu_save_as);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), file_menu_quit);

	/* view menu */
	GtkWidget *view = gtk_menu_item_new_with_label("View");
	GtkWidget *view_menu = gtk_menu_new();
	GtkWidget *view_menu_numbers = gtk_menu_item_new_with_label("Line Numbers");
	GtkWidget *view_menu_highlight = gtk_menu_item_new_with_label("Highlight Line");
	GtkWidget *view_menu_style = gtk_menu_item_new_with_label("Styles");
	GtkWidget *view_style_menu = gtk_menu_new();
	GtkWidget *view_style_menu_classic = gtk_menu_item_new_with_label("Classic");
	GtkWidget *view_style_menu_cobalt = gtk_menu_item_new_with_label("Cobalt");
	GtkWidget *view_style_menu_kate = gtk_menu_item_new_with_label("Kate");
	GtkWidget *view_style_menu_oblivion = gtk_menu_item_new_with_label("Oblivion");
	GtkWidget *view_style_menu_zolarized_dark = gtk_menu_item_new_with_label("Zolarized Dark");
	GtkWidget *view_style_menu_zolarized_light = gtk_menu_item_new_with_label("Zolarized Light");
	GtkWidget *view_style_menu_tango = gtk_menu_item_new_with_label("Tango");

	/* add view menu to menu bar */
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_bar), view);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(view), view_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_menu_numbers);
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_menu_highlight);
	gtk_menu_shell_append(GTK_MENU_SHELL(view_menu), view_menu_style);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(view_menu_style), view_style_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(view_style_menu), view_style_menu_classic);
	gtk_menu_shell_append(GTK_MENU_SHELL(view_style_menu), view_style_menu_cobalt);
	gtk_menu_shell_append(GTK_MENU_SHELL(view_style_menu), view_style_menu_kate);
	gtk_menu_shell_append(GTK_MENU_SHELL(view_style_menu), view_style_menu_oblivion);
	gtk_menu_shell_append(GTK_MENU_SHELL(view_style_menu), view_style_menu_zolarized_dark);
	gtk_menu_shell_append(GTK_MENU_SHELL(view_style_menu), view_style_menu_zolarized_light);
	gtk_menu_shell_append(GTK_MENU_SHELL(view_style_menu), view_style_menu_tango);

	/* code widget */
	GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	buffer = gtk_source_buffer_new(NULL);
	code = gtk_source_view_new_with_buffer(buffer);
	gtk_container_add(GTK_CONTAINER(scrolled_window), code);
	PangoFontDescription *font_desc = pango_font_description_from_string("Mono 10");
	gtk_widget_modify_font(code, font_desc);
	gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(code), TRUE);
	gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(code), TRUE);

	/* c language loder */
	GtkSourceLanguageManager *language_manager = gtk_source_language_manager_get_default();
	gchar *language_search_path[1000];
	*language_search_path = "/usr/share/gtksourceview-4/language-specs/";
	gtk_source_language_manager_set_search_path(language_manager, language_search_path);
	GtkSourceLanguage *language = gtk_source_language_manager_get_language(language_manager, "c");
	gtk_source_buffer_set_language(buffer, language);

	/* style theme */
	style_scheme_manager = gtk_source_style_scheme_manager_get_default();
	char *style_path[100];
	*style_path = "/usr/share/gtksourceview-4/styles";
	gtk_source_style_scheme_manager_set_search_path(style_scheme_manager, style_path);
	style_scheme = gtk_source_style_scheme_manager_get_scheme(style_scheme_manager, "classic");
	gtk_source_buffer_set_style_scheme(buffer, style_scheme);

	/* window signals */
	g_signal_connect(win, "delete_event", G_CALLBACK(program_end), NULL);

	/* file menu item signals */
	g_signal_connect(file_menu_new, "activate", G_CALLBACK(file_new), NULL);
	g_signal_connect(file_menu_open, "activate", G_CALLBACK(file_open), NULL);
	g_signal_connect(file_menu_save, "activate", G_CALLBACK(file_save), NULL);
	g_signal_connect(file_menu_save_as, "activate", G_CALLBACK(file_save_as), NULL);
	g_signal_connect(file_menu_quit, "activate", G_CALLBACK(program_end), NULL);

	/* view menu item signals */
	g_signal_connect(view_menu_numbers, "activate", G_CALLBACK(toggle_line_numbers), NULL);
	g_signal_connect(view_menu_highlight, "activate", G_CALLBACK(toggle_line_highlight), NULL);
	g_signal_connect(view_style_menu_classic, "activate", G_CALLBACK(set_style), GINT_TO_POINTER(0));
	g_signal_connect(view_style_menu_cobalt, "activate", G_CALLBACK(set_style), GINT_TO_POINTER(1));
	g_signal_connect(view_style_menu_kate, "activate", G_CALLBACK(set_style), GINT_TO_POINTER(2));
	g_signal_connect(view_style_menu_oblivion, "activate", G_CALLBACK(set_style), GINT_TO_POINTER(3));
	g_signal_connect(view_style_menu_zolarized_dark, "activate", G_CALLBACK(set_style), GINT_TO_POINTER(4));
	g_signal_connect(view_style_menu_zolarized_light, "activate", G_CALLBACK(set_style), GINT_TO_POINTER(5));
	g_signal_connect(view_style_menu_tango, "activate", G_CALLBACK(set_style), GINT_TO_POINTER(6));

	/* widget accelerators */
	gtk_widget_add_accelerator(file_menu_new, "activate", accel_group, GDK_KEY_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);	
	gtk_widget_add_accelerator(file_menu_open, "activate", accel_group, GDK_KEY_o, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(file_menu_save, "activate", accel_group, GDK_KEY_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(file_menu_save_as, "activate", accel_group, GDK_KEY_s, GDK_SHIFT_MASK | GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_widget_add_accelerator(file_menu_quit, "activate", accel_group, GDK_KEY_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	
	/* display widgets */
	GtkWidget *box = gtk_vbox_new(FALSE, 5);
	gtk_box_pack_start(GTK_BOX(box), menu_bar, FALSE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(win), box);
	gtk_widget_show_all(win);
	
	/* program loop */
	gtk_main();
	
	/* exit */
	gtk_source_finalize();
	return 0;
}

/* end program handler */
void program_end(GtkWidget *widget, gpointer data)
{
	/* end gtk main loop */
	gtk_main_quit();
}

/* new file handler */
void file_new(GtkWidget *widget, gpointer data)
{
	printf("New File!\n");
}

/* open file handler */
void file_open(GtkWidget *widget, gpointer data)
{
	/* open file dialog */
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
		"Open file", GTK_WINDOW(win), GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL,
		GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL); 
	
	/* user accept */
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		/* open new file */
		source_file = gtk_source_file_new();
		gchar *chooser = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
		GFile *path = g_file_new_for_path(chooser);
		gtk_source_file_set_location(source_file, path);
		GtkSourceFileLoader *loader = gtk_source_file_loader_new(buffer, source_file);
		gtk_source_file_loader_load_async(loader, G_PRIORITY_DEFAULT, NULL, NULL, NULL, NULL, NULL, NULL);
		g_free(chooser);

		
	}

	/* destroy dialog */
	gtk_widget_destroy(dialog);
}

/* save file handler */
void file_save(GtkWidget *widget, gpointer data)
{
	/* save file */
	GtkSourceFileSaver *saver = gtk_source_file_saver_new(buffer, source_file);
	gtk_source_file_saver_save_async(saver, G_PRIORITY_DEFAULT, NULL, NULL, NULL, NULL, NULL, NULL);
	gtk_source_file_saver_save_finish(saver, NULL, NULL);
}

/* save file as handler */
void file_save_as(GtkWidget *widget, gpointer data)
{
	/* save file dialog */
	GtkWidget *dialog = gtk_file_chooser_dialog_new(
		"Save file", GTK_WINDOW(win), GTK_FILE_CHOOSER_ACTION_SAVE, GTK_STOCK_CANCEL,
		GTK_RESPONSE_CANCEL, GTK_STOCK_SAVE_AS, GTK_RESPONSE_ACCEPT, NULL);

	/* save file */
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
		char *chooser = gtk_file_chooser_get_current_name(GTK_FILE_CHOOSER(dialog));
		GFile *path = g_file_new_for_path(chooser);
		GtkSourceFileSaver *saver = gtk_source_file_saver_new_with_target(buffer, source_file, path);
		gtk_source_file_saver_save_async(saver, G_PRIORITY_DEFAULT, NULL, NULL, NULL, NULL, NULL, NULL);
		gtk_source_file_saver_save_finish(saver, NULL, NULL);
		g_free(chooser);		
	}

	/* destroy dialog */
	gtk_widget_destroy(dialog);
}

/* toggle line numbes handler */
void toggle_line_numbers(GtkWidget *widget, gpointer data)
{
	/* store line numbers status */
	gboolean status = gtk_source_view_get_show_line_numbers(GTK_SOURCE_VIEW(code));

	/* toggle line numbers */
	if (status == TRUE) {
		gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(code), FALSE);
	}
	else if (status == FALSE) {
		gtk_source_view_set_show_line_numbers(GTK_SOURCE_VIEW(code), TRUE);
	}
}

/* toggle line highlight handler */
void toggle_line_highlight(GtkWidget *widget, gpointer data)
{
	/* store line highlight status */
	gboolean status = gtk_source_view_get_highlight_current_line(GTK_SOURCE_VIEW(code));

	/* toggle line highlight */
	if (status == TRUE) {
		gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(code), FALSE);
	}
	else if (status == FALSE) {
		gtk_source_view_set_highlight_current_line(GTK_SOURCE_VIEW(code), TRUE);
	}
}

/* style handler */
void set_style(GtkWidget *widget, gpointer data)
{
	/* store gpointer value in integer */
	int style_number = GPOINTER_TO_INT(data);
	
	/* change style */	
	if (style_number == 0) {
		style_scheme = gtk_source_style_scheme_manager_get_scheme(style_scheme_manager, "classic");
	}
	else if (style_number == 1) {
		style_scheme = gtk_source_style_scheme_manager_get_scheme(style_scheme_manager, "cobalt");
	}
	else if (style_number == 2) {
		style_scheme = gtk_source_style_scheme_manager_get_scheme(style_scheme_manager, "kate");
	}
	else if (style_number == 3) {
		style_scheme = gtk_source_style_scheme_manager_get_scheme(style_scheme_manager, "oblivion");
	}
	else if (style_number == 4) {
		style_scheme = gtk_source_style_scheme_manager_get_scheme(style_scheme_manager, "solarized-dark");
	}
	else if (style_number == 5) {
		style_scheme = gtk_source_style_scheme_manager_get_scheme(style_scheme_manager, "solarized-light");
	}
	else if (style_number == 6) {
		style_scheme = gtk_source_style_scheme_manager_get_scheme(style_scheme_manager, "tango");
	}

	/* apply style */
	gtk_source_buffer_set_style_scheme(buffer, style_scheme);
}
