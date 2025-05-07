#include "queue.h"
#include <stdlib.h>

/**
 * @brief 큐 초기화
 */
int queueInit(QUEUE* pstQueue, int iMaxCount) {
    pstQueue->front = NULL;
    pstQueue->rear = NULL;
    pstQueue->iCount = 0;
    pstQueue->iMaxCount = iMaxCount;

    pthread_mutex_init(&pstQueue->mutex, NULL);
    pthread_cond_init(&pstQueue->cond, NULL);

    return 0;
}

/**
 * @brief 큐 해제
 */
void queueDestroy(QUEUE* pstQueue) {
    void* pvData;
    while (!queueIsEmpty(pstQueue)) {
        queuePop(pstQueue, &pvData);
    }

    pthread_mutex_destroy(&pstQueue->mutex);
    pthread_cond_destroy(&pstQueue->cond);
}

/**
 * @brief 큐에 데이터 삽입
 */
int queuePush(QUEUE* pstQueue, void* pvData) {
    pthread_mutex_lock(&pstQueue->mutex);

    if (queueIsFull(pstQueue)) {
        pthread_mutex_unlock(&pstQueue->mutex);
        return 0;
    }

    QUEUE_NODE* pstNewNode = (QUEUE_NODE*)malloc(sizeof(QUEUE_NODE));
    if (!pstNewNode) {
        pthread_mutex_unlock(&pstQueue->mutex);
        return 0;
    }

    pstNewNode->pvData = pvData;
    pstNewNode->next = NULL;

    if (pstQueue->rear) {
        pstQueue->rear->next = pstNewNode;
    } else {
        pstQueue->front = pstNewNode;
    }

    pstQueue->rear = pstNewNode;
    pstQueue->iCount++;

    pthread_cond_signal(&pstQueue->cond);
    pthread_mutex_unlock(&pstQueue->mutex);
    return 1;
}

/**
 * @brief 큐에서 데이터 제거
 */
int queuePop(QUEUE* pstQueue, void** pvData) {
    pthread_mutex_lock(&pstQueue->mutex);

    while (queueIsEmpty(pstQueue)) {
        pthread_cond_wait(&pstQueue->cond, &pstQueue->mutex);
    }

    QUEUE_NODE* pstQueueNode = pstQueue->front;
    *pvData = pstQueueNode->pvData;

    pstQueue->front = pstQueueNode->next;
    if (!pstQueue->front) {
        pstQueue->rear = NULL;
    }

    pstQueue->iCount--;
    free(pstQueueNode);

    pthread_mutex_unlock(&pstQueue->mutex);
    return 1;
}

/**
 * @brief 큐가 비어있는지 확인
 */
int queueIsEmpty(QUEUE* pstQueue) {
    return pstQueue->iCount == 0;
}

/**
 * @brief 큐가 가득 찼는지 확인
 */
int queueIsFull(QUEUE* pstQueue) {
    return pstQueue->iCount >= pstQueue->iMaxCount;
}

/**
 * @brief 큐에 저장된 데이터 개수 반환
 */
int queueSize(QUEUE* pstQueue) {
    return pstQueue->iCount;
}