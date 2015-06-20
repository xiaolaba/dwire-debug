// OpenFile.c

#ifdef windows

  OPENFILENAME OpenFileName = {sizeof(OpenFileName), 0};

  void OpenFileDialog()
  {
    OpenFileName.lpstrFile   = CurrentFilename;
    OpenFileName.nMaxFile    = countof(CurrentFilename);
    OpenFileName.Flags       = OFN_FILEMUSTEXIST;
    OpenFileName.lpstrFilter = "All files\0*.*\0*.bin\0*.bin\0\0";
    if (!GetOpenFileName(&OpenFileName)) {CurrentFilename[0] = 0;}
  }

#else

  #include <gtk/gtk.h>

  static void Activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);

    GtkWidget *dialog = gtk_file_chooser_dialog_new(
      "Open File", GTK_WINDOW(window), GTK_FILE_CHOOSER_ACTION_OPEN,
      "Cancel", GTK_RESPONSE_CANCEL,
      "Open",   GTK_RESPONSE_ACCEPT,
      NULL
    );

    gint result = gtk_dialog_run(GTK_DIALOG(dialog));
    if (result == GTK_RESPONSE_ACCEPT) {
      char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
      strncpy(CurrentFilename, filename, sizeof(CurrentFilename)-1);
      CurrentFilename[sizeof(CurrentFilename)-1] = 0;
      g_free(filename);
      Wsl(CurrentFilename);
    }
    gtk_widget_destroy(dialog);
    gtk_widget_destroy(window);
  }

  void OpenFileDialog() {

    int dupstdin = dup(0);
    if (dupstdin < 0) {Ws("dup(0) failed: "); Wsl(strerror(errno));}
    Ws("dupstdin = "); Wd(dupstdin,1); Wl();

    GtkApplication *App = gtk_application_new("com.dcwbrown.dwdebug", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(App, "activate", G_CALLBACK(Activate), NULL);
    g_application_run(G_APPLICATION(App), 0,0);
    g_object_unref(App);

    int result = dup2(dupstdin, 0);
    Ws("dup(dupstdin, 0) returned "); Wd(result,1); Wl();
    close(dupstdin);

  }


#endif

