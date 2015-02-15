#define DEF_LANGUAGE 4
//#define DEF_TAIWAN	1
//#define DEF_CHINESE	2
//#define DEF_KOREAN	3
//#define DEF_ENGLISH	4 
//#define DEF_JAPANESE	5
//#define DEF_GIZON

#if DEF_LANGUAGE == 1	// Taiwan
	#define DEF_UPPERVERSION		2
	#define DEF_LOWERVERSION		19
#elif DEF_LANGUAGE == 2	// China
	#define DEF_UPPERVERSION		2
	#define DEF_LOWERVERSION		19
#elif DEF_LANGUAGE == 3	// Korea
	#define DEF_UPPERVERSION		2
	#define DEF_LOWERVERSION		20
#elif DEF_LANGUAGE == 4	// English
	#define DEF_UPPERVERSION		2
	#define DEF_LOWERVERSION		20
	// Fuck USA : 2.20     International : 2.20
#elif DEF_LANGUAGE == 5	// Japan
	#define DEF_UPPERVERSION		2
	#define DEF_LOWERVERSION		20
	// Terra : 2.185     Inter Japan : 2.20
#endif

//#define DEF_COLOR	//	색상 테스트 부분.
//#define DEF_XMAS	//	크리스마스 부분^^;
//#define DEF_TESTSERVER
//#define DEF_JAPAN_FOR_TERRA    // Terra 에서 서비스 하는 일본버전을 위한...
//#define DEF_FUCK_USA		// 미국에서 서비스 하는 버전.
//#define DEF_SHOWCURSORPOS
//
/*
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////            프로그램 수정 내역                       ///////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
v2.195 김민수 크리스마스관련 눈 & 나무효과	#define DEF_XMAS..20021212
v2.195 안상하 미니맵 더블클릭하면 그곳으로 계속 가기
v2.195 안상하 창고 인터페이스 수정
v2.195 안상하 몬스터 이벤트 미니맵에 보여주기
v2.19  김민수 [중국] Html공지사항 적용..#define DEF_HTMLCOMMOM..20021207
v2.19  김민수 중국_길드생성시 조건추가,유효날짜(계정) Define,콘트롤키 적 자동 공격..20021128
v2.19  안상하 단축키및 F4마법키 레지스트리 CURRENT_USER에 저장했다가 불러서 씀.
v2.19  김민수 한국 2.19 -> 중국,대만 2.18 Define작업..
v2.184 안상하 NPC 이름 찍어주는 루틴 단일화
v2.184 안상하 미니맵에서 주요지역 외에는 가리킨 곳의 좌표를 찍어준다.
v2.184 안상하 CTile 클래스 최적화
v2.184 안상하 Frame 볼수 있는 기능 추가.(채팅창에서 '/showframe'이라고 치면 된다)
v2.184 안상하 길드명 출력 루틴 전부 변경(기존의 버그를 수정하기 위해)
v2.184 안상하 F5창 수정내용 중 더블클릭은 빠졌던거 마져 추가
v2.183 안상하 미니맵에 대한 가이드 루틴 집어넣음.(덩달아 기본 인터페이스도 약간)
v2.183 안상하 F5창에서 아이템 집는거 제대로 집음.
v2.183 안상하 인비캐릭터 제대로 투명하게 그림.
v2.183 안상하 채팅 메세지 변경 원래대로 복원
v2.182 안상하 특수능력설정 메세지에서 버그 수정
v2.182 안상하 강제공격모드 : 컨트롤키를 안눌러도 적은 공격되는 모드
v2.182 안상하 같은편인 인비캐릭터 보여주되 공격은 안되게 수정.
v2.182 안상하 채팅 메시지 올라오는거 약간 바꿔봄.(심심해서)
v2.182 안상하 화면에 타일및 오브젝트 찍어주는 루틴 바뀜
v2.18  정진광 추가. 국내, 인터서버, 테라..아이템 버릴 때, 확인 메세지 출력.
v2.18  10 21일 정진광 추가. 국내, 인터서버, 테라..귓속말과 외치기에 대한 듣기/듣지않기 기능 추가.
2002-10-17 #1 문성훈 수정 인터섭 암호 바꾸기
v2.18 10 1일 정진광수정. 국내. 교전지역에 대한 경고메세지 출력.
v2.18 9 30일 정진광수정 국내. 몬스터와 캐릭터..피격동작에서 핏방울 튀는 효과 제거.
v2.18 9 30일 정진광수정 국내. 적 마을에서 강콜시, 종종 적마을의 건물로 들어가는 버그 수정
v2.18 9 25일 정진광수정 테라 버전은, 비번 변경이 안되게 수정.
v2.18 9 25일 정진광수정 테라 관련 코드 수정...
v2.18 2002-9-24 문성훈 수정 중국 버전.계정에 특수 문자가 있으면 접속하지 않는다. 
v2.18 2002-9-24  문성훈 수정 중국 버전.적이나 범죄자는 강제 공격 하지 않아도 공격되게 
v2.18 9 23일 정진광수정 식인식물은 그림자를 그리지 않게...
v2.18 9 18일 정진광수정 죽은 사람 위에 올라타면, 죽은 사람의 길드가 바뀌는 버그 수정.
v2.18 9 16일 정진광수정 전면전시에 적의 길드이름이 보이는 버그 수정.
v2.18 9 13일 정진광수정 새 몬스터의 사운드... 
v2.18 9 10일 정진광수정 연금술, 무기제작. 재료를 드래그 하지 않고 더블클릭으로 처리..
v2.18 9 10일 정진광수정 새로운 몬스터 세 종류 추가
v2.18 9 10일 정진광수정 전면전시가 아니라면 적 국가 건물을 이용할 수 없도록 수정.
                        CGame::RequestTeleportAndWaitData() 함수.
v2.18 9 10일 정진광추가 RequestTeleportAndWaitData() 함수에 쓰이는 매크로 문장 추가. 다국어화 안되어있음.
v2.18 8  8일 정진광수정 디버그모드로 작업할 때, 게임 종료시 메인메뉴 안나오게 수정.
v2.18 8  8일 정진광수정 디버그모드로 작업할 때, 로그아웃 카운트를 하지 않도록 수정..그래서 난 디게 좋던디 ^^
v2.18 8  8일 정진광추가 HANDLE 형 변수 m_hPakFile을 CGame 클래스에 선언. 스프라이트 파일을 읽을 때 사용한다.
v2.18 8  8일 정진광추가 CSprite Class 의 constructor 를 OverLoad.
v2.18 8  8일 정진광수정 CGame::bInit() 초기 스프라이트 로딩 부분, 
                        OverLoad된 CSprite Class 의 constructor 를 사용하도록 수정,
v2.18 8  8일 정진광수정 CGame::UpdateScreen_OnLoading() 함수 전체를
                        OverLoad된 CSprite Class 의 constructor 를 사용하도록 수정,
v2.18 8  8일 정진광수정 아이템을 주을때, 수량개념이 없는 아이템은 가방 안에 동일 아이템이
                        이미 있을 때 그 아이템과 같은 위치로 들어간다.
						CGame::NotifyMsg_ItemObtained() 함수.
						창고에서 아이템 찾을 때, 수량개념이 없는 아이템은 가방 안에 동일 아이템이
                        이미 있을 때 그 아이템과 같은 위치로 들어간다.
						CGame::RetrieveItemHandler() 함수.
                        아이템을 구입할 때, 수량개념이 없는 아이템은 가방 안에 동일 아이템이
                        이미 있을 때 그 아이템과 같은 위치로 들어간다.
						CGame::NotifyMsg_ItemPurchased() 함수
v2.18 8  8일 정진광수정 쉬프트키를 이용한 가방정리, 수량 개념이 없는 모든 아이템에 적용시켜보았다.
                        CGame::bItemDrop_Inventory() 함수.
v2.18 8  6일 안상하추가 맵나오는 곳에 마우스 갖다대면 다음 레벨업까지 남은 경험치비례로 한 공정도 나온다.
v2.18 8  6일 안상하수정 인터페이스 작업 완료
v2.17 8  1일 안상하추가 시약류 한번에 아이템 정렬기능
v2.17 8  1일 안상하추가 대장간에서 가방 더블클릭하면 수리창 뜬다.
v2.17 7 31일 안상하수정 강제 접속종료 시켰는데 창 내려노면 로그아웃 안되는 버그 수정..
v2.17 7 30일 안상하추가 창고에서 드래그로 아이템 옮기면 바로 맡겨짐..
v2.17 7 30일 안상하추가 로그아웃중에 Esc를 누르면 접속 종료 취소..
v2.17 7 30일 안상하수정 ForceLogOut중에 피가 달면 카운트 취소되던거 수정
v2.17 7 24일 성후니수정 탭키를 이용한 속도 방지 
v2.17 7 24일 안상하수정 계속되는 Refactoring.....ㅠ.ㅠ.
v2.17 7 24일 안상하추가 UpdateScreen_Loading 스레드화.
v2.17 7 23일 안상하추가 창고및 SellList에서 서브특성치 보이도록..
v2.17 7 21일 아이템 업그레이드 실패 원인을 사용자에게 보여준다. (번역 필요)

v2.16 7 18일 고광현수정 죽은척하기 하고 있는 상태에서 강콜되면 와서도 기술 사용중 버그 수정 
			 일루젼 걸렸는데도 길드 이름 보임 수정
			 길드 이름 딴걸로 나오는 버그 수정
v2.17 7 15일 강콜 시간을 사용자에게 보여준다. (이벤트 용 )

v2.15 5 27일 안상하수정 중문 Select Server 인터페이스 WS4개까지로 변경..
v2.15 5 24일 안상하수정 넥슨제공 욕설컨버트 시스템 추가..
v2.15 5 23일 안상하수정 클라이언트 종료시 홈페이지로 자동으로 감.
v2.15 5 22일 안상하수정 영문과 일문시 메인메뉴에서 New Account누르면 홈페이지로 갊.
v2.15 5  9일 안상하수정 User Agreement를 Login시 하는거에서 New Account로 옮김.
v2.15 4 20일 성후니수정 커멘더 뿐만 아니라 전사나 건축가 캐릭터가 마나스톤의 위치를 볼수 있게 수정함
v2.15 4 20일 성후니수정 약물 단축키 작은 물약 부터 먹게 수정함
v2.15 4 20일 성후니수정 전면전시 남은 건물의 내구도 볼수 있게 수정함
v2.15 4 19일 성후니수정 돌 이나 나무 같은 갈수 없는 곳을 클릭하면 근처 가면 자동으로 멈추게 수정 
v2.15 4 19일 성후니수정 SP가 0 보다 적으면 자동으로 걷기로 변경하게 수정함 
v2.15 4  7일 안상하수정 DCX_ddraw.DrawText에 DT_NOPREFIX를 추가..(&연산 Disable)
v2.15 3 30일 안상하수정 마법창에 성공률 실제와 똑같이 나오게 변경..
v2.15 3 30일 안상하추가 !!!마우스 휠 기능!!! 추가.(m_DInput.m_sZ관련 모두)
v2.15 3 29일 안상하수정 상점 메뉴 제한(DEF_MAXMENUITEMS) 120->140..
v2.15 3 30일 안상하수정 화면 밖을 찍으면 마법 적용되는 거 안되게 수정..
v2.15 3 29일 안상하수정 화면 스크롤 사이즈에 맞게 수정.
v2.15 3 29일 안상하수정 Agreement인터페이스 엔터키로 통과하게 변경.
v2.15 3 28일 성후니수정 운영자가 /fi 하면 캐릭터의 맵위치랑 좌표 나오게 수정 
v2.15 3 28일 성후니수정 지존아이템 관련 수정함 !!
v2.15 3 25일 안상하수정 단축키로 귓말 및 사운드, 지도 사용하도록 수정함
v2.15 3 23일 성후니수정 단축키로 큰붉은색 시약과 큰푸른색 시약이 먹어지도록 수정함
v2.15 3 22일 안상하수정 UpdateScreen_OnLoading에서 새로운 Inventory이미지 TESTSERVER215에 한해 TestSprite에서 읽어옴
v2.15 3 22일 안상하수정 UpdateScreen_OnLogin, OnCreateNewAccount 등 새로운 인터페이스 영문버젼에 적용
v2.15 3 21일 안상하수정 v2.13영문서버 돌리면서 나타난 메시지버그 수정작업 v2.15에 적용
v2.15 3 21일 안상하수정 TIIME관련 함수 모두 Client에서 제거
v2.15 3 21일 안상하수정 ExID.txt 파일 읽어들이지 않고 게임중에 저장하고 사용하는 방식으로 변경
v2.15 3 21일 안상하수정 login.cfg파일 읽는 모듈 영문과 일문클라이언트는 강제IP로 변경(자꾸 port바꿔서 장난치는 애들땜시)
v2.15 3 8일  void CGame::DlgBoxClick_Inventory(short msX, short msY) 인벤토리 버튼 추가 korean.h 에 관련 메시지 추가 
				 BDLBBOX_DOUBLE_CLICK_INVENTORY14
v2.15 3 5일  DrawDialogBox_ItemUpgrade() 부분 추가 Korean.h 추가 DRAW_DIALOGBOX_ITEMUPGRADE1,2,3... 
                  DlgBoxClick_ItemUpgrade(int msX, int msY)
				 DEF_NOTIFY_ITEMATTRIBUTECHANGE
v2.15 3 4일  DrawDialogBox_ItemUpgrade(int msX, int msY)
v2.14 2 27일 고광현 수정 교환 버그(캐릭터 장착창을 사용한 것) 테스트 버젼 스프라이트 수정 
v2.14 2 26일 안상하 수정 Taiwan Ime 함수 모듈화 ! 
v2.14 2 26일 안상하 추가 Sprite File과 MapData와 Content 폴더 변경 Test~~붙여서

///////////////////////////////////////////////////////////////////////////////////////////////////////*/
#ifndef DEF_LANGUAGE
#define DEF_LANGUAGE 3		// 언어가 실수로 정의되지 않았다면 Default로 한국어
#endif
#if DEF_LANGUAGE > 5
#define DEF_LANGUAGE 3		// 언어가 실수로 잘못 정의되었다면 Default로 한국어
#endif

