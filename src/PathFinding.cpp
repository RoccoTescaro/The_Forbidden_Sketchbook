#include "../hdr/PathFinding.h"
#include "../hdr/Map.h"

//PATHALGORITHM

bool PathAlgorithm::isValid(Map &map, const sf::Vector2<int> pos, bool flying, bool ignoreGC)
{
    if(ignoreGC){
        return map.isOccupied(pos, flying);
    }else{
        return !(map.get<GameCharacter>(pos)||map.isOccupied(pos,flying));
    }
}

//ASTAR

std::deque<sf::Vector2<float>> AStar::tracePath(Map &map, std::unordered_map<sf::Vector2i, node, hash> nodeInfo, sf::Vector2<int> target)
{

    std::deque<sf::Vector2f> finalPath;

    //aggiungo il target al percorso e mi sposto sul nodo genitore
    while (!(nodeInfo[target].parentPos.x == target.x && nodeInfo[target].parentPos.y == target.y))
    {
        finalPath.emplace_front(map.posIntToFloat(target));
        target = nodeInfo[target].parentPos;
    }

    //aggiungo il nodo di partenza
    //finalPath.emplace_front(map.posIntToFloat(target));
    return finalPath;
};

std::deque<sf::Vector2<float>> AStar::findPath( Map &map, sf::Vector2<float> fStart,sf::Vector2<float> fTarget, bool flying) 
{

    sf::Vector2<int> start=map.posFloatToInt(fStart);
    sf::Vector2<int> target=map.posFloatToInt(fTarget);

    if(start==target)
        return {};
        //dichiarazione strutture dati necessarie

    //mappa dei nodi
    std::unordered_map<sf::Vector2<int>, node, hash> nodeInfo;
    //nodo di partenza
    nodeInfo[start] = {start, 0, INT_MAX, 0};
    //coda di esplorazione
    std::set<Pair, fCompare> openList;

        //inizializzazione strutture e dati
    openList.insert(std::make_pair(0, start));
    int startH = Utils::Math::distance(start, target);
    Pair minimumNode = {startH, start};
    int maxIter=20;
    Pair p = *openList.begin();
    
        //esplorazione
    while (!openList.empty()&&nodeInfo[p.second].g!=maxIter)
    {
        
        int gNew;

        //caricamento nuovo nodo
        Pair p = *openList.begin();
        openList.erase(openList.begin());
        
        //generazione possibili passi
        std::vector<sf::Vector2i> possibleSteps = {{p.second.x - 1, p.second.y}, {p.second.x + 1, p.second.y}, {p.second.x, p.second.y - 1}, {p.second.x, p.second.y + 1}};
        auto rd = std::random_device{};
        auto r = std::default_random_engine{rd()};
        std::shuffle(possibleSteps.begin(), possibleSteps.end(), r);

        //esplorazione passi
        for (auto &k : possibleSteps)
        {
                //passo valido
            if (!(isValid(map, k, flying)||isDestination(k,target)))
                continue;

                //destinazione trovata
            if (isDestination(k, target))
            {
                //destinazione valida o nemico
                if(isValid(map, k,flying) || map.get<GameCharacter>(k)){
                    nodeInfo[k].parentPos = p.second;
                    // costruzione e resa della lista di passi
                    return tracePath(map, nodeInfo, target);
                }
                //destinazione non valida
                else{
                    nodeInfo[k].parentPos = p.second;
                    std::deque<sf::Vector2f> path= tracePath(map, nodeInfo, target);
                    //rimozione ultimo elemento non valido
                    path.pop_back();
                    if(path.size()==1)
                        //coda vuota(solo posizione attuale)
                        path.clear();
                    return path;
                    }
                
            }

            //calcolo informazioni di esplorazione del nodo esplorato
            gNew = nodeInfo[p.second].g + 1;

                //controllo se l'elemento in esame è gia stato esplorato o se il percorso attuale è più ottimale
            if ((nodeInfo.find(k) != nodeInfo.end() && nodeInfo[k].g > gNew) || nodeInfo.find(k) == nodeInfo.end())
            {
                //assegnazione nuovi dati
                nodeInfo[k].g = gNew;
                nodeInfo[k].h = Utils::Math::distance(k, target);
                nodeInfo[k].f = nodeInfo[k].g + nodeInfo[k].h;
                nodeInfo[k].parentPos = p.second;
                
                //inserimento in coda
                openList.insert({nodeInfo[k].f, k});
            }
        }
    }
    std::deque<sf::Vector2f> path=tracePath(map, nodeInfo,(*std::min_element(nodeInfo.begin(),nodeInfo.end(),[]
                                                    (const std::pair<const sf::Vector2<int>, node> &p1, const std::pair<const sf::Vector2<int>, node> &p2)
                                                    {return (p1.second.h<p2.second.h);})).first );
    path.push_back(map.posIntToFloat(target));

    return path;
};

