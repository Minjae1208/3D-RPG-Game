# 3D-RPG-Game

Unreal Engine4 + IOCP를 활용한 3D RPG 게임

개발 환경  
Client : Unreal Engine 4.25.4  
Server : IOCP ( Visual Studio 2017 )  
DB : MySQL 8.0  
Data : XML( TinyXml2 & Google Sheet )    

Test 영상 : <https://www.youtube.com/watch?v=HlGKacfPWtw>

#

서버 동작 ( 모든 데이터 처리는 서버에서 동작 )
1. 로그인 / 로그아웃 / 회원가입
2. 유저 정보 관리 ( MySQL )
3. 기본 아이템 데이터 ( XML )
4. 유저 동기화
5. 채집 및 채집물 관리
6. 아이템 관리 ( 조합, 사용, 착용/해제, 버리기 )
7. 교환 관리
8. 상점 관리 ( 직업에 따른 상점 아이템 변동 )

#


서버는 4개의 Manager를 사용
1. DB Manager
2. Level Manager ( Map )
3. Timer Manager
4. Trade Manager ( 교환 )

#

Zone 동기화 방식을 Level_Manager를 통하여 구현  
Zone의 종류  
마을맵, 채집맵  

 #

시간을 통한 동기화가 필요한 부분은 Multimedia Timer를 활용하여 구현
 - 동물 AI
 - 채집 시간
 - 채집물 리스폰
 - 피로도 회복


 #
 
 클라이언트 관련
 - 클라이언트에서의 인벤토리, 장비창, 교환창 등 UI관련된 부분은 위젯 블루프린트를 통해 직접 제작   
 - 캐릭터/맵(Level)/Interaction과 관련된 것들은 Unreal Engine Store에서 구하여 활용 
