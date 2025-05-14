/**
 * @file queue.c
 * @brief 쓰레드 안전 큐 (Thread-safe Queue) 구현
 *
 * 이 파일은 동기화된 큐 자료구조의 구현을 포함합니다.
 * 각 큐는 앞(front)과 뒤(rear) 포인터로 연결된 노드 리스트를 유지하며,
 * 데이터를 FIFO 방식으로 저장/삭제할 수 있습니다.
 *
 * 멀티 쓰레드 환경에서 안전하게 사용될 수 있도록 `pthread_mutex`를 이용한 동기화를 제공합니다.
 *
 * 주요 기능:
 * - queueInit(): 큐 초기화
 * - queuePush(): 데이터 삽입
 * - queuePop(): 데이터 제거 및 반환
 * - queueIsEmpty(): 큐가 비었는지 확인
 * - queueReset(): 큐의 모든 데이터를 제거
 * - queueDestroy(): 큐 해제 및 뮤텍스 제거
 *
 * @author 박철우
 * @date 2025-05
 */

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
}

/**
 * @brief 큐에 데이터 삽입
 */
int queuePush(QUEUE* pstQueue, void* pvData, int iDataSize) {
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
    pstNewNode->iDataSize = iDataSize;
    pstNewNode->next = NULL;

    if (pstQueue->rear) {
        pstQueue->rear->next = pstNewNode;
    } else {
        pstQueue->front = pstNewNode;
    }

    pstQueue->rear = pstNewNode;
    pstQueue->iCount++;

    pthread_mutex_unlock(&pstQueue->mutex);
    return 1;
}

/**
 * @brief 큐에서 데이터 제거
 */
int queuePop(QUEUE* pstQueue, void** pvData) {
    int iDataSize = 0;
    pthread_mutex_lock(&pstQueue->mutex);

    if (!queueIsEmpty(pstQueue)) {
        QUEUE_NODE* pstQueueNode = pstQueue->front;
        *pvData = pstQueueNode->pvData;

        pstQueue->front = pstQueueNode->next;
        if (!pstQueue->front) {
            pstQueue->rear = NULL;
        }

        pstQueue->iCount--;
        iDataSize = pstQueueNode->iDataSize;
        free(pstQueueNode);
    }
    pthread_mutex_unlock(&pstQueue->mutex);
    return iDataSize;
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