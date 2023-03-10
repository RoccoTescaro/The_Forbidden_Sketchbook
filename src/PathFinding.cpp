#include "../hdr/PathFinding.h"
#include "../hdr/Map.h"

//PATHALGORITHM

bool PathAlgorithm::isValid(Map &map, const sf::Vector2<int> pos, bool flying, bool ignoreGC)
{
    if(ignoreGC)
        return map.isOccupied(pos, flying);
    else
        return !(map.get<GameCharacter>(pos)||map.isOccupied(pos,flying));
}

//ASTAR

std::deque<sf::Vector2<float>> AStar::tracePath(Map &map, std::unordered_map<sf::Vector2i, Node, Hash> nodeInfo, sf::Vector2<int> target)
{

    std::deque<sf::Vector2<float>> finalPath;

    //trace back adding last node to the finalPath
    while (!(nodeInfo[target].parentPos.x == target.x && nodeInfo[target].parentPos.y == target.y))
    {
        finalPath.emplace_front(map.posIntToFloat(target));
        target = nodeInfo[target].parentPos;
    }

    return finalPath;
};

std::deque<sf::Vector2<float>> AStar::findPath( Map &map, sf::Vector2<float> fStart,sf::Vector2<float> fTarget, bool flying) 
{
    sf::Vector2<int> start=map.posFloatToInt(fStart);
    sf::Vector2<int> target=map.posFloatToInt(fTarget);

    if( start == target )
        return std::deque<sf::Vector2<float>>{};

    //Node map
    std::unordered_map<sf::Vector2<int>, Node, Hash> nodeInfo;
    //Starting node
    nodeInfo[start] = {start, 0, INT_MAX, 0};
    //Exploaration queue
    std::set<Pair, Compare> openList;

    openList.insert(std::make_pair(0, start));
    int startH = Utils::Math::distance(start, target);
    Pair minimumNode = {startH, start};
    int maxIter=20;
    Pair p = *openList.begin();
    
    //EXPLORATION
    while (!openList.empty()&&nodeInfo[p.second].g!=maxIter)
    {
        
        int gNew;

        Pair p = *openList.begin();
        openList.erase(openList.begin());
        
        //new possible steps generation
        std::vector<sf::Vector2i> possibleSteps = {{p.second.x - 1, p.second.y}, {p.second.x + 1, p.second.y}, {p.second.x, p.second.y - 1}, {p.second.x, p.second.y + 1}};
        auto rd = std::random_device{};
        auto r = std::default_random_engine{rd()};
        std::shuffle(possibleSteps.begin(), possibleSteps.end(), r);

        //new possible steps evaluation
        for (auto &k : possibleSteps)
        {
            if (!(isValid(map, k, flying)|| k == target))
                continue;

            if (k == target)
            {
                if(isValid(map, k,flying) || map.get<GameCharacter>(k))
                {
                    nodeInfo[k].parentPos = p.second;
                    return tracePath(map, nodeInfo, target);
                }
                else
                {
                    nodeInfo[k].parentPos = p.second;
                    std::deque<sf::Vector2f> path = tracePath(map, nodeInfo, target);
                    path.pop_back();
                    if(path.size() == 1) path.clear();
                    return path;
                }
            }

            gNew = nodeInfo[p.second].g + 1;

            //check if the node is already explored or if the current path is more optimal
            if ((nodeInfo.find(k) != nodeInfo.end() && nodeInfo[k].g > gNew) || nodeInfo.find(k) == nodeInfo.end())
            {
                nodeInfo[k].g = gNew;
                nodeInfo[k].h = Utils::Math::distance(k, target);
                nodeInfo[k].f = nodeInfo[k].g + nodeInfo[k].h;
                nodeInfo[k].parentPos = p.second;
                openList.insert({nodeInfo[k].f, k});
            }
        }
    }

    std::deque<sf::Vector2f> path = tracePath(map, nodeInfo,(*std::min_element(nodeInfo.begin(),nodeInfo.end(),
                                              [](const std::pair<const sf::Vector2<int>, Node> &p1, const std::pair<const sf::Vector2<int>, Node> &p2)
                                              { return (p1.second.h < p2.second.h); })).first );
    path.push_back(map.posIntToFloat(target));

    return path;
};

//DIGLETMOVEMENT

std::deque<sf::Vector2<float>> DigletMovement::findPath( Map &map, sf::Vector2<float> fStart,sf::Vector2<float> fTarget, bool flying) 
{

    sf::Vector2<int> start=map.posFloatToInt(fStart);
    sf::Vector2<int> target= map.posFloatToInt(fTarget);

    std::list<sf::Vector2<int>> openList;
    std::unordered_map<sf::Vector2<int>, int, Hash> nodeInfo;
    std::vector<sf::Vector2<int>> possibleTargets;

    openList.push_back({0, 0});
    nodeInfo[{0, 0}] = 0;
    int targetDistance = -1; 
    const int maxDistance = 3; 

    while (!openList.empty())
    {   

        sf::Vector2i p = *openList.begin();
        openList.erase(openList.begin());

        if (isValid(map, sf::Vector2<int>{(int)target.x + p.x,(int) target.y + p.y}, true)&&nodeInfo[{p.x, p.y}] >= targetDistance)
        {
           
            if (nodeInfo[{p.x, p.y}] > targetDistance)
            {
                possibleTargets.clear();
                targetDistance = nodeInfo[{p.x, p.y}];
            }
            possibleTargets.emplace_back(target.x + p.x , target.y + p.y );
        }

        if (nodeInfo[{p.x, p.y}] < maxDistance)
        {
            std::vector<sf::Vector2i> possibleSteps = {{p.x - 1, p.y - 1}, {p.x + 1, p.y + 1}, {p.x + 1, p.y - 1}, {p.x - 1, p.y + 1}};
            for (auto k : possibleSteps)
            {
                if (nodeInfo.find(k)==nodeInfo.end())
                {   
                    if (isValid(map, sf::Vector2<int>{(int)target.x + k.x,(int) target.y + k.y}, false, true))
                        openList.push_back(k);
                    nodeInfo[k] = nodeInfo[{p.x, p.y}] + 1; 
                }
            }
        }
    }
    std::deque<sf::Vector2<float>> destination;
    sf::Vector2<int> &d = possibleTargets.at(rand() % possibleTargets.size());
    destination.push_front(fTarget);
    destination.emplace_front(d.x*map.getCellDim().x,d.y*map.getCellDim().y);
    destination.push_front(fStart);
    return destination;
}