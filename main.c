#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
typedef struct partition
{
    int start_adr;
    int size;
    bool free;
} part;

typedef struct program {
    char name[20];
    int size;
    int time;
} prg;

typedef struct NODE
{
    prg data;
    struct NODE *next;
}node;

typedef struct liste {
    node *head;
    node *fin;
}Queue;

part * create_part (int num_part)
{
    part *partitions = (part*) malloc (num_part *sizeof(part));// table of structures
    int start = 0,i;
    for (i=0;i<num_part;i++)
    {
        printf("Enter the size of partition %d: ",i+1);
        scanf("%d",&(partitions [i].size));
        partitions[i].start_adr = start;
        partitions[i].free = true;
        start = start + partitions[i].size;
    }
    return partitions;
}

void print_part (part *partitions,int num_part)
{
    int i;
    for (i=0;i<num_part;i++)
    {
        if (partitions[i].free)
        {
            printf("partition %d: starting @: %d kb, size: %d kb, status:free\n",i+1,partitions[i].start_adr,partitions[i].size);
        }else{
            printf("partition %d: starting @: %d kb, size: %d kb, status:allocated\n",i+1,partitions[i].start_adr,partitions[i].size);
        }
    }
}
void addtoQ (Queue *q, prg p)
{
    node *n = (node*)malloc(sizeof(node));
    n->data = p;
    n->next = NULL;
    if (q->head==NULL)
    {
        q->head = n;
    }else{
        q->fin->next=n;
    }
    q->fin = n;
}

prg deleteQ (Queue *q)
{
    node *head = q->head;
    prg p = head->data;

    q->head = head->next;
    if (q->head ==NULL)
    {
        q->fin =NULL;
    }
    free (head);
    return p;
}
void printlist (Queue *wait)
{
    node *current = wait->head;
    while (current != NULL)
    {
        printf("[%s]|[%d]-->",current->data.name,current->data.size);
        current = current->next;
    }
    printf("NULL");
}

void first_fit (part *partitons,int num_part,Queue *wait)
{
    node *current = wait->head;
    while (current !=NULL)
    {
        prg curr_prg = current->data;

        bool found_part = false ;
        for (int i=0;i<num_part;i++)
        {
            if(partitons[i].free && partitons[i].size >= curr_prg.size)
            {
                partitons[i].free = false ;
                partitons[i].size = partitons[i].size - curr_prg.size;
                found_part = true;
                printf("your program is loaded into partition: [%d] free remaining size is %d kb",i,partitons[i].size);
                break;
            }
        }
         if (!found_part)
        {
           printf("\n error program %s couldnt be loaded in memory\n",curr_prg.name);
        }
        printf("\n program %s removed from list going to the next program\n",curr_prg.name);
        deleteQ(wait);
        current = current->next;

    }
        printf("\nthere is no next program you reached the last program\n");

}


int main (){
    int num_part,choice,num_prg;

    printf("enter the number of partitions");
    scanf("%d",&num_part);

    part *partitions = create_part(num_part);
    printf("\n------ PDT -------\n");
    printf("\n| Partition Number | Starting Address | Size (in KB)     | Status           |\n");
    printf("\n|------------------|------------------|------------------|-------------------\n");
    for (int i=0;i<num_part;i++)
    {
        printf("| %-16d | %-16d | %-16d | %-16s |\n", i+1, partitions[i].start_adr, partitions[i].size, partitions[i].free ? "Free" : "Allocated");
    }

    Queue waiting;
    waiting.head = NULL;
    waiting.fin = NULL;

    printf("\n Enter the number of programs: ");
    scanf("%d",&num_prg);

    for (int i=0;i<num_prg;i++)
    {
        prg p;
        printf("enter the name and size of program in %d: ",i+1);
        scanf("%s %d",p.name,&p.size);
        addtoQ(&waiting,p);
    }

    printf("\n--------the list is-------------------- :\n");
    printf("\n");
    printlist(&waiting);
    printf("\n");
    printf("\n--------------------------------------------\n");



    printf("\nUsing First Fit algorithm:\n");
    first_fit(partitions, num_part, &waiting);

    printf("\n------ PDT -------\n");
    printf("\n| Partition Number | Starting Address | Remaining Size(in KB)| Status           |\n");
    printf("\n|------------------|------------------|----------------------|------------------|\n");

    for (int i=0;i<num_part;i++)
    {
        printf("| %-16d | %-16d | %-16d | %-20s |\n", i+1, partitions[i].start_adr, partitions[i].size, partitions[i].free ? "Free" : "Allocated");
    }

    return 0;
}
