#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct // 구조체 선언(별칭을 Queue로 지정하기 위해 typedef를 사용)
{
    char names[MAX_SIZE][100]; // 학생 목록을 담을 배열을 선언
    int front, rear; // 큐의 위치를 가리킬 멤버 변수 front, rear를 선언
} Queue; // 별칭(Alias)을 Queue로 설정

// 함수 선언(프로토타입))
void initializeQueue(Queue *queue);
int isEmpty(Queue *queue);
int isFull(Queue *queue);
void enqueue(Queue *queue, const char *name);
void dequeue(Queue *queue, const char *name);
void processing(Queue *queue, const char *name);
void moveItems(Queue *sourceQueue, Queue *destinationQueue, int count);
void displayQueue(Queue *queue, int displayCount);

Queue loadingQueue, waitingQueue, successQueue; // 인스턴스 선언(처음에 지정한 Queue라는 구조를 가진 인스턴스를 3개 생성합니다.)

int main(void)
{
    // 인스턴스들의 멤버 변수를 초기화(구조체와 동일한 인스턴스를 생성만 한 상태이기에 쓰레기 값이 할당되어 있을 것입니다.)
    initializeQueue(&loadingQueue);
    initializeQueue(&waitingQueue);
    initializeQueue(&successQueue);

    int maxLoadingCapacity;
    printf("한 번에 처리할 수 있는 최대 학생 수를 입력하세요: ");
    scanf("%d", &maxLoadingCapacity);

    int choice;

    char name[50];

    do
    {
        printf("\n1. 큐에 추가 (학생 추가)\n");
        printf("2. 큐에서 제거 (학생 제거)\n");
        printf("3. 로딩 큐 표시\n");
        printf("4. 대기 큐 표시\n");
        printf("5. 프로세스 처리\n");
        printf("6. 성공 큐 표시\n");
        printf("0. 프로그램 종료\n");
        printf("원하는 작업을 선택하세요: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1: // 학생 추가
            printf("학생 이름을 입력하세요: ");
            scanf("%s", name); // 1. 입력
            if (loadingQueue.rear - loadingQueue.front + 1 < maxLoadingCapacity) // 로딩 큐의 원소 개수가 한 번에 처리할 수 있는 프로세스의 개수보다 적다면
            {
                enqueue(&loadingQueue, name);
            }
            else
            {
                enqueue(&waitingQueue, name); //
            }
            break;
        case 2: // 학생 제거
            printf("제거할 학생의 이름을 입력하세요: ");
            scanf("%s", name);
            dequeue(&loadingQueue, name);
            dequeue(&waitingQueue, name);
            break;
        case 3: // 로딩 큐 출력
            displayQueue(&loadingQueue, maxLoadingCapacity);
            break;
        case 4: // 대기 큐 출력
            displayQueue(&waitingQueue, MAX_SIZE - maxLoadingCapacity);
            break;
        case 5: // 로딩 중인 프로세스(대기 인원) 처리
            // 1. 로딩 큐의 인원을 성공 큐로 이동.
            moveItems(&loadingQueue, &successQueue, maxLoadingCapacity);

            // 2. 대기 큐의 인원 중 들어온 순서대로 maxLoadingCapacity만큼 로딩 큐로 이동
            moveItems(&waitingQueue, &loadingQueue, maxLoadingCapacity);

            if (isEmpty(&loadingQueue) && isEmpty(&waitingQueue))
            {
                printf("더 이상 처리할 프로세스가 없습니다.\n");
            }
            break;

        case 6: // 성공 큐 표시
            displayQueue(&successQueue, MAX_SIZE);
            break;
        case 0: // 프로그램 종료
            printf("프로그램을 종료합니다.\n");
            initializeQueue(&loadingQueue);
            initializeQueue(&waitingQueue);
            initializeQueue(&successQueue);
            break;
        default: // 예외처리: 잘못된 선택
            printf("잘못된 선택입니다. 올바른 옵션을 입력하세요.\n");
            break;
        }
    } while (choice != 0);

    return 0;
}

// ---------------함수 정의--------------

void initializeQueue(Queue *queue) // 큐 초기화 함수
{
    queue->front = -1; // 큐의 맨 앞을 가리키는 포인터: front와
    queue->rear = -1;  // 큐의 맨 뒤를 가리키는 포인터: rear를 -1로 초기화 해줍니다.
}

int isEmpty(Queue *queue) // 빈 큐인지 체킹
{
    return (queue->front == -1 && queue->rear == -1); // 큐의 front와 rear의 상태가 초기값과 같다면 해당 큐는 빈 것이라고 판단하게 됩니다.
}

int isFull(Queue *queue) // 가득 찬 큐인지 체킹
{
    return (queue->rear + 1) % MAX_SIZE == queue->front; // 큐의 rear+1 값을 MAX_SIZE로 나누었을 때, 나머지가 front와 같다면 가득 차있음을 뜻합니다.
}

