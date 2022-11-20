#include <iostream>
#include <vector>
#include <array>
#include <cstdlib>

using namespace std;

vector<int> cs;
vector<int> steps;

bool isValid(array<int, 3> state){
    if((state[0] < 0 || state[0] > 3) || (state[1] < 0 || state[1] > 3) || (state[0] > 0 && (state[0] < state[1])) || (state[0] < 3 && (state[0] > state[1]))){
       return false;
    }
    return true;
}

bool isFinal(array<int, 3> state){
    if(state[0] == 0){
        if(state[1] == 0){
            if(state[2] == 0){
                return true;
            }
        }
    }
    return false;
}


void addToSteps(array<int, 3> state){
    steps.push_back(state[0]);
    steps.push_back(state[1]);
    steps.push_back(state[2]);
}

bool checkClosedSet(array<int, 3> state){
    if(!cs.empty()) {
        for(unsigned int i = 0; i < cs.size(); i += 3){
            if(cs[i] == state[0]){
                if(cs[i + 1] == state[1]){
                    if(cs[i + 2] == state[2]){
                        return true;
                    }
                }
            }
        }
    }
    cs.push_back(state[0]);
    cs.push_back(state[1]);
    cs.push_back(state[2]);
    return false;
}

void generateMessage(int a, int b, int c){
    cout << "Moved ";
    if(a == 1){
        cout << a << " missionary ";
    } else if(a == 2){
        cout << a << " missionaries ";
    }
    if(a == b){
        cout << "and ";
    }
    if(b == 1){
        cout << b << " cannibal ";
    } else if(b == 2){
        cout << b << " cannibals ";
    }
    cout << "to the ";
    if (c == 0){
        cout << "right ";
    }else{
        cout << "left ";
    }
    cout << "bank." << endl;
}

bool blindSearch(array<int, 3> state){
    bool f;
    if(!checkClosedSet(state)){
        if(!isFinal(state)){
            if(isValid(state)){
                if(state[2] == 1){
                    f = blindSearch({state[0] - 1, state[1] - 1, 0});
                    if(!f) {
                        f = blindSearch({state[0] - 2, state[1], 0});
                        if (!f) {
                            f = blindSearch({state[0], state[1] - 2, 0});
                            if (!f) {
                                f = blindSearch({state[0] - 1, state[1], 0});
                                if(!f){
                                    f = blindSearch({state[0], state[1] - 1, 0});
                                }
                            }
                        }
                    }
                }
                else{
                    f = blindSearch({state[0] + 1, state[1] + 1, 1});
                    if(!f) {
                        f = blindSearch({state[0] + 2, state[1], 1});
                        if (!f) {
                            f = blindSearch({state[0], state[1] + 2, 1});
                            if (!f) {
                                f = blindSearch({state[0] + 1, state[1], 1});
                                if(!f){
                                    f = blindSearch({state[0], state[1] + 1, 1});
                                }
                            }
                        }
                    }
                }
                if(f){
                    addToSteps(state);
                }
                return f;
            }
            return false;
        }
        addToSteps(state);
        return true;
    }
    return false;
}

int manhattan(int m, int c){
    return abs(m) + abs(c);
}

array<array<int, 3>, 5> sort(array<array<int, 3>, 5> children, array<int, 5> mv){
    array<int, 3> temp1;
    int temp2;
    for(int i = 0; i < mv.size() - 1; i++){
        for(int j = i + 1; j < mv.size(); j++) {
            if(mv[i] > mv[j]){
                temp1 = children[j];
                children[j] = children[i];
                children[i] = temp1;
                temp2 = mv[j];
                mv[j] = mv[i];
                mv[i] = temp2;
            }
        }
    }
    return children;
}

bool heuristicSearch(array<int, 3> state) {
    array<array<int, 3>, 5> children;
    array<int, 5> mv;
    bool f;
    if (!checkClosedSet(state)) {
        if (!isFinal(state)) {
            if (isValid(state)) {
                if (state[2] == 1) {
                    children[0] = {state[0] - 1, state[1] - 1, 0};
                    children[1] = {state[0] - 2, state[1], 0};
                    children[2] = {state[0], state[1] - 2, 0};
                    children[3] = {state[0] - 1, state[1], 0};
                    children[4] = {state[0], state[1] - 1, 0};
                } else {
                    children[0] = {state[0] + 1, state[1] + 1, 1};
                    children[1] = {state[0] + 2, state[1], 1};
                    children[2] = {state[0], state[1] + 2, 1};
                    children[3] = {state[0] + 1, state[1], 1};
                    children[4] = {state[0], state[1] + 1, 1};
                }
                for (int i = 0; i < children.size(); i++) {
                    mv[i] = manhattan(children[i][0], children[i][1]);
                }
                children = sort(children, mv);
                f = heuristicSearch(children[0]);
                if(!f) {
                    f = heuristicSearch(children[1]);
                    if (!f) {
                        f = heuristicSearch(children[2]);
                        if (!f) {
                            f = heuristicSearch(children[3]);
                            if(!f){
                                f = heuristicSearch(children[4]);
                            }
                        }
                    }
                }
                if(f){
                    addToSteps(state);
                }
                return f;
            }
            return false;
        }
        addToSteps(state);
        return true;
    }
    return false;
}

int main() {
    array<int, 3> state = {3, 3, 1};
    int selection;
    bool f;
    cout << "Select search algorithm:" << endl << "1.Depth First Search(blind)" << endl << "2.Best-First Search(heuristic)" << endl;
    cin >> selection;
    while(selection != 1 && selection != 2) {
        cin >> selection;
    }
    if (selection == 1){
        f = blindSearch(state);
    }
    else{
        f = heuristicSearch(state);
    }
    if(f){
       for(unsigned int i = steps.size() - 4; i < 4294967295; i -= 3){
           generateMessage(abs(steps[i - 2] - steps[i + 1]), abs(steps[i - 1] - steps[i + 2]), steps[i]);
       }
       cout << "Problem successfully solved!" << endl;
    }
    else{
        cout << "No solution found.";
    }
    return 0;
}