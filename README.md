# Philosophers

## 1️⃣ 프로젝트 소개

Philosophers 프로젝트는 고전적인 식사하는 철학자 문제를 기반으로 한 동시성 프로그래밍 과제입니다. 철학자들이 원형 테이블에서 동시에 행동(생각, 식사, 수면)을 반복하면서 공유 자원(포크)을 사용할 때 발생하는 데드락, 기아, 동기화 문제를 해결하는 것이 핵심입니다.
Mandatory는 pthread와 mutex를 이용한 멀티스레드 구현, Bonus는 fork와 semaphore를 활용한 멀티프로세스 구현을 요구합니다.

---

## 2️⃣ 과제 목표
	•	스레드와 프로세스를 활용한 동시성 프로그래밍 학습
	•	데드락/기아 상태를 방지하는 알고리즘 설계 및 검증
	•	마이크로초 단위의 타이밍 제어를 통한 실시간성 보장
	•	출력의 원자성을 확보하여 로그가 꼬이지 않도록 관리
	•	Bonus: 프로세스 간 동기화 및 IPC(세마포어) 활용

---

## 3️⃣ 구현 사항
* Mandatory (pthread + mutex)
  - pthread_create로 철학자 스레드 생성
  - 각 포크를 pthread_mutex_t로 관리
  - 홀/짝 철학자의 포크 집기 순서를 다르게 하여 데드락 방지
  - smart_usleep 구현으로 마이크로초 단위 정확도 확보
  - monitor 스레드로 철학자의 생존 여부와 종료 조건 체크

* Bonus (process + semaphore)
  - fork()로 철학자를 개별 프로세스로 생성
  - sem_open으로 fork, print, stop 세마포어 관리
  - 각 자식 프로세스에서 death monitor를 두어 부모가 전체 종료 관리
  - waitpid, kill, sem_close/unlink로 자원 해제 및 정리

---

## 4️⃣ 다른 학습자와의 차별점
* 차별화된 출력 구조
  - 다른 구현: 각 철학자가 printf 직전에 mutex를 걸어 출력
  - 내 구현: 출력 전담 스레드를 두어 모든 로그를 큐에 기록 후
    - 전담 스레드가 로그를 수집 → 시간순 정렬 → 출력
    - 출력이 병목되지 않고 대규모 스레드에서도 안정성 확보
  - 결과:
    - 일반 구현 → 200 스레드 수준에서 1분 제한
    - 내 구현 → 1,100 스레드 이상 1분 이상 정상 동작

---

## 5️⃣ 배운 점
* 데드락 방지 기법
  - 교차 집기, N-1 제한, 모니터 스레드 방식 실험
  - 자원 접근 순서를 통제하는 것의 중요성 체득
* 기아(Starvation) 방지
  - 무한 대기 상태를 피하기 위해 철학자의 행동 패턴을 균형 있게 분배
  - 특정 철학자가 반복적으로 기회를 잃지 않도록 설계
* 정확한 타이밍 제어
  - usleep 단독 사용의 부정확성을 경험
  - busy-wait와 clock_gettime을 혼합하여 오차를 최소화
* 출력 원자성, 출력 효율 확보
  - 출력이 꼬이는 문제, mutext를 걸어두고 print를 하는 도중 context switching이 되어서 다른 thread에서 print를 못 하는 문제를 해결하기 위해 출력 전담 스레드 구조를 고안
  - 이 구조 덕분에 대규모 동시성 환경(1100 스레드)에서도 안정적 로그 유지
  - 병렬성 유지 + I/O 병목 최소화라는 설계적 장점을 확인
* 스레드 vs 프로세스의 차이
  - 스레드는 메모리 공유가 자연스럽지만 동기화 부담이 크다는 점
  - 프로세스는 독립적이나 IPC 오버헤드가 존재한다는 점을 직접 비교
* 대규모 부하 테스트의 중요성
  - 작은 규모에선 잘 보이지 않는 병목, 리소스 경쟁, 출력 꼬임 문제를
  - 스케일 업 테스트(200 → 1100)를 통해 드러내고 해결
* 시스템콜/동기화 원리 심화 이해
  - pthread_mutex_lock/unlock, sem_wait/post 호출 패턴 최적화
  - 시스템콜의 오버헤드와 캐시 미스의 영향을 체험

---

## 6️⃣ 사용 방법
bash
```
# 빌드
make        # mandatory
make bonus  # bonus

# 실행 형식
./philo number_of_philos time_to_die time_to_eat time_to_sleep [must_eat]
./philo_bonus number_of_philos time_to_die time_to_eat time_to_sleep [must_eat]

# 예시
./philo 5 800 200 200
./philo 5 800 200 200 7
./philo_bonus 5 800 200 200
```
* 단위: 밀리초(ms)
* must_eat 옵션 지정 시, 모든 철학자가 지정 횟수 이상 먹으면 종료
* 철학자 중 하나라도 time_to_die 초과 시 즉시 종료

 ---
 
 ## 7️⃣ 기술 스택
	•	언어: C
 	•	운영체제/환경: Unix/Linux
 	•	빌드 도구: GNU Make
 	•	버전관리 도구: Git
 	•	Coding Conventions: Norminette
	•	스레드 동기화: pthread_create, pthread_mutex_*
	•	프로세스 동기화: fork, sem_open/close/unlink, waitpid, kill
	•	타이밍 제어: usleep, gettimeofday, clock_gettime
	•	기타: write, malloc/free
