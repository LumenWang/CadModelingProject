/* 该文件用于定义基于B-rep模型的头文件 */
#ifndef MODEL_H
#define MODEL_H
#include "ds.h"
#include <iostream>
#include <QDebug>
#include "Singleton.h"
using namespace std;

class BrepModel:public Singleton<BrepModel>
{
public:
    BrepModel();
    static BrepModel* Instance(){return Singleton<BrepModel>::GetSingletonPtr();}

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

private:
    void updateGUI();
    Edge* getEdgeBy2Vertex(const Vertex* v1,const Vertex* v2);
    HalfEdge* gethfEdgeInLoopByVertex(const Loop* loop, const Vertex* v, int rank=1);
    inline void deleteEdgeInList(Edge* edgeToDelete);
    inline void addEdge(Edge* edgeToAdd);
    inline void deleteFaceInList(Face* faceToDelete);
    inline void addFace(Face* faceToAdd);
    bool hfEdgeInLoop(HalfEdge* he,Loop* l);
};

#endif // MODEL_H
