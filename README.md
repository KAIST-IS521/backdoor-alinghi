#Backdoor-Alinghi

###디렉토리 구조
* ./interpreter : 구현된 정상 인터프리터

* ./login/login.mini : 미니랭귀지로 만든 로그인 프로그램

* ./test/test.mini : strlen와 if-else를 포함한 테스트 프로그램

* ./test/test.md : 테스트 프로그램 설명 파일

* ./backdoor : 백도어가 심어져 있는 인터프리터

* ./compiler/compiler.ml : 처음 클로한것과 동일한 컴파일러

* ./README.md 현재 읽고 있는 파일

###백도어 원리
입력 ID를 저장하게 되는 지점을 계속 체크해서(heap[100]부터 시작) superuser가 heap에 저장되는 것을 체크하면 PC를 success를 출력하는 지점으로 바꾸었습니다.

###배운 점
1. 다양한 방법으로 시스템을 조작할 수 있다는 것을 알았습니다. 같은 로그인 프로그램이라도 제가 한 방법 외에 수많은 방법이 존재할 수 있습니다.
2. 또한 CPU level, OS level, Compilier level등 다양한 레이어에 보안 취약점을 무시하면 안된다는 사실도 알게 되었습니다.
3. 마지막으로 저는 제가 만든 login 프로그램의 구조를 완벽하게 알고 있어서 이번 백도어를 만들 수 있었습니다.(특정 heap영역에 대한 지속적인 scan) 그러므로 프로그램의 구조를 아는 것이 compiler, interpreter를 활용한 백도어 제작에 중요한 요소임을 알게 되었습니다.