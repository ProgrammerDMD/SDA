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
    int id;
    int rows;
    int printedRows;
} Document;

typedef struct Printer {
    int id;
    int speed;

    int documentsSize;
    Queue* documentsQueue;
    LinkedList* printers;

    struct Printer* right;
    struct Printer* left;
} Printer;

typedef struct PrintingDetails {
    int largestPrintingTime, lowestPrintingTime, largestRowSize, lowestRowSize;
    long averagePrintingTime;
    double ratio;
} PrintingDetails;

typedef struct ThreadStartDetails {
    Printer* head;
    int maxThreads;
    int jobId;
    int* threadsResult;
} ThreadStartDetails;

Document* createDocument(int id, int* maxRows);
Printer* createPrinter(int id, int* minSpeed, int* maxSpeed);
Node* createNode(void* data);
Queue* createQueue();
LinkedList* createList(Node* node);
void push_printer(Printer* head, Printer* printer);
void push_front(LinkedList* list, Node* node);
void push_back(Queue* queue, Node* node);
Node* pop_front(Queue* queue);

Document* createDocument(int id, int* maxRows) {
    Document* document = malloc(sizeof(Document));
    document->id = id;
    document->printedRows = 0;
    document->rows = rand() % (*maxRows) + 1;
    return document;
}

Printer* createPrinter(int id, int* minSpeed, int* maxSpeed) {
    Printer* printer = malloc(sizeof(Printer));
    printer->id = id;
    printer->speed = rand() % (*maxSpeed - *minSpeed) + *minSpeed;

    printer->documentsSize = 0;
    printer->documentsQueue = createQueue();

    printer->printers = NULL;
    printer->right = NULL;
    printer->left = NULL;

    return printer;
}

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

Queue* createQueue() {
    Queue* queue = malloc(sizeof(Queue));
    queue->head = NULL;
    queue->tail = NULL;
    queue->size = 0;
    return queue;
}

Queue* allocateDocuments(int totalDocuments, int maxRows, PrintingDetails* details) {
    Queue* queue = createQueue();

    details->largestRowSize = 0;
    details->lowestRowSize = INT_MAX;

    for (int i = 0; i < totalDocuments; i++) {
        Document* document = createDocument(i, &maxRows);

        if (document->rows > details->largestRowSize) {
            details->largestRowSize = document->rows;
        }

        if (document->rows < details->lowestRowSize) {
            details->lowestRowSize = document->rows;
        }

        push_back(queue, createNode(document));
    }
    return queue;
}