//DIGLETMOVEMENT

std::deque<sf::Vector2<float>> DigletMovement::findPath( Map &map, sf::Vector2<float> fStart,sf::Vector2<float> fTarget, bool flying) 
{

    sf::Vector2<int> start=map.posFloatToInt(fStart);
    sf::Vector2<int> target= map.posFloatToInt(fTarget);

        //dichiarazione strutture e dati
    //coda di esplorazione
    std::list<sf::Vector2i> openList;
    //mappa dei nodi
    std::unordered_map<sf::Vector2i, int, hash> nodeInfo;
    //possibili destinazioni finali
    std::vector<sf::Vector2<int>> possibleTargets;

        //inizializzazione strutture e dati
    openList.push_back({0, 0});
    nodeInfo[{0, 0}] = 0;
    //distanza minima attuale dal target
    int targetDistance = -1; 
    //distanza massima accettabile
    const int maxDistance = 3; 

        //esplorazione

    while (!openList.empty())
    {   

        //caricamento nuovo nodo
        sf::Vector2i p = *openList.begin();
        openList.erase(openList.begin());

        //controllo se il nodo è valido o a distanza >= dalla distanza minima attuale

                //TODO fix exploration conditions
        if (isValid(map, sf::Vector2<int>{(int)target.x + p.x,(int) target.y + p.y}, true)&&nodeInfo[{p.x, p.y}] >= targetDistance)
        {
            //nuova distanza minima
            if (nodeInfo[{p.x, p.y}] > targetDistance)
            {
                //clear del il vettore di possibili destinazioni e aggiornamento della nuova distanza minima
                possibleTargets.clear();
                targetDistance = nodeInfo[{p.x, p.y}];
            }
            //aggiunta nuova destinazione
            possibleTargets.emplace_back(target.x + p.x , target.y + p.y );
            
        }

        //esplorazione nuovi nodi 
        if (nodeInfo[{p.x, p.y}] < maxDistance)
        {
            std::vector<sf::Vector2i> possibleSteps = {{p.x - 1, p.y - 1}, {p.x + 1, p.y + 1}, {p.x + 1, p.y - 1}, {p.x - 1, p.y + 1}};
            for (auto k : possibleSteps)
            {
                //controllo se il nodo è già stato esplorato
                if (nodeInfo.find(k)==nodeInfo.end())
                {   
                    if (isValid(map, sf::Vector2<int>{(int)target.x + k.x,(int) target.y + k.y}, false, true))
                        //se valido lo aggiungo ai possibili bersagli
                        openList.push_back(k);
                    //aggiorno la sua distanza
                    nodeInfo[k] = nodeInfo[{p.x, p.y}] + 1; 
                }
            }
        }
    }
    std::deque<sf::Vector2f> destination;
	//estrazione casuale della destinazione
    sf::Vector2i &d =possibleTargets.at(rand() % possibleTargets.size());
    destination.push_front(fTarget);
    destination.emplace_front(d.x*map.getCellDim().x,d.y*map.getCellDim().y);
    destination.push_front(fStart);
    return destination;
}