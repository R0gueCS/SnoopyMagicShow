#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>

#define GRID_SIZE 11
#define NUM_WALLS 5 // Define the number of walls
#define GAME_DURATION 60 // Specify the game duration in seconds

GtkWidget *grid[GRID_SIZE][GRID_SIZE];
int currentRow = GRID_SIZE / 2;
int currentCol = GRID_SIZE / 2;
int score = 0; // Initialize the score
int remainingTime = GAME_DURATION; // Initialize the remaining time

// Declare the scoreLabel and timeLabel variables
GtkWidget *scoreLabel;
GtkWidget *timeLabel;

// Function to generate random positions for walls
void generate_walls()
{
    srand(time(NULL));

    for (int i = 0; i < NUM_WALLS; i++)
    {
        int row = rand() % GRID_SIZE;
        int col = rand() % GRID_SIZE;

        // Make sure the wall is not placed on the initial position of the smiley face
        while (row == currentRow && col == currentCol)
        {
            row = rand() % GRID_SIZE;
            col = rand() % GRID_SIZE;
        }

        // Set the label of the button at the wall position
        gtk_button_set_label(GTK_BUTTON(grid[row][col]), "🧱");
    }
}

// Function to add bird icons to the corners of the grid
void add_birds()
{
    // Place bird icons in the corners
    gtk_button_set_label(GTK_BUTTON(grid[0][0]), "🐦‍");
    gtk_button_set_label(GTK_BUTTON(grid[0][GRID_SIZE - 1]), "🐦‍");
    gtk_button_set_label(GTK_BUTTON(grid[GRID_SIZE - 1][0]), "🐦‍");
    gtk_button_set_label(GTK_BUTTON(grid[GRID_SIZE - 1][GRID_SIZE - 1]), "🐦‍");
}

// Function to update the score and check for victory
void update_score_and_check_victory()
{
    score++;
    gtk_label_set_text(GTK_LABEL(scoreLabel), g_strdup_printf("Score: %d", score));

    if (score == 4) // If all birds are captured
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                    "Congratulations! Snoopy captured all birds!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_main_quit();
    }
}

// Function to update the time
gboolean update_time()
{
    remainingTime--;

    if (remainingTime < 0)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK,
                                                    "Time's up! Snoopy couldn't capture all birds in time.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        gtk_main_quit();
        return G_SOURCE_REMOVE;
    }

    gtk_label_set_text(GTK_LABEL(timeLabel), g_strdup_printf("Time: %d seconds", remainingTime));
    return G_SOURCE_CONTINUE;
}

// Callback function for arrow key presses
void on_key_press(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
    // Check for arrow key presses
    switch (event->keyval)
    {
    case GDK_KEY_Up:
        if (currentRow > 0 && strcmp(gtk_button_get_label(GTK_BUTTON(grid[currentRow - 1][currentCol])), "🧱") != 0)
        {
            gtk_button_set_label(GTK_BUTTON(grid[currentRow][currentCol]), " ");
            currentRow--;
            if (strcmp(gtk_button_get_label(GTK_BUTTON(grid[currentRow][currentCol])), "🐦‍") == 0)
            {
                update_score_and_check_victory();
            }
            gtk_button_set_label(GTK_BUTTON(grid[currentRow][currentCol]), "😊");
        }
        break;
    case GDK_KEY_Down:
        if (currentRow < GRID_SIZE - 1 && strcmp(gtk_button_get_label(GTK_BUTTON(grid[currentRow + 1][currentCol])), "🧱") != 0)
        {
            gtk_button_set_label(GTK_BUTTON(grid[currentRow][currentCol]), " ");
            currentRow++;
            if (strcmp(gtk_button_get_label(GTK_BUTTON(grid[currentRow][currentCol])), "🐦‍") == 0)
            {
                update_score_and_check_victory();
            }
            gtk_button_set_label(GTK_BUTTON(grid[currentRow][currentCol]), "😊");
        }
        break;
    case GDK_KEY_Left:
        if (currentCol > 0 && strcmp(gtk_button_get_label(GTK_BUTTON(grid[currentRow][currentCol - 1])), "🧱") != 0)
        {
            gtk_button_set_label(GTK_BUTTON(grid[currentRow][currentCol]), " ");
            currentCol--;
            if (strcmp(gtk_button_get_label(GTK_BUTTON(grid[currentRow][currentCol])), "🐦‍") == 0)
            {
                update_score_and_check_victory();
            }
            gtk_button_set_label(GTK_BUTTON(grid[currentRow][currentCol]), "😊");
        }
        break;
    case GDK_KEY_Right:
        if (currentCol < GRID_SIZE - 1 && strcmp(gtk_button_get_label(GTK_BUTTON(grid[currentRow][currentCol + 1])), "🧱") != 0)
        {
            gtk_button_set_label(GTK_BUTTON(grid[currentRow][currentCol]), " ");
            currentCol++;
            if (strcmp(gtk_button_get_label(GTK_BUTTON(grid[currentRow][currentCol])), "🐦‍") == 0)
            {
                update_score_and_check_victory();
            }
            gtk_button_set_label(GTK_BUTTON(grid[currentRow][currentCol]), "😊");
        }
        break;
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *gridContainer;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Snoopy Game");
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Set the window position to the center of the screen
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    // Set up keyboard event handling
    gtk_widget_add_events(GTK_WIDGET(window), GDK_KEY_PRESS_MASK);
    g_signal_connect(window, "key-press-event", G_CALLBACK(on_key_press), NULL);

    // Create a grid
    gridContainer = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), gridContainer);

    // Add buttons to the grid
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            GtkWidget *button = gtk_button_new_with_label(" ");
            gtk_grid_attach(GTK_GRID(gridContainer), button, j, i, 1, 1);
            grid[i][j] = button;
        }
    }

    // Place the smiling face in the middle of the grid
    gtk_button_set_label(GTK_BUTTON(grid[currentRow][currentCol]), "😊");

    // Generate random walls
    generate_walls();

    // Add bird icons to the corners
    add_birds();

    // Create and show the score label
    scoreLabel = gtk_label_new("Score: 0");
    gtk_grid_attach(GTK_GRID(gridContainer), scoreLabel, 0, GRID_SIZE, GRID_SIZE / 2, 1);

    // Create and show the time label
    timeLabel = gtk_label_new("Time: 60 seconds");
    gtk_grid_attach(GTK_GRID(gridContainer), timeLabel, GRID_SIZE / 2, GRID_SIZE, GRID_SIZE / 2, 1);

    // Show all elements
    gtk_widget_show_all(window);

    // Start updating the time every second
    g_timeout_add_seconds(1, (GSourceFunc)update_time, NULL);

    // Start the GTK main loop
    gtk_main();

    return 0;
}