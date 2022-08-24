#include <iostream>
#include "../GridHash.hpp"

using namespace std;

// Define a type with the location attribute. The type should comparable.
struct FooType {
    int id;
    pair<double, double> location;
    friend ostream& operator << (ostream &out, const FooType &foo);
    bool operator < (const FooType &foo2) const {
        return location < foo2.location;
    }
};

ostream& operator << (ostream &out, const FooType &foo) {
    out << "id: " << foo.id << ", x: " << foo.location.first << ", y: " << foo.location.second << "." << endl;
    return out;
}

int main() {
    FooType elements[] = {
            {1, {0.5, 0.5}},
            {2, {0.6, 0.4}},
            {3, {1.2, 0.4}},
            {4, {-0.5, -0.5}},
            {5, {-0.1, 0.1}},
            {6, {0.1, -0.1}},
            {7, {-1, 1}},
            {8, {1, 1}},
            {9, {-1, -1}},
    };

    // Define a grid with 1-size cells.
    gridHash::GridHash<FooType> grid(1.0);
    for(auto &element : elements) {
        grid.insert(element);
    }

    // Fetch candidate objects within the distance of 0.5 from the position <0, 0>.
    auto candidates = grid.getCandidates({0, 0}, 0.5);
    // Fetch accurate objects within the distance of 0.5 from the position <0, 0>.
    auto accurate = grid.getAccurate({0, 0}, 0.5);

    cout << "Candidate: " << endl;
    for(auto &candidate : candidates) {
        cout << candidate << endl;
    }

    cout << "Accurate: " << endl;
    for(auto &accurateElement : accurate) {
        cout << accurateElement << endl;
    }

    return 0;
}
