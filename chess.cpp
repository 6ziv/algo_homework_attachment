#include <deque>
#include <vector>
#include <map>
#include <set>

std::vector<std::vector<int>> func(std::deque<std::vector<std::vector<int>>> x) {
    std::vector<std::vector<int>> current;
    current.resize(1);
    while (!x.empty()) {
        auto v = x.front();
        x.pop_front();
        std::vector<std::vector<int>> replacement;
        for (auto t : v) {
            auto tmp = current;
            for (auto& vec : tmp) {
                for (auto x : t)
                    vec.push_back(x);
            }
            for (auto& x : tmp)
                replacement.emplace_back(std::move(x));
        }
        current = replacement;
    }
    return current;
}
bool inbound(int y, int x) {
    if (y < 0 || y >= 10)return false;
    if (x < 0 || x >= 9)return false;
    return true;
}
int pos_to_id(int y, int x) {
    return y * 9 + x + 1;

}

void print_restrictions(std::vector<std::vector<int>> r) {
    bool first = true;
    for (auto v : r) {
        if (!first)printf("and ");
        first = false;

        bool first2 = true;
        printf("(");
        for (auto i : v) {
            if (!first2)printf("or ");
            first2 = false;

            int pos = i;
            if (pos < 0)pos = -pos;
            int y = (pos - 1) / 9;
            int x = (pos - 1) % 9;
            if (i < 0)printf("not ");
            printf("(%d, %d)", y, x);
        }
        printf(")");
    }
}
int main()
{
    std::vector<std::pair<int, int>> targets;
    std::vector<std::pair<int, int>> directions = { {-1,-1},{-1,1},{1,-1},{1,1} };
    for (int i = 0; i < 4; i++) {
        auto t1 = directions[i];
        auto t2 = directions[i];
        t1.first *= 2;
        t2.second *= 2;
        targets.push_back(t1);
        targets.push_back(t2);
    }
    
    std::vector<std::vector<int>> total_restrictions;
    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 9; x++) {
            std::deque<std::vector<std::vector<int>>> res_pos;
            res_pos.resize(1);
            res_pos[0].resize(1);
            res_pos[0][0].push_back(pos_to_id(y, x));

            for (int d = 0; d < 4; d++) {
                if (!inbound(y + directions[d].first, x + directions[d].second))continue;
                int block_at = pos_to_id(y + directions[d].first, x + directions[d].second);
                std::vector<int> target_at;
                for (int i = 0; i < 2; i++) {
                    auto jump = targets[d * 2 + i];
                    if (!inbound(y + jump.first, x + jump.second))continue;
                    target_at.push_back(pos_to_id(y + jump.first, x + jump.second));
                }
                if (target_at.empty())continue;

                std::vector<int> no_block;
                no_block.push_back(-block_at);
                std::vector<std::vector<int>> restrictions;
                restrictions.push_back(no_block);
                restrictions.push_back(target_at);
                res_pos.push_back(restrictions);
            }

            //printf("%d,%d = ", y, x);
            //print_restrictions(func(res_pos));
            //printf("\n");

            auto ret = func(res_pos);
            for (auto res : ret) {
                total_restrictions.push_back(res);
            }
        }
    }
    
    printf("p wcnf %d %ld %d \n", 9 * 10, total_restrictions.size() + 9 * 10, 16);
    for (auto r : total_restrictions) {
        printf("%d ", 16);
        for (auto v : r) {
            printf("%d ", v);
        }
        printf("%d ", 0);
        printf("\n");
    }

    for (int i = 1; i <= 90; i++) {
        printf("%d ", 1);
        printf("%d ", -i);
        printf("%d ", 0);
        printf("\n");
    }
}
