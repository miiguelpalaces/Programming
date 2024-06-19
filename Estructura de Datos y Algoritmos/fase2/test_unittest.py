# -*- coding: utf-8 -*-
"""
Test program comparing solutions with the builtin list-based one.

@author: EDA Team
"""

# Classes provided by EDA Team
from bst import BinarySearchTree
from phase2 import BST2, create_tree
import unittest


class Test(unittest.TestCase):
    def setUp(self):
        
        self.tree0 = BST2()
        elems0 = [14, 11, 18, 10, 13, 16, 19, 5, 12, 15, 17,
                30, 4, 6, 29, 31, 2, 8, 24, 33, 1, 3, 7, 9,
                23, 25, 32, 34, 21, 27, 36, 20, 22, 26, 28, 
                35, 37]
        
        for i in elems0:
            self.tree0.insert(i)

        self.empty = BST2()

        self.tree1 = BinarySearchTree()
        self.tree2 = BinarySearchTree()
        
        elems1 = [5, 12, 2, 1, 3, 9, 7]
        elems2 = [9, 3, 21, 4, 7, 23]

        for e in elems1:
            self.tree1.insert(e)

        for e in elems2:
            self.tree2.insert(e)

        self.merge = [1, 2, 3, 4, 5, 7, 9, 12, 21, 23]
        self.intersection = [3, 7, 9]
        self.difference = [1, 2, 5, 12]

    # ----- find_dist_k unittests -----
    def test1_find_dist_k(self):
        
        input = self.tree0.find_dist_k(30, 0)

        expected = [30]

        self.assertEqual(input, expected, "find_dist_k ERROR: CANNOT HANDLE FINDING NODES")

    def test2_find_dist_k(self):

        input = self.tree0.find_dist_k(30, 2)

        expected = [18, 24, 33]

        self.assertEqual(input, expected, "find_dist_k ERROR: CANNOT HANDLE FINDING NODES")

    def test3_find_dist_k(self):

        input = self.tree0.find_dist_k(12, 6)

        expected = [2, 8, 15, 17, 30]

        self.assertEqual(input, expected, "find_dist_k ERROR: CANNOT HANDLE FINDING NODES")

    
    def test4_find_dist_k(self):

        input = self.tree0.find_dist_k(17, 7)

        expected = [4, 6, 23, 25, 32, 34]

        self.assertEqual(input, expected, "find_dist_k ERROR: CANNOT HANDLE FINDING NODES")

    
    def test5_find_dist_k(self):

        input = self.tree0.find_dist_k(26, 9)

        expected = [11, 15, 17, 36]

        self.assertEqual(input, expected, "find_dist_k ERROR: CANNOT HANDLE FINDING NODES")
    
    def test6_find_dist_k(self):

        input = self.tree0.find_dist_k(0, 2)

        expected = []

        self.assertEqual(input, expected, "find_dist_k ERROR: CANNOT HANDLE FINDING NODES")
    
    def test7_find_dist_k(self):

        input = self.tree0.find_dist_k(100, 4)

        expected = []

        self.assertEqual(input, expected, "find_dist_k ERROR: CANNOT HANDLE FINDING NODES")

    def test8_find_dist_k(self):

        input = self.empty.find_dist_k(None, None)

        expected = []

        self.assertEqual(input, expected, "find_dist_k ERROR: CANNOT HANDLE FINDING NODES")

    def test9_find_dist_k(self):

        input = self.empty.find_dist_k(7, None)

        expected = []

        self.assertEqual(input, expected, "find_dist_k ERROR: CANNOT HANDLE FINDING NODES")

    def test10_find_dist_k(self):

        input = self.empty.find_dist_k(None, 5)

        expected = []

        self.assertEqual(input, expected, "find_dist_k ERROR: CANNOT HANDLE FINDING NODES")

    # ----- create_tree unittests -----

    # opc = "merge"
    def test1_merge(self):
        opc = "merge"

        input = create_tree(self.tree1, self.tree2, opc).inorder_list()

        expected = self.merge

        self.assertEqual(input, expected, "create_tree ERROR: CANNOT HANDLE opc = 'merge'")

    def test2_merge(self):
        opc = "merge"

        input = create_tree(BinarySearchTree(), self.tree2, opc).inorder_list()

        expected = self.tree2.inorder_list()

        self.assertEqual(input, expected, "create_tree ERROR: CANNOT HANDLE opc = 'merge'")

    def test3_merge(self):
        opc = "merge"

        input = create_tree(BinarySearchTree(), BinarySearchTree(), opc).inorder_list()

        expected = []

        self.assertEqual(input, expected, "create_tree ERROR: CANNOT HANDLE opc = 'merge'")

    # opc = "intersection"
    def test1_intersection(self):
        opc = "intersection"

        input = create_tree(self.tree1, self.tree2, opc).inorder_list()

        expected = self.intersection

        self.assertEqual(input, expected, "create_tree ERROR: CANNOT HANDLE opc = 'intersection'")

    def test2_intersection(self):
        opc = "intersection"

        input = create_tree(self.tree1, BinarySearchTree(), opc).inorder_list()

        expected = []

        self.assertEqual(input, expected, "create_tree ERROR: CANNOT HANDLE opc = 'intersection'")

    # opc = "difference"
    def test1_difference(self):
        opc = "difference"

        input = create_tree(self.tree1, self.tree2, opc).inorder_list()

        expected = self.difference

        self.assertEqual(input, expected, "create_tree ERROR: CANNOT HANDLE opc = 'difference'")

    def test2_difference(self):
        opc = "difference"

        input = create_tree(self.tree1, BinarySearchTree(), opc).inorder_list()

        expected = self.tree1.inorder_list()

        self.assertEqual(input, expected, "create_tree ERROR: CANNOT HANDLE opc = 'difference'")

    def test3_difference(self):
        opc = "difference"

        input = create_tree(self.tree1, self.tree1, opc).inorder_list()

        expected = []

        self.assertEqual(input, expected, "create_tree ERROR: CANNOT HANDLE opc = 'difference'")


if __name__ == '__main__':
    unittest.main()
