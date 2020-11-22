/* B-rep */
#include "BrepModel.h"
#include "mainwindow.h"

BrepModel::BrepModel()
{
}

void BrepModel::mvfs(QVector3D coordinate)
{
    Vertex* v=new Vertex(coordinate);
    Solid::GetSingletonPtr()->faces=new Face();
    Face* f=Solid::GetSingletonPtr()->faces;
    Loop* l=new Loop();
    v->pre=v;
    v->next=v;
    f->outLp=l;
    f->next=f;
    f->pre=f;
    l->face=f;
    f->solid=Solid::GetSingletonPtr();
    Solid::GetSingletonPtr()->faces=f;
    GUI->print("mvfs  coordinate: "+QString::number(coordinate.x())+" "+QString::number(coordinate.y())+" "+QString::number(coordinate.z()));
    updateGUI();
}

Vertex *BrepModel::mev(Vertex *sv, QVector3D coordinate, Loop *loop)
{
    //make point
    Vertex* ev=new Vertex(coordinate);

    //make edge
    Edge* newEdge=new Edge();
    HalfEdge* hfEdgeLeft=new HalfEdge();
    HalfEdge* hfEdgeRight=new HalfEdge();

    //initial pointer
    newEdge->half_l=hfEdgeLeft;
    newEdge->half_r=hfEdgeRight;
    hfEdgeLeft->edge=newEdge;
    hfEdgeLeft->brother=hfEdgeRight;
    hfEdgeRight->brother=hfEdgeLeft;
    hfEdgeLeft->ev=ev;
    hfEdgeLeft->sv=sv;
    hfEdgeRight->sv=ev;
    hfEdgeRight->ev=sv;

    ev->next=sv->next;
    ev->pre=sv;
    sv->next->pre=ev;
    sv->next=ev;

    //loop operation
    if(loop->halfedges){
        HalfEdge* currentHalfEdge=gethfEdgeInLoopByVertex(loop,sv);
        currentHalfEdge->next->pre=hfEdgeRight;
        hfEdgeLeft->next=hfEdgeRight;
        hfEdgeRight->pre=hfEdgeLeft;
        hfEdgeRight->next=currentHalfEdge->next;
        hfEdgeLeft->pre=currentHalfEdge;
        currentHalfEdge->next=hfEdgeLeft;
    }
    else{
        loop->halfedges=hfEdgeLeft;
        hfEdgeLeft->next=hfEdgeRight;
        hfEdgeLeft->pre=hfEdgeRight;
        hfEdgeRight->next=hfEdgeLeft;
        hfEdgeRight->pre=hfEdgeLeft;
    }

    //edge operation
    if(Solid::GetSingletonPtr()->edges!=nullptr){
        addEdge(newEdge);
    }
    else{
        Solid::GetSingletonPtr()->edges=newEdge;
        newEdge->pre=newEdge;
        newEdge->next=newEdge;
    }  
    GUI->print("mev start vertex: "+QString::number(sv->id) +" new vertex coordinate: "+QString::number(coordinate.x())+" "+QString::number(coordinate.y())+" "+QString::number(coordinate.z())+" loop:"+QString::number(loop->id));
    updateGUI();
    return ev;
    //vertex operation

}

void BrepModel::mef(Vertex *sv, Vertex *ev,Loop* loop)
{
    bool findLoop=false;
    if(loop==nullptr)
        findLoop=true;
    else if(gethfEdgeInLoopByVertex(loop,sv)==nullptr||gethfEdgeInLoopByVertex(loop,ev)==nullptr){
        loop=nullptr;
        findLoop=true;
    }
    if(findLoop){
        for(unsigned int i=0;i<Loop::loopArray.size();i++){
            if(gethfEdgeInLoopByVertex(Loop::loopArray[i],sv)&&gethfEdgeInLoopByVertex(Loop::loopArray[i],ev)){
                loop=Loop::loopArray[i];
                break;
            }
        }
    }

    if(loop==nullptr){
        GUI->print("error occur in finding loop",InfoType::error);
        return;
    }
    HalfEdge* startHfEdge=gethfEdgeInLoopByVertex(loop,sv);
    HalfEdge* endHfEdge=gethfEdgeInLoopByVertex(loop,ev);
    if(hfEdgeInLoop(endHfEdge->brother,loop)){
        endHfEdge=endHfEdge->brother->pre;
    }

    //new element
    Loop *newLoop=new Loop();
    Face* newFace=new Face();
    Edge* newEdge=new Edge();
    HalfEdge* newHfEdLeft=new HalfEdge();
    HalfEdge* newHfEdRight=new HalfEdge();


    //build ds
    newHfEdLeft->sv=sv;
    newHfEdLeft->ev=ev;
    newHfEdRight->ev=sv;
    newHfEdRight->sv=ev;
    newEdge->half_l=newHfEdLeft;
    newEdge->half_r=newHfEdRight;
    newHfEdLeft->brother=newHfEdRight;
    newHfEdRight->brother=newHfEdLeft;
    newHfEdLeft->edge=newEdge;
    newHfEdRight->edge=newEdge;


    newFace->outLp=newLoop;
    newLoop->face=newFace;
    newFace->solid=Solid::GetSingletonPtr();

    //build half edge
    newHfEdLeft->next=endHfEdge->next;
    newHfEdLeft->pre=startHfEdge;
    newHfEdRight->next=startHfEdge->next;
    newHfEdRight->pre=endHfEdge;
    /**/
    endHfEdge->next->pre=newHfEdLeft;
    endHfEdge->next=newHfEdRight;
    startHfEdge->next->pre=newHfEdRight;
    startHfEdge->next=newHfEdLeft;

    newLoop->halfedges=newHfEdRight;
    loop->halfedges=newHfEdLeft;

    //add face
    addFace(newFace);

    //add edge
    addEdge(newEdge);
    GUI->print("mef edge:"+QString::number(sv->id)+"---"+QString::number(ev->id)+" loop: "+QString::number(loop->id));
    updateGUI();

}

