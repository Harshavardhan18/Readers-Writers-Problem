#include<gtk/gtk.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#define max_count 10

pthread_mutex_t mutex;
pthread_t threads[max_count];

sem_t reads,writes;

FILE *fp;

int k = 0;
char c;
int count = 0;
int reader_count = 0;
int writer_count = 0;

void initialize()
{
    pthread_mutex_init(&mutex,NULL);
    sem_init(&reads,0,1);
    sem_init(&writes,0,1);
}

void *readers(void *arg)
{
    int i = reader_count;
    g_print("\nOpening file for Reader::%d\n",reader_count);
    sem_wait(&reads);
    count++;
    if (count == 1)
    {
        sem_wait(&writes);
	fp = fopen("file.txt","r");
        if (fp == NULL)
        {
            g_print("File not found\n");
            exit(0);
        }
        c = fgetc(fp);
        while (c != EOF)
        {
            printf("%c",c);
       	    c = fgetc(fp);
    	}
        fclose(fp);        
    }
    sem_post(&reads);
    sem_wait(&reads);
    count--;
    if (count == 0)
    {
        sleep(5);
        g_print("\n\nReader %d has readed the file\n",i); 
        g_print("\n");
        sem_post(&writes);
        reader_count--;
    }
    sem_post(&reads);
    k--;   
}

void * writers(void *arg)
{
    char text[50];
    g_print("Opening file for appending\n\n");
    sem_wait(&writes);
    //pthread_mutex_lock(&mutex);


    printf("Enter the text to be appended:");
    //scanf("%[^\n]s",text);
    scanf("%s",text);
    fp = fopen("file.txt","a+");
    if (fp == NULL)
    {
        g_print("File not found\n");
        exit(0);
    }
    fprintf(fp,"\n%s",text);
    fclose(fp);
    
    //pthread_mutex_unlock(&mutex);
    sem_post(&writes);
    writer_count--;
    g_print("\nWriter has modified the file\n\n");
    k--;
}


void r_click(GtkWidget *widget, gpointer data)
{
    if (reader_count >= max_count)
        g_print("CAN'T OPEN ANOTHER READER\nMaximum readers alloted\n\n");
    else if (writer_count >= 1)
        g_print("Writer is altering the file\nPlease wait...\n\n");
    else
    {
        reader_count++;
        pthread_create(&threads[k++],NULL,readers,NULL);
    }
}

void w_click(GtkWidget *widget, gpointer data)
{
    if (writer_count >= 1)
        g_print("\nCannot give you permission\nWriter is modifing a file\n\n");
    else if (writer_count == 0 && reader_count == 0)
    {
        writer_count++;
        pthread_create(&threads[k++],NULL,writers,NULL);      
    }
    else
        g_print("\nReaders are reading the file\nPlease wait...\n\n");
}

void exit_fn(GtkWidget *widget, gpointer data)
{
    gtk_main_quit();
}

int main(int argc,char* argv[])
{
    system("clear");

    printf("\n\n	                                                Readers and Writers Problem\n\n\n\n");
    
    gtk_init(&argc,&argv);
    GtkWidget *window1,*window2,*window3;
    GtkWidget *r_button,*w_button,*exit;
    GtkWidget *r_button_box,*w_button_box;

    initialize();

    window1 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window1),"Reader's and Writer's");  
    gtk_widget_set_size_request (window1,400,200);

    window2 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window2),"Reader's and Writer's");  
    gtk_widget_set_size_request (window2,400,200);
 
    window3 = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window3),"EXIT");  
    gtk_widget_set_size_request (window3,200,100);

    r_button = gtk_button_new_with_label ("Readers"); 
    r_button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add (GTK_CONTAINER (window1), r_button_box);
    gtk_container_add (GTK_CONTAINER (r_button_box), r_button);

    w_button = gtk_button_new_with_label ("Writers"); 
    w_button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add (GTK_CONTAINER (window2), w_button_box);
    gtk_container_add (GTK_CONTAINER (w_button_box), w_button);
   
    exit = gtk_button_new_with_label ("EXIT");
    gtk_container_add (GTK_CONTAINER (window3), exit);
    
    //reader_signal
    g_signal_connect (r_button,"clicked",G_CALLBACK(r_click),NULL);
    //writer_signal
    g_signal_connect (w_button,"clicked",G_CALLBACK(w_click),NULL);
    //exit_signal
    g_signal_connect (exit,"clicked",G_CALLBACK(exit_fn),NULL);
    //exit_window
    g_signal_connect (window3,"destroy",G_CALLBACK(gtk_main_quit),NULL);
    
    gtk_widget_show_all (window1);
    gtk_widget_show_all (window2);
    gtk_widget_show_all (window3);
  
    for(int i=0;i<max_count;i++)
        pthread_join (threads[i],NULL);

    gtk_main();
    return 0;
}
