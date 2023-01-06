#include <Collision/cellList.h>
#include <iostream>

void CellList::clear(bool full){
    for (int l = 0; l < nCells; l++){
        uint64_t last;
        if (full){last = MAX_PARTICLES_PER_CELL;}
        if (!full){if (lastElementInCell[l]!= NULL_INDEX){last = lastElementInCell[l]+1;}else{last=0;}}
        if (last == NULL_INDEX){
            last = 0;
        }
        if (last >= 0){
            for (int i = 0; i < last; i++){
                cells[l*MAX_PARTICLES_PER_CELL+i] = NULL_INDEX;
            }
        }
        lastElementInCell[l] = NULL_INDEX;
    }
    uint64_t last;
    if (full){last = MAX_PARTICLES;}
    if (!full){if (lastElement != NULL_INDEX){last = lastElement+1;}else{last=0;}}
    if (last == NULL_INDEX){
        last = 0;
    }
    if (last >= 0){
        for (int i = 0; i < last; i++){
            id[i] = std::pair("",NULL_INDEX);
        }
    }
    lastElement = NULL_INDEX;
}

void CellList::populate(ObjectManager * manager, std::set<Id> objects){
    clear();

    int o = 0;
    for (auto it = objects.begin(); it != objects.cend(); it++){

        cCollideable & data = manager->getComponent<cCollideable>(*it);
        cTransform & dataT = manager->getComponent<cTransform>(*it);

        uint64_t meshSize = data.mesh.size();

        for (int i = 0; i < meshSize; i++){
            const CollisionVertex p = data.mesh[i];
            if (p == NULL_COLLISION_VERTEX){continue;}
            uint64_t h = hash(p.x,p.y);
            if (0 <= h && h < nCells){
                if (lastElementInCell[h] == NULL_INDEX || lastElementInCell[h] < MAX_PARTICLES_PER_CELL){
                    // insert
                    uint64_t next, nextId;
                    if (lastElementInCell[h]==NULL_INDEX){next = 0;}
                    else {next = lastElementInCell[h]+1;}

                    if (lastElement == NULL_INDEX){nextId = 0;}
                    else {nextId = lastElement+1;} 

                    cells[h*MAX_PARTICLES_PER_CELL+next]=nextId;
                    id[nextId] = std::pair(it->idStr,i);

                    lastElement = nextId;
                    lastElementInCell[h] = next;
                }
            }
        }
        o++;
    }
}

void CellList::cellCollisions(
    uint64_t a1,
    uint64_t b1,
    uint64_t a2,
    uint64_t b2,
    ObjectManager * manager,
    CollisionResolver * resolver
){
    if (a1 < 0 || b1 < 0 || a2 < 0 || b2 < 0 || a1 >= rootNCells || b1  >= rootNCells || b2  >= rootNCells || a2  >= rootNCells){
        return;
    }

    uint64_t c1 = (a1*rootNCells+b1)*MAX_PARTICLES_PER_CELL;
    uint64_t c2 = (a2*rootNCells+b2)*MAX_PARTICLES_PER_CELL;

    if (cells[c1] == NULL_INDEX || cells[c2] == NULL_INDEX){return;}

    uint64_t p1 = 0;
    uint64_t p2;

    while (cells[c1+p1] != NULL_INDEX){
        p2 = 0;
        while (cells[c2+p2] != NULL_INDEX){
            uint64_t i,j;
            i = cells[c1+p1]; j = cells[c2+p2];
            resolver->handleObjectCollision(
                id[i].first,id[j].first,
                id[i].second,id[j].second,
                manager
            );
            p2++;
        }
        p1++;
    }
}

void CellList::handleObjectCollisions(
    ObjectManager * manager,
    CollisionResolver * resolver,
    std::set<Id> objects
){
    populate(manager,objects);
    for (int a = 0; a < rootNCells; a++){
        for (int b = 0; b < rootNCells; b++){
            // takes advantage of symmetry
            //  i.e cell a-1,b-1 will collide with
            //  cell a,b so no need to double up!
            cellCollisions(a,b,a,b,manager,resolver);
            cellCollisions(a,b,a+1,b+1,manager,resolver);
            cellCollisions(a,b,a+1,b-1,manager,resolver);
            cellCollisions(a,b,a+1,b,manager,resolver);
            cellCollisions(a,b,a,b+1,manager,resolver);
        }
    }
}

void CellList::handleWorldCollisions(
    ObjectManager * manager,
    CollisionResolver * resolver,
    World * world,
    std::set<Id>
){

}