void BrepModel::kemr(Vertex *sv, Vertex *ev, Loop *loop)
{
    Edge* edgeToKill=getEdgeBy2Vertex(sv,ev);
    //GUI->print("find edge by two vertexs done",InfoType::info);
    kemr(edgeToKill,loop);
}

void BrepModel::kemr(Edge *e, Loop *loop)
{
    GUI->print("kemr edge:"+QString::number(e->half_l->sv->id)+"---"+QString::number(e->half_l->ev->id));
    Loop* newLoop=new Loop();
    HalfEdge* left=e->half_l;
    HalfEdge* right=e->half_r;
    if(loop->halfedges==left||loop->halfedges==right){
        GUI->print(" kill a begin edge",InfoType::error);
        return;
    }
    HalfEdge* it=left;
    while(it!=right){
        if(it==loop->halfedges){
            newLoop->halfedges=right->next;
            break;
        }
        it=it->next;
    }
    if(it==right){
        newLoop->halfedges=left->next;
    }
    //GUI->print("find new loop begin hfedge done",InfoType::info);
    HalfEdge* preNewLoop=newLoop->halfedges->pre->brother->next;
    HalfEdge* postNewLoop=newLoop->halfedges->pre->pre;
    //build new loop ds
    newLoop->halfedges->pre=preNewLoop->pre->pre;
    preNewLoop->pre->pre->next=newLoop->halfedges;

    //rebuild old loop ds
    preNewLoop->pre=postNewLoop;
    postNewLoop->next=preNewLoop;

    //add loop
    loop->face->addInerLoop(newLoop);

    //delete edge
    deleteEdgeInList(e);

    updateGUI();

}

void BrepModel::semv(Vertex *sv, Vertex *ev, QVector3D coordinate)
{
    Edge* edgeToSp=getEdgeBy2Vertex(sv,ev);
    //GUI->print("find edge by two vertexs done",InfoType::info);
    semv(edgeToSp,coordinate);
    updateGUI();
}

void BrepModel::kfmrh(Face *fa, Face *fb)
{
    GUI->print("kfmrh  kill face:"+QString::number(fa->id)+" Loop add to face:"+QString::number(fb->id));
    if(!fa->outLp){
        GUI->print("input face is empty",InfoType::error);
        return;
    }
    fb->addInerLoop(fa->outLp);
    fa->outLp->face=fb;
    deleteFaceInList(fa);
    updateGUI();
}

void BrepModel::semv(Edge *e, QVector3D coordinate)
{
    HalfEdge* left=e->half_l;
    HalfEdge* right=e->half_r;

    Vertex* v=new Vertex(coordinate);
    Edge* newEdge=new Edge();
    HalfEdge* newHfEdLeft=new HalfEdge();
    HalfEdge* newHfEdRight=new HalfEdge();


    //build ds
    newEdge->half_l=newHfEdLeft;
    newEdge->half_r=newHfEdRight;
    newHfEdLeft->brother=newHfEdRight;
    newHfEdRight->brother=newHfEdLeft;
    newHfEdLeft->edge=newEdge;
    newHfEdRight->edge=newEdge;

    //vertex pointer op
    newHfEdLeft->sv=v;
    newHfEdLeft->ev=left->ev;
    newHfEdRight->ev=v;
    newHfEdRight->sv=left->ev;
    left->ev=v;
    right->sv=v;

    //edge pointer op
    newHfEdLeft->next=left->next;
    newHfEdLeft->pre=left;
    newHfEdRight->pre=newHfEdLeft->next;
    newHfEdRight->next=right;
    right->pre=newHfEdRight;
    left->next=newHfEdLeft;
    addEdge(newEdge);
    GUI->print("semv edge:"+QString::number(e->half_l->sv->id)+"---"+QString::number(e->half_l->ev->id)+"coordinate: "+QString::number(coordinate.x())+" "+QString::number(coordinate.y())+" "+QString::number(coordinate.z()));
    updateGUI();
}

