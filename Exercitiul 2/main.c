#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct Node {
    void* data;
    struct Node* next;
} Node;

typedef struct LinkedList {
    Node* head;
    Node* tail;
    Node* current;
} LinkedList;

typedef struct Queue {
    Node* head;
    Node* tail;
    int size;
} Queue;

typedef struct Document {
    unsigned int id;

//    char* name;

    int rows;
    int printedRows;
} Document;

typedef struct Printer {
    int id;
    int speed;

    int queueSize;
    Queue* queue;
    LinkedList* list;

    struct Printer* right;
    struct Printer* left;
} Printer;

typedef struct ThreadStartDetails {
    Printer* head;
    int maxThreads;
    int jobId;
} ThreadStartDetails;

typedef struct PrintingDetails {
    int largestPrintingTime, lowestPrintingTime, largestRowSize, lowestRowSize;
    long averagePrintingTime;
    double ratio;
} PrintingDetails;

Node* createNode(void* data) {
    Node* node = malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    return node;
}

LinkedList* createList(Node* node) {
    LinkedList* list = malloc(sizeof(LinkedList));

    list->head = node;
    list->head->next = node;
    list->tail = NULL;
    list->current = list->head;

    return list;
}

void ll_push_front(LinkedList* list, Node* node) {
    Node* oldHead = list->head;
    if (list->head->next == list->head) {
        list->head = node;
        node->next = oldHead;
        list->tail = oldHead;
        return;
    }

    list->head = node;
    node->next = oldHead;
    list->tail->next = node;
}

