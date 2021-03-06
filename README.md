HCI_2014_projectD
=================
GRAVITY (by Choi Gyeongwook)

Kinect를 이용해서 사용자가 media wall에 있는 이미지를 입체적으로 느낄 수 있게 이미지를 변형
- 참고 영상 : http://www.youtube.com/watch?v=Jd3-eiid-Uw

NEXT 생활이 이제 1년이 조금 넘었지만 이제까지 홀에 있는 미디어월을 적극적으로 활용하는 모습을 볼 수 없었다.
그리고 NEXT 생활은 조금 답답하다. 우리에겐 운동장도 노천 극장이나 잔디밭도 없다. 
조금이라도 실내에서 학교라를 건물을 벗어난 시각적 경험을 할 수 있다면 더 좋지 않을까.


   
구성
- 미디어월과 연결된 컴퓨터에는 Kinect가 연결되어 있다.
- 이 Kinect는 미디어월 앞에 사람이 나타나면 그 사람의 머리 위치를 추적해서 컴퓨터로 전송한다.
- 컴퓨터는 머리 위치(미디어월에서 떨어진 거리, 높이, 좌우 위치)를 바탕으로 화면에 보여지는 이미지를 변형한다.
- 이미지의 변형은 미디어월을 바라보는 사람이 마치 넓은 창문 밖을 보는 듯한 경험을 주도록 변형된다.
- 결과물은 참고 영상의 뒷부분에서 스타디움을 비추는 이미지가 바뀌는 모습과 유사함



상세 :
- 사용할 이미지는 NASA에서 제공하는 고해상도 우주 사진들 가운데 적당한 걸로 (효과가 커질 수 있게 도시 풍경으로 대체)
- 동시에 여러명을 인식할 경우 제일 먼저 인식한 사람을 기준으로 이미지 변형
- 대기 중에는 이미지 없이 주의를 끌 수 있는 텍스트만 화면에 표시 (미구현)
   


기술
- Kinect의 위치 가지 기능을 적극적으로 활용
- openGL 또는 openCV를 활용한 이미지 프로세싱
- 사용자는 특별한 조작없이 단지 자연스럽게 움직임으로써 시스템에 인풋을 제공한다. - natural interface



도전 과제 :   
- media wall와 추가적인 디스플레이 장치 연동 (media wall 주변에 모니터나 TV를 놓고 같이 활용)
- 대기 모드에서 사용자를 인식하면 주변 조명을 끄는 기능
   


준비물 :   
- Kinect for Windows
- NEXT media wall
- PC
- openFrameworks with openNI lib

(+ arduino + display devices)




개발 계획은 링크 참조
https://github.com/wooq17/HCI_2014_projectD/issues/milestones



동영상
https://vimeo.com/99287181
