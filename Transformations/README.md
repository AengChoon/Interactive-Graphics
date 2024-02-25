# [Project 2 - Transformations](https://graphics.cs.utah.edu/courses/cs6610/spring2021/?prj=2)
이 프로젝트에서는 정점 버퍼를 생성하고 카메라 변환을 계산한 후 매우 간단한 GLSL 셰이더를 사용하여 모델의 정점을 표시해봅시다.

## 요구 사항
이 프로젝트의 요구 사항은 세 가지 단계로 나뉘어 있습니다. 각 단계의 끝에서 구현을 확인할 수도 있지만 세 단계를 모두 완료하기 전에는 올바른 동작이 보장되지는 않습니다.
* **단계 1:** 정점 버퍼
  - .obj 파일에서 정점 데이터를 읽어옵니다. <br>
    테스트에는 이 [**teapot.obj**](https://github.com/AengChoon/Interactive-Graphics/blob/9ad839944c71c9a7237e8283fd9bfbb6ead5d261/Transformations/assets/teapot.obj) 파일을 사용할 수 있습니다. <br>
    .obj 파일을 파싱하기 위해 [cyTriMesh 코드 릴리스](http://www.cemyuksel.com/cyCodeBase/code.html#cyTriMesh)를 사용할 수 있습니다.
  - 프로그램은 .obj 파일의 이름을 첫 번째 명령줄 인수로 사용해야 합니다.
  - 정점 배열 객체를 생성하고 바인딩합니다. <br>
    OpenGL 확장 함수들을 초기화하기 위해 [GLEW](https://glew.sourceforge.net/) 라이브러리를 사용할 수 있습니다. OpenGL 창을 만든 후 glewInit를 호출하는 것을 잊지 마세요.
  - 정점 버퍼를 생성하고 .obj 파일에서 읽은 정점을 사용하여 데이터를 설정합니다. <br>
    이 [튜토리얼](https://ogldev.org/www/tutorial02/tutorial02.html)을 참조하여 정점 버퍼를 생성하는 단계에 대해 알아볼 수 있습니다. 이 튜토리얼에는 정점 배열 객체가 포함되어 있지 않지만 여러분의 구현에는 포함되어야 합니다. 정점 배열 객체 및 사용 방법에 대한 자세한 설명은 [여기](https://yaakuro.gitbook.io/opengl-4-5/vertex-array-object-vao)에서 찾을 수 있습니다.
  - 버퍼 내용을 Display 함수에서 그립니다.
 
이 단계의 끝에서는 화면 중앙에 하나의 (흰색) 점이 보이지만 OpenGL 환경 설정에 따라 완전히 비어 있는 화면이 보일 수도 있습니다.

* **단계 2:** GLSL 셰이더
  - 간단한 정점 셰이더 및 프래그먼트 셰이더를 작성합니다.
  - 정점 셰이더는 모든 정점을 모델-뷰-프로젝션 행렬과 곱하여 변환해야 합니다.
  - 프래그먼트 셰이더는 상수 색상 값을 반환해야 합니다. <br>
    여러분이 원하는 색상 값을 사용할 수 있습니다.
  - 셰이더를 GLSL 프로그램으로 컴파일하고 Draw Call에 사용합니다.
    셰이더를 처리하는 데 [cyGL 코드 릴리스](http://www.cemyuksel.com/cyCodeBase/code.html#cyGL)를 사용할 수 있습니다.
 
만약 정점 셰이더가 주어진 정점 위치를 (행렬과 곱하지 않고) 사용하는 경우, 이 단계의 끝에서 화면 중앙에 하나의 점이 보일 것입니다. 정점 셰이더가 주어진 정점 위치에 0.05를 곱하는 경우에는 차례대로 teapot의 모든 정점이 표시될 것입니다.

* **단계 3:** 변환
  - 두 회전 각도와 모델과 카메라 사이의 거리를 결정하는 이동을 결합하여 카메라 변환을 설정합니다. <br>
    변환 행렬을 설정하는 데 [cyMatrix 코드 릴리스](http://www.cemyuksel.com/cyCodeBase/code.html#cyMatrix)를 사용할 수 있습니다.
  - 왼쪽 마우스 버튼을 누르고 드래그하여 카메라 각도를 조절할 수 있어야 합니다.
  - 오른쪽 마우스 버튼을 누르고 드래그하여 카메라 거리를 조절할 수 있어야 합니다.
  - 투영에는 원근 변환을 사용하십시오.
  - 행렬을 Uniform 매개 변수로 정점 셰이더에 전달하십시오.

## CS 6610 학생들에 대한 추가 요구 사항
* F6 키를 누르면 GLSL 셰이더를 다시 컴파일해야 합니다.
* 적절한 변환 행렬로 모델을 (모델의 Bounding Box를 사용하여) 중앙에 배치하십시오.

## 선택 사항
* P 키를 누르면 원근 및 직교 변환 간 전환이 이루어져야 합니다.
* 직교 변환을 사용할 때 카메라 거리의 역수를 Uniform Scale Factor로 사용하십시오.

## 참고 사항
GLUT / [FreeGLUT](https://freeglut.sourceforge.net/)에서 문제가 있거나 [GLFW](https://www.glfw.org/)를 선호하는 경우 GLFW를 사용할 수 있습니다.
Mac OS X 사용자는 GLEW를 설치하기 위해 이 [튜토리얼](https://lazyfoo.net/tutorials/OpenGL/01_hello_opengl/mac/)을 따를 수 있습니다.

## 결과
