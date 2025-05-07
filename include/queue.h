#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
/**
 * @brief 큐 노드 구조체
 */
typedef struct queue_node {
    void* pvData;                    /**< 사용자 데이터 포인터 */
    int iDataSize;
    struct queue_node* next;      /**< 다음 노드를 가리키는 포인터 */
} QUEUE_NODE;

/**
 * @brief 큐 구조체
 */
typedef struct {
    QUEUE_NODE* front;            /**< 큐의 프론트 노드 포인터 */
    QUEUE_NODE* rear;             /**< 큐의 리어 노드 포인터 */
    int iCount;                   /**< 현재 큐에 저장된 항목 수 */
    int iMaxCount;                /**< 큐 최대 크기 */
    pthread_mutex_t mutex;       /**< 큐 보호용 뮤텍스 */
    pthread_cond_t cond;         /**< 조건 변수 (데이터 대기) */
} QUEUE;

/**
 * @brief 큐 초기화
 * @param pstQueue 큐 포인터
 * @param iMaxCount 큐 최대 크기
 * @return 0 성공, 실패 시 -1
 */
int queueInit(QUEUE* pstQueue, int iMaxCount);

/**
 * @brief 큐 해제 및 리소스 정리
 * @param pstQueue 큐 포인터
 */
void queueDestroy(QUEUE* pstQueue);

/**
 * @brief 큐에 데이터 삽입
 * @param pstQueue 큐 포인터
 * @param pvData 삽입할 데이터 포인터
 * @return 1 성공, 0 실패
 */
int queuePush(QUEUE* pstQueue, void* pvData);

/**
 * @brief 큐에서 데이터 제거 및 반환
 * @param pstQueue 큐 포인터
 * @param pvData 제거된 데이터 포인터 주소
 * @return 1 성공, 0 실패
 */
int queuePop(QUEUE* pstQueue, void** pvData);

/**
 * @brief 큐가 비어있는지 확인
 * @param pstQueue 큐 포인터
 * @return 1 비어있음, 0 비어있지 않음
 */
int queueIsEmpty(QUEUE* pstQueue);

/**
 * @brief 큐가 가득 찼는지 확인
 * @param pstQueue 큐 포인터
 * @return 1 가득 참, 0 공간 있음
 */
int queueIsFull(QUEUE* pstQueue);

/**
 * @brief 큐에 저장된 데이터 개수 반환
 * @param pstQueue 큐 포인터
 * @return 데이터 개수
 */
int queueSize(QUEUE* pstQueue);
#endif // QUEUE_H
