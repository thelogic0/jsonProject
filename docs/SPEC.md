# 연락처 관리 JSON CRUD 콘솔 애플리케이션 명세서

## 1. 개요

데이터를 JSON 파일(`data.json`)로 관리하는 연락처(Contact) CRUD 콘솔 애플리케이션이다.
C++20 / Visual Studio(vcxproj) 기반이며, 외부 JSON 라이브러리 없이 자체 구현한 파서를 사용한다.

## 2. 요구사항 대응

| 요구사항 | 구현 |
|---|---|
| Create | 이름/전화번호/이메일을 입력받아 `data.json`에 새 레코드 저장 |
| Read (전체 목록) | 저장된 모든 연락처를 콘솔에 출력 |
| Read (검색) | ID 또는 이름으로 일치하는 레코드를 검색 |
| Update | ID로 대상 선택 후 이름/전화번호/이메일 중 하나를 수정 |
| Delete | ID로 대상 선택 후 확인(y/n) 절차를 거쳐 삭제 |

## 3. 아키텍처 (OOP)

```
Main.cpp
  ├─ (Debug) gtest 실행
  └─ (Release) ConsoleApp 실행

ConsoleApp            메뉴 출력, 사용자 입력 처리, ContactRepository 호출
  └─ ContactRepository  데이터 로드/저장 및 CRUD 로직
        └─ Contact       연락처 엔티티 (getter/setter, JSON 직렬화/역직렬화)
              └─ JsonParser  범용 JSON 파싱 유틸리티 (문자열/숫자 파싱, escape)
```

### 3.1 클래스 책임

- **`JsonParser`** (`Json.h/.cpp`)
  텍스트와 현재 위치(position)를 캡슐화한 저수준 JSON 파서. `skipWhitespace`, `parseString`, `parseNumber`, 정적 `escape`를 제공한다. 특정 엔티티를 알지 못하는 순수 파싱 유틸리티다.

- **`Contact`** (`Contact.h/.cpp`)
  연락처 엔티티. `id`, `name`, `phone`, `email` 필드와 getter/setter를 가진다. `toJson()`으로 자신을 JSON 오브젝트 문자열로 직렬화하고, 정적 `fromJson(JsonParser&)`로 역직렬화한다. `toDisplayString()`으로 콘솔 출력용 문자열을 만든다.

- **`ContactRepository`** (`ContactRepository.h/.cpp`)
  `data.json` 파일 하나를 담당하는 저장소. 생성 시 파일을 로드하고, 모든 변경(`add`/`remove`/`updateName`/`updatePhone`/`updateEmail`) 후 즉시 파일에 저장한다. `findById`, `search`, `generateNextId` 등 조회 기능을 제공한다.
  (초기 설계에서는 `JsonRepository<T>` 템플릿으로 일반화했으나, 실제 사용처가 `Contact` 하나뿐이라 YAGNI 원칙에 따라 단일 클래스로 재통합했다.)

- **`ConsoleApp`** (`ConsoleApp.h/.cpp`)
  메뉴 출력과 `std::cin` 입력 처리를 전담한다. `ContactRepository`를 멤버로 소유하며, 입력을 파싱해 저장소 API를 호출하고 결과를 출력한다.

- **`Main.cpp`**
  빌드 구성에 따라 진입점 동작이 분기된다.
  - `_DEBUG`가 정의된 경우(Debug 빌드): `gtest`를 초기화하고 전체 단위 테스트를 실행한다.
  - 그 외(Release 빌드): `ConsoleApp`을 생성해 실행한다.

## 4. 데이터 저장 형식

`data.json` 예시:

```json
[
  {
    "id": 1,
    "name": "홍길동",
    "phone": "010-1111-2222",
    "email": "hong@test.com"
  }
]
```

- 배열 안에 연락처 오브젝트가 나열된다.
- 문자열 값의 `"`와 `\`는 저장 시 이스케이프된다.
- 파일이 없으면 빈 목록으로 시작한다.

## 5. 빌드 및 실행

- **Debug 빌드**: 실행하면 gtest 기반 단위 테스트가 자동으로 실행되고 결과가 콘솔에 출력된다.
- **Release 빌드**: 실행하면 메뉴 기반 CRUD 콘솔 앱이 시작된다.

콘솔 메뉴:

```
1. Create - 데이터 추가
2. Read   - 전체 목록 보기
3. Read   - ID/이름으로 검색
4. Update - 데이터 수정
5. Delete - 데이터 삭제
0. 종료
```

## 6. 단위 테스트 (gtest)

Google Test/Mock(NuGet `gmock` 패키지)을 프로젝트에 직접 포함해 빌드하며, Debug 빌드에서만 컴파일·실행된다(Release 빌드에서는 `ExcludedFromBuild`로 제외).

- **`JsonParserTests.cpp`**: 문자열/숫자 파싱, escape, 공백 스킵, EOF 판정 (7건)
- **`ContactTests.cpp`**: getter/setter, `toJson`/`fromJson` 라운드트립, 특수문자 escape, `toDisplayString` (5건)
- **`ContactRepositoryTests.cpp`**: 파일 없을 때 초기 상태, add 저장, 재로드, ID 자동증가, `findById`, `search`, update 3종, remove (8건)

각 리포지토리 테스트는 임시 파일(`test_contacts.json`)을 `SetUp`/`TearDown`에서 생성·삭제하여 서로 독립적으로 동작한다.

## 7. 파일 구조

```
PocTest/
├── Json.h / Json.cpp                 JSON 저수준 파서
├── Contact.h / Contact.cpp           연락처 엔티티
├── ContactRepository.h / .cpp        저장/조회/CRUD 로직
├── ConsoleApp.h / .cpp               메뉴·입력 처리
├── Main.cpp                          진입점 (Debug: 테스트 / Release: 콘솔 앱)
├── JsonParserTests.cpp                단위 테스트
├── ContactTests.cpp                   단위 테스트
├── ContactRepositoryTests.cpp          단위 테스트
└── data.json                          런타임 데이터 (git에는 포함하지 않음)
```
