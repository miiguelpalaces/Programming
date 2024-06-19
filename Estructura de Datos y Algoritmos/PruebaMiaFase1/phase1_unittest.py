
import unittest
from phase1 import SList2

class Test(unittest.TestCase):

    def setUp(self):

        self.lrshift_input = SList2()
        for i in range(1, 8):
            self.lrshift_input.addLast(i)

    
    # ---- delLargestSeq unittests ----

    def test1_delLargestSeq(self):

        input = SList2()
        for i in (3, 3, 3, 4, 5, 6, 6, 6, 7, 7, 7, 7, 2):
            input.addLast(i)
        
        expected = SList2()
        for i in (3, 3, 3, 4, 5, 6, 6, 6, 2):
            expected.addLast(i)
        
        input.delLargestSeq()

        self.assertEqual(input, expected, "delLargestSeq ERROR: CANNOT HANDLE REMOVING LARGEST SEQUENCE")
        
        
        

















        
if __name__ == "__main__":
    unittest.main()