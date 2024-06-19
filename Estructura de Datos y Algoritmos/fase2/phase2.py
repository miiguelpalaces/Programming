"""
Case #2. Exercise  3
@author: EDA Team
"""

# Classes provided by EDA Team
from dlist import DList
from bintree import BinaryNode
from bst import BinarySearchTree
from avl import AVLTree

# EJERCICIO 1
class BST2(BinarySearchTree):
    def find_dist_k(self, n: int, k:int) -> list:
        """
        Devuelve una lista de Python que contiene todos nodos que se
        encuentren a una distancia k del nodo cuyo elemento es n.
        No se tiene en cuenta el sentido de las distancias.
        Complejidad temporal: O(log2(n)**2)
        Complejidad espacial: O(log2(n))
        """
        result = [] # Lista de python a devolver.

        # Encontrar el nodo con elemento n y guardar en una DList (path) los nodos visitados para ello
        path = DList()
        current = self._root

        while current and current.elem != n:
            path.add_last(current)
            if n < current.elem:
                current = current.left
            elif n > current.elem:
                current = current.right

        path.add_last(current) # Añadir también el nodo con elemento n

        # Comprobar que el nodo n existe. Si no existe, devolver una lista vacía
        if not current:
            print("Error: BST2 does not have a node with n as its element.")
            return result # result está vacía

        # Para cada nodo de path, encontrar los nodos que estén a una distancia k - distancia del nodo de path a n
        distance_to_n = 0 # El último nodo de path es n
        path_direction = None

        while not path.is_empty() and distance_to_n <= k:
            # Añadir a result los nodos que se encuentran a una distancia k - distance_to_n del útimo nodo de path
            if path_direction == "right":
                result += self.dist_k_subtree(path._tail.elem, k - distance_to_n, "left")
            elif path_direction == "left":
                result += self.dist_k_subtree(path._tail.elem, k - distance_to_n, "right")
            else: # path_direction == None (estamos en el nodo n)
                result += self.dist_k_subtree(path._tail.elem, k - distance_to_n, "both")

            removed = path.remove_last() # Guardar el último elemento eliminado del path para poder calcular path_direction

            # Calcular path_direction teniendo en cuenta el último elemento de path
            if not path.is_empty():
                if path._tail.elem.right == removed:
                    path_direction = "right"
                elif path._tail.elem.left == removed:
                    path_direction = "left"

            distance_to_n += 1

        return sorted(result) # Ordenar los resultados en orden creciente

    def dist_k_subtree(self, node: BinaryNode, k: int, direction: str) -> list:
        """
        Devuelve una lista de python con los nodos a una distancia k de node en su subarbol (hacia abajo).
        La variable direction sirve para saber si hay que buscar en todo el subarbol, en la parte izquierda o en la parte dcha.
        """

        if not node: # En caso de que no haya un nodo a una distancia k, devolver una lista vacía.
            return []
        elif k == 0:
            return [node.elem]
        else:
            if direction == "right":
                return self.dist_k_subtree(node.right, k-1, "both")
            elif direction == "left":
                return self.dist_k_subtree(node.left, k-1, "both")
            else:
                # estamos en nodo n, hay q buscar en todo su subárbol
                return self.dist_k_subtree(node.left, k-1, "both") + self.dist_k_subtree(node.right, k-1, "both")

# EJERCICIO 2

def create_tree(input_tree1: BinarySearchTree, input_tree2: BinarySearchTree, opc: str) -> AVLTree:
    """
    Complejidad temporal: O(nlog2(n))
    Complejida espacial: O(n)
    """
    if opc == "merge":
        result = copy_tree(input_tree1) # Insertar todos los elementos de input_tree1 en result
        _difference(input_tree1, input_tree2._root, result) # Insertar los elementos de input_tree2 en result sin repetir elementos
        return result

    elif opc == "intersection":
        result = AVLTree()
        _intersection(input_tree1, input_tree2._root, result) # Insertar en result todos los elementos en común de input_tree1 y input_tree2
        return result
    
    elif opc == "difference":
        result = AVLTree()
        _difference(input_tree2, input_tree1._root, result) # Insertar en result todos los elementos que aparecen en imput_tree1 y no en input_tree2
        return result
    
    else:
        print("Error: create_tree's opc attribute does nos fit inside {'merge', 'intersection', 'difference'}")
        return None
    
def copy_tree(original_tree: AVLTree) -> AVLTree:
    """Returns a deep copy of tree so you can modify it without touching the original one. Time complexity: O(n)"""

    new_tree = AVLTree()
    new_tree._root = _copy_tree(original_tree._root)

    return new_tree

def _copy_tree(original_node: BinaryNode) -> BinaryNode:
    """Recursive function for copy_tree."""
    if original_node:
        new_node = BinaryNode(original_node.elem)
        new_node.left = _copy_tree(original_node.left)
        new_node.right = _copy_tree(original_node.right)

        return new_node
    
    
def _difference(input_tree1: BinarySearchTree, node_input_tree2: BinaryNode, result: AVLTree) -> None:
    """Función resursiva para opc == "merge" y opc == "difference", inserta en result el elemento del nodo si no se encuentra en input_tree1"""
    if node_input_tree2:
        if not input_tree1.search(node_input_tree2.elem):
            result.insert(node_input_tree2.elem)

        _difference(input_tree1, node_input_tree2.left, result)
        _difference(input_tree1, node_input_tree2.right, result)

def _intersection(input_tree1: BinarySearchTree, node_input_tree2: BinaryNode, result: AVLTree) -> None:
    """Función resursiva para opc == "intersection", inserta en result el elemento del nodo si se encuentra en input_tree1"""
    if node_input_tree2:
        if input_tree1.search(node_input_tree2.elem):
            result.insert(node_input_tree2.elem)

        _intersection(input_tree1, node_input_tree2.left, result)
        _intersection(input_tree1, node_input_tree2.right,result)



# Some usage examples
if __name__ == '__main__':
    input_list = [50, 55, 54, 20, 60, 15, 18, 5, 25, 24, 75, 80]
    # input_list_01 = [5, 1, 7, 9, 23]
    # input_list_02 = [1, 9, 11]

    tree0 = BST2()
    for x in input_list:
        tree0.insert(x)

    print("---tree0---")
    tree0.draw()

    n = 5
    k = 5
    result = tree0.find_dist_k(n, k)

    print(f"Find_dist_k({n}, {k}): {result}") 
    print()

    # -------------------------------------------

    input_list_01 = [5, 12, 2, 1, 3, 9, 7]
    input_list_02 = [9, 3, 21, 4, 7, 23]
    
    # Build and draw first tree
    print("---tree1---")
    tree1 = AVLTree()
    for x in input_list_01:
        tree1.insert(x)
    tree1.draw()

    # Build and draw second tree
    print("---tree2---")
    tree2 = AVLTree()
    for x in input_list_02:
        tree2.insert(x)
    tree2.draw()

    function_names = ["merge", "intersection", "difference"]

    for op_name in function_names:
        res = create_tree(tree1, tree2, op_name)
        print(f"-- Result for {op_name} method. #{res.size()} nodes")
        res.draw()
    
    print((3^1492) % 20)