/* FILE: moves.h */

#ifndef MOVES_H
#define MOVES_H

/* the format of a move {START, JUMP, END} */
/* the position in which the move is started */
#define START       0
/* the position that is jumped over in the move */
#define JUMP        1
/* the ending position of the move */
#define END         2
/* the number of elements in a move */
#define MOVE_LENGTH 3

struct move_array {
    int length;
    int (*values)[MOVE_LENGTH];
};

/*
 * allMoves: return all moves that can be made in the triangular board
 *           (diagonal or horizontal jumping over one space to end
 *           in another valid position)
 * arguments: triangle_layers: the number of layers (and base length)
 *                             in the equilateral triangle
 * return value: an array of all valid moves
 *               (arrays of length three, {START, JUMP, END})
 */
extern struct move_array allMoves(int triangle_layers);

/*
 * movesAt: return an array of all possible moves in the category
 * arguments: start_pos: the starting point for any moves
 *            num_layers: the number of layers in the triangle
 * return value: an array of all the possible moves
 */
extern struct move_array movesAt(int position, int num_layers);

#endif /* _MOVES_H */
