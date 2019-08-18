#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <ctype.h>

/*
    Nome: Thomas William do Prado Paiva RA: 95741
    Nome: Renan Prada 				   RA: 103263
    Nome: João Luiz Vieira                              RA: 102020
*/
#define WR 1
#define WC 2
#define MAX_BUFF (4*1024)
#define MAX 10

sem_t semaforo[MAX]; // um semaforo por endereco de memoria
sem_t izinho[WC]; // um semaforo por endereco de memoria

// sem_t i; //N: sincroniza consumo, determina elementos a serem poduzidos


sem_t mutex; //1
sem_t mutex_map; //1

// pthread_barrier_t barreira;


//em c váriaveis globais são inicializadas com zero
int j; // offset de escrita p/ produtores


typedef struct MapItem
{
    int value;
    char * key;
    struct MapItem * next;
} Map;

typedef struct ReduceItem
{
    Map **m;

} Reduce;

typedef struct
{
    Map ** m;
    char * filename;
    int bstart, bend;
} Work;


Map ** RWF;


char *Ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}

char *Rtrim(char *s)
{
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *Trim(char *s)
{
    return Rtrim(Ltrim(s));
}

int is_delimiter(char ch)
{
  return (ch>=32 && ch<=46);
}

void initializes(void){
  // pthread_barrier_init(&barreira, NULL, WR);
  sem_init(&mutex, 0, 1);
  sem_init(&mutex_map, 0, 1);


  for(int i = 0; i < MAX; i++)
  {
    sem_init(&semaforo[i], 0, 1);
  }

}

int Hash(char * key)
{
    unsigned long hash = MAX;
    int c;
    while((c = *key++))
      hash = ((hash << 5) + hash) + c;
    return hash % MAX;
}

Map ** create_map()
{
    int i;
    Map ** m = malloc(MAX*sizeof(Map));

    for (i = 0; i < MAX; i++)
        m[i] = NULL;

    return m;
}

Map * create_map_element(int value, char * key)
{
    Map *mpi = (Map*) malloc(sizeof(Map));
    mpi->value = value;
    mpi->key = strdup(key);
    mpi->next = NULL;
    return mpi;
}

void push_map(Map **m, int value, char* key, int reduceFlag)
{
    int h;
    Map * aux, * ant;
    Map * mpi = create_map_element(value, key);

    if(mpi == NULL || m == NULL) return;

    h = Hash(key);
    aux = ant = m[h];

    if(reduceFlag == 1) //caso 1, precisa de semaforo
    {
        sem_wait(&semaforo[h]);
    }


    while(aux != NULL)
    {
        if (!strcmp(aux->key, mpi->key)) break;

        ant = aux;
        aux = aux->next;
    }

    if(aux != NULL)
      aux->value += value;
    else if(ant == NULL)
      m[h] = mpi;
    else
      ant->next = mpi;


    if(reduceFlag == 1) //caso 1, precisa liberar o semaforo
    {
        sem_post(&semaforo[h]);
    }

}

void reduce(Map ** m)
{
    int i;
    Map *aux;
    for(i = 0; i < MAX; i++)
    {
        aux = m[i];
        while(aux != NULL)
        {
            push_map(RWF, aux->value, aux->key, 1);
            aux = aux->next;
        }
    }
}


void parse(char * buffer, Map **m)
{
    char *token = strtok(buffer, " ");
    while(token != NULL)
    {
        strcpy(token, Trim(token));
        push_map(m, 1, token, 0);
        token = strtok(NULL, " ");
    }
}

int produce_map(Work *w)
{
    int block_size;
    char ch, buffer[MAX_BUFF];

    int fd = open(w->filename, O_RDONLY);

    //ajustar o início do bloco
    lseek(fd, w->bstart, SEEK_SET);

    read(fd, &ch, 1);
    while(!is_delimiter(ch) && w->bstart > 0)
    {
        w->bstart--;
        lseek(fd, w->bstart, SEEK_SET);
        read(fd, &ch, 1);
    }

    //ajustar o fim do bloco
    lseek(fd, w->bend, SEEK_SET);
    read(fd, &ch, 1);
    while(!is_delimiter(ch) && w->bend > 0)
    {
        w->bend--;
        lseek(fd, w->bend, SEEK_SET);
        read(fd, &ch, 1);
    }
    //retornar para o ínicio
    lseek(fd, w->bstart, SEEK_SET);

    block_size = w->bend - w->bstart;

    read(fd, buffer, block_size);
    buffer[block_size] = '\0';

    w->bstart = w->bend;

    parse(buffer, w->m);
    return 0;

}

void * Worker_Reduce(void * args)
{
    Reduce *rw = (Reduce*)  args;

    sem_wait(&mutex);    
    reduce(rw[j].m);
    j++;
    sem_post(&mutex);
    return NULL;
}

void * Worker_Map(void * args)
{
    Work *w = (Work*) args;
    sem_wait(&mutex_map);
    produce_map(w);
    sem_post(&mutex_map);

    return NULL;
}

void print_map(Map **m)
{
    Map *aux;
    int i;
    for(i = 0; i < MAX; i++)
    {
        aux = m[i];
        while(aux != NULL)
        {
          printf("%d : %s\n", aux->value, aux->key);
          aux = aux->next;
        }
    }
}

int get_file_size(char *filename)
{
    struct stat fileStat;
    stat(filename, &fileStat);
    return fileStat.st_size;
}

int main(int argc, char **argv)
{
    if(argc != 2)
        return 1;

    int size, size_block, i;

    pthread_t wreduce[WC], work[WC];

    Work w[WC];
    Reduce rw[WC]; //rw tem que ter a mesma quantidade de maps gerado

    initializes();
    size = get_file_size(argv[1]);
    size_block = size/WC;

    printf("file size: %d\n", size);


    for(i=0; i < WC; i++){
        w[i].filename = argv[1];
        w[i].bstart = size - size_block;
        w[i].bend = size;
        w[i].m = create_map();
        rw[i].m = w[i].m;
        pthread_create(&work[i], NULL, Worker_Map, &w[i]);
        size -= size_block;
    }


    for(i=0; i < WC; i++)
    {
        pthread_join(work[i], NULL);
    }

    RWF = create_map();

    for(i = 0; i < WC; i++)
    {
        pthread_create(&wreduce[i], NULL, Worker_Reduce, &rw[i]);
    }

    for(i = 0; i < WC; i++)
      pthread_join(wreduce[i], NULL);



    for(i=0; i < WC; i++)
    {
        print_map(w[i].m);
        printf("%s\n", "----");
    }

    printf("\n%s\n", "Reduce");


    print_map(RWF);


    for(int i = 0; i < MAX; i++)
    {
        sem_destroy(&semaforo[i]);
    }


    return 0;
}
