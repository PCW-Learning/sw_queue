# 큐(Queue) 라이브러리

멀티 스레드 환경에서 안전하게 사용할 수 있도록 설계된 **쓰레드 안전 큐(Queue)** 라이브러리입니다.  연결 리스트 기반의 FIFO 구조로 구현되어 있으며, `pthread_mutex`를 통해 동기화를 보장합니다.

---



## 📁 파일 구성

```
include/
├── queue.h			# 큐 자료구조 및 API 선언부
src/
├── queue.c			# 큐 구현 소스
gtest/
├── queue-gtest.cc	# Google Test 기반의 테스트 코드
Makefile 			# 라이브러리 및 테스트 빌드용 Makefile
```





---

## 🔧 빌드 방법

### 1. 라이브러리 빌드

```bash
make desktop       # libqueue_desktop.so 생성
```

생성 결과 libuds_desktop.so 공유라이브러리 



### 2. 구글테스트 빌드 및 실행

```bash
make gtest
./queue-gtest
```

생성 결과 queue-gtest 실행 파일



---

## 📌 Makefile 주요 타겟

| 명령어         | 설명                                    |
| -------------- | --------------------------------------- |
| `make` desktop | 라이브러리 빌드 (`libqueue_desktop.so`) |
| `make gtest`   | GoogleTest 기반 테스트 빌드             |
| `make clean`   | 빌드된 파일 정리                        |
| `make install` | `/usr/lib` 및 `/usr/include`에 설치     |



------

## 📦 설치 파일 경로

설치 시 다음 경로에 배치됩니다:

- `/usr/lib/libqueue_desktop.so`
- `/usr/include/queue.h



------

## 🧪 의존성

- POSIX Thread (`pthread`)
- GoogleTest (테스트용)



------

## 📄 라이선스

누구나 필요하시다면 사용하세요.


------

## ✍️ 작성자

- **박철우 (Cheolwoo Park)**
- Embedded Systems / Signal Processing
- GitHub: [PCW-Learning](https://github.com/PCW-Learning)
