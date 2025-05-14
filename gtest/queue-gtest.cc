/**
 * @file queue-gtest.cc
 * @brief QUEUE 자료구조에 대한 Google Test 기반 테스트 코드
 *
 * 쓰레드 안전 큐(QUEUE)의 주요 기능에 대해 단일 쓰레드 및 멀티 쓰레드 환경에서
 * 동작의 정확성과 안정성을 검증합니다.
 */
#include <gtest/gtest.h>
#include "queue.h"
#include <thread>
#include <vector>
#include <chrono>

/**
 * @test BasicPushPopTest
 * @brief 기본 Push/Pop 기능 테스트
 *
 * 단일 쓰레드 환경에서 두 개의 정수를 큐에 저장하고,
 * 이를 순서대로 꺼내는 동작이 정상적으로 수행되는지 확인합니다.
 */
TEST(QueueTest, BasicPushPopTest) {
    QUEUE q;
    queueInit(&q, 5);

    int a = 10, b = 20;
    EXPECT_TRUE(queuePush(&q, &a, 4));
    EXPECT_TRUE(queuePush(&q, &b, 4));

    void* data = nullptr;
    EXPECT_EQ(queuePop(&q, &data), 4);
    EXPECT_EQ(*(int*)data, 10);

    EXPECT_EQ(queuePop(&q, &data), 4);
    EXPECT_EQ(*(int*)data, 20);

    EXPECT_TRUE(queueIsEmpty(&q));
    queueDestroy(&q);
}

/**
 * @test QueueFullTest
 * @brief 큐 최대 용량 초과 테스트
 *
 * 용량 2의 큐에 데이터를 3개 이상 넣으려 할 때,
 * 마지막 Push가 실패해야 함을 검증합니다.
 */
TEST(QueueTest, QueueFullTest) {
    QUEUE q;
    queueInit(&q, 2);

    int a = 1, b = 2, c = 3;
    EXPECT_TRUE(queuePush(&q, &a, 4));
    EXPECT_TRUE(queuePush(&q, &b, 4));
    EXPECT_FALSE(queuePush(&q, &c, 4));  // 최대 용량 초과

    queueDestroy(&q);
}

#define MAX_QUEUE_SIZE 50

static int test_values[MAX_QUEUE_SIZE];

/**
 * @brief 생산자 쓰레드 함수
 *
 * 50개의 정수 데이터를 큐에 순차적으로 삽입합니다.
 *
 * @param arg QUEUE 포인터
 * @return NULL
 */
void* producer_fn(void* arg) {
    QUEUE* pstQueue = (QUEUE*)arg;
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i) {
        EXPECT_TRUE(queuePush(pstQueue, &test_values[i], 4));
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return nullptr;
}

/**
 * @brief 소비자 쓰레드 함수
 *
 * 큐가 비어있지 않다면 데이터를 꺼내며 총 50개의 데이터를 수신합니다.
 *
 * @param arg QUEUE 포인터
 * @return NULL
 */
void* consumer_fn(void* arg) {
    QUEUE* pstQueue = (QUEUE*)arg;
    for (int i = 0; i < MAX_QUEUE_SIZE; ) {
        if(!queueIsEmpty(pstQueue)){
            void* data = nullptr;
            EXPECT_EQ(queuePop(pstQueue, &data), 4);
            EXPECT_EQ(*(int*)data, i);
            ++i;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(300));
    }
    return nullptr;
}

/**
 * @test MultiThreadedQueueTest
 * @brief 멀티 쓰레드 환경에서의 큐 동작 테스트
 *
 * 생산자-소비자 모델을 기반으로, 두 개의 쓰레드가 동시에 queuePush와 queuePop을 수행하며,
 * 모든 데이터가 순서대로 안정적으로 처리되는지를 검증합니다.
 */
TEST(QueueTest, ThreadSeparatedPushPop) {
    QUEUE q;
    queueInit(&q, MAX_QUEUE_SIZE);

    for (int i = 0; i < MAX_QUEUE_SIZE; ++i) test_values[i] = i;

    pthread_t producer_thread, consumer_thread;

    pthread_create(&producer_thread, NULL, producer_fn, (void*)&q);
    pthread_create(&consumer_thread, NULL, consumer_fn, (void*)&q);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    queueDestroy(&q);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}