Queue* createQueue() {
    Queue* queue = malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

void q_push_back(Queue* queue, Node* node) {
    if (queue->head == NULL) {
        queue->head = node;
        return;
    }

    if (queue->tail == NULL) {
        queue->tail = node;
        queue->head->next = node;
        return;
    }

    queue->tail->next = node;
    queue->tail = node;
}

Node* q_pop_front(Queue* queue) {
    if (queue->head == NULL) return NULL;

    Node* node = queue->head;
    if (node->next == queue->tail) {
        queue->head = queue->tail;
        queue->tail = NULL;
    } else if (node->next != NULL) {
        queue->head = node->next;
    }

    node->next = NULL;
    return node;
}

Printer* createPrinter(int id, int* maxSpeed) {
    Printer* printer = malloc(sizeof(Printer));

    printer->id = id;
    printer->speed = rand() % (*maxSpeed) + 1;

    printer->queueSize = 0;
    printer->queue = createQueue();
    printer->right = NULL;
    printer->left = NULL;
    printer->list = NULL;

    return printer;
}

Document* createDocument(int id, int* maxRows) {
    Document* document = malloc(sizeof(Document));
    document->id = id;
//    document->name = NULL;
    document->printedRows = 0;
    document->rows = rand() % (*maxRows) + 1;
    return document;
}

void push_printer(Printer* head, Printer* printer) {
    if (head->speed == printer->speed) {
        if (head->list == NULL) head->list = createList(createNode(head));
        ll_push_front(head->list, createNode(printer));
        return;
    }

    if (printer->speed > head->speed) {
        if (head->right == NULL) {
            head->right = printer;
        } else {
            push_printer(head->right, printer);
        }
    } else {
        if (head->left == NULL) {
            head->left = printer;
        } else {
            push_printer(head->left, printer);
        }
    }
}

Printer* allocatePrinters(int totalPrinters, int maxSpeed, PrintingDetails* details) {
    if (totalPrinters == 0) {
        printf("Numarul de printere nu poate sa fie 0!");
        exit(-1);
    }

    Printer* head = createPrinter(0, &maxSpeed);
    details->averagePrintingTime = 0;

    details->largestPrintingTime = head->speed;
    details->lowestPrintingTime = head->speed;

    for (int i = 1; i < totalPrinters; i++) {
        Printer* printer = createPrinter(i, &maxSpeed);

        if (printer->speed > details->largestPrintingTime) {
            details->largestPrintingTime = printer->speed;
        }

        if (printer->speed < details->lowestPrintingTime) {
            details->lowestPrintingTime = printer->speed;
        }

        details->averagePrintingTime += printer->speed;
        push_printer(head, printer);
    }

    details->averagePrintingTime /= totalPrinters;
    details->averagePrintingTime /= 20;

    return head;
}

Queue* allocateDocuments(int totalDocuments, int maxRows, PrintingDetails* details) {
    Queue* queue = createQueue();

    details->largestRowSize = 0;
    details->lowestRowSize = INT_MIN;

    for (int i = 0; i < totalDocuments; i++) {
        Document* document = createDocument(i, &maxRows);

        if (document->rows < details->largestRowSize) {
            details->largestRowSize = document->rows;
        }

        if (document->rows > details->lowestRowSize) {
            details->lowestRowSize = document->rows;
        }

        q_push_back(queue, createNode(document));
    }
    return queue;
}

Printer* findPrinterByApproximateTime(Printer* head, int* time, int* lowerSpeed, int* upperSpeed) {
    if (head == NULL) return NULL;

    int timePerRow = head->speed;
    if (timePerRow >= *lowerSpeed && timePerRow <= *upperSpeed) return head;

    Printer* search;

    if (*time >= head->speed) {
        search = findPrinterByApproximateTime(head->right, time, lowerSpeed, upperSpeed);
    } else {
        search = findPrinterByApproximateTime(head->left, time, lowerSpeed, upperSpeed);
    }

    return search == NULL ? head : search;
}

Printer* findPrinterByExactTime(Printer* head, int* time) {
    if (head == NULL) return NULL;
    if (head->speed == *time) return head;

    Printer* search;

    if (*time >= head->speed) {
        search = findPrinterByExactTime(head->right, time);
    } else {
        search = findPrinterByExactTime(head->left, time);
    }

    return search == NULL ? head : search;
}

int getTimeToProcessDocument(Document* document, PrintingDetails* details) {
    return (document->rows - details->lowestRowSize) * details->ratio + details->lowestPrintingTime;
}

Printer* findApproximatePrinterForDocument(Document* document, Printer* head, PrintingDetails* details) {
    int timeToProcess = getTimeToProcessDocument(document, details);
    int lower = timeToProcess - details->averagePrintingTime;
    int upper = timeToProcess + details->averagePrintingTime;

    return findPrinterByApproximateTime(head, &timeToProcess, &lower, &upper);
}

Printer* findExactPrinterForDocument(Document* document, Printer* head, PrintingDetails* details) {
    int timeToProcess = getTimeToProcessDocument(document, details);
    return findPrinterByExactTime(head, &timeToProcess);
}

void getTotalRemainingDocuments(Printer* head, int* sum) {
    if (head == NULL) return;

    *sum += head->queueSize;
    getTotalRemainingDocuments(head->right, sum);
    getTotalRemainingDocuments(head->left, sum);
}

int increaseRowSize(Printer* printer) {
    Node* node = printer->queue->head;
    if (node) {
        Document* document = node->data;
        document->printedRows += printer->speed;

        if (document->printedRows >= document->rows) {
            node = q_pop_front(printer->queue);
            free(document);
            free(node);
            return 1;
        }
    }

    return 0;
}

void processDocumentsHelper(Printer* head, int* shouldContinue) {
    if (head->list == NULL) {
        if (increaseRowSize(head)) {
            head->queueSize -= 1;
        }
    } else {
        for (Node* node = head->list->head; ;node = node->next) {
            if (increaseRowSize(node->data)) {
                head->queueSize -= 1;
            }
            if (node->next == head->list->head) break;
        }
    }

    if (!(*shouldContinue)) *shouldContinue = head->queueSize > 0;
}

void processDocumentsRecursive(Printer* head, int* maxThreads, int* jobId, int* shouldContinue) {
    if (head == NULL) return;

    if (head->id % (*maxThreads) == *jobId) {
        processDocumentsHelper(head, shouldContinue);
    }

    processDocumentsRecursive(head->right, maxThreads, jobId, shouldContinue);
    processDocumentsRecursive(head->left, maxThreads, jobId, shouldContinue);
}

void* processDocuments(void* arg) {
    ThreadStartDetails* details = arg;

    int shouldContinue;
    do {
        shouldContinue = 0;
        processDocumentsRecursive(details->head, &details->maxThreads, &details->jobId, &shouldContinue);
        sleep(1);
    } while (shouldContinue);

    pthread_exit(NULL);
}

void delay(long milliseconds)
{
    clock_t start_time = clock();
    while (clock() < start_time + milliseconds);
}

void createThreadsRecursive(Printer* head, int* maxThreads, int* threadsList) {
    if (head == NULL) return;

    if (!threadsList[head->id % (*maxThreads)]) {

        ThreadStartDetails* details = malloc(sizeof(ThreadStartDetails));
        details->head = head;
        details->jobId = head->id % (*maxThreads);
        details->maxThreads = *maxThreads;

        pthread_t id;
        pthread_create(&id, NULL, processDocuments, details);
        threadsList[details->jobId] = 1;
    }

    createThreadsRecursive(head->right, maxThreads, threadsList);
    createThreadsRecursive(head->left, maxThreads, threadsList);
}

int main() {
    srand(time(NULL));

    int totalPrinters, maxSpeed, totalDocuments, maxRows;

    printf("Care este numarul total de printere? ");
    scanf("%d", &totalPrinters);
    printf("Care este viteza maxima a fiecarui printer? (rand/s) ");
    scanf("%d", &maxSpeed);
    printf("Care este numarul total de documente? ");
    scanf("%d", &totalDocuments);
    printf("Care este numarul maxim de randuri pentru fiecare document? ");
    scanf("%d", &maxRows);

    printf("\nSe aloca in memorie documentele si printerele...\n\n");
    clock_t start = clock();

    /*
     * Alocarea memoriei pentru printere si documente
     */
    PrintingDetails* details = malloc(sizeof(PrintingDetails));
    Printer* head = allocatePrinters(totalPrinters, maxSpeed, details);
    Queue* documents = allocateDocuments(totalDocuments, maxRows, details);

    details->ratio = (double) (details->largestPrintingTime - details->lowestPrintingTime) / (details->lowestRowSize - details->largestRowSize);

    clock_t end = clock();
    printf("Memoria a fost alocata in %.4lfs\n\n", (double) (end - start) / CLOCKS_PER_SEC);

    /*
     * Adaugarea documentelor in lista de asteptare a printerelor, optimizat pentru fiecare printer
     */

    system("pause");
    system("cls");
    Node* node = q_pop_front(documents);

    printf("Se adauga toate documentele in lista de printare.\n\n");
    start = clock();

    while (node) {
        Printer* printer = findExactPrinterForDocument(node->data, head, details);
        node->next = NULL;

        printer->queueSize += 1;

        LinkedList* list = printer->list;
        if (list != NULL) {
            printer = list->current->data;
            list->current = list->current->next;
        }

        q_push_back(printer->queue, node);
        node = q_pop_front(documents);
    }

    end = clock();
    printf("Documentele au fost adaugate in %.4lfs\n", (double) (end - start) / CLOCKS_PER_SEC);
    system("pause");

    system("cls");
    printf("Care este numarul maxim de Thread-uri? ");

    int maxThreads;
    scanf("%d", &maxThreads);

    int* threadsList = calloc(maxThreads, sizeof(int));

    createThreadsRecursive(head, &maxThreads, threadsList);

    int printedDocuments = 0;
    getTotalRemainingDocuments(head, &printedDocuments);

    start = clock();
    while (printedDocuments > 0) {
        system("cls");
        printf("Are loc procesul de printare...\n");

        printedDocuments = 0;
        getTotalRemainingDocuments(head, &printedDocuments);

        printf("Documente printate: %d/%d", printedDocuments, totalDocuments);
        sleep(1);
    }

    end = clock();
    printf("\nDocumentele au fost printate in %lfs",(double) (end - start) / CLOCKS_PER_SEC);

    return 0;
}