Printer* allocatePrinters(int totalPrinters, int minSpeed, int maxSpeed, PrintingDetails* details) {
    if (totalPrinters == 0) {
        printf("Numarul de printere nu poate sa fie 0!");
        exit(-1);
    }

    Printer* head = createPrinter(0, &minSpeed, &maxSpeed);
    details->averagePrintingTime = 0;

    details->largestPrintingTime = head->speed;
    details->lowestPrintingTime = head->speed;

    for (int i = 1; i < totalPrinters; i++) {
        Printer* printer = createPrinter(i, &minSpeed, &maxSpeed);

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
    details->averagePrintingTime /= 100;

    return head;
}

void push_printer(Printer* head, Printer* printer) {
    if (head == NULL) return;

    if (head->speed == printer->speed) {
        if (head->printers == NULL) head->printers = createList(createNode(head));
        push_front(head->printers, createNode(printer));
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

void push_front(LinkedList* list, Node* node) {
    if (list == NULL) return;

    Node* oldHead = list->head;
    if (list->head->next == list->head) {
        list->head = node;
        node->next = oldHead;
        list->tail = oldHead;
        oldHead->next = list->head;
        return;
    }

    list->head = node;
    node->next = oldHead;
    list->tail->next = node;
}

void push_back(Queue* queue, Node* node) {
    if (queue == NULL) return;

    queue->size += 1;
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

Node* pop_front(Queue* queue) {
    if (queue->head == NULL) return NULL;

    queue->size -= 1;

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

int getRemainingDocuments(Printer* head) {
    if (head == NULL) return 0;
    return head->documentsSize + getRemainingDocuments(head->right) + getRemainingDocuments(head->left);
}

int getTimeToProcessDocument(Document* document, PrintingDetails* details) {
    return (document->rows - details->lowestRowSize) * details->ratio + details->lowestPrintingTime;
}

Printer* __findExactPrinterForDocument__(Printer* head, int* time) {
    if (head == NULL) return NULL;
    if (head->speed == *time) return head;

    Printer* search;

    if (*time >= head->speed) {
        search = __findExactPrinterForDocument__(head->right, time);
    } else {
        search = __findExactPrinterForDocument__(head->left, time);
    }

    return search == NULL ? head : search;
}

Printer* findExactPrinterForDocument(Document* document, Printer* head, PrintingDetails* details) {
    int timeToProcess = getTimeToProcessDocument(document, details);
    return __findExactPrinterForDocument__(head, &timeToProcess);
}

Printer* __findApproximatePrinterForDocument__(Printer* head, int* time, int* lowerSpeed, int* upperSpeed) {
    if (head == NULL) return NULL;

    int timePerRow = head->speed;
    if (timePerRow >= *lowerSpeed && timePerRow <= *upperSpeed) return head;

    Printer* search;

    if (*time >= head->speed) {
        search = __findApproximatePrinterForDocument__(head->right, time, lowerSpeed, upperSpeed);
    } else {
        search = __findApproximatePrinterForDocument__(head->left, time, lowerSpeed, upperSpeed);
    }

    return search == NULL ? head : search;
}

Printer* findApproximatePrinterForDocument(Document* document, Printer* head, PrintingDetails* details) {
    int timeToProcess = getTimeToProcessDocument(document, details);
    int lower = timeToProcess - details->averagePrintingTime;
    int upper = timeToProcess + details->averagePrintingTime;

    return __findApproximatePrinterForDocument__(head, &timeToProcess, &lower, &upper);
}

int increaseRowsSize(Printer* printer) {
    int rowsToPrint = printer->speed;
    int documentsRemoved = 0;

    do {
        Node* node = printer->documentsQueue->head;
        if (node == NULL) return documentsRemoved;

        Document* document = node->data;
        document->printedRows += rowsToPrint;

        rowsToPrint = 0;

        if (document->printedRows >= document->rows) {
            node = pop_front(printer->documentsQueue);

            documentsRemoved++;
            rowsToPrint += document->printedRows - document->rows;

            free(node->data);
            free(node);
        }
    } while (rowsToPrint > 0);

    return documentsRemoved;
}

void __processDocuments__(Printer* head, int* maxThreads, int* jobId, int* shouldContinue, int* threadsResult) {
    if (head == NULL) return;

    if (head->id % (*maxThreads) == *jobId) {
        if (head->printers == NULL) {
            int removedDocuments = increaseRowsSize(head);
            head->documentsSize -= removedDocuments;
            threadsResult[*jobId] += removedDocuments;
        } else {
            for (Node* node = head->printers->head; ;node = node->next) {
                int removedDocuments = increaseRowsSize(node->data);
                head->documentsSize -= removedDocuments;
                threadsResult[*jobId] += removedDocuments;
                if (node->next == head->printers->head) break;
            }
        }

        if (!(*shouldContinue)) *shouldContinue = head->documentsSize > 0;
    }

    __processDocuments__(head->right, maxThreads, jobId, shouldContinue, threadsResult);
    __processDocuments__(head->left, maxThreads, jobId, shouldContinue, threadsResult);
}

void* processDocuments(void* arg) {
    ThreadStartDetails* details = arg;

    int shouldContinue;
    do {
        shouldContinue = 0;
        __processDocuments__(details->head, &details->maxThreads, &details->jobId, &shouldContinue, details->threadsResult);
        sleep(1);
    } while (shouldContinue);

    pthread_exit(NULL);
}

void createThreads(Printer* root, Printer* head, int* maxThreads, int* threadsList, int* threadsResult) {
    if (head == NULL) return;

    if (!threadsList[head->id % (*maxThreads)]) {
        ThreadStartDetails* details = malloc(sizeof(ThreadStartDetails));

        details->head = root;
        details->jobId = head->id % (*maxThreads);
        details->maxThreads = *maxThreads;
        details->threadsResult = threadsResult;

        pthread_t id;
        pthread_create(&id, NULL, processDocuments, details);
        threadsList[details->jobId] = 1;
    }

    createThreads(root, head->right, maxThreads, threadsList, threadsResult);
    createThreads(root, head->left, maxThreads, threadsList, threadsResult);
}

int main() {
    srand(time(NULL));

    int totalPrinters, maxSpeed, minSpeed, totalDocuments, maxRows;

    printf("Care este numarul total de printere? ");
    scanf("%d", &totalPrinters);
    printf("Care este viteza maxima a printerului? (rand/s) ");
    scanf("%d", &maxSpeed);
    printf("Care este viteza minima a printerului? (rand/s) ");
    scanf("%d", &minSpeed);
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

    Printer* head = allocatePrinters(totalPrinters, minSpeed, maxSpeed, details);
    Queue* documents = allocateDocuments(totalDocuments, maxRows, details);

    details->ratio = (double) (details->largestPrintingTime - details->lowestPrintingTime) / (details->largestRowSize - details->lowestRowSize);

    clock_t end = clock();
    printf("Memoria a fost alocata in %.4lfs\n\n", (double) (end - start) / CLOCKS_PER_SEC);

    /*
     * Adaugarea documentelor in lista de asteptare a printerelor, optimizat pentru fiecare printer
     */

    system("pause");
    system("cls");

    printf("Se adauga toate documentele in lista de printare.\n\n");
    start = clock();

    Node* node = pop_front(documents);
    while (node) {
        Printer* printer = findExactPrinterForDocument(node->data, head, details);
        printer->documentsSize += 1;

        LinkedList* list = printer->printers;
        if (list != NULL) {
            printer = (Printer*) list->current->data;
            list->current = list->current->next;
        }

        push_back(printer->documentsQueue, node);
        node = pop_front(documents);
    }

    end = clock();
    printf("Documentele au fost adaugate in %.4lfs\n\n", (double) (end - start) / CLOCKS_PER_SEC);

    system("pause");
    system("cls");

    printf("Care este numarul maxim de Thread-uri? ");
    int maxThreads;
    scanf("%d", &maxThreads);

    int* threadsList = calloc(maxThreads, sizeof(int));
    int* threadsResult = calloc(maxThreads, sizeof(int));

    createThreads(head, head, &maxThreads, threadsList, threadsResult);

    int printedDocuments = getRemainingDocuments(head);

    start = clock();
    while (printedDocuments > 0) {
        sleep(1);
        system("cls");
        printf("Are loc procesul de printare...\n\n");

        printedDocuments = getRemainingDocuments(head);

        for (int i = 0; i < maxThreads; i++) {
            printf("Thread #%d, documente printate: %d\n", i, threadsResult[i]);
        }

        printf("\nDocumente ramase de printat: %d", printedDocuments);
    }

    end = clock();
    printf("\nDocumentele au fost printate in %lfs\n\n",(double) (end - start) / CLOCKS_PER_SEC);
    system("pause");

    return 0;
}
