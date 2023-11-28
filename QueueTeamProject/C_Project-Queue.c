#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct {
    char names[MAX_SIZE][50];
    int front, rear;
} Queue;

void initializeQueue(Queue *queue) {
    // 큐 초기화
    queue->front = -1;
    queue->rear = -1;
}

int isEmpty(Queue *queue) {
    // 큐가 비어있는지 확인
    return (queue->front == -1 && queue->rear == -1);
}

int isFull(Queue *queue) {
    // 큐가 가득 찼는지 확인
    return (queue->rear + 1) % MAX_SIZE == queue->front;
}

void enqueue(Queue *queue, const char *name) {
    // 큐에 데이터 추가
    if (isFull(queue)) {
        printf("큐가 가득 찼습니다. 추가할 수 없습니다.\n");
        return;
    }

    if (isEmpty(queue)) {
        // 큐가 비어있을 경우 초기화
        queue->front = 0;
        queue->rear = 0;
    } else {
        queue->rear = (queue->rear + 1) % MAX_SIZE;
    }

    strcpy(queue->names[queue->rear], name);
    printf("%s 학생이 성공적으로 등록되었습니다.\n", name);
}

void dequeue(Queue *queue) {
    // 큐에서 데이터 삭제
    if (isEmpty(queue)) {
        printf("큐가 비어있습니다. 삭제할 수 없습니다.\n");
        return;
    }

    printf("%s 학생이 큐에서 제거되었습니다.\n", queue->names[queue->front]);

    if (queue->front == queue->rear) {
        // 큐에 마지막 원소가 있었을 경우 초기화
        initializeQueue(queue);
    } else {
        queue->front = (queue->front + 1) % MAX_SIZE;
    }
}

void displayQueue(Queue *queue, const char *queueName) {
    // 큐 출력
    if (isEmpty(queue)) {
        printf("%s이 비어있습니다.\n", queueName);
        return;
    }

    printf("%s 내용:\n", queueName);
    int i = queue->front;
    do {
        printf("%s\n", queue->names[i]);
        i = (i + 1) % MAX_SIZE;
    } while (i != (queue->rear + 1) % MAX_SIZE);
}

int main(void) {
    Queue loadingQueue, waitingQueue, successQueue; // queue자료형의 3가지 상태 변수를 선언
    // 모든 변수를 초기화(argument로 주소를 전달해줘야 포인터로 값을 받는 initializeQueue함수에서 parameter로 받아 해당 주소의 값을 서칭)
    initializeQueue(&loadingQueue);
    initializeQueue(&waitingQueue);
    initializeQueue(&successQueue);

    int maxLoadingCapacity;
    printf("한 번에 처리할 수 있는 최대 학생 수를 입력하세요: ");
    scanf("%d", &maxLoadingCapacity);

    int choice;
    char name[50];

    do {
        printf("\n1. 큐에 추가 (학생 추가)\n");
        printf("2. 큐에서 제거 (학생 제거)\n");
        printf("3. 로딩 큐 표시\n");
        printf("4. 대기 큐 표시\n");
        printf("5. 수강신청 성공 표시\n");
        printf("0. 프로그램 종료\n");
        printf("원하는 작업을 선택하세요: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("학생 이름을 입력하세요: ");
                scanf("%s", name);
                // rear의 idx가 front+1한 idx
                if (loadingQueue.rear - loadingQueue.front + 1 < maxLoadingCapacity) {
                    enqueue(&loadingQueue, name);
                } else {
                    enqueue(&waitingQueue, name);
                }
                break;
            case 2:
                dequeue(&loadingQueue);
                // 대기 큐에 있는 학생이 있다면 로딩 큐에 추가
                if (!isEmpty(&waitingQueue)) {
                    char waitingName[50];
                    dequeue(&waitingQueue);
                    enqueue(&loadingQueue, waitingName);
                }
                break;
            case 3:
                displayQueue(&loadingQueue, "로딩 큐");
                break;
            case 4:
                displayQueue(&waitingQueue, "대기 큐");
                break;
            case 5:
                // 로딩 완료된 학생을 성공 큐에 추가
                if (!isEmpty(&loadingQueue)) {
                    char successName[50];
                    dequeue(&loadingQueue);
                    enqueue(&successQueue, successName);
                } else {
                    printf("로딩 중인 학생이 없습니다.\n");
                }
                break;
            case 0:
                printf("프로그램을 종료합니다.\n");
                break;
            default:
                printf("잘못된 선택입니다. 올바른 옵션을 입력하세요.\n");
        }
    } while (choice != 0);
    

    return 0;
}
