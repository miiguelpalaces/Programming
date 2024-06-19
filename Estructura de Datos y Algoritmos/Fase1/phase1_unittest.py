
import unittest
from phase1 import SList2

class Test(unittest.TestCase):

    def setUp(self):

        self.lrshift_input = SList2()
        for i in range(1, 8):
           self.lrshift_input.addLast(i)


    # ----- delLargestSeq unittests -----
    def test1_delLargestSeq(self):

        input = SList2()
        for i in (3, 3, 3, 4, 5, 6, 6, 6, 7, 7, 7, 7, 2):
            input.addLast(i)
        
        expected = SList2()
        for i in (3, 3, 3, 4, 5, 6, 6, 6, 2):
            expected.addLast(i)

        input.delLargestSeq()
        
        self.assertEqual(input, expected, "delLargestSeq ERROR: CANNOT HANDLE REMOVING LARGEST SEQUENCE")

    def test2_delLargestSeq2(self):

        input = SList2()
        for i in (8, 8, 8, 8, 4, 5, 6, 6, 6, 7, 7, 7, 7, 2):
            input.addLast(i)
        
        expected = SList2()
        for i in (8, 8, 8, 8, 4, 5, 6, 6, 6, 2):
            expected.addLast(i)

        input.delLargestSeq()

        self.assertEqual(input, expected, "delLargestSeq ERROR: CANNOT HANDLE REMOVING LARGEST SEQUENCE WHEN TIE")

    
    def test3_delLargestSeq3(self):

        input = SList2()
        for i in (6, 6, 8, 8, 4, 4, 12, 12):
            input.addLast(i)

        expected = SList2()
        for i in (6, 6, 8, 8, 4, 4):
            expected.addLast(i)

        input.delLargestSeq()

        self.assertEqual(input, expected, "delLargestSeq ERROR: CANNOT HANDLE REMOVING REMOVING LAST SEQUENCE WHEN TIE")

    
    def test4_delLargestSeq(self):
    
        input = SList2()
        input.addLast(10)

        expected = SList2()

        input.delLargestSeq()

        self.assertEqual(input, expected, "delLargestSeq ERROR: CANNOT HANDLE 1 ELEMENT LISTS")
    

    def test5_delLargestSeq(self):
    
        input = SList2()

        expected = SList2()

        input.delLargestSeq()


        self.assertEqual(input, expected, "delLargestSeq ERROR: CANNOT HANDLE EMPTY LISTS")

    
    def test6_delLargestSeq(self):
    
        input = SList2()
        for i in (1, 2, 3, 4, 5):
            input.addLast(i)

        expected = SList2()
        for i in (1, 2, 3, 4):
            expected.addLast(i)

        input.delLargestSeq()

        self.assertEqual(input, expected, "delLargestSeq ERROR: CANNOT HANDLE REMOVING LAST SEQUENCE WHEN TIE AND SEQ'S SIZE IS 1")

    def test7_delLargestSeq(self):

        input = SList2()
        for i in ("a", "a", "b", "c", "c", "c", "d", "d", "d", "c", "c"):
            input.addLast(i)

        expected = SList2()
        for i in ("a", "a", "b", "c", "c", "c", "c", "c"):
            expected.addLast(i)

        input.delLargestSeq()

        self.assertEqual(input, expected, "delLargestSeq ERROR: CANNOT HANDLE ALPHABETIC ELEMENTS")


    # ----- fixloop unittests -----
    def test1_fixloop(self):

        input = SList2()
        for i in ("X", "Y", "Z"):
            input.addLast(i)

        expected = SList2()
        for i in ("X", "Y", "Z"):
            expected.addLast(i)

        input.fix_loop()

        self.assertEqual(input, expected, "fixloop ERROR: CANNOT HANDLE AN ALREADY FIXED LIST")
    
    def test2_fixloop(self):

        input = SList2()
        for i in (1, 2, 3):
            input.addLast(i)
        input.create_loop(0)

        expected = SList2()
        for i in (1, 2, 3):
            expected.addLast(i)

        input.fix_loop()

        self.assertEqual(input, expected, "fixloop ERROR: CANNOT FIX A LOOPED LIST LOOPED TO THE FIRST ELEMENT")

    def test3_fixloop(self):

        input = SList2()
        for i in ("A", "B", "C"):
            input.addLast(i)
        input.create_loop(1)

        expected = SList2()
        for i in ("A", "B", "C"):
            expected.addLast(i)

        input.fix_loop()

        self.assertEqual(input, expected, "fixloop ERROR: CANNOT FIX A LOOPED LIST ANOTHER ELEMENT ELEMENT OHER THAN THE FIRST ONE")

    def test4_fixloop(self):

        input = SList2()
        
        expected = SList2()

        self.assertEqual(input, expected, "fixloop ERROR: CANNOT HANDLE AN EMPTY LIST")


    def test5_fixloop(self):

        input = SList2()
        input.addLast("A")
        input.create_loop(0)

        expected = SList2()
        expected.addLast("A")

        input.fix_loop()

        self.assertEqual(input, expected, "fixloop ERROR: CANNOT HANDLE AN LIST WITH A SINGLE ELEMENT")



    # ----- leftrightShif unittests -----
    def test1_leftrightShift(self):

        expected = SList2()
        for i in (3, 4, 5, 6, 7, 1, 2):
            expected.addLast(i)

        self.lrshift_input.leftrightShift(True, 2)

        self.assertEqual(self.lrshift_input, expected, "ERROR leftrightShift: CANNOT HANDLE left = True, n = 2")


    def test2_leftrightShift(self):

        expected = SList2()
        for i in (2, 3, 4, 5, 6, 7, 1):
            expected.addLast(i)

        self.lrshift_input.leftrightShift(True, 1)

        self.assertEqual(self.lrshift_input, expected, "ERROR leftrightShift: CANNOT HANDLE left = True, n = 1")


    def test3_leftrightShift(self):

        expected = SList2()
        for i in (5, 6, 7, 1, 2, 3, 4):
            expected.addLast(i)

        self.lrshift_input.leftrightShift(True, 4)

        self.assertEqual(self.lrshift_input, expected, "ERROR leftrightShift: CANNOT HANDLE left = True, n = 4")


    def test4_leftrightShift(self):

        expected = SList2()
        for i in (6, 7, 1, 2, 3, 4, 5):
            expected.addLast(i)

        self.lrshift_input.leftrightShift(False, 2)

        self.assertEqual(self.lrshift_input, expected, "ERROR leftrightShift: CANNOT HANDLE left = Faslse, n = 2")


    def test5_leftrightShift(self):

        expected = SList2()
        for i in (7, 1, 2, 3, 4, 5, 6):
            expected.addLast(i)

        self.lrshift_input.leftrightShift(False, 1)

        self.assertEqual(self.lrshift_input, expected, "ERROR leftrightShift: CANNOT HANDLE left = Fasle, n = 1")


    def test6_leftrightShift(self):

        expected = SList2()
        for i in (4, 5, 6, 7, 1, 2, 3):
            expected.addLast(i)

        self.lrshift_input.leftrightShift(False, 4)

        self.assertEqual(self.lrshift_input, expected, "ERROR leftrightShift: CANNOT HANDLE left = False, n = 4")


    def test7_leftrightShift(self):

        expected = SList2()
        for i in (1, 2, 3, 4, 5, 6, 7):
            expected.addLast(i)

        self.lrshift_input.leftrightShift(True, 7)

        self.assertEqual(self.lrshift_input, expected, "ERROR leftrightShift: CANNOT HANDLE left = True, n = 7")


    def test8_leftrightShift(self):

        expected = SList2()
        for i in (1, 2, 3, 4, 5, 6, 7):
            expected.addLast(i)

        self.lrshift_input.leftrightShift(True, 10)

        self.assertEqual(self.lrshift_input, expected, "ERROR leftrightShift: CANNOT HANDLE left = True, n = 10")

    
    def test9_leftrightShift(self):
        
        expected = SList2()
        self.lrshift_input = SList2()
        
        self.lrshift_input.leftrightShift(True, 10)

        self.assertEqual(self.lrshift_input, expected, "ERROR leftrightShift: CANNOR HANDDLE left = True, n = 10")
    

if __name__ == "__main__":

    unittest.main()