#if DEF_LANGUAGE == 1		// Taiwan
#define DEF_USING_WIN_IME
#define DEF_ENGLISHITEM
#define DEF_USING_GATEWAY
#define DEF_SHORTCUT
#define DEF_GIZON

#elif DEF_LANGUAGE == 2		// China
#define DEF_USING_WIN_IME
#define DEF_ENGLISHITEM
#define DEF_SELECTSERVER
#define DEF_MAKE_ACCOUNT
#define DEF_SHORTCUT
#define DEF_GIZON
#define DEF_FEEDBACKCARD
#define DEF_HTMLCOMMOM

#elif DEF_LANGUAGE == 3		// Korea
#define DEF_SELECTSERVER
#define DEF_GIZON

#elif DEF_LANGUAGE == 4		// English
#define DEF_ENGLISHITEM   // 아이템이름 영문화를 위한 정의 함수
#define DEF_SELECTSERVER

#elif DEF_LANGUAGE == 5		// Japan
 #define DEF_USING_WIN_IME
 #define DEF_SELECTSERVER
 #define DEF_ENGLISHITEM
 #ifdef DEF_JAPAN_FOR_TERRA
  #define DEF_ACCOUNTLONG		// 10글자 이상의 계정명을 요구할때
  #define DEF_ACCOUNTLEN	16	// 일본어판은 16글자의 계정명 사용
 #endif
#endif

#ifdef DEF_TESTSERVER
#undef DEF_SELECTSERVER
#endif

#ifdef DEF_FUCK_USA
#define DEF_ENGLISHITEM	//	미국버젼 영문 아이템 이름으로..^^ 030122..mando
#undef DEF_SELECTSERVER
#endif

#ifdef _DEBUG
#undef DEF_USING_GATEWAY
#endif