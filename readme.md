## 《三维CAD建模》课程作业
#### 基于B-rep的欧拉操作与图形界面设计(2020年11月)

>作业内容描述：
>1)定义半边数据结构
>2)基于半边数据结构，实现5个欧拉操作
>3)基于欧拉操作，实现扫琼操作，并将基于扫琼操作构建的实体模型进行图形显示

#### 1.程序设计架构
```
CadModelingProject  // 工程目录
├── CadModelingProject.pro  //QT底层配置文件
│── Headers                 //C++ 头文件
│   ├── BrepModel.h         //定义欧拉操作
│   ├── ds.h                //数据结构
│   ├── glwidget.h          //基于Opengl的图形操作
│   ├── mainwindow.h        //主界面设计
│   ├── matrix4d.h          //图形设计
│   └── Singleton.h         //单例模式设计
│── Sources               //C++源文件
│   ├── BrepModel.cpp       //欧拉操作的具体实现
│   ├── ds.cpp              //数据结构
│   ├── glwidget.cpp        //基于Opengl的图形操作
│   ├── main.cpp            //程序入口
│   ├── mainwindow.cpp      //主界面设计
│   └── matrix4d.cpp        //图形设计
└── Forms
    └── mainwindow.ui       // ui文件
```

#### 2.核心数据结构

#####(1)实体模型

```
class Solid:public Singleton<Solid>{
public:
    static Solid* GetSingletonPtr(){return Singleton<Solid>::GetSingletonPtr();}
    Face *faces;
    Edge *edges;
    void printEdge();
    Solid();
};

```
#####（2）面
```
class Face{
public:
    int id;
    Solid *solid;
    Loop *outLp;
    Loop *innerLp;
    Face *next;
    Face *pre;
    static int numFace;
    void addInerLoop(Loop *&l);
    Face();
    ~Face();
};

```

#####（3）环
```
class Loop
{
public :
    int id;
    static vector<Loop*> loopArray;
    static int numLoop;
    HalfEdge *halfedges;
    Face *face;
    Loop *next;
    Loop *pre;
    QVector3D normal;
    void printLoop();
    QVector3D getOritation();
    Loop();
};

```


#####（4）实体边
```
class Edge{
public:
    HalfEdge *half_l;
    HalfEdge *half_r;
    Edge *next;
    Edge *pre;
    int id;
    static int numEdge;
    Edge();
};
```


#####（5）半边
```
class HalfEdge
{
public:
    Edge *edge;
    Vertex *sv;
    Vertex *ev;
    Loop *lp;
    HalfEdge *next;
    HalfEdge *pre;
    HalfEdge *brother;
    void printHfEdge();
    HalfEdge();
};
```

#####（6）顶点
```
class Vertex
{
public:
    int id;
    static vector<Vertex*> vertexArray;
    QVector3D coordinate;
    Vertex *next;
    Vertex *pre;
    static int  numVertex;
    Vertex(QVector3D q);
};
```
#####（7）欧拉操作
```
    void mvfs(QVector3D coordinate);
    Vertex* mev(Vertex *sv, QVector3D coordinate, Loop *loop);
    void mef(Vertex* sv, Vertex *ev, Loop *loop=nullptr);
    void kemr(Vertex *sv, Vertex *ev, Loop *loop);
    void kemr(Edge* e, Loop *loop);
    void semv(Vertex *sv, Vertex *ev,QVector3D coordinate);
    void kfmrh(Face *fa, Face *fb);
    void semv(Edge* e,QVector3D coordinate);
    void sweep(QVector3D oritation, Loop *loop);
    void clearData();
```
#### 3.实现的主要功能
#####(1)通过课上讲解的知识点以及相关伪代码案例实现了基于B-rep的数据结构
#####(2)实现了6个最常用的欧拉操作,以及扫琼操作
>mvfs():定义一个体、一个面、一个外环、一个点
>mev():构造一个新点，同时构造一条连接新点与一给定点的边
>mef():构造一条新边，同时构造一个新面
>kemr():消去环中的一条边，构造一个内环
>kfmrh():删除一个面，并将其定义为一个内环，进而在体中构造一个柄，或将两个物体合并一个物体
>semv():在给定边上构造一个新点，同时将边分裂为两条边
>sweep():基于B-rep及相关的欧拉操作进行扫成操作
#####(3)利用Qt的OpenGL库进行图形绘制，将基于扫琼操作的实体模型进行可视化
#####(4)支持交互操作
#### 4.开发环境和配置
```
操作系统:Windows10 x64
集成开发环境:Qt5.12.6 Qt Creator4.10.2
编译器：msvc x64 2017
```

#### 5.实体模型生成案例
利用欧拉操作mvfs,mev,mef,kemr,kfmrh以及sweep生成带有底面为三角形和梯形的亏格的实体模型，实体模型的Face,Loop,Vertex,HalfEdge等元素可追溯查找，每个拓扑元素带有其标号，并有与其对应的list收集信息，如下图所示
![实体模型](https://github.com/LumenWang/Java_Eclipse/raw/master/2.png)

#### 6.课程心得体会
本门课程是在CAD实验室上的第一门专业课，在课上学到了很多图形学/CAD方面的知识，包括CAD系统的发展，三种实体模型，欧拉操作，扫成操作，B-rep，CSG树，Voexl、八叉树，参数化特征建模等等，对于入门图形学/CAD领域很有帮助，希望今后能够基于这些知识在专业领域内有所研究和突破。同时也感谢老师的细心讲解，谢谢。
