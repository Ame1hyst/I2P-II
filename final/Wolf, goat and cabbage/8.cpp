#include "function.h"
#include <utility>

State Crossing::Go(State s, int wolf, int goat, int cabbage) {
    State next = s;
    int dir = s[6];
    if (dir == 1) {
        next[0] -= wolf; next[1] -= goat; next[2] -= cabbage;
        next[3] += wolf; next[4] += goat; next[5] += cabbage;
    } else {
        next[3] -= wolf; next[4] -= goat; next[5] -= cabbage;
        next[0] += wolf; next[1] += goat; next[2] += cabbage;
    }
    next[6] = -dir;
    return next;
}

bool Crossing::valid(State s) {
    for (int i = 0; i < 6; i++)
        if (s[i] < 0) return false;
    int lw=s[0], lg=s[1], lc=s[2];
    int rw=s[3], rg=s[4], rc=s[5];
    int boat=s[6];
    if (boat == -1) {
        if (lw > lg && lg > 0) return false;
        if (lg > lc && lc > 0) return false;
    }
    if (boat == 1) {
        if (rw > rg && rg > 0) return false;
        if (rg > rc && rc > 0) return false;
    }
    return true;
}

bool Crossing::found(State s) {
    return s[0]==0 && s[1]==0 && s[2]==0;
}

set<State> Crossing::extend(State s) {
    set<State> nexts;
    int dir = s[6];
    int sw = (dir==1) ? s[0] : s[3];
    int sg = (dir==1) ? s[1] : s[4];
    int sc = (dir==1) ? s[2] : s[5];
    int options[4][3] = {{0,0,0},{1,0,0},{0,1,0},{0,0,1}};
    for (int i = 0; i < 4; i++) {
        int w=options[i][0], g=options[i][1], c=options[i][2];
        if (w > sw || g > sg || c > sc) continue;
        State next = Go(s, w, g, c);
        if (valid(next))
            nexts.insert(next);
    }
    return nexts;
}

void Crossing::solve() {
    int X=_items[0], Y=_items[1], Z=_items[2];
    State init;
    init.push_back(X); init.push_back(Y); init.push_back(Z);
    init.push_back(0); init.push_back(0); init.push_back(0);
    init.push_back(1);

    if (!valid(init)) return;

    typedef list<State> Path;
    typedef set<State> Explored;
    typedef std::pair<Path, Explored> Frame;

    std::vector<Frame> stk;
    Path startPath;
    startPath.push_back(init);
    Explored startExplored;
    startExplored.insert(init);
    stk.push_back(std::make_pair(startPath, startExplored));

    while (!stk.empty()) {
        Frame frame = stk.back();
        stk.pop_back();

        Path path = frame.first;
        Explored explored = frame.second;
        State cur = path.back();

        if (found(cur)) {
            _solutions.insert(path);
            continue;
        }

        set<State> nexts = extend(cur);
        for (set<State>::iterator it = nexts.begin(); it != nexts.end(); ++it) {
            const State& next = *it;
            if (explored.count(next)) continue;
            Path newPath = path;
            newPath.push_back(next);
            Explored newExplored = explored;
            newExplored.insert(next);
            stk.push_back(std::make_pair(newPath, newExplored));
        }
    }
}