# --- Equilibrado ---

from bst import *

class tree(BinarySearchTree):

    def height(self, node) -> int:
        return self._height(node)
    
    def _height(self, node:BinaryNode) -> int:
        """ return the height of a node """
        if node is None:
            return -1
        else:
            return 1 + max(self._height(node.left), self._height(node.right))
    
    def factor_equilibrio(self, node: BinaryNode):
        """ chekea si un nodo esta desequilibrado """

        height_left = self.height(node.left)
        height_right = self.height(node.right)

        return True if abs(height_left - height_right) < 2 else False
    
    def rotate_right(self, node: BinaryNode):
        """ rotación de derechas"""
        new_root = node.left
        subtree = new_root.right
        new_root.right = node
        node.left = subtree
        return node
    
    def rotate_left(self, node:BinaryNode):
        """ rotación de izquierdas """
        new_root = node.right
        subtree= new_root.left
        new_root.right = node
        node.right = subtree
        return node


    def _rebalance(self, node: BinaryNode):
        
        if abs(self.factor_equilibrio(node)) <= 1:
            return node
        
        height_left = self._height(node.left)
        height_right = self._height(node.right)

        # la rama izquierda es más larga que la derecha
        # hacemos una rotación de derechas
        if height_left > height_right:
            height_left_left = self._height(node.left.left)
            height_left_right = self._height(node.left.right)
            if height_left_left < height_left_right:
                # doble rotación 
                node.left = self.rotate_left(node.left)
            node = self.rotate_right(node)
        else:
            height_right_left = self._height(node.right.left)
            height_right_right = self._height(node.right.right)
            if height_right_right < height_right_left:
                # doble rotación
                node.right = self.rotate_right(node.right)
            node = self.rotate_left(node)
        return node