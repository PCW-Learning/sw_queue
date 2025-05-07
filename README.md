# 큐(Queue) 라이브러리

이 저장소는 C언어로 작성된 간단한 쓰레드 안전(스레드 세이프) 큐 자료구조를 포함하고 있으며, 이를 검증하기 위한 Google Test 기반의 테스트 코드도 함께 포함되어 있습니다. 이 코드를 정적 라이브러리 형태로 빌드하여 다양한 프로젝트에서 재사용할 수 있도록 구성하였습니다.

---



## 📁 파일 구성

├── queue.c             # 큐 구현 소스
├── queue.h             # 큐 헤더 파일 (인터페이스)
├── queueGtest.cc       # Google Test 기반의 테스트 코드
├── Makefile            # 빌드 및 테스트 자동화

---



## 🔧 빌드 방법

### 1. 사전 준비 사항

- GCC 또는 호환 가능한 C 컴파일러
- [GoogleTest](https://github.com/google/googletest) 설치 및 빌드
- `make` 유틸리티

Makefile 내 또는 환경 변수로 GoogleTest 경로를 설정해야 합니다:

```make
GTEST_DIR = /your/path/to/googletest
```



### 2. 라이브러리 및 테스트 빌드

```
bash


복사편집
make all
```

실행 시 다음 작업이 수행됩니다:

- `queue.c` → 오브젝트 파일로 컴파일
- 정적 라이브러리 `libqueue.a` 생성
- 테스트 실행파일 `queueGtest` 생성

------



## 📦 라이브러리 사용 방법

### 1. 헤더 포함

```c
#include "queue.h"
```



### 2. 라이브러리 링크

`libqueue.a`가 생성된 디렉토리에서 다음과 같이 빌드:

```bash
gcc -o my_app my_app.c -L. -lqueue -lpthread
```

------



## ✅ 테스트 실행

```bash
./queueGtest
```

Google Test의 결과를 통해 큐 기능이 정상 동작하는지 확인할 수 있습니다.

------



## 📄 사용 예시

```c
QUEUE stQueue;
queueInit(&stQueue, 10);

int data = 100;
queuePush(&stQueue, &data);

void* pOut = NULL;
if (queuePop(&stQueue, &pOut)) {
    printf("Pop된 값: %d\n", *(int*)pOut);
}
```

------

