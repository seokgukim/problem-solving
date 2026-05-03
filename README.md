# SeokguKim's Problem Solving Repository

**김석구의 문제풀이 레포지토리**

주로 Competitive Programming, Computer Science 공부를 위한 내용들입니다.

![seokgukim](https://lh3.googleusercontent.com/a/ACg8ocJ5-kaOh2c-Jw-chrmaKsPu94TP3mWjfwp6qsVm5JvUcS1-er0u=s432-c-no)

---
## About

- `/samples`: 실제 구현 코드들이 들어있는 디렉토리
- `/docs`: 설명이나 기타 정리를 위한 디렉토리
- `/run.sh`: Bash 환경에서 C/C++/Ruby/Python3 소스 파일을 입력/출력 리다이렉션과 함께 실행하는 헬퍼 스크립트
- `/run.ps1`: PowerShell 환경에서 같은 방식으로 실행하는 헬퍼 스크립트

---
## Helper Script

```bash
./run.sh path/to/main.cpp input.txt output.txt
./run.sh path/to/main.py
./run.ps1 path/to/main.cpp input.txt output.txt
./run.ps1 path/to/main.py
```

- 첫 번째 인자는 실행할 소스 파일 경로입니다.
- 두 번째 인자를 주지 않으면 `stdin` 을 사용합니다.
- 세 번째 인자를 주지 않으면 `stdout` 으로 출력합니다.
- 입력 파일 없이 출력 파일만 지정하려면 `./run.sh main.py "" output.txt` 형태로 두 번째 인자에 빈 문자열을 넣으면 됩니다.
- `run.ps1` 도 같은 인자 규칙을 사용합니다.
- `.c` 는 `gcc`, `.cc/.cpp/.cxx` 는 `g++`, `.rb` 는 `ruby`, `.py` 는 `python3` 로 실행합니다.

---
## Acknowledgement



---
This project is licensed under the terms of the MIT license.