void enqueue(Queue *queue, const char *name) // 데이터 삽입 함수
{
    if (isFull(queue))
    {
        printf("큐가 가득 찼습니다. 추가할 수 없습니다.\n");
        return;
    }

    if (isEmpty(queue)) // 빈 경우
    {
        queue->front = 0; // front와 rear를 0으로 셋팅하여
        queue->rear = 0; // 데이터를 삽입할 위치를 설정합니다.
    }
    else // 이미 데이터가 존재하는 경우
    {
        queue->rear = (queue->rear + 1) % MAX_SIZE; // 원형 큐 개념 사용하여 데이터를 삽입할 다음 위치를 셋팅합니다.
    }

    strcpy(queue->names[queue->rear], name); // 타겟팅된 위치에 입력받은 name을 삽입하고, 
    printf("%s 학생이 성공적으로 등록되었습니다.\n", name); // 확인 문구를 출력합니다.
}

void dequeue(Queue *queue, const char *name) // 데이터 삭제 함수
{
    // -------------효율을 위해 빈 큐 탐색을 가장 먼저 실시-------------
    if (isEmpty(queue)) // 큐가 비었다면
    {
        printf("큐가 비어있습니다. 삭제할 수 없습니다.\n"); // 경고문구를 발생시키고 메뉴에서 빠져나오도록
        return;
    }

    // -------------큐에 값이 있다는 정보를 얻은 상태에서 서칭 시작-------------
    int current = queue->front;
    int found = 0; // 초기 값을 0으로 설정합니다(true: 1/false: 0으로 표시할 예정입니다)

    // 
    while (current != (queue->rear + 1) % MAX_SIZE)
    {
        if (name == NULL || strcmp(queue->names[current], name) == 0) // 1. 이름을 입력하지 않았거나, 2. 입력한 이름과 names배열에서 찾은 결과가 일치한다면
        {
            found = 1; // found를 true를 의미하는 0으로 셋팅하고
            break; // 루프를 종료합니다.
        }
        current = (current + 1) % MAX_SIZE; // 못찾았다면 current의 값을 증가
    }

    if (found) 
    {
        printf("%s 학생이 큐에서 제거되었습니다.\n", queue->names[current]); 

        if (queue->front == queue->rear) // 찾았는데 큐에 데이터가 1개 뿐이라면 
        {
            initializeQueue(queue); // 해당 큐를 초기화합니다.
        }
        else // 데이터가 2개 이상이라면
        { // 일치하는 값이 해당되는 current를 없애기 위해 current 뒤의 값들을 한 칸씩 앞으로 당기는 방식으로 진행
            for (int i = current; i != queue->rear; i = (i + 1) % MAX_SIZE) // current부터 rear까지
            {
                strcpy(queue->names[i], queue->names[(i + 1) % MAX_SIZE]); // 다음 i+1번째 값을 i번째로 당겨옵니다
            }
            queue->rear = (queue->rear - 1 + MAX_SIZE) % MAX_SIZE; // 이전 위치로 이동하는 연산을 진행하여 마지막 인덱스를 앞으로 옮김으로써 마지막 값을 삭제합니다.
        }
    }
    else
    {
        printf("입력한 이름의 학생이 큐에 존재하지 않습니다.\n");
    }
}

void processing(Queue *queue, const char *name)
{
    if (isFull(queue))
    {
        printf("성공 큐가 가득 찼습니다. 추가할 수 없습니다.\n");
        return;
    }

    if (isEmpty(queue))
    {
        queue->front = 0;
        queue->rear = 0;
    }
    else
    {
        queue->rear = (queue->rear + 1) % MAX_SIZE;
    }

    strcpy(queue->names[queue->rear], name);
    printf("%s 학생이 성공 큐에 등록되었습니다.\n", name);
}

void moveItems(Queue *sourceQueue, Queue *destinationQueue, int count)
{
    for (int i = 0; i < count && !isEmpty(sourceQueue); ++i)
    {
        enqueue(destinationQueue, sourceQueue->names[sourceQueue->front]); // 이동할 큐에 소스 큐의 첫번째 값을 이동시키고
        dequeue(sourceQueue, NULL); // 첫번째 값을 지웁니다.(하나씩 이동시키며 동시에 삭제함으로써 계속해서 첫번째 값을 그 다음 값으로 이동시킬 수 있습니다.)
    }
}

void displayQueue(Queue *queue, int displayCount) // 화면에 큐를 출력
{
    // 이 역시 효율을 위해 비어있는지 먼저 확인
    if (isEmpty(queue))
    {
        printf("큐가 비어있습니다.\n");
        return;
    }

    printf("내용:\n");
    int i = queue->front; // 증감 변수에 큐의 첫 index를 삽입
    int count = 0;
    do
    {
        if (strlen(queue->names[i]) > 0) // 큐의 명단 배열이 존재한다면
        {
            printf("%s\n", queue->names[i]); // 해당 큐의 명단을 출력합니다.
            count++; // 이 count를 증가하는 이유는 아래의 !!과 같습니다.
        }
        i = (i + 1) % MAX_SIZE; // 다음 index의 학생을 출력하기 위해 count를 ++합니다.
    } while (i != (queue->rear + 1) % MAX_SIZE && count < displayCount); // !!: 지정된 출력 개수만큼 출력하기 위해 count하는 것입니다.
}
