//
//  main.cpp
//  Cldlib
//
//  Created by Clyde Wu on 2020/5/11.
//  Copyright © 2020 Clyde Wu. All rights reserved.
//

#include <iostream>
#include <string>
#include "SemaphoreContainer.hpp"

using namespace std;
using namespace cld;

int main(int argc, const char * argv[]) {
    cld::SemaphoreContainer<deque<int>> queue(100);
    for (int i = 0; i < 101; i++) {
        cout << "Push " << i << " into queue" << endl;
        queue.push(i);
    }
    return 0;
}
