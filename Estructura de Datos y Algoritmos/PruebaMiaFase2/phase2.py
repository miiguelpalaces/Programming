"""
Case #2. Exercise  3
@author: EDA Team
"""

import random

# Classes provided by EDA Team
from dlist import DList
from bintree import BinaryNode
from bst import BinarySearchTree
from EjersExtra import *

class BST2(BinarySearchTree):
    def find_dist_k(self, n: int, k:int) -> list:
        array = DList()
        ascendentes = DList()
        self._find_dist_k(self._root, n, k, array, ascendentes)
        l = len(ascendentes)
        if l > 0:
            for i in range(l):
                t = ascendentes[i]
                node = t[0]
                der = t[i]
                d = k -(l-i)
                if d > 0:
                    if der:
                        # busco por la izquierda
                        find_descendencia(node.left, d-1, array)
                    else:
                        # busco por la derecha
                        find_descendencia(node.right, d-1, array)
                elif d == 0:
                    # me añado
                    array.append(node)
        
        return array


    def _find_dist_k(self, node: BinaryNode, n:int, k:int, array:DList, ascendentes:DList):
        if node is not None:
            if node.elem == n:
                find_descendencia(node, k, array)
            else:
                if n < node.elem:
                    ascendentes.add_last((node, False))
                    self._find_dist_k(node.left, n, k, array, ascendentes)
                elif n > node.elem:
                    ascendentes.add_last((node, True))
                    self._find_dist_k(node.right, n, k, array, ascendentes)

def find_descendencia(node: BinaryNode, k:int, array:DList):
    """ encuentra los descendientes de un nodo """
    if node is not None:
        if k == 0:
            array.add_last(node)
        else:
            find_descendencia(node.left, k-1, array)
            find_descendencia(node.right, k-1, array)


def create_tree(input_tree1: BinarySearchTree, input_tree2: BinarySearchTree, opc: str) -> BinarySearchTree:
    """ merge: Árbol formado por los elementos de ambos sin duplicados.
        intersection: Árbol formado únicamente por los elementos duplicados.
        difference: Árbol formado por los elementos de tree1 que no están en tree2. """
    
    array = DList() 
    result_merge = BST2()
    result_inter = BST2()
    # introducimos los elementos de los árboles en array
    _list_elem_tree(input_tree1._root, array) # O(h)
    _list_elem_tree(input_tree2._root, array) # O(h)
    if opc == "merge":
        output_tree = _mix_trees(array, result_merge, result_inter, input_tree1, opc)
    elif opc == "intersection":
        output_tree = _mix_trees(array, result_merge, result_inter, input_tree1, opc)
    else:  # opc == "difference"
        output_tree = _mix_trees(array, result_merge, result_inter, input_tree1, opc)
    return output_tree


def _mix_trees(array: DList, result_merge: BST2, result_inter: BST2, result_diff: BST2, opc: str):
    """ crea un nuevo arbol con los elementos de 2 árboles"""
    if not array.is_empty():
        # array contiene algún elemento
        dato = array.remove_first() # 0(1)
        if result_merge.searchit(dato) == None: # 0(n)
            # no está duplicado, insert el elem (merge)
            result_merge.insert(dato) # 0(n)
        else:
            # está duplicado, insert en el árbol (intersection)
            result_inter.insert(dato) # 0(n)
            # está duplicado, remove del tree1 (difference)
            result_diff.remove(dato) # 0(n)
        
        return _mix_trees(array, result_merge, result_inter, result_diff, opc)
    else:
        # return soluciones en función de la opción
        if opc == "merge":
            return result_merge
        elif opc == "intersection":
            return result_inter
        else: # opc == "difference"
            return result_diff

def _list_elem_tree(node: BinaryNode, array:DList): # O(h)
    """ añade los elementos de un árbol a una Dlist """
    if node is not None:
        # añadimos los elementos a la lista
        array.add_last(node.elem)
        _list_elem_tree(node.left, array)
        _list_elem_tree(node.right, array)
    
    return array



# Some usage examples
if __name__ == '__main__':
    input_list = [50, 55, 54, 20, 60, 15, 18, 5, 25, 24, 75, 80]
    # input_list_01 = [5, 1, 7, 9, 23]
    # input_list_02 = [1, 9, 11]

    input_list_01 = [5, 12, 2, 1, 9, 10, 15]
    input_list_02 = [9, 3, 21]

    # Build and draw first tree
    tree1 = BST2()
    for x in range(10):
        x = random.randint(0, 50)
        tree1.insert(x)
    print("tree 1")
    tree1.draw()

    """for nodo in l:
        print(nodo.elem)"""



    # Build and draw second tree
    tree2 = BST2()
    for x in range(10):
        x = random.randint(0, 70)
        tree2.insert(x)
    print("tree 2")
    tree2.draw()

    function_names = ["merge", "intersection", "difference"]
    
    result = create_tree(tree1, tree2, "difference")
    print("tree 1 y 2")
    result.draw()

    """for op_name in function_names:
        res = create_tree(tree1, tree2, op_name)
        print(f"-- Result for {op_name} method. #{res.size()} nodes")
        res.draw()"""
