/* 数据结构 */
#include "ds.h"
#include "mainwindow.h"
int Vertex::numVertex=0;
int Edge::numEdge=0;
int Loop::numLoop=0;
int Face::numFace=0;
vector<Vertex*> Vertex::vertexArray=vector<Vertex*>();
vector<Loop*> Loop::loopArray=vector<Loop*>();

void Solid::printEdge()
{
    Edge* temp=edges;
    while(true){
        MainWindow::GetSingletonPtr()->print(QString::number(temp->id),InfoType::error);
        temp=temp->next;
        if(temp==edges)
            break;
    }
}

void Loop::printLoop()
{
    HalfEdge* temp=halfedges;
    MainWindow::GetSingletonPtr()->print("Begin print Loop"+QString::number(this->id)+":");
    while(true){
        MainWindow::GetSingletonPtr()->print(QString::number(temp->sv->id)+"--->"+QString::number(temp->ev->id));
        temp=temp->next;
        if(temp==halfedges)
            break;
    }
}

QVector3D Loop::getOritation()
{
    QVector3D temp;
    HalfEdge* it=halfedges;
    if(it==nullptr)
        return QVector3D(1,0,0);
    while(true){
        temp+=QVector3D::crossProduct(it->sv->coordinate-it->ev->coordinate,it->next->sv->coordinate-it->next->ev->coordinate);
        it=it->next;
        if(it==halfedges)
            break;
    }
    return temp.normalized();
}

void Face::addInerLoop(Loop* &l)
{

    if(innerLp!=nullptr){
        innerLp->pre->next=l;
        l->next=innerLp;
        l->pre=innerLp->pre;
        innerLp->pre=l;
        l->face=this;
    }
    else{
        innerLp=l;
        l->face=this;
        l->next=l;
        l->pre=l;
    }
}

Face::~Face()
{
    this->pre->next=this->next;
    this->next->pre=this->pre;
}

void HalfEdge::printHfEdge()
{
    MainWindow::GetSingletonPtr()->print(QString::number(sv->id)+"--->"+QString::number(ev->id));
}

