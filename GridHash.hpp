//
// Created by CrazySpy on 2022/8/23.
//

#ifndef GRIDHASH_GRIDHASH_HPP
#define GRIDHASH_GRIDHASH_HPP

#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <algorithm>

namespace gridHash {
    // Define the location type.
    using LocationType = std::pair<double, double>;
    using CellIndexType = int;
    using CellIdType = std::pair<CellIndexType, CellIndexType>;

    // Define a hash function for Cell id type.
    struct CellHash {
        size_t operator() (const CellIdType &cellId) const noexcept {
            size_t hashVal = std::hash<CellIndexType>()(cellId.first) + 0x9e3779b9;
            hashVal ^= std::hash<CellIndexType>()(cellId.second) + 0x9e3779b9 + (hashVal << 6) + (hashVal >> 2);

            return hashVal;
        }
    };

    template <typename T>
    struct LocationFetcher {
        LocationType operator() (const T &element) {
            return element.location;
        }
    };

    // T is the type of elements in the grid. Comparable.
    // LocationFetcher transforms the element type T to CellIdType.
    template <typename T, typename LocationFetcher=LocationFetcher<T>>
    class GridHash {
    private:
        double _cellSize;

        // Get the elements in the cell by cell id.
        std::unordered_map<CellIdType, std::set<T>, CellHash> _cellElements;

        // Map the element to the cell id.
        CellIdType _getCellId(const T &element);
        CellIdType _getCellId(const LocationType &location);
    public:
        GridHash(double cellSize);

        // Insert an element to the grid.
        void insert(const T &element);

        // Erase an element from the grid.
        void erase(const T &element);

        // Get all the possible elements in the grid map.
        std::vector<T> getCandidates(const LocationType &location, double range);

        // Get accurate elements within the range in the grid map.
        std::vector<T> getAccurate(const LocationType &location, double range);
    };

    template <typename T, typename LF>
    CellIdType GridHash<T, LF>::_getCellId(const T &element) {
        auto location = LF()(element);

        return _getCellId(location);
    }

    template <typename T, typename LF>
    CellIdType GridHash<T, LF>::_getCellId(const LocationType &location) {
        double x = location.first;
        double y = location.second;

        int xIndex = x >= 0 ? x / _cellSize : std::floor(x / _cellSize);
        int yIndex = y >= 0 ? y / _cellSize : std::floor(y / _cellSize);

        return {xIndex, yIndex};
    }

    template <typename T, typename LF>
    GridHash<T, LF>::GridHash(double cellSize) :_cellSize(cellSize) {
    }

    template <typename T, typename LF>
    void GridHash<T, LF>::insert(const T &element) {
        CellIdType cellId = _getCellId(element);
        _cellElements[cellId].insert(element);
    }

    template <typename T, typename LF>
    void GridHash<T, LF>::erase(const T &element) {
        auto cellId = _getCellId(element);

        if(!_cellElements.count(cellId)) return;
        _cellElements[cellId].erase(element);
    }

    template <typename T, typename LF>
    std::vector<T> GridHash<T, LF>::getCandidates(const LocationType &location, double range) {
        double minX = location.first - range;
        double maxX = location.first + range;
        double minY = location.second - range;
        double maxY = location.second + range;

        CellIdType minIndex = _getCellId({minX, minY});
        CellIdType maxIndex = _getCellId({maxX, maxY});

        std::vector<T> candidates;
        // Traverse all the possible cells.
        for(int x = minIndex.first; x <= maxIndex.first; ++x) {
            for(int y = minIndex.second; y <= maxIndex.second; ++y) {
                if(!_cellElements.count({x, y})) continue;
                std::set<T> &cellElements = _cellElements[{x, y}];

                std::vector<T> tmp;
                std::set_union(candidates.cbegin(), candidates.cend(),
                               cellElements.cbegin(), cellElements.cend(),
                               std::back_inserter(tmp));
                candidates = std::move(tmp);
            }
        }

        return candidates;
    }

    template <typename T, typename LF>
    std::vector<T> GridHash<T, LF>::getAccurate(const LocationType &location, double range) {
        auto candidates = getCandidates(location, range);
        std::vector<T> accurate;

        for(auto &candidate : candidates) {
            LocationType location2 = LF()(candidate);
            double distanceSquare = std::pow((location.first -location2.first), 2) +
                                    std::pow((location.second -location2.second), 2);
            if(distanceSquare <= std::pow(range, 2)) {
                accurate.push_back(candidate);
            }
        }

        return accurate;
    }
};

#endif //GRIDHASH_GRIDHASH_HPP
