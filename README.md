# Grid hash library

This project aims to map any object with any location attribute into a grid by hashing its location attribute.

First, the global space is divided into cells according to a user-specific parameter cellSize.
Then, a hash value is granted to each object, and the value is the index of the cell it lies in.

By using the grid hash, we can rapidly find out all the objects located in a specific range of a position.

The usage of this library can refer to the demo.

Contact me by submitting an issue if you have any problems with the project.
