# [Project 3 - Shading](https://graphics.cs.utah.edu/courses/cs6610/spring2021/?prj=3)
이 프로젝트에서는 간단한 조명과 셰이딩을 소개합니다.

## 요구 사항
이 프로젝트의 요구 사항은 세 가지 단계로 나뉘어 있습니다. 각 단계의 끝에서 구현을 확인할 수 있습니다.
* **단계 1:** 삼각형 그리기
  - .obj 파일의 삼각형 정점에 대한 버퍼를 생성하세요. <br>
  
  - glDrawArrays 또는 glDrawElements를 사용하여 삼각형을 그리세요.
 
  이 시점에서 물체는 평평하게 보여야하지만 삼각형 사이에 이상한 빈 공간이 보이지 않아야합니다.

* **단계 2:** Normal 버퍼
  - 삼각형을 셰이딩하기 위해서는 surface normal들이 필요합니다. Normal 버퍼를 생성하세요.
    
  - [정점 셰이더는 뷰 행렬의 역전치 행렬을 사용하여 normal을 변환해야합니다(프로젝션은 제외).](https://stackoverflow.com/questions/13654401/why-transform-normals-with-the-transpose-of-the-inverse-of-the-modelview-matrix)

  Surface normal 정보를 사용하여 삼각형을 색칠해 보는 것이 좋습니다. 이를 통해 normal이 제대로 사용되고 있고 올바르게 변환됐는지 확인할 수 있습니다. <br>
  
  Surface가 위(+Y 방향)를 향한 부분은 녹색, 오른쪽(+X 방향)을 향한 부분은 빨간색, 나머지 부분은 파란색으로 나타납니다. 음수의 색상 값은 [0,1]로 클램프됩니다.
 
* **단계 3:** Blinn 셰이딩
  - 프래그먼트 셰이더에서 ambient, diffuse 및 specular 요소를 포함한 Blinn 셰이딩을 구현하세요.
    
  - Material 속성은 상수 또는 uniform 매개변수로 하드 코딩 될 수 있습니다.
    
  - 빛의 위치는 상수 또는 uniform 매개변수로 하드 코딩 될 수 있습니다.
    
  각 요소(ambient, diffuse 및 specular)를 개별적으로 구현하고 결과를 확인한 다음에 모든 요소를 결합하는 것이 좋습니다.

## CS 6610 학생들에 대한 추가 요구 사항
* 두 개의 각도를 사용하여 빛을 물체 중심 주위로 회전시키세요.
  
* CTRL 키를 누른 상태에서 왼쪽 마우스 버튼을 누르고 드래그하여 빛의 회전을 조절할 수 있어야 합니다.

## 선택 사항
* 빛을 별도의 물체로 나타내세요.

## 결과