void BrepModel::sweep(QVector3D oritation, Loop *loop)
{
    HalfEdge* it=loop->halfedges;
    vector<Vertex*> vertexArray1;
    vector<Vertex*> vertexArray2;
    while(true){
        vertexArray1.push_back(it->sv);
        it=it->next;
        if(it==loop->halfedges)
            break;
    }
    for(unsigned int i=0;i<vertexArray1.size();i++){
        vertexArray2.push_back(mev(vertexArray1[i],vertexArray1[i]->coordinate+oritation,loop));
    }
    for(unsigned int i=0;i<vertexArray2.size();i++){
        mef(vertexArray2[i],vertexArray2[(i+1)%vertexArray2.size()],loop);
    }
    //Loop::loopArray[Loop::loopArray.size()-3]->printLoop();
    updateGUI();
}

void BrepModel::clearData()
{
    while(Solid::GetSingletonPtr()->edges){
        deleteEdgeInList(Solid::GetSingletonPtr()->edges);
    }
    while(Solid::GetSingletonPtr()->faces){
        deleteFaceInList(Solid::GetSingletonPtr()->faces);
    }

}

void BrepModel::updateGUI()
{
    GUI->updateGLWidget();
    GUI->updateTreeTable();
}

Edge *BrepModel::getEdgeBy2Vertex(const Vertex *v1, const Vertex *v2)
{
    Edge*  it=Solid::GetSingletonPtr()->edges;
    if(!it){
        GUI->print("return a nullptr in getEdgeBy2Vertex;",InfoType::error);
        return nullptr;
    }
    while(true){
        if((it->half_l->ev==v1&&it->half_l->sv==v2)||(it->half_r->ev==v1&&it->half_r->sv==v2)){
            return it;
        }
        it=it->next;
        if(it==Solid::GetSingletonPtr()->edges){
            GUI->print("return a nullptr in getEdgeBy2Vertex;",InfoType::error);
            return nullptr;
        }
    }
}

HalfEdge *BrepModel::gethfEdgeInLoopByVertex(const Loop *loop, const Vertex *v,int rank)
{
    if(!loop->halfedges){
        GUI->print("gethfEdgeInLoopByVertex, loop is nullptr",InfoType::error);
        return nullptr;
    }
    HalfEdge* it=loop->halfedges;
    int t=0;
    while(true){
        if(it->ev==v){
            t++;
            if(t==rank)
                return it;
        }
        it=it->next;
        if(it==loop->halfedges)
            return nullptr;
    }


}

void BrepModel::deleteEdgeInList(Edge *edgeToDelete)
{
    Edge* it=Solid::GetSingletonPtr()->edges;
    while(true){//do not
        if(it==edgeToDelete){
            if(it->next==it){
                Solid::GetSingletonPtr()->edges=nullptr;
            }
            else {
                if(Solid::GetSingletonPtr()->edges==it){Solid::GetSingletonPtr()->edges=it->next;}
                it->pre->next=it->next;
                it->next->pre=it->pre;
            }
            if(it->half_l)
            delete it->half_l;
            if(it->half_r)
            delete it->half_r;
            delete it;
            return;
        }
        it=it->next;
        if(it==Solid::GetSingletonPtr()->edges)
            return ;
    }
}

void BrepModel::addEdge(Edge* edgeToAdd)
{
    Solid::GetSingletonPtr()->edges->pre->next=edgeToAdd;
    edgeToAdd->pre=Solid::GetSingletonPtr()->edges->pre;
    edgeToAdd->next=Solid::GetSingletonPtr()->edges;
    Solid::GetSingletonPtr()->edges->pre=edgeToAdd;
}

void BrepModel::deleteFaceInList(Face *faceToDelete)
{
    if(faceToDelete->next==faceToDelete){
        Solid::GetSingletonPtr()->faces=nullptr;
        delete faceToDelete;
        return;
    }
    if(faceToDelete==Solid::GetSingletonPtr()->faces){
        Solid::GetSingletonPtr()->faces=faceToDelete->next;
        deleteFaceInList(faceToDelete);
        return ;
    }
    faceToDelete->next->pre=faceToDelete->pre;
    faceToDelete->pre->next=faceToDelete->next;
    delete faceToDelete;
}

void BrepModel::addFace(Face *faceToAdd)
{
    Solid::GetSingletonPtr()->faces->pre->next=faceToAdd;
    faceToAdd->pre=Solid::GetSingletonPtr()->faces->pre;
    faceToAdd->next=Solid::GetSingletonPtr()->faces;
    Solid::GetSingletonPtr()->faces->pre=faceToAdd;
}

bool BrepModel::hfEdgeInLoop(HalfEdge *he, Loop *l)
{
    HalfEdge* it=l->halfedges;
    while(true){
        if(it==he)
            return true;
        it=it->next;
        if(it==l->halfedges)
            return false;
    }
}


