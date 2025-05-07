// test_queue.cpp
#include <gtest/gtest.h>
#include "queue.h"
#include <thread>
#include <vector>
#include <chrono>

/**
 * @brief 단일 쓰레드 환경에서 queuePush/queuePop 기능을 검증합니다.
 */
TEST(QueueTest, BasicPushPopTest) {
    QUEUE q;
    queueInit(&q, 5);

    int a = 10, b = 20;
    EXPECT_TRUE(queuePush(&q, &a));
    EXPECT_TRUE(queuePush(&q, &b));

    void* data = nullptr;
    EXPECT_TRUE(queuePop(&q, &data));
    EXPECT_EQ(*(int*)data, 10);

    EXPECT_TRUE(queuePop(&q, &data));
    EXPECT_EQ(*(int*)data, 20);

    EXPECT_TRUE(queueIsEmpty(&q));
    queueDeinit(&q);
}

/**
 * @brief 큐가 가득 찬 경우 push에 실패해야 함을 테스트합니다.
 */
TEST(QueueTest, QueueFullTest) {
    QUEUE q;
    queueInit(&q, 2);

    int a = 1, b = 2, c = 3;
    EXPECT_TRUE(queuePush(&q, &a));
    EXPECT_TRUE(queuePush(&q, &b));
    EXPECT_FALSE(queuePush(&q, &c));  // 최대 용량 초과

    queueDeinit(&q);
}

#define MAX_QUEUE_SIZE 50

static int test_values[MAX_QUEUE_SIZE];

// 생산자 쓰레드 함수
void* producer_fn(void* arg) {
    QUEUE* pstQueue = (QUEUE*)arg;
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i) {
        EXPECT_TRUE(queuePush(pstQueue, &test_values[i]));
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return nullptr;
}

// 소비자 쓰레드 함수
void* consumer_fn(void* arg) {
    QUEUE* pstQueue = (QUEUE*)arg;
    for (int i = 0; i < MAX_QUEUE_SIZE; ++i) {
        void* data = nullptr;
        EXPECT_TRUE(queuePop(pstQueue, &data));
        EXPECT_EQ(*(int*)data, i);
    }
    return nullptr;
}

/**
 * @brief 다중 쓰레드 환경에서 push/pop을 분리된 함수로 동작 테스트
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

    queueDeinit(&q);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}