# NumBaseball

Unreal Engine 5.5 기반 숫자 야구 게임 프로젝트입니다.

## 개요

멀티플레이어를 지원하는 턴 기반 숫자 야구 게임입니다.
플레이어들이 번갈아가며 숫자를 추측하고, 스트라이크와 볼을 통해 정답을 맞추는 게임입니다.

## 주요 기능

- **턴 기반 게임플레이**: 플레이어들이 번갈아가며 숫자를 입력
- **멀티플레이어 채팅**: 실시간 채팅 시스템
- **게임 로직**: 랜덤 숫자 생성 및 스트라이크/볼 판정
- **UI 시스템**: 입력 위젯 및 알림 시스템
- **턴 타이머**: 각 턴마다 시간 제한 (30초)

## 프로젝트 구조

```
Source/NumBaseball/
├── Controller/
│   └── JinPlayerController      # 플레이어 컨트롤러
├── System/
│   ├── JinGameModeBase          # 게임 모드 (게임 로직)
│   ├── JinGameStateBase         # 게임 상태
│   └── JinPlayerState           # 플레이어 상태
└── UI/
    └── JinInputWidget           # 입력 UI 위젯
```

## 빌드 및 실행

1. `NumBaseball.uproject` 파일을 더블클릭하여 Unreal Editor에서 열기
2. 또는 Visual Studio에서 `NumBaseball.sln`을 열어 빌드

## 게임 플레이

1. 서버에 2명의 플레이어가 연결되면 게임 시작
2. 각 플레이어는 자신의 턴에 3자리 숫자를 입력
3. 서버가 스트라이크와 볼을 판정하여 결과 반환
4. 3스트라이크를 달성하면 승리

